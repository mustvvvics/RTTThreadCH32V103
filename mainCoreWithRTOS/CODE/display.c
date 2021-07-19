#include "headfile.h"


int8 menuX = 0;      //菜单左右
int8 menuY = 0;      //菜单上下
uint8 parameterAdjustButton = 0;//左右参数调整响应
uint8 confirmButton = 0;//确认按键
uint32 servoDuty = 0;

uint8 turnpage = 0;
int16 parameterTest16 = 0;
/*
*Pass variable data on the first page
*/
void transfetFunctionFirst(int8 targetRow,char *targetBuff){
    if ((3 - menuY) == targetRow) {                //BLACK
        rt_sprintf(targetBuff,"CarSpeed=%04d ",expected_y);
    }
    else if ((4 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"Cargo&Winner  ");
    }
    else if ((5 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"Turnn_P=%04d  ",(int16)(yaw_pid.Kp*100));
    }
    else if ((6 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"Turnn_I=%04d  ",(int16)(yaw_pid.Ki*100));
    }
    else if ((7 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"Turnn_D=%04d  ",(int16)(yaw_pid.Kd*100));
    }
    else if ((8 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"ServoMotor     ");
    }
    else if ((9 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"Servo=%04d        ",servoDuty);
    }
    else if ((10 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"                   ");
    }

    else {
        rt_sprintf(targetBuff,"                   ");
    }
}
/*
*Assign value to data
*/

void assignValueFirst(void){
    int8 signData;
    if (parameterAdjustButton == 4 && confirmButton == 1) {signData = 1; }//increase
    if (parameterAdjustButton == 1 && confirmButton == 1) {signData = -1;} //decrease

    if (confirmButton == 1 && (menuY + 3) == 4 && car_flag == 0) {car_flag = 1;}
    else if(confirmButton == 0 && (menuY + 3) == 4 && car_flag == 1){car_flag = 0;}

    if ((parameterAdjustButton == 4 || parameterAdjustButton == 1) && confirmButton == 1) { //按下确认键才响应修改
        switch (menuY + 3) {
            case 3:expected_y = expected_y + 10 * signData;break;
            case 5:yaw_pid.Kp = yaw_pid.Kp + 0.1 * signData;break;
            case 6:yaw_pid.Ki = yaw_pid.Ki + 0.01 * signData;break;
            case 7:yaw_pid.Kd = yaw_pid.Kd + 0.01 * signData;break;
            case 8:
                if (parameterAdjustButton == 1) {servoDuty = 990;pwm_duty(PWM1_CH1_A8, servoDuty);}
                else if (parameterAdjustButton == 4){servoDuty = 670;pwm_duty(PWM1_CH1_A8, servoDuty);};break;
            case 9:
                servoDuty = servoDuty + 1 * signData;
                pwm_duty(PWM1_CH1_A8, servoDuty);break;
            default:break;
        }
    }
    else {return;}
}


/*
*Pass variable data on the second page
*/
void transfetFunctionSecond(int8 targetRow,char *targetBuff){
    if ((3 - menuY) == targetRow) {                //BLACK
        rt_sprintf(targetBuff,"Image Flipping ");//图像核翻页
    }
    else if ((4 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"Test16=%03d     ",parameterTest16);
    }
    else if ((5 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"                   ");
    }
    else if ((6 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"                   ");
    }
    else if ((7 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"                   ");
    }
    else if ((8 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"                   ");
    }
    else if ((9 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"                   ");
    }
    else if ((10 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"                   ");
    }

    else {
        rt_sprintf(targetBuff,"                   ");
    }
}
/*
*Assign value to data on the second page
*/

void assignValueSecond(void){
    int8 signData;
    if (parameterAdjustButton == 4 && confirmButton == 1) {signData = 1; }//increase
    if (parameterAdjustButton == 1 && confirmButton == 1) {signData = -1;} //decrease

    if ((parameterAdjustButton == 4 || parameterAdjustButton == 1) && confirmButton == 1) { //按下确认键才响应修改
        switch (menuY + 3) {
            case 3://图像核翻页
                turnpage = turnpage + 1 * (uint8)signData;
                if(turnpage < 0){turnpage = 0;}else if(turnpage > 1){turnpage = 1;}
                sendParameterToCam(8,0xE1,0,turnpage,0,0);break;
            case 4:
                parameterTest16 = parameterTest16 + 10 * signData;
                sendParameterToCam(16,0xE2,0,0,parameterTest16,0);break;
            case 5:  break;
            case 6:  break;
            case 7:  break;
            default:break;
            /*
             * Eg;sendParameterToCam(0,0xE1,xxx,0,0,0);break;
             *    sendParameterToCam(8,0xE1,0,xxx,0,0);break;
             *    sendParameterToCam(16,0xE1,0,0,xxx,0);break;
             *    sendParameterToCam(32,0xE1,0,0,0,xxx);break;
             */
        }
    }
    else {return;}
}

/*
*Disaplay Menu
*/


uint8 maxMenuRow = 9;//下滑选择限制
uint8 maxMenuPage = 1;//左右选择限制
char txt1[20]={0},txt2[20]={0},txt3[20]={0},txt4[20]={0},
                  txt5[20]={0},txt6[20]={0},txt7[20]={0};
void disaplayMenu(void){

/***********************状态栏*******************************************/
    rt_sprintf(txt1,"carF=%01d|Fg=%02d",car_flag,elementFlag);
    rt_sprintf(txt2,"Vc=%03d|AC=%02d",Vc,accelerate);
/************************************************************************/

    if (menuY < 0) {menuY = 0;} //限制选择范围
    else if (menuY > maxMenuRow - 3) {menuY = maxMenuRow - 3;} //max - 3;now max = 15
    if (menuX < 0) {menuX = 0;} //限制选择范围
    else if (menuX > maxMenuPage) {menuX = maxMenuPage;} //
/***********************参数调整*****************************************/
    if (menuX == 0) {
        assignValueFirst(); //更改数值
        transfetFunctionFirst(3,txt3); transfetFunctionFirst(4,txt4);//传递显示
        transfetFunctionFirst(5,txt5);transfetFunctionFirst(6,txt6);transfetFunctionFirst(7,txt7);
    }
    else if (menuX == 1) {
        assignValueSecond(); //更改数值
        transfetFunctionSecond(3,txt3); transfetFunctionSecond(4,txt4);//传递显示
        transfetFunctionSecond(5,txt5);transfetFunctionSecond(6,txt6);transfetFunctionSecond(7,txt7);
    }
    else if (menuX == 2) {

    }

/***********************显示每一行***************************************/

    ips114_showstr(0,1,txt1);
    ips114_showstr(0,2,txt2);
    if (confirmButton == 1) {ips114_showstrCheck(0,3,txt3);}else {ips114_showstrBlack(0,3,txt3);}//确定按键改变不同的颜色状态
    ips114_showstr(0,4,txt4);
    ips114_showstr(0,5,txt5);
    ips114_showstr(0,6,txt6);
    ips114_showstr(0,7,txt7);
/***********************等待按键信号*************************************/
    rt_mb_recv(key_mailbox, &key_data, RT_WAITING_FOREVER);
//    ips114_clear(WHITE);    //清屏成白色
    switch (key_data) {
        case 1:
            if(confirmButton == 0) {menuX = menuX - 1;} //无确认按键按下 则可翻页选择
            else if(confirmButton == 1){parameterAdjustButton = 1;}else{return;}break;//left 确认按键按下 进行参数调整
        case 4:
            if(confirmButton == 0) {menuX = menuX + 1;}
            else if(confirmButton == 1){parameterAdjustButton = 4;}else {return;}break;//right

        case 3:
            if(confirmButton == 0) {menuY = menuY - 1;} //up    保持在按下确定键后不响应上下
            else if(confirmButton == 1){parameterAdjustButton = 0;}else {return;}break;
        case 5:
            if(confirmButton == 0) {menuY = menuY + 1;} //down
            else if(confirmButton == 1){parameterAdjustButton = 0;}else {return;}break;
        case 2://确定键
            if (confirmButton == 0) {confirmButton = 1;} else {confirmButton = 0;parameterAdjustButton = 0;}break;
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
    }
}

rt_thread_t tidDisplay;
void display_init(void)
{
//    rt_thread_t tidDisplay;

    //初始化屏幕
    ips114_init();
    
    //创建显示线程 优先级设置为5
    tidDisplay = rt_thread_create("display", display_entry, RT_NULL, 1024, 5, 50);
    
    //启动显示线程
    if(RT_NULL != tidDisplay)
    {
        rt_thread_startup(tidDisplay);
    }
}
