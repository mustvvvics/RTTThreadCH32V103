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
//�����Ƽ�IO�鿴Projecct�ļ����µ�TXT�ı�

//���µĹ��̻��߹����ƶ���λ�����ִ�����²���
//�Ҽ��������̣�ѡ��ˢ��

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

    //elec_init();��г�ʼ��
    //timer_pit_init();

    //�����ʼ����Ĭ������λ��
    pwm_init(PWM1_CH1_A8, 50, 650);//��� TIMER1

    //LED�Ƴ�ʼ��
//    gpio_init(B15, GPO, 1, GPIO_PIN_CONFIG); //icm20602 ʱ����ʹ��

    //˫��ͨ�ŷ����

    //����3��ʼ��
    uart_init(UART_3, 921600, UART3_TX_B10, UART3_RX_B11);  //����3��ʼ����������115200
    //������3�ж�
    uart_rx_irq(UART_3, ENABLE);                            //Ĭ����ռ���ȼ�1 �����ȼ�0��
    nvic_init((IRQn_Type)(53 + UART_3), 0, 0, ENABLE);      //������3����ռ���ȼ�����Ϊ��ߣ������ȼ�����Ϊ��ߡ�
    //���ⲿ�ж�
    gpio_interrupt_init(B2, RISING, GPIO_INT_CONFIG);       //B2��ʼ��ΪGPIO �����ش���
    nvic_init(EXTI2_IRQn, 1, 1, ENABLE);                    //EXTI2���ȼ����ã���ռ���ȼ�1�������ȼ�1

//    ips114_showstr(0, 6, "Master Init OK!");
    while(1)
    {
        rt_thread_mdelay(10);//new delay
//        gpio_toggle(B15);
        Tcp_Decode();
        sendMessage();
    }
}
