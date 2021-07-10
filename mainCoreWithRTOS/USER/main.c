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



int main(void)
{
    button_init();
    icm20602_init_spi();
    display_init();
    encoder_init();
    motor_init();
    //timer_pit_init();

    //舵机初始化，默认在中位上
    pwm_init(PWM1_CH1_A8, 50, 650);//舵机 TIMER1

    esp8266Init();
    PID_Init();

    //串口3初始化
    uart_init(UART_3, 921600, UART3_TX_B10, UART3_RX_B11);  //串口3初始化，波特率115200
    //开串口3中断
    uart_rx_irq(UART_3, ENABLE);                            //默认抢占优先级1 次优先级0。
    nvic_init((IRQn_Type)(53 + UART_3), 0, 0, ENABLE);      //将串口3的抢占优先级设置为最高，次优先级设置为最高。
    //开外部中断
    gpio_interrupt_init(B2, RISING, GPIO_INT_CONFIG);       //B2初始化为GPIO 上升沿触发
    nvic_init(EXTI2_IRQn, 1, 1, ENABLE);                    //EXTI2优先级配置，抢占优先级1，次优先级1

    while(1)
    {
//        rt_thread_mdelay(20);//new delay

        rt_mb_recv(key_mailbox, &key_data, RT_WAITING_FOREVER);
        if (key_data == 1) { //左
            car_flag = 0;
        }
        else if (key_data == 2) {
            if (car_flag == 0) {
                car_flag = 1;
            }
            else {
                car_flag = 0;
                clearError();//清除累计误差
            }
        }
        else if (key_data == 3) { //上
            expected_y = expected_y + 10;
        }
        else if (key_data == 4) { //右
            car_flag = 0;
        }
        else if (key_data == 5) { //下
            expected_y = expected_y - 10;
        }
        else if (key_data == 6 || key_data == 7 || key_data == 8 || key_data == 9) {//遥控
            manual_control();
        }
    }
}
