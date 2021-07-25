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

rt_sem_t camera_sem;

int main(void)
{
    camera_sem = rt_sem_create("camera", 0, RT_IPC_FLAG_FIFO);
    mt9v03x_init();
    rt_thread_mdelay(3000);                 //��֤����ͷ��ʼ�����

    laneInit();

    display_init();
    encoder_init();
    timer_pit_init();                       //��ʱ����ʼ��
    gpio_init(D2,GPO,0,OUT_PP); //������


    uart_init(UART_3,921600,UART3_TX_B10,UART3_RX_B11);     //ͨѶ

    uart_rx_irq(UART_3, ENABLE);                            //Ĭ����ռ���ȼ�1 �����ȼ�0��
    nvic_init((IRQn_Type)(53 + UART_3), 1, 1, ENABLE);      //������3����ռ���ȼ�����Ϊ��ߣ������ȼ�����Ϊ��ߡ�

    gpio_init(B12, GPO, 0, GPIO_PIN_CONFIG);                 //ͬ�����ų�ʼ�� time_pit
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
        //�ȴ�����ͷ�ɼ����
        rt_sem_take(camera_sem, RT_WAITING_FOREVER);
        //��ʼ��������ͷͼ��
        rt_thread_mdelay(10);
        timet1 = rt_tick_get();
        laneAnalyze(mt9v03x_image);
        computeError();
        timet2 = rt_tick_get();
        timeControl = timet2 - timet1;
//        sendMessageData();
    }
}
