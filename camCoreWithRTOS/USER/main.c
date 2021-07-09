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


rt_sem_t camera_sem;

int main(void)
{
    camera_sem = rt_sem_create("camera", 0, RT_IPC_FLAG_FIFO);
    mt9v03x_init();
    rt_thread_mdelay(3000);//保证摄像头初始化完成

    laneInit();
    display_init();
    encoder_init();
    timer_pit_init();//软定时器初始化
//
//    esp8266Init();

    gpio_init(B12, GPO, 0, GPIO_PIN_CONFIG);                 //同步引脚初始化 time_pit
    uart_init(UART_3,921600,UART3_TX_B10,UART3_RX_B11);//通讯
    gpio_init(B15, GPO, 0, GPIO_PIN_CONFIG);

    while(1)
    {
        //等待摄像头采集完毕
        rt_sem_take(camera_sem, RT_WAITING_FOREVER);
//        rt_thread_mdelay(200);
//        gpio_toggle(B15);
        laneAnalyze(mt9v03x_image);
        computeError();
        //开始处理摄像头图像
        //mt9v03x_image[0]
    }
}
