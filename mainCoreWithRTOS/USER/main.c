/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file            main
 * @company         成都逐飞科技有限公司
 * @author          逐飞科技(QQ790875685)
 * @version         查看doc内version文件 版本说明
 * @Software        MounRiver Studio V1.3
 * @Target core     CH32V103R8T6
 * @Taobao          https://seekfree.taobao.com/
 * @date            2020-12-04
 ********************************************************************************************************************/
//整套推荐IO查看Projecct文件夹下的TXT文本

//打开新的工程或者工程移动了位置务必执行以下操作
//右键单击工程，选择刷新

#include "headfile.h"



int main(void)
{
    //uint16 duty;
    //camera_sem = rt_sem_create("camera", 0, RT_IPC_FLAG_FIFO);
    //mt9v03x_init();

//    esp8266Init();
    icm20602_init_spi();
    Esp_Init();
    PID_Init();
    display_init();
    encoder_init();
    button_init();
    motor_init();
//    buzzer_init();

    //elec_init();电感初始化
    //timer_pit_init();

    //舵机初始化，默认在中位上
    pwm_init(PWM1_CH1_A8, 50, 650);//舵机 TIMER1

    //LED灯初始化
//    gpio_init(B15, GPO, 1, GPIO_PIN_CONFIG); //icm20602 时不能使用

    //双核通信放最后

    //串口3初始化
    uart_init(UART_3, 921600, UART3_TX_B10, UART3_RX_B11);  //串口3初始化，波特率115200
    //开串口3中断
    uart_rx_irq(UART_3, ENABLE);                            //默认抢占优先级1 次优先级0。
    nvic_init((IRQn_Type)(53 + UART_3), 0, 0, ENABLE);      //将串口3的抢占优先级设置为最高，次优先级设置为最高。
    //开外部中断
    gpio_interrupt_init(B2, RISING, GPIO_INT_CONFIG);       //B2初始化为GPIO 上升沿触发
    nvic_init(EXTI2_IRQn, 1, 1, ENABLE);                    //EXTI2优先级配置，抢占优先级1，次优先级1

//    ips114_showstr(0, 6, "Master Init OK!");
    while(1)
    {
        rt_thread_mdelay(10);//new delay
//        gpio_toggle(B15);
        Tcp_Decode();
        sendMessage();
    }
}
