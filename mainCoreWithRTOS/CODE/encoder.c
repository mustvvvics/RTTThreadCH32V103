#include "headfile.h"

int16 encoder_data[4];

int16 ABS(int16 x){
    return x>0?x:-x;
}

void clearError(void){  //清空误差
    expected_omega = 0;
    position_front = 0;
    g_fGyroAngleSpeed_z = 0;
//    expected_y = 0;
    speed_conversion(0,0,0);
    yaw_pid.LocSum = 0;
    yaw_pid.Ek = 0;
    yaw_pid.Ek1 = 0;
    yaw_w_pid.target_val = 0;
    yaw_w_pid.err_next = 0;
    yaw_w_pid.err = 0;
    yaw_w_pid.err_last = 0;
    yaw_w_pid.actual_val = 0;
    manual_y=0;manual_z=0;
    go_forward=0;go_backward=0;
    go_left=0;go_right=0;
//    pidModel = 0;
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
    timer_quad_clear(TIMER_3);                          //清空计数器
    if(gpio_get(B3))
        encoder_data[1] = -ABS(timer_quad_get(TIMER_2));
    else
        encoder_data[1] = ABS(timer_quad_get(TIMER_2));
    timer_quad_clear(TIMER_2);                          //清空计数器
}
