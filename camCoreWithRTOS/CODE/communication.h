/*
 * communication.h
 *
 *  Created on: Jun 30, 2021
 *      Author: 29275
 */

#ifndef CODE_COMMUNICATION_H_
#define CODE_COMMUNICATION_H_

#include "headfile.h"

#define LINE_LEN                15              //数据长度
#define GyroLINE_LEN             5

extern uint8 temp_buff[LINE_LEN];
extern int16 encoder_left_front,encoder_left_rear;
extern int16 left,right;
extern uint8 Gyro_buff[5];
extern int16 gyroData;//陀螺仪数据

void encoder_init(void);
void get_sensor_data(void);
void process_data(void);
void gyroData_analysis(uint8 *line);

#endif /* CODE_COMMUNICATION_H_ */
