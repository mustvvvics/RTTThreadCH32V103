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

extern float S_P;
extern float S_I;
extern float S_D;

extern float yaw_w_P;
extern float yaw_w_I;//0.01
extern float yaw_w_D;//0.15

extern float yaw_P;
extern float yaw_I;
extern float yaw_D;

extern uint16 limit_pwm;

extern PID motor1_pid;
extern PID motor2_pid;
extern PID motor3_pid;
extern PID motor4_pid;

extern PID yaw_w_pid;
extern PID_LocTypeDef yaw_pid;

extern int16 Left_rear;
extern int16 Left_front;
extern int16 Right_front;
extern int16 Right_rear;

extern float leftRearADRC;
extern float leftFrontADRC;
extern float rightFrontADRC;
extern float rightRearADRC;


extern int32 total_z;

extern uint8 esp8266_buf[64];

extern uint16 esp8266_cnt;

extern uint8 CameraShow_flag;

extern uint8 go_forward;
extern uint8 go_backward;
extern uint8 go_right;
extern uint8 go_left;

extern uint8 elementFlag;
extern uint8 accelerate;
extern uint8 ThreeWayIntersectionFlag;
extern uint8 garage;
extern uint8 roundFinishFlag;
extern uint8 roundIslandBegin;

extern uint8 pidModel;
extern rt_tick_t timet1;
extern rt_tick_t timet2;
extern rt_tick_t timeControl;

extern int16 motor1_pwm,motor2_pwm,motor3_pwm,motor4_pwm;
extern int16 real_x,real_y;
#endif /* CODE_VALUE_H_ */
