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

//void timer1_threeWay_entry(void *parameter)
//{
////    getAdc();
//
//}
//rt_timer_t timerThreeWay;
//void timer_threeWay_init(void)
//{
//    //����һ����ʱ�� ��������
//    timerThreeWay = rt_timer_create("timerThreeWay", timer1_threeWay_entry, RT_NULL,4 , RT_TIMER_FLAG_PERIODIC);
//
//    //������ʱ��
//    if(RT_NULL != timerThreeWay)
//    {
//        rt_timer_start(timerThreeWay);
//    }
//}
