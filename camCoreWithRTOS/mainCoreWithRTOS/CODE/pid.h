/*
 * pid.h
 *
 *  Created on: Jan 22, 2021
 *      Author: xm2000
 */

#ifndef CODE_PID_H_
#define CODE_PID_H_

#include "headfile.h"

typedef struct
{
    float target_val;
    float actual_val;
    float err;
    float err_next;
    float err_last;
    float Kp, Ki, Kd;
}PID;

typedef struct
{
    float Kp;
    float Ki;
    float Kd;

    float Ek;
    float Ek1;
    float LocSum;
}PID_LocTypeDef;

void PID_Init(void);
float PID_Speed(float target_val,float actual_val,PID *pid);
float PID_Angle(float target_val,float actual_val,PID *pid);
float PID_Loc(float SetValue,float ActualValue,PID_LocTypeDef *PID);


#endif /* CODE_PID_H_ */
