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


//�ٶȻ�PID���� 8000�޷�
extern float S_P;
extern float S_I;
extern float S_D;

//���ٶȻ�����
extern float yaw_w_P;
extern float yaw_w_I;//0.01
extern float yaw_w_D;//0.15

//������ ת�򻷲���
extern float yaw_P;
extern float yaw_I;
extern float yaw_D;

//���PWM�޷�
extern uint16 limit_pwm;

//�ٶȻ�PID����
extern PID motor1_pid;
extern PID motor2_pid;
extern PID motor3_pid;
extern PID motor4_pid;
//���ٶȻ�����
extern PID yaw_w_pid;
extern PID_LocTypeDef yaw_pid;

extern int16 Left_rear;
extern int16 Left_front;
extern int16 Right_front;
extern int16 Right_rear;

//�����ǽ����ʻ���
extern int32 total_z;
////������ռ�ձ�
//extern uint16 duty;
//���ڽ��ջ�����
extern uint8 esp8266_buf[64];
//���ڽ��ռ�������
extern uint16 esp8266_cnt;
//����ͷ���ݻش���־λ
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
