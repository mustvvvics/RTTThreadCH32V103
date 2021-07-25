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
#include "headfile.h"

rt_sem_t camera_sem;

int main(void)
{
    camera_sem = rt_sem_create("camera", 0, RT_IPC_FLAG_FIFO);
    mt9v03x_init();
    rt_thread_mdelay(3000);                 //保证摄像头初始化完成

    laneInit();

    display_init();
    encoder_init();
    timer_pit_init();                       //软定时器初始化
    gpio_init(D2,GPO,0,OUT_PP); //蜂鸣器


    uart_init(UART_3,921600,UART3_TX_B10,UART3_RX_B11);     //通讯

    uart_rx_irq(UART_3, ENABLE);                            //默认抢占优先级1 次优先级0。
    nvic_init((IRQn_Type)(53 + UART_3), 1, 1, ENABLE);      //将串口3的抢占优先级设置为最高，次优先级设置为最高。

    gpio_init(B12, GPO, 0, GPIO_PIN_CONFIG);                 //同步引脚初始化 time_pit
    gpio_init(B15, GPO, 1, GPIO_PIN_CONFIG);


    flagCameraElement = 5;
    if (flagCameraElement == 5 && camFlashWriteFlag == 1) {
        itoaChar(elementTableFromMain,elementQueue,10);
    }
//    BEEP_ON;
//    rt_thread_mdelay(1000);
//    BEEP_OFF;
//    esp8266Init();

    while(1)
    {
        //等待摄像头采集完毕
        rt_sem_take(camera_sem, RT_WAITING_FOREVER);
        //开始处理摄像头图像
        rt_thread_mdelay(10);
        timet1 = rt_tick_get();
        laneAnalyze(mt9v03x_image);
        computeError();
        timet2 = rt_tick_get();
        timeControl = timet2 - timet1;
//        sendMessageData();
    }
}
