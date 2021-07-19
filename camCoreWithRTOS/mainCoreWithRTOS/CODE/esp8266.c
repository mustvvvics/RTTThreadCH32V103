#include "headfile.h"

rt_sem_t esp8266_sem;

void ESP8266_Clear(void)
{
    memset(esp8266_buf, 0, sizeof(esp8266_buf));
    esp8266_cnt = 0;
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

int32 recevePidData(int16 receiveWay){
    int32 pidData = 0;
    if (receiveWay == 100) {
        pidData = 100 * (esp8266_buf[9] - '0') + 10 * (esp8266_buf[10] - '0')
                        + (esp8266_buf[11] - '0');
    }
    else if (receiveWay == 1000) {
        pidData = 1000 * (esp8266_buf[9] - '0') + 100 * (esp8266_buf[10] - '0')
                + 10 * (esp8266_buf[11] - '0') + (esp8266_buf[12] - '0');
    }
    return pidData;
}

void Tcp_Decode(void)
{
    if(esp8266_buf[esp8266_cnt-1] != 0x0A)return;
    int32 dataChange;

    if(strcmp((char *)esp8266_buf,"init\r\n") == 0) { //
        uart_putstr(UART_2,"#0008control\n");
    }
    /**************************************************************************/
    //启停
    else if(strcmp((char *)esp8266_buf,"car_go\r\n") == 0){
        car_flag = 1;
        uart_putstr(UART_2,"#0013received_go!\n");
    }

    else if (strcmp((char *)esp8266_buf,"car_stop\r\n") == 0) {
        car_flag =0;
        clearError();//清除累计误差
        uart_putstr(UART_2,"#0015received_stop!\n");
    }
    /**************************************************************************/
    //参数修改
    else if(esp8266_buf[4] == 'S' && esp8266_buf[0] == 'C')
    {
        expected_y = 100 * (esp8266_buf[10] - '0') + 10 * (esp8266_buf[11] - '0') + (esp8266_buf[12] - '0');
//        sscanf(esp8266_buf,"Car_Speed:%d\n",&expected_y); //致命因子
        uart_putstr(UART_2,"#0016received_speed!\n");
    }
    /*********************************************************************************************/
    //速度环整定   "Speed_Kp:%d\n"  float S_P=136;
    else if(esp8266_buf[7] == 'p' && esp8266_buf[0] == 'S'){
        dataChange = recevePidData(100);
        uart_putstr(UART_2,"#0010Speed_Kp!\n");
        S_P=(float)dataChange;
        motor1_pid.Kp = (float)dataChange;
        motor2_pid.Kp = (float)dataChange;
        motor3_pid.Kp = (float)dataChange;
        motor4_pid.Kp = (float)dataChange;
    }
    else if(esp8266_buf[7] == 'i' && esp8266_buf[0] == 'S'){
        dataChange = recevePidData(100);
        uart_putstr(UART_2,"#0010Speed_Ki!\n");
        S_I=(float)dataChange;
        motor1_pid.Ki = (float)dataChange;
        motor2_pid.Ki = (float)dataChange;
        motor3_pid.Ki = (float)dataChange;
        motor4_pid.Ki = (float)dataChange;
    }
    else if(esp8266_buf[7] == 'd' && esp8266_buf[0] == 'S'){
        dataChange = recevePidData(100);
        uart_putstr(UART_2,"#0010Speed_Kd!\n");
        S_D=(float)dataChange;
        motor1_pid.Kd = (float)dataChange;
        motor2_pid.Kd = (float)dataChange;
        motor3_pid.Kd = (float)dataChange;
        motor4_pid.Kd = (float)dataChange;
    }
    /*********************************************************************************************/
    //角速度环整定   "Angle_Kp:%d\n" float yaw_w_I=0.01; EG: 10  ---> 0.010
    else if(esp8266_buf[7] == 'p' && esp8266_buf[0] == 'A'){
        dataChange = recevePidData(1000);
        uart_putstr(UART_2,"#0010Angle_Kp!\n");
        yaw_w_P=(float)dataChange*0.001f;
        yaw_w_pid.Kp =(float)dataChange*0.001f;
    }
    else if(esp8266_buf[7] == 'i' && esp8266_buf[0] == 'A'){
        dataChange = recevePidData(1000);
        uart_putstr(UART_2,"#0010Angle_Ki!\n");
        yaw_w_I=(float)dataChange*0.001f;
        yaw_w_pid.Ki =(float)dataChange*0.001f;
    }
    else if(esp8266_buf[7] == 'd' && esp8266_buf[0] == 'A'){
        dataChange = recevePidData(1000);
        uart_putstr(UART_2,"#0010Angle_Kd!\n");
        yaw_w_D=(float)dataChange*0.001f;
        yaw_w_pid.Kd =(float)dataChange*0.001f;
    }
    /*********************************************************************************************/
    //转向环整定    "Turnn_Kp:%d\n"   eg: 4000 ----> 4
    else if(esp8266_buf[7] == 'p' && esp8266_buf[0] == 'T'){
        dataChange = recevePidData(1000);
        uart_putstr(UART_2,"#0010Turnn_Kp!\n");
        yaw_P=(float)dataChange*0.01f;
        yaw_pid.Kp = (float)dataChange*0.01f;

    }
    else if(esp8266_buf[7] == 'i' && esp8266_buf[0] == 'T'){
        dataChange = recevePidData(1000);
        uart_putstr(UART_2,"#0010Turnn_Ki!\n");
        yaw_I=(float)dataChange*0.01f;
        yaw_pid.Ki = (float)dataChange*0.01f;
    }
    else if(esp8266_buf[7] == 'd' && esp8266_buf[0] == 'T'){
        dataChange = recevePidData(1000);
        uart_putstr(UART_2,"#0010Turnn_Kd!\n");
        yaw_D=(float)dataChange*0.01f;
        yaw_pid.Kd = (float)dataChange*0.01f;
    }
    /**************************************************************************/
    //控制前后左右
    else if(strcmp((char *)esp8266_buf,"Forward\r\n") == 0)
    {go_forward=1;uart_putstr(UART_2,"#0018received_Forward!\n");rt_mb_send(key_mailbox, 6);}

    else if(strcmp((char *)esp8266_buf,"Backward\r\n") == 0)
    {go_backward=1;uart_putstr(UART_2,"#0019received_Backward!\n");rt_mb_send(key_mailbox, 7);}

    else if(strcmp((char *)esp8266_buf,"Right\r\n") == 0)
    {go_right=1;uart_putstr(UART_2,"#0016received_Right!\n");rt_mb_send(key_mailbox, 8);}

    else if(strcmp((char *)esp8266_buf,"Left\r\n") == 0)
    {go_left=1;uart_putstr(UART_2,"#0015received_Left!\n");rt_mb_send(key_mailbox, 9);}

    else if(strcmp((char *)esp8266_buf,"ThreeWay\r\n") == 0)
    {ThreeWayIntersectionFlag = 1;uart_putstr(UART_2,"#ThreeWay!\n");}

    ESP8266_Clear();
}

const char* message0 = ",";
const char* message1 = "\n";

void sendMessage(void) {                //发送数据曲线进行分析
    char txtA[6];
    if (pidModel == 2) { //速度环整定
//        rt_sprintf(txtA,"%04d",(int16)(leftFrontADRC ) );
        rt_sprintf(txtA,"%04d",(int16)(Left_front) );
        uart_putstr(UART_1,txtA); uart_putstr(UART_1,message0);
//        rt_sprintf(txtA,"%04d",(int16)( leftFrontADRC + PID_Speed(Left_front,-encoder_data[3],&motor1_pid) ));
        rt_sprintf(txtA,"%04d",(int16)(-encoder_data[3]) );
        uart_putstr(UART_1,txtA);uart_putstr(UART_1,message0);

//        rt_sprintf(txtA,"%04d",(int16)( rightFrontADRC) );
        rt_sprintf(txtA,"%04d",(int16)(Right_front) );
        uart_putstr(UART_1,txtA); uart_putstr(UART_1,message0);
//        rt_sprintf(txtA,"%04d",(int16)( rightFrontADRC + PID_Speed(Right_front,-encoder_data[2],&motor2_pid) ));
        rt_sprintf(txtA,"%04d",(int16)(-encoder_data[2]) );
        uart_putstr(UART_1,txtA);uart_putstr(UART_1,message0);

//        rt_sprintf(txtA,"%04d",(int16)( rightRearADRC) );
        rt_sprintf(txtA,"%04d",(int16)(Right_rear) );
        uart_putstr(UART_1,txtA); uart_putstr(UART_1,message0);
//        rt_sprintf(txtA,"%04d",(int16)( rightRearADRC + PID_Speed(Right_rear,-encoder_data[0],&motor3_pid) ));
        rt_sprintf(txtA,"%04d",(int16)(-encoder_data[0]) );
        uart_putstr(UART_1,txtA);uart_putstr(UART_1,message0);


//        rt_sprintf(txtA,"%04d",(int16)( leftRearADRC) );
        rt_sprintf(txtA,"%04d",(int16)(Left_rear) );
        uart_putstr(UART_1,txtA); uart_putstr(UART_1,message0);
        rt_sprintf(txtA,"%04d",(int16)(-encoder_data[1]) );
//        rt_sprintf(txtA,"%04d",(int16)( leftRearADRC + PID_Speed(Left_rear,-encoder_data[1],&motor4_pid) ));
        uart_putstr(UART_1,txtA);

        uart_putstr(UART_1,message1);
    }
    else if (pidModel == 3) { //角度环整定
        rt_sprintf(txtA,"%04d",manual_z);uart_putstr(UART_1,txtA);uart_putstr(UART_1,message0);
//        rt_sprintf(txtA,"%04d",(int16)PID_Angle(manual_z,g_fGyroAngleSpeed_z,&yaw_w_pid));
        rt_sprintf(txtA,"%04d",(int16)g_fGyroAngleSpeed_z);

        uart_putstr(UART_1,txtA);

        uart_putstr(UART_1,message1);
    }
    else if (pidModel == 4){ //转向环整定
        rt_sprintf(txtA,"%04d",0);uart_putstr(UART_2,txtA);uart_putstr(UART_2,message0);
        rt_sprintf(txtA,"%04d",(int16)PID_Loc(0,-position_front,&yaw_pid));uart_putstr(UART_2,txtA);

        uart_putstr(UART_2,message1);
    }
    else if (pidModel == 1){ //开环计算
        rt_sprintf(txtA,"%04d",-encoder_data[3]);uart_putstr(UART_1,txtA); uart_putstr(UART_1,message0);
        rt_sprintf(txtA,"%04d",-encoder_data[2]);uart_putstr(UART_1,txtA); uart_putstr(UART_1,message0);
        rt_sprintf(txtA,"%04d",-encoder_data[0]);uart_putstr(UART_1,txtA); uart_putstr(UART_1,message0);
        rt_sprintf(txtA,"%04d",-encoder_data[1]);uart_putstr(UART_1,txtA);
        uart_putstr(UART_1,message1);

    }

}

void esp8266Entry(void *parameter)
{
    rt_sem_take(esp8266_sem, RT_WAITING_FOREVER);
    esp8266_buf[esp8266_cnt-2] = '\0'; //消除显示乱码
    ips114_showstrGRAY(45,0,esp8266_buf); //显示在标题栏目
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
    }
}
