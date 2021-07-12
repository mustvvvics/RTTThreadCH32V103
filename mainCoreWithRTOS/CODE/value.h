/*
 * value.h
 *
 *  Created on: Jun 9, 2021
 *      Author: 29275
 */

#ifndef CODE_VALUE_H_
#define CODE_VALUE_H_

#include "headfile.h"

//extern uint16 fivems_tick;
//extern uint16 fives_tick;
extern int16 g_fGravityAngle_z,g_fGyroAngleSpeed_z;
extern int16 position_front;

extern uint8 car_flag;

//期望直线速度和角速度
extern int16 expected_omega;
extern int32 expected_y;//转个环岛试试28 53cm OMEGA速度 80    OMEGA 195 Y轴 90
extern int16 manual_y;
extern int16 manual_z;
extern int32 dx,dy,dz,dist;
//extern uint8 feedBadDog;

//速度环PID参数
//8000限幅
extern float S_P;
extern float S_I;
extern float S_D;

//角速度环参数
extern float yaw_w_P;
extern float yaw_w_I;//0.01
extern float yaw_w_D;//0.15

//待整定 转向环参数
extern float yaw_P;
extern float yaw_I;
extern float yaw_D;

//电机PWM限幅
extern uint16 limit_pwm;

//速度环PID参数
extern PID motor1_pid;
extern PID motor2_pid;
extern PID motor3_pid;
extern PID motor4_pid;
//角速度环参数
extern PID yaw_w_pid;
extern PID_LocTypeDef yaw_pid;


//四个轮子的目标速度值
extern int16 Left_rear;
extern int16 Left_front;
extern int16 Right_front;
extern int16 Right_rear;


//陀螺仪角速率积分
extern int32 total_z;
//舵机输出占空比
extern uint16 duty;
//串口接收缓冲区
extern uint8 esp8266_buf[64];
//串口接收计数变量
extern uint16 esp8266_cnt;
//IP地址接收标志位
extern uint8 IP_Start_Rec;
//摄像头数据回传标志位
extern uint8 CameraShow_flag;

extern uint8 count_en;
extern uint8 go_forward;
extern uint8 go_backward;
extern uint8 go_right;
extern uint8 go_left;

extern uint8 elementFlag;

#endif /* CODE_VALUE_H_ */
