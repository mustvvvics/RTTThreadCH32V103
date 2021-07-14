/*
 * communication.h ������
 *
 *  Created on: Jun 30, 2021
 *      Author: 29275
 */

#ifndef CODE_COMMUNICATION_H_
#define CODE_COMMUNICATION_H_

#include "headfile.h"

#define LINE_LEN                15      //���ݳ���

#define E_START                 0       //׼��״̬
#define E_OK                    1       //�ɹ�
#define E_FRAME_HEADER_ERROR    2       //֡ͷ����
#define E_FRAME_RTAIL_ERROR     3       //֡β����

extern vuint8  uart_flag;                   //�������ݱ�־λ
extern uint8   temp_buff[LINE_LEN];         //�������ڽ������ݵ�BUFF

void get_slave_data(uint8 data);
void data_analysis(uint8 *line);

#endif /* CODE_COMMUNICATION_H_ */
