/*
 * esp8266.c
 *
 *  Created on: Jun 12, 2021
 *      Author: 29275
 */
#include "headfile.h"

//接线顺序
//C12 - WiFi-RST
//C11 - WiFi-TX
//C10 - WiFi-RX

void ESP8266_Clear(void)
{
    memset(esp8266_buf, 0, sizeof(esp8266_buf));
    esp8266_cnt = 0;
}

void Esp_IP_Get(uint8 data_temp)
{
    static uint8 IP_Start=0;
    static uint8 IP_REC=0;
    if(IP_REC)
    {
        esp8266_buf[esp8266_cnt++]=data_temp;
    }
    else
    {
        if(data_temp == ':')IP_Start=1;

        if(data_temp >= '0' && data_temp <='9' && IP_Start == 1)
        {
            esp8266_buf[esp8266_cnt++]=data_temp;
        }
        else if(data_temp == '.'&& IP_Start == 1)
        {
            esp8266_buf[esp8266_cnt++]=data_temp;
        }
        else if(data_temp == 0x0A&& IP_Start == 1)
        {
            oled_p6x8str(0, 0, esp8266_buf);
            IP_REC=1;
            IP_Start_Rec=1;
            ESP8266_Clear();
        }
    }

}

void Esp_Init(void)
{
    //初始化串口3
    uart_init(UART_2,921600,UART2_TX_A2,UART2_RX_A3);//    UART2_TX_A2 = 0x10,     UART2_RX_A3,    //默认引脚

    //使能串口接收中断
    uart_rx_irq(UART_2,ENABLE);
    //初始化复位引脚
    gpio_init(A4, GPO, 1, GPIO_PIN_CONFIG);

    gpio_set(A4,0);
    systick_delay_ms(250);
    gpio_set(A4,1);
    systick_delay_ms(500);

    //ESP8266_Clear();
}
uint8 run_mode=0;
//命令范例：ch.laTh.110\n
void Tcp_Decode(void)
{
    if(esp8266_buf[esp8266_cnt-1] != 0x0A)return;

    //预留指令 低电量报警 接收到上位机控制指令 回传Hungry

    char txt[32];
    int32 Int2Float;


    if(strcmp((char *)esp8266_buf,"init\n") == 0)
    {ips114_showstr(0,6,"TCP Init Ok");uart_putstr(UART_2,"#0005init\n");}

//    else if(esp8266_buf[4] == 'S' && esp8266_buf[0] == 'C')
//    {
//        sscanf(esp8266_buf,"Car_Speed:%d\n",&expected_y);
//        uart_putstr(UART_2,"#0016received_speed!\n");
//        sprintf(txt,"Speed:%05d",expected_y);
//        ips114_showstr(0,4,txt);
//    }
//    //Turn_Kp:6026/n
//    else if(esp8266_buf[6] == 'p' && esp8266_buf[0] == 'T')
//    {
//        sscanf(esp8266_buf,"Turn_Kp:%d\n",&Int2Float);
//        uart_putstr(UART_2,"#0013received_Kp!\n");
//        yaw_pid.Kp=(float)Int2Float*0.001f;
//        oled_printf_float(0,5,yaw_pid.Kp,3,3);
//    }
//    //Turn_Kd:19775/n
//    else if(esp8266_buf[6] == 'd' && esp8266_buf[0] == 'T')
//    {
//        sscanf(esp8266_buf,"Turn_Kd:%d\n",&Int2Float);
//        uart_putstr(UART_2,"#0013received_Kd!\n");
//        yaw_pid.Kd=(float)Int2Float*0.001f;
//        oled_printf_float(0,6,yaw_pid.Kd,3,3);
//    }


    ESP8266_Clear();
}





