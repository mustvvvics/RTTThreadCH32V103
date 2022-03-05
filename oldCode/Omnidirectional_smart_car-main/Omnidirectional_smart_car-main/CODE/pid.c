/*
 * pid.c
 *
 *  Created on: Jan 22, 2021
 *      Author: xm2000
 */
#include "pid.h"
#include "value.h"


void PID_Init(void)
{
    motor1_pid.target_val=0;
    motor1_pid.actual_val=0.0;
    motor1_pid.err = 0.0;
    motor1_pid.err_last = 0.0;
    motor1_pid.err_next = 0.0;

    motor2_pid.target_val=0;
    motor2_pid.actual_val=0.0;
    motor2_pid.err = 0.0;
    motor2_pid.err_last = 0.0;
    motor2_pid.err_next = 0.0;

    motor3_pid.target_val=0;
    motor3_pid.actual_val=0.0;
    motor3_pid.err = 0.0;
    motor3_pid.err_last = 0.0;
    motor3_pid.err_next = 0.0;

    motor4_pid.target_val=0;
    motor4_pid.actual_val=0.0;
    motor4_pid.err = 0.0;
    motor4_pid.err_last = 0.0;
    motor4_pid.err_next = 0.0;

    turn_pid.target_val=0;
    turn_pid.actual_val=0.0;
    turn_pid.err = 0.0;
    turn_pid.err_last = 0.0;
    turn_pid.err_next = 0.0;

    motor1_pid.Kp = S_P;
    motor1_pid.Ki = S_I;
    motor1_pid.Kd = S_D;

    motor2_pid.Kp = S_P;
    motor2_pid.Ki = S_I;
    motor2_pid.Kd = S_D;

    motor3_pid.Kp = S_P;
    motor3_pid.Ki = S_I;
    motor3_pid.Kd = S_D;

    motor4_pid.Kp = S_P;
    motor4_pid.Ki = S_I;
    motor4_pid.Kd = S_D;

    turn_pid.Kp = T_P;
    turn_pid.Kd = T_D;

    turn_pid.Kp2 = T2_P;
    turn_pid.Kd2 = T2_D;
}


//速度环PID
float PID_Speed(float target_val,float actual_val,PID *pid)
{
    pid->err=target_val-actual_val;

    pid->actual_val += pid->Kp*(pid->err - pid->err_next)
                 + pid->Ki*pid->err
                 + pid->Kd*(pid->err - 2 * pid->err_next + pid->err_last);

    //pid限幅
    if(pid->actual_val > limit_pwm) pid->actual_val = limit_pwm;
    else if(pid->actual_val < -limit_pwm) pid->actual_val = -limit_pwm;

    pid->err_last = pid->err_next;
    pid->err_next = pid->err;

    return pid->actual_val;
}

//转向环PD
//方向环PD
//PID_Dir dire_pid;
float direction_pid(int x,PID_Dir *pid,int rank)
{
    //偏差方向 与打角放向相反
    pid->err=x;

    if(rank == 0)
    {
        pid->actual_val = pid->Kp*pid->err + pid->Kd*(pid->err - pid->err_next);
    }
    else if (rank == 1) {
        pid->actual_val = pid->Kp2*pid->err + pid->Kd2*(pid->err - pid->err_next);
    }

    //保存偏差数据
    pid->err_next = pid->err;

    //yaw角转向限幅，限幅511/0.18
    //编码器速度上限，-511~511，yaw轴转向限幅
    if(pid->actual_val > 300) pid->actual_val = 300;
    else if(pid->actual_val < -300) pid->actual_val = -300;

    return pid->actual_val;
}




