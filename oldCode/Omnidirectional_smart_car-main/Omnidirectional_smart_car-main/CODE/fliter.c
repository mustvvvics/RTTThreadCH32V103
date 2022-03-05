/*
 * fliter.c
 *
 *  Created on: Jan 13, 2021
 *      Author: xm2000
 */
#include "headfile.h"
#include <math.h>

void kalman1_init(kalman1_state *state, float init_x, float init_p,float init_q,float init_r)
{
    state->x = init_x;
    state->p = init_p;
    state->A = 1;
    state->H = 1;
    state->q = init_q;//2e2;//10e-6;  /* predict noise convariance */
    state->r = init_r;//5e2;//10e-5;  /* measure error convariance */
}


float kalman1_filter(kalman1_state *state, float z_measure)
{
    /* Predict 先验*/
    state->x = state->A * state->x;
    state->p = state->A * state->A * state->p + state->q;  /* p(n|n-1)=A^2*p(n-1|n-1)+q */

    /* Measurement 后验*/
    state->gain = state->p * state->H / (state->p * state->H * state->H + state->r);
    state->x = state->x + state->gain * (z_measure - state->H * state->x);
    state->p = (1 - state->gain * state->H) * state->p;

    return state->x;
}

kalman1_state gyro_z_kalman;

//icm20602数据解算
void icm_analysis(void)
{
    //加速度
    g_fGravityAngle_x = atan2(icm_acc_y/16384.0,icm_acc_z/16384.0) * 180.0 / 3.14;
    g_fGravityAngle_y = atan2(icm_acc_x/16384.0,icm_acc_z/16384.0) * 180.0 / 3.14;
    g_fGravityAngle_z = atan2(icm_acc_x/16384.0,icm_acc_y/16384.0) * 180.0 / 3.14;

    //角速度
    icm_gyro_x  = icm_gyro_x / 16.4;  //计算角速度值
    g_fGyroAngleSpeed_x = icm_gyro_x;
    icm_gyro_y  = icm_gyro_y / 16.4;  //计算角速度值
    g_fGyroAngleSpeed_y = icm_gyro_y;
    icm_gyro_z  = icm_gyro_z / 16.4;  //计算角速度值
    g_fGyroAngleSpeed_z = icm_gyro_z;

}


void kalman_all_init(void)
{
    kalman1_init(&gyro_z_kalman,0,200,200,200);
}

//gpio_init(C10, GPI, 0, GPIO_INT_CONFIG);
//gpio_init(C11, GPI, 0, GPIO_INT_CONFIG);
//gpio_init(C12, GPI, 0, GPIO_INT_CONFIG);
//gpio_init(C7, GPI, 0, GPIO_INT_CONFIG);
//
//gpio_init(C0, GPI, 0, GPIO_INT_CONFIG);
//gpio_init(C1, GPI, 0, GPIO_INT_CONFIG);
//gpio_init(C2, GPI, 0, GPIO_INT_CONFIG);
//gpio_init(C3, GPI, 0, GPIO_INT_CONFIG);
uint8_t read_signal(uint8 mode)
{
  uint8_t state=0;

  //头
  //左->右
  if(mode == 0)
  {
      if(gpio_get(C10) == 1)
        state |= 0x01;
      if(gpio_get(C12) == 1)
        state |= 0x02;
      if(gpio_get(C11) == 1)
        state |= 0x04;
      if(gpio_get(C7) == 1)
        state |= 0x08;
  }
  else
  {
      //尾
      //1->4
       if(gpio_get(C1) == 1)
        state |= 0x10;
       if(gpio_get(C2) == 1)
        state |= 0x20;
       if(gpio_get(C3) == 1)
        state |= 0x40;
       if(gpio_get(C0) == 1)
        state |= 0x80;
  }

   return state;
}
