#include "headfile.h"
#include "display.h"
#include "communication.h"

void show_speed(void)
{
    char txt[32]={0};

    sprintf(txt,"left_front = %05d",encoder_left_front);
    oled_p6x8str(0, 2, txt);
    sprintf(txt,"right_rear = %05d",encoder_left_rear);
    oled_p6x8str(0, 3, txt);
}



void display_entry(void *parameter)
{
    //uint32 count=0;

    while(1)
    {
        //ips114_displayimage032(mt9v03x_image[0], MT9V03X_W, MT9V03X_H);
        //count++;
        show_speed();
        oled_p6x8str(0, 5, "Init OK!");
    }
    
}


void display_init(void)
{
    rt_thread_t tid1;

    //初始化屏幕
    oled_init();
    
    //创建显示线程 优先级设置为6 
    tid1 = rt_thread_create("display", display_entry, RT_NULL, 1024, 6, 50);
    
    //启动显示线程
    if(RT_NULL != tid1)
    {
        rt_thread_startup(tid1);
    }
}
