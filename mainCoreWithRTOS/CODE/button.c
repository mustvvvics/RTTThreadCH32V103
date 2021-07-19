#include "headfile.h"

//���尴������
#define KEY1    C4
#define KEY2    C5
#define KEY3    C6
#define KEY4    C7
#define KEY5    C8

//����״̬����
uint8 key1_status = 1;
uint8 key2_status = 1;
uint8 key3_status = 1;
uint8 key4_status = 1;
uint8 key5_status = 1;

//��һ�ο���״̬����
uint8 key1_last_status;
uint8 key2_last_status;
uint8 key3_last_status;
uint8 key4_last_status;
uint8 key5_last_status;

//���ر�־λ
uint8 key1_flag = 0;
uint8 key2_flag = 0;
uint8 key3_flag = 0;
uint8 key4_flag = 0;
uint8 key5_flag = 0;

uint32 key_data = 0;

rt_mailbox_t key_mailbox;

void button_entry(void *parameter)
{
    //���水��״̬
    key1_last_status = key1_status;
    key2_last_status = key2_status;
    key3_last_status = key3_status;
    key4_last_status = key4_status;
    key5_last_status = key5_status;
    
    //��ȡ��ǰ����״̬
    key1_status = gpio_get(KEY1);
    key2_status = gpio_get(KEY2);
    key3_status = gpio_get(KEY3);
    key4_status = gpio_get(KEY4);
    key5_status = gpio_get(KEY5);

    //��⵽��������֮�󲢷ſ� �ͷ�һ���ź���
    if(key1_status && !key1_last_status && key1_flag == 0){
        rt_mb_send(key_mailbox, 1);
    }
    if(key2_status && !key2_last_status && key2_flag == 0){
        rt_mb_send(key_mailbox, 2);
    }
    if(key3_status && !key3_last_status && key3_flag == 0){
        rt_mb_send(key_mailbox, 3);
    }
    if(key4_status && !key4_last_status && key4_flag == 0){
        rt_mb_send(key_mailbox, 4);
    }
    if(key5_status && !key5_last_status && key5_flag == 0){
        rt_mb_send(key_mailbox, 5);
    }
}

void button_init(void)
{
    rt_timer_t timer1;
    
    gpio_init(KEY1, GPI, 0, GPIO_INT_CONFIG);
    gpio_init(KEY2, GPI, 0, GPIO_INT_CONFIG);
    gpio_init(KEY3, GPI, 0, GPIO_INT_CONFIG);
    gpio_init(KEY4, GPI, 0, GPIO_INT_CONFIG);
    gpio_init(KEY5, GPI, 0, GPIO_INT_CONFIG);

    key_mailbox = rt_mb_create("key", 5, RT_IPC_FLAG_FIFO);

    timer1 = rt_timer_create("button", button_entry, RT_NULL, 20, RT_TIMER_FLAG_PERIODIC);

    if(RT_NULL != timer1) 
    {
        rt_timer_start(timer1);
    }
}
