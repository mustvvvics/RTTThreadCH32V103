#include "headfile.h"

int16 displayFlag = 1;


void show_speed(void)
{
    char txt[32]={0};
    display_is_working = 1;
    if (displayFlag == 0) {
        sprintf(txt,"l_f=%05d|r_f=%05d",encoder_left_rear,encoder_left_front);
        ips114_showstr(0, 0, txt);
        sprintf(txt,"cameraError = %05d",cameraError);
        ips114_showstr(0, 1, txt);
    }
    else {
        sprintf(txt,"l_f=%05d|r_f=%05d",encoder_left_rear,encoder_left_front);
        oled_p6x8str(0, 0, txt);
        sprintf(txt,"cameraError = %05d",cameraError);
        oled_p6x8str(0, 1, txt);
    }

    if (displayFlag == 0) {
//        ips114_showstr(0,7,esp8266_buf);
        ips114_showstr(0,6,"TCP Init Ok");
        sprintf(txt,"pMeanT=%d",pixelMeanThres);
        ips114_showstr(0, 2, txt);
        sprintf(txt,"det_Dis=%d",Int2Float);
        ips114_showstr(0, 3, txt);
    }
    else {
//        oled_p6x8str(0,7,esp8266_buf);
        oled_p6x8str(0,6,"TCP Init Ok");
        sprintf(txt,"pMeanT=%d",pixelMeanThres);
        oled_p6x8str(0, 2, txt);
        sprintf(txt,"pMeanT=%d",Int2Float);
        oled_p6x8str(0, 3, txt);
    }



    display_is_working = 0;
    rt_thread_mdelay(100);
}



void display_entry(void *parameter)
{
    while(1)
    {
        show_speed();
    }
    
}


void display_init(void)
{
    rt_thread_t tid1;

    //初始化屏幕
    if (displayFlag == 0) {
        ips114_init();
    }
    else {
        oled_init();
    }
    
    //创建显示线程 优先级设置为6 
    tid1 = rt_thread_create("display", display_entry, RT_NULL, 1024, 5, 50);
    
    //启动显示线程
    if(RT_NULL != tid1)
    {
        rt_thread_startup(tid1);
    }
}
