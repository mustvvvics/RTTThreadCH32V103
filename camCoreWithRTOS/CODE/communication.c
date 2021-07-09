/*
 * communication.c
 *
 *  Created on: Jun 30, 2021
 *      Author: 29275
 */
#include "headfile.h"


uint8 temp_buff[LINE_LEN];                      //从机向主机发送数据BUFF
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
    timer_quad_clear(TIMER_3);                      //清空计数器 对于1024

    //读计数值
    encoder_left_rear = timer_quad_get(TIMER_2);   //清空计数器 对于1024
    //清空计数器
    timer_quad_clear(TIMER_2);
}
int16 slave_position=0;
void process_data(void)
{
    slave_position = cameraError;//传递误差

    temp_buff[0] = 0xD8;                         //帧头

    temp_buff[1] = 0xB0;                         //功能字
    temp_buff[2] = encoder_left_front>>8;        //数据高8位
    temp_buff[3] = encoder_left_front&0xFF;      //数据低8位

    temp_buff[4] = 0xB1;                         //功能字
    temp_buff[5] = encoder_left_rear>>8;       //数据高8位
    temp_buff[6] = encoder_left_rear&0xFF;     //数据低8位

    temp_buff[7] = 0xB2;                         //功能字
    temp_buff[8] = slave_position>>8;            //数据高8位
    temp_buff[9] = slave_position&0xFF;          //数据低8位

    temp_buff[10] = 0xEE;                        //帧尾
}

