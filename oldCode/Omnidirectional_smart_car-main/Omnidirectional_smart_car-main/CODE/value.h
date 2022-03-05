/*
 * value.h
 *
 *  Created on: Jan 14, 2021
 *      Author: xm2000
 */
#ifndef CODE_VALUE_H_
#define CODE_VALUE_H_

#include "common.h"
#include "pid.h"

extern uint32 runtime;
extern uint8 runtime_switch;
extern uint32 ms_clock;
extern uint8 ms_clock_switch;

extern uint16 five_mstick;
extern uint16 fivehund_mstick;
extern uint16 key_tick;
extern uint32 systick_time;
//速度环PID参数
extern float S_P;
extern float S_I;
extern float S_D;

extern float T_P;
extern float T_I;
extern float T_D;

extern float T2_P;
extern float T2_I;
extern float T2_D;

//PWM限幅
extern uint16 limit_pwm;

//pid结构体
extern PID motor1_pid;
extern PID motor2_pid;
extern PID motor3_pid;
extern PID motor4_pid;

//编码器数值
extern int16 encoder_data[4];


//陀螺仪Z轴数据
extern float g_fGyroAngleSpeed_x;
extern float g_fGyroAngleSpeed_y;
extern float g_fGyroAngleSpeed_z;
extern float g_fGravityAngle_x;
extern float g_fGravityAngle_y;
extern float g_fGravityAngle_z;

//每个轮子的目标速度
extern int16 Left_rear;
extern int16 Left_front;
extern int16 Right_front;
extern int16 Right_rear;
extern uint32 dx,dy,dz,dist;
extern int16_t enconder_input;
extern uint8 car_flag;
extern uint16 adc_value[30];
extern int16 ad[6];
extern int16 ad_max[6];
extern int16 ad_min[6];
extern int16 ad_real[6];
extern int16 position[2];
extern int16 ad_sum;
extern uint8 rx_buf[100];
extern uint8 rx_cnt;

extern uint8 rx_num[10];
extern int8 direction;
extern int16 angle;
extern int16 last_angle;
extern int16 basic_speed;

extern PID_Dir turn_pid;
extern uint8 key_cnt;

extern uint32 write_buff[10];
extern uint32 read_buff[10];

extern uint8 car_mode;
extern uint8 sensor_state;
extern uint8 start_flag;
//for show
extern uint8 island_flag;
extern uint8 island_finish;
extern int16 ad_dif0;
extern int16 ad_dif1;
extern int16 ad_dif2;
extern int16 ad_dif3;
extern int16 ad_sum2;
extern int16 ad_sum3;
extern int16 island_sum;
extern int16 island_need;
extern int basic_data;
extern uint32 distance_real;
#endif /* CODE_VALUE_H_ */
