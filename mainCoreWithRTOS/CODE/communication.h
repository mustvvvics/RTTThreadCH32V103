/*
 * communication.h 主核心
 *
 *  Created on: Jun 30, 2021
 *      Author: 29275
 */

#ifndef CODE_COMMUNICATION_H_
#define CODE_COMMUNICATION_H_

#include "headfile.h"

#define LINE_LEN                15      //数据长度

#define E_START                 0       //准备状态
#define E_OK                    1       //成功
#define E_FRAME_HEADER_ERROR    2       //帧头错误
#define E_FRAME_RTAIL_ERROR     3       //帧尾错误

extern vuint8  uart_flag;                   //接收数据标志位
extern uint8   temp_buff[LINE_LEN];         //主机用于接收数据的BUFF

void get_slave_data(uint8 data);
void data_analysis(uint8 *line);

#endif /* CODE_COMMUNICATION_H_ */
