#include "headfile.h"

void timer1_pit_entry(void *parameter)
{
//    static uint32 timeControl;
//    timeControl++;

    
    //�ɼ�����������
    //get_icm20602_gyro_spi();
    



}


void timer_pit_init(void)
{
    rt_timer_t timer;
    
    //����һ����ʱ�� ��������
    timer = rt_timer_create("timer1", timer1_pit_entry, RT_NULL, 2, RT_TIMER_FLAG_PERIODIC);
    
    //������ʱ��
    if(RT_NULL != timer)
    {
        rt_timer_start(timer);
    }

    
}
