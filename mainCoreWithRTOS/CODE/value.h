#ifndef CODE_VALUE_H_
#define CODE_VALUE_H_

#include "headfile.h"

extern int16 g_fGravityAngle_z,g_fGyroAngleSpeed_z;
extern int16 position_front;

extern uint8 car_flag;
extern uint8 carFlagPre;

extern int16 expected_omega;
extern int32 expected_y;
extern int16 manual_y;
extern int16 manual_z;


//速度环PID参数 8000限幅
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

extern int16 Left_rear;
extern int16 Left_front;
extern int16 Right_front;
extern int16 Right_rear;

//陀螺仪角速率积分
extern int32 total_z;
////舵机输出占空比
//extern uint16 duty;
//串口接收缓冲区
extern uint8 esp8266_buf[64];
//串口接收计数变量
extern uint16 esp8266_cnt;
//摄像头数据回传标志位
extern uint8 CameraShow_flag;

extern uint8 go_forward;
extern uint8 go_backward;
extern uint8 go_right;
extern uint8 go_left;

extern uint8 elementFlag;
extern uint8 accelerate;
extern uint8 ThreeWayIntersection;
extern uint8 garage;

extern uint8 pidModel;
extern rt_tick_t timet1;
extern rt_tick_t timet2;
extern rt_tick_t timeControl;
#endif /* CODE_VALUE_H_ */
