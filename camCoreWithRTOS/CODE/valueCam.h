/*
 * value.h
 *
 *  Created on: Jun 9, 2021
 *      Author: 29275
 */

#ifndef CODE_VALUECAM_H_
#define CODE_VALUECAM_H_

#include "headfile.h"



//串口接收缓冲区
extern uint8 esp8266_buf[64];
//串口接收计数变量
extern uint16 esp8266_cnt;
//摄像头数据回传标志位
extern uint8 CameraShow_flag;

extern rt_tick_t timet1;           //计算运行时间
extern rt_tick_t timet2;
extern rt_tick_t timeControl;
extern int8 turnPage;
extern uint8 clearCamFlags;

#endif /* CODE_VALUECAM_H_ */
