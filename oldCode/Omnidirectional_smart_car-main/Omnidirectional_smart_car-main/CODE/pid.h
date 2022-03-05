/*
 * pid.h
 *
 *  Created on: Jan 22, 2021
 *      Author: xm2000
 */

#ifndef CODE_PID_H_
#define CODE_PID_H_

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
    float target_val;
    float actual_val;
    float err;
    float err_next;
    float err_last;
    float Kp, Ki, Kd;
    float Kp2, Ki2, Kd2;
}PID_Dir;

void PID_Init(void);
float PID_realize(float target_val,float actual_val,PID *pid);
float PID_Speed(float target_val,float actual_val,PID *pid);
float direction_pid(int x,PID_Dir *pid,int rank);

#endif /* CODE_PID_H_ */
