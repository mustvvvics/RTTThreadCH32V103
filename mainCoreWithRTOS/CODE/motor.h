#ifndef _motor_h
#define _motor_h

#include "headfile.h"

//���Ͽ�ʼ ˳ʱ�� 1 2 3 4
#define MOTOR4_A   B12              //����1�������ת����
#define MOTOR4_B   PWM4_CH3_B8      //����1���PWM����

#define MOTOR1_A   C12              //����4�������ת����
#define MOTOR1_B   PWM4_CH4_B9      //����4���PWM����

#define MOTOR3_A   C10              //����3�������ת����
#define MOTOR3_B   PWM4_CH1_B6      //����3���PWM����

#define MOTOR2_A   C11               //����2�������ת����
#define MOTOR2_B   PWM4_CH2_B7      //����2���PWM����


void motor_init(void);
void motor1_ctl(int32 speed1_power);
void motor2_ctl(int32 speed2_power);
void motor3_ctl(int32 speed3_power);
void motor4_ctl(int32 speed4_power);

void speed_conversion(float Vx, float Vy, float Vz);
void testSideAdvancementAbility(void);

#endif
