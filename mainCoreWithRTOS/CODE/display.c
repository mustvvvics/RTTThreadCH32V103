#include "headfile.h"

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

}
int8 menuData = 0,menuY = 0,menuIn = 0;      //定义菜单左右上下

void transfetFunction(int8 targetRow,char *targetBuff){

    if ((1 - menuY) == targetRow) {
        sprintf(targetBuff,"Speed_P=%04d",(int16)S_P);
    }
    else if ((2 - menuY) == targetRow) {
        sprintf(targetBuff,"Speed_I=%04d",(int16)S_I);
    }
    else if ((3 - menuY) == targetRow) {
        sprintf(targetBuff,"Speed_D=%04d",(int16)S_D);
    }
    else if ((4 - menuY) == targetRow) {              //BLACK
        sprintf(targetBuff,"Angle_P=%04d",(int16)(yaw_w_pid.Kp*1000));
    }
    else if ((5 - menuY) == targetRow) {
        sprintf(targetBuff,"Angle_I=%04d",(int16)(yaw_w_pid.Ki*1000));
    }
    else if ((6 - menuY) == targetRow) {
        sprintf(targetBuff,"Angle_D=%04d",(int16)(yaw_w_pid.Kd*1000));
    }
    else if ((7 - menuY) == targetRow) {
        sprintf(targetBuff,"Turn_P=%04d",(int16)(yaw_pid.Kp*1000));
    }
    else if ((8 - menuY) == targetRow) {
        sprintf(targetBuff,"Turn_I=%04d",(int16)(yaw_pid.Ki*1000));
    }
    else if ((9 - menuY) == targetRow) {
        sprintf(targetBuff,"Turn_D=%04d",(int16)(yaw_pid.Kd*1000));
    }
    else {
//       targetBuff = {0};
    }
}


void disaplayMenu(void){
    char txt[20]={0};
    sprintf(txt,"CQUPT    key=%d",key_data);
    ips114_showstr(0,0,txt);
    char txt1[20]={0},txt2[20]={0},txt3[20]={0},txt4[20]={0},
                      txt5[20]={0},txt6[20]={0},txt7[20]={0};
    if (menuY < (-3)) {menuY = -3;}
    else if (menuY > 5) {menuY = 5;}

    transfetFunction(1,txt1);
    transfetFunction(2,txt2);
    transfetFunction(3,txt3);
    transfetFunction(4,txt4);
    transfetFunction(5,txt5);
    transfetFunction(6,txt6);
    transfetFunction(7,txt7);

    ips114_showstr(0,1,txt1);
    ips114_showstr(0,2,txt2);
    ips114_showstr(0,3,txt3);
    ips114_showstrBlack(0,4,txt4);
    ips114_showstr(0,5,txt5);
    ips114_showstr(0,6,txt6);
    ips114_showstr(0,7,txt7);

    rt_mb_recv(key_mailbox, &key_data, RT_WAITING_FOREVER);
    ips114_clear(WHITE);    //清屏成白色
    switch (key_data) {
        case 1://left
            menuData = menuData - 1;
            break;
        case 4://right
            menuData = menuData + 1;
            break;
        case 3://up
            menuY = menuY - 1;
            break;
        case 5://down
            menuY = menuY + 1;
            break;
        case 2://on
            if (menuIn == 0) {menuIn = 1;} else {menuIn = 0;}
            break;
        default:
            break;
    }
}


void display_entry(void *parameter)
{
    while(1)
    {
//        show_speed();
        disaplayMenu();
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
