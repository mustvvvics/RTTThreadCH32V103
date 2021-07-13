#include "headfile.h"

//int32 count=0;

void show_speed(void)
{
    char txt[32]={0};

    //ip in 7
//    sprintf(txt,"l_f=%05d|r_f=%05d",encoder_data[3],encoder_data[2]);//
//    ips114_showstr(0,0,txt);
//    sprintf(txt,"l_r=%05d|r_r=%05d",encoder_data[1],encoder_data[0]);
//    ips114_showstr(0,1,txt)
//    sprintf(txt,"timeControl=%05d",timeControl);//
//    ips114_showstr(0,0,txt);
/**********************************************************************/
    sprintf(txt,"GA=%05d",g_fGyroAngleSpeed_z);//
    ips114_showstr(0,0,txt);
//    sprintf(txt,"T1=%05d,T2=%05d",timet1,timet2);//
//    ips114_showstr(0,1,txt);
//    sprintf(txt,"m_z=%03d,m_y=%03d,pidModel=%01d",manual_z,manual_y,pidModel); //查看速度
//    ips114_showstr(0,1,txt);
    sprintf(txt,"car_flag=%01d|ERROR=%05d",car_flag,position_front);
    ips114_showstr(0,2,txt);
    sprintf(txt,"key=%01d|Fg=%01d|sp=%03d|AC=%02d",key_data,elementFlag,expected_y,accelerate);
    ips114_showstr(0,3,txt);
//    sprintf(txt,"ex_omega=%04d,po_fron=%04d",expected_omega,position_front);
//    ips114_showstr(0,4,txt);
    sprintf(txt,"SP=%04d|SI=%04d|SD=%04d",(int16)S_P,(int16)S_I,(int16)S_D);
    ips114_showstr(0,4,txt);
    sprintf(txt,"AP=%04d|AI=%04d|AD=%04d",(int16)(yaw_w_pid.Kp*1000),
            (int16)(yaw_w_pid.Ki*1000),(int16)(yaw_w_pid.Kd*1000));
    ips114_showstr(0,5,txt);
    sprintf(txt,"TP=%04d|TI=%04d|TD=%04d",(int16)(yaw_pid.Kp*1000),
            (int16)(yaw_pid.Ki*1000),(int16)(yaw_pid.Kd*1000));
    ips114_showstr(0,6,txt);
//
//    sendMessage();//发送曲线
    /**********************************************************************/
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
//    oled_init();
    
    //创建显示线程 优先级设置为5
    tid1 = rt_thread_create("display", display_entry, RT_NULL, 1024, 5, 50);
    
    //启动显示线程
    if(RT_NULL != tid1)
    {
        rt_thread_startup(tid1);
    }
}
