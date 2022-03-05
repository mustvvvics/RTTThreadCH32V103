/*
 * value.c
 *
 *  Created on: Jan 14, 2021
 *      Author: xm2000
 */
#include "value.h"

uint32 runtime=0;
uint8 runtime_switch=0;
uint32 ms_clock=0;
uint8 ms_clock_switch=0;

uint16 five_mstick=0;
uint16 fivehund_mstick=0;
uint16 key_tick=0;
uint32 systick_time;
uint32 dx=0,dy=0,dz=0,dist;
//速度环PID参数
//8000限幅
float S_P=136;
float S_I=33;
float S_D=45;

//300限幅
//分段PID
//转向环PID参数
float T_P=0.2;
float T_I=0;
float T_D=0.2;

float T2_P=0.5;
float T2_I=0;
float T2_D=0.2;

uint8 car_flag=0;
uint32 write_buff[10];
uint32 read_buff[10];
//PWM限幅
uint16 limit_pwm=8000;

//pid结构体
PID motor1_pid;
PID motor2_pid;
PID motor3_pid;
PID motor4_pid;

PID_Dir turn_pid;

//编码器数值
int16 encoder_data[4];

//目标速度值
int16 Left_rear;
int16 Left_front;
int16 Right_front;
int16 Right_rear;

//加速度计获取的加速度
float g_fGravityAngle_x;
float g_fGravityAngle_y;
float g_fGravityAngle_z;
//陀螺仪获取的角速度
float g_fGyroAngleSpeed_x;
float g_fGyroAngleSpeed_y;
float g_fGyroAngleSpeed_z;

int16_t enconder_input=0;

uint16 adc_value[30];
int16 ad[6];
int16 ad_max[6];
int16 ad_min[6];
int16 position[2];
int16 ad_real[6];
int16 ad_sum;

uint8 rx_buf[100];
uint8 rx_cnt=0;

uint8 rx_num[10];
int8 direction;
int16 angle;
int16 last_angle;
int16 basic_speed=5;//车速
uint8 key_cnt=0;

uint8 car_mode=0;
uint8 sensor_state;
uint8 start_flag=0;
//for show
uint8 island_flag=0;
uint8 island_finish=0;
int16 ad_dif0;
int16 ad_dif1;
int16 ad_dif2;
int16 ad_dif3;
int16 ad_sum2;
int16 ad_sum3;
int16 island_sum;
int16 island_need=5;


int basic_data=425;

uint32 distance_real=0;
