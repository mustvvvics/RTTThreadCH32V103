#include "headfile.h"



void timer1_pitAdc_entry(void *parameter)
{
    getAdc();
}


rt_timer_t timerAdc;
void timer_pitAdc_init(void)
{

    adc_init(ADC_IN9_B1); //��ʼ��ADC����

    //����һ����ʱ�� ��������
    timerAdc = rt_timer_create("timerAdc", timer1_pitAdc_entry, RT_NULL, 2000, RT_TIMER_FLAG_PERIODIC);

    //������ʱ��
    if(RT_NULL != timerAdc)
    {
        rt_timer_start(timerAdc);
    }
}
