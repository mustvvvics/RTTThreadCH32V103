#include "headfile.h"

/*
*Pass variable data
*/

int8 menuY = 0;      //菜单上下
char txta[32]={0},txtb[32]={0},txtc[32]={0},txtd[32]={0};
void showEncoder(void){
    rt_sprintf(txta,"enc 3=%d",encoder_data[3]);
    rt_sprintf(txtb,"enc 2=%d",encoder_data[2]);
    rt_sprintf(txtc,"enc 0=%d",encoder_data[0]);
    rt_sprintf(txtd,"enc 1=%d",encoder_data[1]);
    ips114_showstr(0,1,txta);
    ips114_showstr(0,2,txtb);
    ips114_showstr(0,3,txtc);
    ips114_showstr(0,4,txtd);

}
//uint8 moterPid;
void transfetFunction(int8 targetRow,char *targetBuff){


    if ((3 - menuY) == targetRow) {                //BLACK
        rt_sprintf(targetBuff,"CarSpeed=%04d",expected_y);
    }
    else if ((4 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"manual_z=%04d",manual_z);
    }
    else if ((5 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"manual_y=%04d",manual_y);
    }
    else if ((6 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"pidModel=%01d   ",pidModel);
    }
    else if ((7 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"Speed_P=%04d ",(int16)(S_P*10));
    }
    else if ((8 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"Speed_I=%04d ",(int16)(S_I*10));
    }
    else if ((9 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"Speed_D=%04d ",(int16)(S_D*10));
    }
    else if ((10 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"Angle_P=%04d ",(int16)(yaw_w_pid.Kp*1000));
    }
    else if ((11 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"Angle_I=%04d ",(int16)(yaw_w_pid.Ki*1000));
    }
    else if ((12 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"Angle_D=%04d ",(int16)(yaw_w_pid.Kd*1000));
    }
    else if ((13 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"Turnn_P=%04d ",(int16)(yaw_pid.Kp*100));
    }
    else if ((14 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"Turnn_I=%04d ",(int16)(yaw_pid.Ki*100));
    }
    else if ((15 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"Turnn_D=%04d ",(int16)(yaw_pid.Kd*100));
    }
    else if ((16 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"GroyFg=%05d",roundFinishFlag);
    }
    else if ((17 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"ThreeWay        ");
    }
    else {
        rt_sprintf(targetBuff,"                   ");
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
                S_P= S_P + + 0.1 * signData;
                motor1_pid.Kp = motor1_pid.Kp + 0.1 * signData;
                motor2_pid.Kp = motor2_pid.Kp + 0.1 * signData;
                motor3_pid.Kp = motor3_pid.Kp + 0.1 * signData;
                motor4_pid.Kp = motor4_pid.Kp + 0.1 * signData;
                break;
            case 8:
                S_I= S_I + + 0.1 * signData;
                motor1_pid.Ki = motor1_pid.Ki + 0.1 * signData;
                motor2_pid.Ki = motor2_pid.Ki + 0.1 * signData;
                motor3_pid.Ki = motor3_pid.Ki + 0.1 * signData;
                motor4_pid.Ki = motor4_pid.Ki + 0.1 * signData;
                break;
            case 9:
                S_D= S_D + + 0.1 * signData;
                motor1_pid.Kd = motor1_pid.Kd + 0.1 * signData;
                motor2_pid.Kd = motor2_pid.Kd + 0.1 * signData;
                motor3_pid.Kd = motor3_pid.Kd + 0.1 * signData;
                motor4_pid.Kd = motor4_pid.Kd + 0.1 * signData;
                break;
            case 10:yaw_w_pid.Kp = yaw_w_pid.Kp + 0.01 * signData;break;
            case 11:yaw_w_pid.Ki = yaw_w_pid.Ki + 0.01 * signData;break;
            case 12:yaw_w_pid.Kd = yaw_w_pid.Kd + 0.01 * signData;break;
            case 13:yaw_pid.Kp = yaw_pid.Kp + 0.01 * signData;break;
            case 14:yaw_pid.Ki = yaw_pid.Ki + 0.01 * signData;break;
            case 15:yaw_pid.Kd = yaw_pid.Kd + 0.01 * signData;break;
            case 16:
                roundFinishFlag = roundFinishFlag + 1 * signData;break;
            case 17:
                if (key_data == 1) {pwm_duty(PWM1_CH1_A8, 990);}
                else if (key_data == 4){pwm_duty(PWM1_CH1_A8, 338);};break;
            default:break;
        }
    }
}

/*
*Disaplay Menu
*/
uint8 maxMenuRow = 17;
char txt1[20]={0},txt2[20]={0},txt3[20]={0},txt4[20]={0},
                  txt5[20]={0},txt6[20]={0},txt7[20]={0};
void disaplayMenu(void){


    if (menuY < 0) {menuY = 0;} //限制选择范围
    else if (menuY > maxMenuRow - 3) {menuY = maxMenuRow - 3;} //max - 3;now max = 15
    assignValue(); //更改数值
/***********************状态栏*******************************************/
    rt_sprintf(txt1,"carF=%01d|Fg=%02d",car_flag,elementFlag);
    rt_sprintf(txt2,"Vc=%03d|AC=%02d",Vc,accelerate);

/***********************参数调整*****************************************/
//    transfetFunction(1,txt1);
//    transfetFunction(2,txt2);
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

char txtcqupt[32]={0};
void display_entry(void *parameter)
{

    rt_sprintf(txtcqupt,"CQUPT                              ");
    ips114_showstrGRAY(0,0,txtcqupt);
    while(1)
    {
        disaplayMenu();
//        showEncoder();
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
