
#include "headfile.h"

rt_sem_t esp8266_sem;

void ESP8266_Clear(void)
{
    memset(esp8266_buf, 0, sizeof(esp8266_buf));
    esp8266_cnt = 0;
}

void clearError(void){
    expected_omega = 0;
    position_front = 0;
    g_fGyroAngleSpeed_z = 0;
//    expected_y = 0;
    speed_conversion(0,0,0);
    yaw_pid.LocSum = 0;
    yaw_pid.Ek = 0;
    yaw_pid.Ek1 = 0;
    yaw_w_pid.target_val = 0;
    yaw_w_pid.err_next = 0;
    yaw_w_pid.err = 0;
    yaw_w_pid.err_last = 0;
    yaw_w_pid.actual_val = 0;
    dy=0;dz=0;count_en=0;
    manual_y=0;manual_z=0;
    go_forward=0;go_backward=0;
    go_left=0;go_right=0;
}

void Esp_Init(void)
{
    //初始化串口2
    uart_init(UART_2,921600,UART2_TX_A2,UART2_RX_A3);//    UART2_TX_A2 = 0x10,     UART2_RX_A3,    //默认引脚 原921600

    //使能串口接收中断
    uart_rx_irq(UART_2,ENABLE);

    //初始化复位引脚
    gpio_init(A4, GPO, 1, GPIO_PIN_CONFIG);

    gpio_set(A4,0);
    systick_delay_ms(250);
    gpio_set(A4,1);
    systick_delay_ms(500);

    ESP8266_Clear();
}


void Tcp_Decode(void)
{
    if(esp8266_buf[esp8266_cnt-1] != 0x0A)return;

//    char txt[32];
//    int32 Int2Float;
//
    if(strcmp((char *)esp8266_buf,"init\n") == 0) { //
        ips114_showstr(0,4,"TCP Init Ok");
        uart_putstr(UART_2,"#0008control\n");
    }
    /**************************************************************************/
    //启停
    else if(strcmp((char *)esp8266_buf,"car_go\n") == 0){
        car_flag = 1;
        uart_putstr(UART_2,"#0013received_go!\n");
    }

    else if (strcmp((char *)esp8266_buf,"car_stop\n") == 0) {
        car_flag =0;
        clearError();//清除累计误差
        uart_putstr(UART_2,"#0015received_stop!\n");
    }
    /**************************************************************************/
    //参数修改
    else if(esp8266_buf[4] == 'S' && esp8266_buf[0] == 'C')
    {
        expected_y = 100 * (esp8266_buf[10] - '0') + 10 * (esp8266_buf[11] - '0') + (esp8266_buf[12] - '0');
//        sscanf(esp8266_buf,"Car_Speed:%d\n",&expected_y);//致命因子
        uart_putstr(UART_2,"#0016received_speed!\n");
    }
    //Turn_Kp:6026/n
//    else if(esp8266_buf[6] == 'p' && esp8266_buf[0] == 'T')
//    {
//        sscanf(esp8266_buf,"Turn_Kp:%d\n",&Int2Float);
//        uart_putstr(UART_2,"#0013received_Kp!\n");
//        yaw_w_pid.Kp=(float)Int2Float*0.001f;
//        ips114_showfloat(0,5,yaw_pid.Kp,3,3);
//    }
//    //Turn_Kd:19775/n yaw_w_pid.Ki
//    else if(esp8266_buf[6] == 'i' && esp8266_buf[0] == 'T')
//    {
//        sscanf(esp8266_buf,"Turn_Ki:%d\n",&Int2Float);
//        uart_putstr(UART_2,"#0013received_Kp!\n");
//        yaw_w_pid.Ki=(float)Int2Float*0.001f;
//        ips114_showfloat(0,6,yaw_pid.Kp,3,3);
//    }
//
//    else if(esp8266_buf[6] == 'd' && esp8266_buf[0] == 'T')
//    {
//        sscanf(esp8266_buf,"Turn_Kd:%d\n",&Int2Float);
//        uart_putstr(UART_2,"#0013received_Kd!\n");
//        yaw_pid.Kd=(float)Int2Float*0.001f;
//        ips114_showfloat(0,7,yaw_pid.Kd,3,3);
//    }
    /**************************************************************************/
    //控制
    else if(strcmp((char *)esp8266_buf,"Forward\n") == 0)
    {go_forward=1;uart_putstr(UART_2,"#0018received_Forward!\n");rt_mb_send(key_mailbox, 6);}

    else if(strcmp((char *)esp8266_buf,"Backward\n") == 0)
    {go_backward=1;uart_putstr(UART_2,"#0019received_Backward!\n");rt_mb_send(key_mailbox, 7);}

    else if(strcmp((char *)esp8266_buf,"Right\n") == 0)
    {go_right=1;uart_putstr(UART_2,"#0016received_Right!\n");rt_mb_send(key_mailbox, 8);}

    else if(strcmp((char *)esp8266_buf,"Left\n") == 0)
    {go_left=1;uart_putstr(UART_2,"#0015received_Left!\n");rt_mb_send(key_mailbox, 9);}

    ESP8266_Clear();
}

void manual_control(void)
{
    if(key_data==6)
    {
        clearError();
        manual_y = 10;
        manual_z = 0;
    }

    if(key_data==7)
    {
        clearError();
        manual_y=-10;
        manual_z = 0;

    }

    if(key_data==8)
    {
        clearError();
        manual_z=-40;
        manual_y = 0;
    }

    if(key_data==9)
    {
        clearError();
        manual_z=40;
        manual_y = 0;
    }

}

const char* message0 = ",";
const char* message1 = "\n";
void sendMessage(void) {
//发送两个数据曲线进行分析
    char txtA[6];
    sprintf(txtA,"%04d",expected_omega);
    uart_putstr(UART_2,txtA);
    uart_putstr(UART_2,message0);
    sprintf(txtA,"%04d",g_fGyroAngleSpeed_z);
    uart_putstr(UART_2,txtA);
    uart_putstr(UART_2,message1);
}

void esp8266Entry(void *parameter)
{
    rt_sem_take(esp8266_sem, RT_WAITING_FOREVER);
    esp8266_buf[esp8266_cnt-2] = '\0';//消除显示乱码
    ips114_showstr(0,7,esp8266_buf);
    ESP8266_Clear();
    while(1)
    {
        rt_sem_take(esp8266_sem, RT_WAITING_FOREVER);
        Tcp_Decode();
    }
}

void esp8266Init(void)
{
    rt_thread_t tidEsp8266;

    //初始化
    Esp_Init();

    esp8266_sem = rt_sem_create("esp8266", 0, RT_IPC_FLAG_FIFO);


    //创建线程
    tidEsp8266 = rt_thread_create("esp8266", esp8266Entry, RT_NULL, 1024, 2, 200);//倒数 心跳 优先级 stack_size

    //启动线程
    if(RT_NULL != tidEsp8266)
    {
        rt_thread_startup(tidEsp8266);
//        rt_kprintf("8266_startup\n");
    }
}
