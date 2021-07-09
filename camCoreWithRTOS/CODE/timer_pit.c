#include "headfile.h"

void timer1_pit_entry(void *parameter)
{
    //static uint32 time;
    //time++;
    get_sensor_data();                          //��ȡ���������ݡ�
    process_data();                             //����Э�鴦�����ݣ�������temp_buff�С�
    uart_putbuff(UART_3, temp_buff, LINE_LEN);  //ͨ������3�����ݷ��ͳ�ȥ��
    GPIO_PIN_SET(B12);
    GPIO_PIN_RESET(B12);                         //A0��������


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
