#include "headfile.h"


int16 encoder_data[4];

int ABS(int x){
    return x>0?x:-x;
}

void encoder_init(void)
{
    timer_quad_init(TIMER_2, TIMER2_CHA_A15, TIMER2_CHB_B3);
    timer_quad_init(TIMER_3, TIMER3_CHA_B4, TIMER3_CHB_B5);
}


void encoder_get(void) //in isr.c :void EXTI2_IRQHandler(void)
{

    encoder_data[0] = timer_quad_get(TIMER_3);
    timer_quad_clear(TIMER_3);                      //清空计数器


    encoder_data[1] = timer_quad_get(TIMER_2);
    //清空计数器
    timer_quad_clear(TIMER_2);
}
