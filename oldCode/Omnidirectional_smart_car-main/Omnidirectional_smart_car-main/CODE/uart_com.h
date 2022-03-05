/*
 * spi_glu.h
 *
 *  Created on: Jan 24, 2021
 *      Author: xm2000
 */

#ifndef CODE_UART_COM_H_
#define CODE_UART_COM_H_

#include "common.h"
//表述数据长度
#define LINE_LEN                11      //数据长度

#define E_START                 0       //准备状态
#define E_OK                    1       //成功
#define E_FRAME_HEADER_ERROR    2       //帧头错误
#define E_FRAME_RTAIL_ERROR     3       //帧尾错误

extern vuint8  uart_flag;                   //接收数据标志位
extern vuint8  hmi_flag;
extern uint8   temp_buff[LINE_LEN];             //主机用于接收数据的BUFF
extern uint8   receiveBuffer[10];
extern uint8   num_flag;


void get_slave_data(uint8 data);
void data_analysis(uint8 *line);
void HMI_Send_Data(char* txt);
void get_hmi_data(uint8 data);
void analysis_hmi_data(uint8 data);


#endif /* CODE_UART_COM_H_ */
