#include "motor.h"
#include "timer_pit.h"
#include "elec.h"

void timer1_pit_entry(void *parameter)
{
    //static uint32 time;
    //time++;

    //采集编码器数据

    
    //电机控制速度环
    
    
    //采集陀螺仪数据
    //get_icm20602_gyro_spi();
    
    //采集电磁信号
    //elec_get();
    
    //根据电磁信号计算车身位置
    //elec_calculate();
    
    
    //将图像计算出来的车身位置与电磁信号计算出的车身位置进行融合得到更好数据
    //然后进行车模控制
    

    
    //控制电机转动
    //motor_control(1000, 1000, 1000, 1000);

}


void timer_pit_init(void)
{
    rt_timer_t timer;
    
    //创建一个定时器 周期运行
    timer = rt_timer_create("timer1", timer1_pit_entry, RT_NULL, 5, RT_TIMER_FLAG_PERIODIC);
    
    //启动定时器
    if(RT_NULL != timer)
    {
        rt_timer_start(timer);
    }

    
}
