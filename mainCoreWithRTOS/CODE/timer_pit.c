#include "headfile.h"

uint8 Gyro_buff[5];

void timer1_pit_entry(void *parameter)
{
    //��������������
    Gyro_buff[0] = 0xD8;                            //֡ͷ

    Gyro_buff[1] = 0xB0;                            //������
    Gyro_buff[2] = g_fGyroAngleSpeed_z>>8;          //���ݸ�8λ
    Gyro_buff[3] = g_fGyroAngleSpeed_z&0xFF;        //���ݵ�8λ

    Gyro_buff[4] = 0xEE;                            //֡β
    uart_putbuff(UART_3, Gyro_buff, 5);             //ͨ������3�����ݷ��ͳ�ȥ��
}


void timer_pit_init(void)
{
    rt_timer_t timer;
    
    //����һ����ʱ�� ��������
    timer = rt_timer_create("timer1", timer1_pit_entry, RT_NULL, 4, RT_TIMER_FLAG_PERIODIC);
    
    //������ʱ��
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

    //����һ����ʱ�� ��������
    timerAdc = rt_timer_create("timerAdc", timer1_pitAdc_entry, RT_NULL, 200, RT_TIMER_FLAG_PERIODIC);

    //������ʱ��
    if(RT_NULL != timerAdc)
    {
        rt_timer_start(timerAdc);
    }
}
