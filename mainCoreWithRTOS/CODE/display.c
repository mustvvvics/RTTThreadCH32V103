#include "headfile.h"

//int32 count=0;

void show_speed(void)
{
    char txt[32]={0};
    //ip in 7
    sprintf(txt,"l_f=%05d|r_f=%05d",encoder_data[3],encoder_data[2]);//
    ips114_showstr(0,0,txt);
    sprintf(txt,"l_r=%05d|r_r=%05d",encoder_data[1],encoder_data[0]);
    ips114_showstr(0,1,txt);
    sprintf(txt,"car_flag=%02d|ERROR=%05d",car_flag,position_front);
    ips114_showstr(0,2,txt);
    sprintf(txt,"key=%02d,sp=%04d",key_data,expected_y);
    ips114_showstr(0,3,txt);
//    sprintf(txt,"ex_omega=%04d,po_fron=%04d",expected_omega,position_front);
//    ips114_showstr(0,4,txt);



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
