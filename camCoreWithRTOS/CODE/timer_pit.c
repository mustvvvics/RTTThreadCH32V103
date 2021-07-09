#include "headfile.h"

void timer1_pit_entry(void *parameter)
{
    //static uint32 time;
    //time++;
    get_sensor_data();                          //获取传感器数据。
    process_data();                             //根据协议处理数据，并存入temp_buff中。
    uart_putbuff(UART_3, temp_buff, LINE_LEN);  //通过串口3将数据发送出去。
    GPIO_PIN_SET(B12);
    GPIO_PIN_RESET(B12);                         //A0引脚拉低


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
