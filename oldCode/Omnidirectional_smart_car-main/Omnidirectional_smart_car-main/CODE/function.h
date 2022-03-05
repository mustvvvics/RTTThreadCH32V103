/*
 * function.h
 *
 *  Created on: Jan 23, 2021
 *      Author: xm2000
 */

#ifndef CODE_FUNCTION_H_
#define CODE_FUNCTION_H_

//常用外设引脚
#define BEEP_PIN   D2       //定义蜂鸣器引脚
#define S_MOTOR_PIN   PWM1_CH4_A11       //定义舵机引脚

void Peripherals_Init(void);
void vision_pc(void);
void show_speed(void);
void vision_screen(void);
void ADC_Function_Init(void);
void DMA_Tx_Init( DMA_Channel_TypeDef* DMA_CHx, u32 ppadr, u32 memadr, u16 bufsize);
void show_ad(void);
int16 adc_mean(uint8 n);
void ad_calculate(void);
void show_simple(void);
void backtoreal(void);
void show_car(void);
void show_time(void);
void ESP8266_Init(void);

#endif /* CODE_FUNCTION_H_ */
