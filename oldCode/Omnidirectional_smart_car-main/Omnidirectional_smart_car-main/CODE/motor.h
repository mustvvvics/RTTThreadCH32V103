/*
 * motor.h
 *
 *  Created on: Jan 13, 2021
 *      Author: xm2000
 */

#ifndef CODE_MOTOR_H_
#define CODE_MOTOR_H_

#include "headfile.h"


#define MOTOR1_A   B10              //����1�������ת����
#define MOTOR1_B   PWM4_CH3_B8      //����1���PWM����

#define MOTOR2_A   B11              //����2�������ת����
#define MOTOR2_B   PWM4_CH4_B9      //����2���PWM����

#define MOTOR3_A   A15              //����3�������ת����
#define MOTOR3_B   PWM4_CH1_B6      //����3���PWM����

#define MOTOR4_A   B3               //����4�������ת����
#define MOTOR4_B   PWM4_CH2_B7      //����4���PWM����


void motor1_ctl(int32 speed1_power);
void motor2_ctl(int32 speed2_power);
void motor3_ctl(int32 speed3_power);
void motor4_ctl(int32 speed4_power);
void speed_conversion(double Vx, double Vy, int Vz);


#endif /* CODE_MOTOR_H_ */
