#include "headfile.h"

void timer1_pit_entry(void *parameter)
{
//    static uint32 timeControl;
//    timeControl++;


    //采集陀螺仪数据
    //get_icm20602_gyro_spi();
    



}


void timer_pit_init(void)
{
    rt_timer_t timer;
    
    //创建一个定时器 周期运行
    timer = rt_timer_create("timer1", timer1_pit_entry, RT_NULL, 2, RT_TIMER_FLAG_PERIODIC);
    
    //启动定时器
    if(RT_NULL != timer)
    {
        rt_timer_start(timer);
    }

    
}
