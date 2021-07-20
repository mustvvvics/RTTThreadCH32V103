/*
 * pid.c
 *
 *  Created on: Jan 22, 2021
 *      Author: xm2000
 */
#include "headfile.h"


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

    yaw_w_pid.target_val=0;
    yaw_w_pid.actual_val=0.0;
    yaw_w_pid.err = 0.0;
    yaw_w_pid.err_last = 0.0;
    yaw_w_pid.err_next = 0.0;

    yaw_pid.Ek=0;
    yaw_pid.Ek1=0;
    yaw_pid.LocSum=0;

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

    yaw_w_pid.Kp = yaw_w_P;
    //yaw_w_pid.Ki = yaw_w_I;
    yaw_w_pid.Kd = yaw_w_D;

    yaw_pid.Kp=yaw_P;
    yaw_pid.Ki=yaw_I;
    yaw_pid.Kd=yaw_D;

}


//速度环PID
float PID_Speed(float target_val,float actual_val,PID *pid)
{
    pid->err=target_val-actual_val;

    pid->actual_val += pid->Kp*(pid->err - pid->err_next)
                 + pid->Ki*pid->err
                 + pid->Kd*(pid->err - 2 * pid->err_next + pid->err_last);

    //限幅
    if(pid->actual_val > limit_pwm) pid->actual_val = limit_pwm;
    else if(pid->actual_val < -limit_pwm) pid->actual_val = -limit_pwm;

    pid->err_last = pid->err_next;
    pid->err_next = pid->err;

    return pid->actual_val;
}

float PID_Angle(float target_val,float actual_val,PID *pid)
{
    pid->err=target_val-actual_val;

    pid->actual_val += pid->Kp*(pid->err - pid->err_next)
                 + pid->Ki*pid->err
                 + pid->Kd*(pid->err - 2 * pid->err_next + pid->err_last);

//    if(pid->err > 50) pid->err = 50;
//    else if(pid->err < -50) pid->err = -50;

    //限幅
    if(pid->actual_val > 1000) pid->actual_val = 1000;
    else if(pid->actual_val < -1000) pid->actual_val = -1000;

    pid->err_last = pid->err_next;
    pid->err_next = pid->err;

    return pid->actual_val;
}


//
float PID_Loc(float SetValue, float ActualValue, PID_LocTypeDef *PID)
{
    float PIDLoc;

    PID->Ek = SetValue - ActualValue;
    PID->LocSum += PID->Ek;

//  PID->LocSum=(PID->LocSum<-1000)?-1000:
//              ((PID->LocSum>1000)?1000:PID->LocSum);

    PIDLoc = PID->Kp * PID->Ek  + PID->Kd * (PID->Ek1 - PID->Ek);
    //+ (PID->Ki * PID->LocSum)
    PID->Ek1 = PID->Ek; return PIDLoc;
}




