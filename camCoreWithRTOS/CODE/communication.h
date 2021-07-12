/*
 * communication.h
 *
 *  Created on: Jun 30, 2021
 *      Author: 29275
 */

#ifndef CODE_COMMUNICATION_H_
#define CODE_COMMUNICATION_H_

#include "headfile.h"

#define LINE_LEN                16              //数据长度
extern uint8 temp_buff[LINE_LEN];
extern int16 encoder_left_front,encoder_left_rear;
extern int16 left,right;
extern uint8 accelerate;
extern uint8 elementFlagCam;

void encoder_init(void);
void get_sensor_data(void);
void process_data(void);

#endif /* CODE_COMMUNICATION_H_ */
