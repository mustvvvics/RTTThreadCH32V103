/*
 * spi_glu.h
 *
 *  Created on: Jan 24, 2021
 *      Author: xm2000
 */

#ifndef CODE_UART_COM_H_
#define CODE_UART_COM_H_

#include "common.h"
//�������ݳ���
#define LINE_LEN                11      //���ݳ���

#define E_START                 0       //׼��״̬
#define E_OK                    1       //�ɹ�
#define E_FRAME_HEADER_ERROR    2       //֡ͷ����
#define E_FRAME_RTAIL_ERROR     3       //֡β����

extern vuint8  uart_flag;                   //�������ݱ�־λ
extern vuint8  hmi_flag;
extern uint8   temp_buff[LINE_LEN];             //�������ڽ������ݵ�BUFF
extern uint8   receiveBuffer[10];
extern uint8   num_flag;


void get_slave_data(uint8 data);
void data_analysis(uint8 *line);
void HMI_Send_Data(char* txt);
void get_hmi_data(uint8 data);
void analysis_hmi_data(uint8 data);


#endif /* CODE_UART_COM_H_ */
