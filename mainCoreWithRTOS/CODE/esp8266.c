/*
 * esp8266.c
 *
 *  Created on: Jun 12, 2021
 *      Author: 29275
 */
#include "headfile.h"

rt_sem_t esp8266_sem;

//接线顺序
//C12 - WiFi-RST
//C11 - WiFi-TX
//C10 - WiFi-RX


void ESP8266_Clear(void)
{
    memset(esp8266_buf, 0, sizeof(esp8266_buf));
    esp8266_cnt = 0;
}

//void Esp_IP_Get(uint8 data_temp)
//{
//    static uint8 IP_Start=0;
//    static uint8 IP_REC=0;
//    if(IP_REC)
//    {
//        esp8266_buf[esp8266_cnt++]=data_temp;
//    }
//    else
//    {
//        if(data_temp == ':')IP_Start=1;
//
//        if(data_temp >= '0' && data_temp <='9' && IP_Start == 1)
//        {
//            esp8266_buf[esp8266_cnt++]=data_temp;
//        }
//        else if(data_temp == '.'&& IP_Start == 1)
//        {
//            esp8266_buf[esp8266_cnt++]=data_temp;
//        }
//        else if(data_temp == 0x0A&& IP_Start == 1)
//        {
//            oled_p6x8str(0, 0, esp8266_buf);
//            IP_REC=1;
//            IP_Start_Rec=1;
//            ESP8266_Clear();
//        }
//    }
//
//}

void Esp_Init(void)
{
    //初始化串口2
    uart_init(UART_2,921600,UART2_TX_A2,UART2_RX_A3);//    UART2_TX_A2 = 0x10,     UART2_RX_A3,    //默认引脚 原921600

    //使能串口接收中断
    uart_rx_irq(UART_2,ENABLE);
//    nvic_init((IRQn_Type)(53 + UART_2), 2, 2, ENABLE);      //将串口3的抢占优先级设置为最高，次优先级设置为最高。
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
    rt_kprintf("TCP in!\n");//打印到终端
    if(esp8266_buf[esp8266_cnt-1] != 0x0A)return;

//    char txt[32];
//    int32 Int2Float;
//

    if(strcmp((char *)esp8266_buf,"init\n") == 0)
    {
        ips114_showstr(0,6,"TCP Init Ok");
        uart_putstr(UART_2,"#0005init\n");
//        rt_kprintf("TCP in!\n");//打印到终端
    }

//    else if(esp8266_buf[4] == 'S' && esp8266_buf[0] == 'C')
//    {
//        sscanf(esp8266_buf,"Car_Speed:%d\n",&expected_y);
//        uart_putstr(UART_2,"#0016received_speed!\n");
//        sprintf(txt,"Speed:%05d",expected_y);
//        ips114_showstr(0,5,txt);
//    }
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


    ESP8266_Clear();
    rt_kprintf("TCP OUT!\n");//打印到终端
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

//    rt_thread_mdelay(300);
    while(1)
    {
//        rt_thread_mdelay(100);
        rt_sem_take(esp8266_sem, RT_WAITING_FOREVER);
        Tcp_Decode();
        rt_kprintf("8266\n");


    }
}


void esp8266Init(void)
{
    rt_thread_t tidEsp8266;

    //初始化
    Esp_Init();

    esp8266_sem = rt_sem_create("camera", 0, RT_IPC_FLAG_FIFO);

    //创建线程
    tidEsp8266 = rt_thread_create("esp8266", esp8266Entry, RT_NULL, 1024, 2, 200);//倒数 心跳 优先级 stack_size

    //启动线程
    if(RT_NULL != tidEsp8266)
    {
        rt_thread_startup(tidEsp8266);
        rt_kprintf("8266_startup\n");
    }
}
