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
//IP地址接收标志位
uint8 IP_Start_Rec=0;
uint8 CameraShow_flag=0;
