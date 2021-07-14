#include "headfile.h"

/*
*Pass variable data
*/

int8 menuY = 0;      //菜单上下

void clearError(void){//清空误差
    expected_omega = 0;
    position_front = 0;
    g_fGyroAngleSpeed_z = 0;
//    expected_y = 0;
    speed_conversion(0,0,0);
    yaw_pid.LocSum = 0;
    yaw_pid.Ek = 0;
    yaw_pid.Ek1 = 0;
    yaw_w_pid.target_val = 0;
    yaw_w_pid.err_next = 0;
    yaw_w_pid.err = 0;
    yaw_w_pid.err_last = 0;
    yaw_w_pid.actual_val = 0;
    manual_y=0;manual_z=0;
    go_forward=0;go_backward=0;
    go_left=0;go_right=0;
    pidModel = 0;
}
uint8 moterPid;
void transfetFunction(int8 targetRow,char *targetBuff){

    if ((1 - menuY) == targetRow){
        sprintf(targetBuff,"carF=%01d|Fg=%02d",car_flag,elementFlag);
    }
    else if ((2 - menuY) == targetRow){
        sprintf(targetBuff,"key=%02d|AC=%02d",key_data,accelerate);
    }
    else if ((3 - menuY) == targetRow) {                //BLACK
        sprintf(targetBuff,"CarSpeed=%03d",expected_y);
    }
    else if ((4 - menuY) == targetRow) {
        sprintf(targetBuff,"manual_z=%03d",manual_z);
    }
    else if ((5 - menuY) == targetRow) {
        sprintf(targetBuff,"manual_y=%03d",manual_y);
    }
    else if ((6 - menuY) == targetRow) {
        sprintf(targetBuff,"pidModel=%01d  ",pidModel);
    }
    else if ((7 - menuY) == targetRow) {
        sprintf(targetBuff,"Speed_P=%04d",(int16)S_P);
    }
    else if ((8 - menuY) == targetRow) {
        sprintf(targetBuff,"Speed_I=%04d",(int16)S_I);
    }
    else if ((9 - menuY) == targetRow) {
        sprintf(targetBuff,"Speed_D=%04d",(int16)S_D);
    }
    else if ((10 - menuY) == targetRow) {
        sprintf(targetBuff,"Angle_P=%04d",(int16)(yaw_w_pid.Kp*1000));
    }
    else if ((11 - menuY) == targetRow) {
        sprintf(targetBuff,"Angle_I=%04d",(int16)(yaw_w_pid.Ki*1000));
    }
    else if ((12 - menuY) == targetRow) {
        sprintf(targetBuff,"Angle_D=%04d",(int16)(yaw_w_pid.Kd*1000));
    }
    else if ((13 - menuY) == targetRow) {
        sprintf(targetBuff,"Turnn_P=%04d",(int16)(yaw_pid.Kp*100));
    }
    else if ((14 - menuY) == targetRow) {
        sprintf(targetBuff,"Turnn_I=%04d",(int16)(yaw_pid.Ki*100));
    }
    else if ((15 - menuY) == targetRow) {
        sprintf(targetBuff,"Turnn_D=%04d",(int16)(yaw_pid.Kd*100));
    }
    else if ((16 - menuY) == targetRow) {
        sprintf(targetBuff,"moterPid=%03d",moterPid);
    }
    else {
        sprintf(targetBuff,"              ");
    }
}
/*
*Assign value to data
*/
void assignValue(void){
    int8 signData;
    if (key_data == 4) {signData = 1; }//increase
    if (key_data == 1) {signData = -1;} //decrease
    if (key_data == 4 || key_data == 1) {
        switch (menuY + 3) {
            case 3:expected_y = expected_y + 10 * signData;;break;
            case 4:manual_z = manual_z + 10 * signData;break;
            case 5:manual_y = manual_y + 10 * signData;break;
            case 6:pidModel = pidModel + 1 * signData;break;
            case 7:
                S_P= S_P + 1 * signData;
                motor1_pid.Kp = motor1_pid.Kp + 1 * signData;
                motor2_pid.Kp = motor2_pid.Kp + 1 * signData;
                motor3_pid.Kp = motor3_pid.Kp + 1 * signData;
                motor4_pid.Kp = motor4_pid.Kp + 1 * signData;
                break;
            case 8:
                S_I= S_I + 1 * signData;
                motor1_pid.Ki = motor1_pid.Ki + 1 * signData;
                motor2_pid.Ki = motor2_pid.Ki + 1 * signData;
                motor3_pid.Ki = motor3_pid.Ki + 1 * signData;
                motor4_pid.Ki = motor4_pid.Ki + 1 * signData;
                break;
            case 9:
                S_D= S_D + 1 * signData;
                motor1_pid.Kd = motor1_pid.Kd + 1 * signData;
                motor2_pid.Kd = motor2_pid.Kd + 1 * signData;
                motor3_pid.Kd = motor3_pid.Kd + 1 * signData;
                motor4_pid.Kd = motor4_pid.Kd + 1 * signData;
                break;
            case 16:
                moterPid = moterPid + 1 * signData;
                break;
            default:break;
        }
    }
}

/*
*Disaplay Menu
*/
void disaplayMenu(void){
    uint8 maxMenuRow = 16;
    char txt1[20]={0},txt2[20]={0},txt3[20]={0},txt4[20]={0},
                      txt5[20]={0},txt6[20]={0},txt7[20]={0};
    if (menuY < 0) {menuY = 0;} //限制选择范围
    else if (menuY > maxMenuRow - 3) {menuY = maxMenuRow - 3;} //max - 3;now max = 15
    assignValue();//更改数值

    transfetFunction(1,txt1);
    transfetFunction(2,txt2);
    transfetFunction(3,txt3);
    transfetFunction(4,txt4);
    transfetFunction(5,txt5);
    transfetFunction(6,txt6);
    transfetFunction(7,txt7);

    ips114_showstr(0,1,txt1);
    ips114_showstr(0,2,txt2);
    ips114_showstrBlack(0,3,txt3);
    ips114_showstr(0,4,txt4);
    ips114_showstr(0,5,txt5);
    ips114_showstr(0,6,txt6);
    ips114_showstr(0,7,txt7);

    rt_mb_recv(key_mailbox, &key_data, RT_WAITING_FOREVER);
//    ips114_clear(WHITE);    //清屏成白色
    switch (key_data) {
//        case 1://left
//            break;
//        case 4://right
//            break;
        case 3://up
            menuY = menuY - 1;
            break;
        case 5://down
            menuY = menuY + 1;
            break;
        case 2://on
            if (car_flag == 0) {car_flag = 1;} else {car_flag = 0;}break;
        case 6:clearError();manual_y = 10;manual_z = 0;break;
        case 7:clearError();manual_y=-10;manual_z = 0;break;
        case 8:clearError();manual_z=-40;manual_y = 0;break;
        case 9:clearError();manual_z=40;manual_y = 0;break;
        default:
            break;
    }
}


void display_entry(void *parameter)
{
    char txt[32]={0};
    sprintf(txt,"CQUPT                              ");
    ips114_showstrGRAY(0,0,txt);
    while(1)
    {
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
