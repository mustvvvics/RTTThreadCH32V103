#include "headfile.h"

uint8 Gyro_buff[5];

void timer1_pit_entry(void *parameter)
{
    //发送陀螺仪数据
    Gyro_buff[0] = 0xD8;                            //帧头

    Gyro_buff[1] = 0xB0;                            //功能字
    Gyro_buff[2] = g_fGyroAngleSpeed_z>>8;          //数据高8位
    Gyro_buff[3] = g_fGyroAngleSpeed_z&0xFF;        //数据低8位

    Gyro_buff[4] = 0xEE;                            //帧尾
    uart_putbuff(UART_3, Gyro_buff, 5);             //通过串口3将数据发送出去。
}


void timer_pit_init(void)
{
    rt_timer_t timer;
    
    //创建一个定时器 周期运行
    timer = rt_timer_create("timer1", timer1_pit_entry, RT_NULL, 4, RT_TIMER_FLAG_PERIODIC);
    
    //启动定时器
    if(RT_NULL != timer)
    {
        rt_timer_start(timer);
    }
}

void timer1_pitAdc_entry(void *parameter)
{
    getAdc();
}
void timer_pitAdc_init(void)
{
    rt_timer_t timerAdc;

    //创建一个定时器 周期运行
    timerAdc = rt_timer_create("timerAdc", timer1_pitAdc_entry, RT_NULL, 200, RT_TIMER_FLAG_PERIODIC);

    //启动定时器
    if(RT_NULL != timerAdc)
    {
        rt_timer_start(timerAdc);
    }
}
