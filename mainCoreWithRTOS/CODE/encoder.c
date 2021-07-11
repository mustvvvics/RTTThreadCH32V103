#include "headfile.h"


int16 encoder_data[4];

int16 ABS(int16 x){
    return x>0?x:-x;
}

void encoder_init(void)
{
    timer_quad_init(TIMER_2, TIMER2_CHA_A15, TIMER2_CHB_B3);
    timer_quad_init(TIMER_3, TIMER3_CHA_B4, TIMER3_CHB_B5);
}


void encoder_get(void) //in isr.c :void EXTI2_IRQHandler(void)
{

    //获取编码器数据
    if(gpio_get(B5))
        encoder_data[0] = ABS(timer_quad_get(TIMER_3));
    else
        encoder_data[0] = -ABS(timer_quad_get(TIMER_3));

    timer_quad_clear(TIMER_3);                      //清空计数器

    //读计数值
    if(gpio_get(B3))
        encoder_data[1] = -ABS(timer_quad_get(TIMER_2));
    else
        encoder_data[1] = ABS(timer_quad_get(TIMER_2));

    //清空计数器
    timer_quad_clear(TIMER_2);
}
