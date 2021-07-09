/*
 * value.h
 *
 *  Created on: Jun 9, 2021
 *      Author: 29275
 */

#ifndef CODE_VALUE_H_
#define CODE_VALUE_H_

#include "headfile.h"



//串口接收缓冲区
extern uint8 esp8266_buf[64];
//串口接收计数变量
extern uint16 esp8266_cnt;
//IP地址接收标志位
extern uint8 IP_Start_Rec;
//摄像头数据回传标志位
extern uint8 CameraShow_flag;


#endif /* CODE_VALUE_H_ */
