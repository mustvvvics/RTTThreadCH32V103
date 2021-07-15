#ifndef _adc_h
#define _adc_h

#include "headfile.h"

//extern uint16_t temp_ADC_Value;
extern float vbat_ad;

void DMA_Tx_Init( DMA_Channel_TypeDef* DMA_CHx, u32 ppadr, u32 memadr, u16 bufsize);
void ADC_Function_Init(void);
void getAdc();


#endif
