/*
 * fliter.h
 *
 *  Created on: Jan 13, 2021
 *      Author: xm2000
 */

#ifndef CODE_FLITER_H_
#define CODE_FLITER_H_

/* 1 Dimension */
typedef struct {
    float x;  /* state */
    float A;  /* x(n)=A*x(n-1)+u(n),u(n)~N(0,q) */
    float H;  /* z(n)=H*x(n)+w(n),w(n)~N(0,r)   */
    float q;  /* process(predict) noise convariance */
    float r;  /* measure noise convariance */
    float p;  /* estimated error convariance */
    float gain;
} kalman1_state;

extern kalman1_state gyro_z_kalman;

void kalman1_init(kalman1_state *state, float init_x, float init_p,float init_q,float init_r);
float kalman1_filter(kalman1_state *state, float z_measure);
void icm_analysis(void);
uint8_t read_signal(uint8 mode);

#endif /* CODE_FLITER_H_ */
