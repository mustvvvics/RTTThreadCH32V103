#include "headfile.h"

void show_speed(void)
{
    char txt[32]={0};

//        sprintf(txt,"l_f=%05d|r_f=%05d",encoder_left_rear,encoder_left_front);
//        ips114_showstr(0, 0, txt);//slopeRowStart
    sprintf(txt,"Start=%03d|End=%03d|t=%03d",slopeRowStart,slopeRowEnd,timeControl);
    ips114_showstr(0, 0, txt);
    sprintf(txt,"Err=%05d|Fg=%01d|AC=%02d",cameraError,flagCameraElement,accelerateRatio);
    ips114_showstr(0, 1, txt);
    sprintf(txt,"pMeanT=%03d,Dis=%03d",pixelMeanThres,(int16)(detectDistance*10));
    ips114_showstr(0, 2, txt);
    sprintf(txt,"LJL=%04d|LJR=%4d",laneJitterLeft, laneJitterRight);
    ips114_showstr(0, 3, txt);
    sprintf(txt,"fl_Rou=% 2d|exRouDe=%4d    ",flagEnterRoundabout, exitRoundaboutDelay);
    ips114_showstr(0, 4, txt);
    sprintf(txt,"gyroData=%05d",gyroData);
    ips114_showstr(0, 5, txt);
    sprintf(txt,"Lo=%04d|Up=%04d",laneLocationShiftedLower,laneLocationShiftedUpper);
    ips114_showstr(0, 6, txt);
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
    ips114_init();
    
    //创建显示线程 优先级设置为5
    tid1 = rt_thread_create("display", display_entry, RT_NULL, 1024, 5, 50);
    
    //启动显示线程
    if(RT_NULL != tid1)
    {
        rt_thread_startup(tid1);
    }
}
