/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897(����)  ��Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file            main
 * @company         �ɶ���ɿƼ����޹�˾
 * @author          ��ɿƼ�(QQ790875685)
 * @version         �鿴doc��version�ļ� �汾˵��
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
    pwm_init(PWM1_CH1_A8, 50, 670);//�����ʼ����Ĭ������λ��       ��� TIMER1  338 670 1000

    esp8266Init();
    PID_Init();
    timer_pit_init();
    //����3��ʼ��
    uart_init(UART_3, 921600, UART3_TX_B10, UART3_RX_B11);  //����3��ʼ����������115200
    //������3�ж�
    uart_rx_irq(UART_3, ENABLE);                            //Ĭ����ռ���ȼ�1 �����ȼ�0��
    nvic_init((IRQn_Type)(53 + UART_3), 0, 0, ENABLE);      //������3����ռ���ȼ�����Ϊ��ߣ������ȼ�����Ϊ��ߡ�
    //���ⲿ�ж�
    gpio_interrupt_init(B2, RISING, GPIO_INT_CONFIG);       //B2��ʼ��ΪGPIO �����ش���
    nvic_init(EXTI2_IRQn, 1, 1, ENABLE);                    //EXTI2���ȼ����ã���ռ���ȼ�1�������ȼ�1
    while(1)
    {
//        char txt[32];
        rt_thread_mdelay(4);//new delay
        sendMessage();//��������
//        sprintf(txt,"carF=%01d|ERR=%05d|GA=%05d",car_flag,position_front,g_fGyroAngleSpeed_z);
//        ips114_showstr(0,1,txt);
//        sprintf(txt,"key=%02d|Fg=%01d|+AC=%02d",key_data,elementFlag,accelerate);
//        ips114_showstr(0,2,txt);
//        else if (key_data == 6 || key_data == 7 || key_data == 8 || key_data == 9) {//ң��
//            manual_control();
//        }
    }
}
