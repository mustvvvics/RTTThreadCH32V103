#ifndef _motor_h
#define _motor_h

#include "headfile.h"

//左上开始 顺时针 1 2 3 4
#define MOTOR4_A   B12              //定义1电机正反转引脚
#define MOTOR4_B   PWM4_CH3_B8      //定义1电机PWM引脚

#define MOTOR1_A   C12              //定义4电机正反转引脚
#define MOTOR1_B   PWM4_CH4_B9      //定义4电机PWM引脚

#define MOTOR3_A   C10              //定义3电机正反转引脚
#define MOTOR3_B   PWM4_CH1_B6      //定义3电机PWM引脚

#define MOTOR2_A   C11               //定义2电机正反转引脚
#define MOTOR2_B   PWM4_CH2_B7      //定义2电机PWM引脚


void motor_init(void);
void motor1_ctl(int32 speed1_power);
void motor2_ctl(int32 speed2_power);
void motor3_ctl(int32 speed3_power);
void motor4_ctl(int32 speed4_power);

void speed_conversion(float Vx, float Vy, float Vz);
void testSideAdvancementAbility(void);

#endif
