#include "headfile.h"

//定义按键引脚
#define KEY1    C4
#define KEY2    C5
#define KEY3    C6
#define KEY4    C7
#define KEY5    C8

//开关状态变量
uint8 key1_status = 1;
uint8 key2_status = 1;
uint8 key3_status = 1;
uint8 key4_status = 1;
uint8 key5_status = 1;

//上一次开关状态变量
uint8 key1_last_status;
uint8 key2_last_status;
uint8 key3_last_status;
uint8 key4_last_status;
uint8 key5_last_status;

//开关标志位
uint8 key1_flag;
uint8 key2_flag;
uint8 key3_flag;
uint8 key4_flag;
uint8 key5_flag;

////开关信号量
//rt_sem_t key1_sem;
//rt_sem_t key2_sem;
//rt_sem_t key3_sem;
//rt_sem_t key4_sem;
//rt_sem_t key5_sem;

uint32 key_data;

rt_mailbox_t key_mailbox;

void button_entry(void *parameter)
{

    //保存按键状态
    key1_last_status = key1_status;
    key2_last_status = key2_status;
    key3_last_status = key3_status;
    key4_last_status = key4_status;
    key5_last_status = key5_status;
    
    //读取当前按键状态
    key1_status = gpio_get(KEY1);
    key2_status = gpio_get(KEY2);
    key3_status = gpio_get(KEY3);
    key4_status = gpio_get(KEY4);
    key5_status = gpio_get(KEY5);

    //检测到按键按下之后并放开 释放一次信号量
    if(key1_status && !key1_last_status)    
    {
//        count = 1;
//        rt_sem_release(key1_sem);
        rt_mb_send(key_mailbox, 1);
    }
    if(key2_status && !key2_last_status)    
    {
//        count = 2;
//        rt_sem_release(key2_sem);

        rt_mb_send(key_mailbox, 2);
    }
    if(key3_status && !key3_last_status)    
    {
//        count = 3;
//        rt_sem_release(key3_sem);
        rt_mb_send(key_mailbox, 3);
    }
    if(key4_status && !key4_last_status)
    {
//        count = 4;
//        rt_sem_release(key4_sem);
        rt_mb_send(key_mailbox, 4);
    }
    if(key5_status && !key5_last_status)
    {
//        count = 5;
//        rt_sem_release(key5_sem);
        rt_mb_send(key_mailbox, 5);
    }
    /*************************************************************************/

}

void button_init(void)
{
    rt_timer_t timer1;
    
    gpio_init(KEY1, GPI, 0, GPIO_INT_CONFIG);
    gpio_init(KEY2, GPI, 0, GPIO_INT_CONFIG);
    gpio_init(KEY3, GPI, 0, GPIO_INT_CONFIG);
    gpio_init(KEY4, GPI, 0, GPIO_INT_CONFIG);
    gpio_init(KEY5, GPI, 0, GPIO_INT_CONFIG);
    
//    key1_sem = rt_sem_create("key1", 1, RT_IPC_FLAG_FIFO);  //创建按键的信号量，当按键按下就释放信号量，在需要使用按键的地方获取信号量即可
//    key2_sem = rt_sem_create("key2", 2, RT_IPC_FLAG_FIFO);
//    key3_sem = rt_sem_create("key3", 3, RT_IPC_FLAG_FIFO);
//    key4_sem = rt_sem_create("key4", 4, RT_IPC_FLAG_FIFO);
//    key5_sem = rt_sem_create("key5", 5, RT_IPC_FLAG_FIFO);

    key_mailbox = rt_mb_create("key", 5, RT_IPC_FLAG_FIFO);

    timer1 = rt_timer_create("button", button_entry, RT_NULL, 20, RT_TIMER_FLAG_PERIODIC);

    if(RT_NULL != timer1) 
    {
        rt_timer_start(timer1);
    }
}
