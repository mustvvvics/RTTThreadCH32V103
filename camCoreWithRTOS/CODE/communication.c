/*
 * communication.c
 *
 *  Created on: Jun 30, 2021
 *      Author: 29275
 */
#include "headfile.h"


uint8 temp_buff[LINE_LEN];                      //�ӻ���������������BUFF
int16 encoder_left_front,encoder_left_rear;

//int ABS(int x){
//    return x>0?x:-x;
//}

void encoder_init(void)
{
    timer_quad_init(TIMER_2, TIMER2_CHA_A15, TIMER2_CHB_B3);
    timer_quad_init(TIMER_3, TIMER3_CHA_B4, TIMER3_CHB_B5);
}

void get_sensor_data(void)
{
    encoder_left_front = timer_quad_get(TIMER_3);
    timer_quad_clear(TIMER_3);                      //��ռ����� ����1024

    //������ֵ
    encoder_left_rear = timer_quad_get(TIMER_2);   //��ռ����� ����1024
    //��ռ�����
    timer_quad_clear(TIMER_2);
}
int16 slave_position=0;
void process_data(void)
{
    slave_position = cameraError;//�������

    temp_buff[0] = 0xD8;                         //֡ͷ

    temp_buff[1] = 0xB0;                         //������
    temp_buff[2] = encoder_left_front>>8;        //���ݸ�8λ
    temp_buff[3] = encoder_left_front&0xFF;      //���ݵ�8λ

    temp_buff[4] = 0xB1;                         //������
    temp_buff[5] = encoder_left_rear>>8;       //���ݸ�8λ
    temp_buff[6] = encoder_left_rear&0xFF;     //���ݵ�8λ

    temp_buff[7] = 0xB2;                         //������
    temp_buff[8] = slave_position>>8;            //���ݸ�8λ
    temp_buff[9] = slave_position&0xFF;          //���ݵ�8λ

    temp_buff[10] = 0xEE;                        //֡β
}

