/*
 * value.c
 *
 *  Created on: Jun 9, 2021
 *      Author: 29275
 */
#include "headfile.h"

//���ڽ��ջ�����
uint8 esp8266_buf[64];
//���ڽ��ռ�������
uint16 esp8266_cnt=0;
uint8 CameraShow_flag=0;

rt_tick_t timet1 = 0;           //��������ʱ��
rt_tick_t timet2 = 0;
rt_tick_t timeControl = 0;

int8 turnPage = 0;
uint8 clearCamFlags = 0;
