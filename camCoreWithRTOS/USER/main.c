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


rt_sem_t camera_sem;

int main(void)
{
    camera_sem = rt_sem_create("camera", 0, RT_IPC_FLAG_FIFO);
    mt9v03x_init();
    rt_thread_mdelay(3000);//��֤����ͷ��ʼ�����

    laneInit();
    display_init();
    encoder_init();
    timer_pit_init();//��ʱ����ʼ��
//
//    esp8266Init();

    gpio_init(B12, GPO, 0, GPIO_PIN_CONFIG);                 //ͬ�����ų�ʼ�� time_pit
    uart_init(UART_3,921600,UART3_TX_B10,UART3_RX_B11);//ͨѶ
    gpio_init(B15, GPO, 0, GPIO_PIN_CONFIG);

    while(1)
    {
        //�ȴ�����ͷ�ɼ����
        rt_sem_take(camera_sem, RT_WAITING_FOREVER);
//        rt_thread_mdelay(200);
//        gpio_toggle(B15);
        laneAnalyze(mt9v03x_image);
        computeError();
        //��ʼ��������ͷͼ��
        //mt9v03x_image[0]
    }
}
