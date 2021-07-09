#include "headfile.h"


void show_speed(void)
{
    char txt[32]={0};

    sprintf(txt,"left_front = %05d",encoder_left_front);
    ips114_showstr(0, 1, txt);
    sprintf(txt,"right_rear = %05d",encoder_left_rear);
    ips114_showstr(0, 2, txt);
    sprintf(txt,"cameraError = %05d",cameraError);
    ips114_showstr(0, 3, txt);
}



void display_entry(void *parameter)
{
    //uint32 count=0;

    while(1)
    {
        //ips114_displayimage032(mt9v03x_image[0], MT9V03X_W, MT9V03X_H);
        //count++;
        show_speed();
    }
    
}


void display_init(void)
{
    rt_thread_t tid1;

    //初始化屏幕
//    oled_init();
    ips114_init();
    
    //创建显示线程 优先级设置为6 
    tid1 = rt_thread_create("display", display_entry, RT_NULL, 1024, 5, 50);
    
    //启动显示线程
    if(RT_NULL != tid1)
    {
        rt_thread_startup(tid1);
    }
}
