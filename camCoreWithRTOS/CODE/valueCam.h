/*
 * value.h
 *
 *  Created on: Jun 9, 2021
 *      Author: 29275
 */

#ifndef CODE_VALUECAM_H_
#define CODE_VALUECAM_H_

#include "headfile.h"



//���ڽ��ջ�����
extern uint8 esp8266_buf[64];
//���ڽ��ռ�������
extern uint16 esp8266_cnt;
//����ͷ���ݻش���־λ
extern uint8 CameraShow_flag;

extern rt_tick_t timet1;           //��������ʱ��
extern rt_tick_t timet2;
extern rt_tick_t timeControl;
extern int8 turnPage;
extern uint8 clearCamFlags;

#endif /* CODE_VALUECAM_H_ */
