/*
 * communication.h
 *
 *  Created on: Jun 30, 2021
 *      Author: 29275
 */

#ifndef CODE_COMMUNICATION_H_
#define CODE_COMMUNICATION_H_

#include "common.h"

#define LINE_LEN                11              //���ݳ���
extern uint8 temp_buff[LINE_LEN];
extern int16 encoder_left_front,encoder_left_rear;

void encoder_init(void);
void get_sensor_data(void);
void process_data(void);

#endif /* CODE_COMMUNICATION_H_ */
