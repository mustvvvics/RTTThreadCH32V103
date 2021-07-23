/*
 * value.c
 *
 *  Created on: Jun 9, 2021
 *      Author: 29275
 */
#include "headfile.h"

//串口接收缓冲区
uint8 esp8266_buf[64];
//串口接收计数变量
uint16 esp8266_cnt=0;
uint8 CameraShow_flag=0;

rt_tick_t timet1 = 0;           //计算运行时间
rt_tick_t timet2 = 0;
rt_tick_t timeControl = 0;

int8 turnPage = 0;
uint8 clearCamFlags = 0;
