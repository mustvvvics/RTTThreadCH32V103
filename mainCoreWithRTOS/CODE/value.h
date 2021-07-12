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

//����ֱ���ٶȺͽ��ٶ�
extern int16 expected_omega;
extern int32 expected_y;//ת����������28 53cm OMEGA�ٶ� 80    OMEGA 195 Y�� 90
extern int16 manual_y;
extern int16 manual_z;
extern int32 dx,dy,dz,dist;
//extern uint8 feedBadDog;

//�ٶȻ�PID����
//8000�޷�
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


//�ĸ����ӵ�Ŀ���ٶ�ֵ
extern int16 Left_rear;
extern int16 Left_front;
extern int16 Right_front;
extern int16 Right_rear;


//�����ǽ����ʻ���
extern int32 total_z;
//������ռ�ձ�
extern uint16 duty;
//���ڽ��ջ�����
extern uint8 esp8266_buf[64];
//���ڽ��ռ�������
extern uint16 esp8266_cnt;
//IP��ַ���ձ�־λ
extern uint8 IP_Start_Rec;
//����ͷ���ݻش���־λ
extern uint8 CameraShow_flag;

extern uint8 count_en;
extern uint8 go_forward;
extern uint8 go_backward;
extern uint8 go_right;
extern uint8 go_left;

extern uint8 elementFlag;

#endif /* CODE_VALUE_H_ */
