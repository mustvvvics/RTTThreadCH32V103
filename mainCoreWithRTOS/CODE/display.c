#include "headfile.h"
/*******************************************************************************************************/
//  邮电右 113636225   邮电左 223636115
//  1 环岛   2 三叉    3十字  4入库   5延时  6延时  7延时
#define    elementTable1            2114
#define    elementTableLength1      4
#define    elementTable2            0
#define    elementTableLength2      0
#define    elementTable3            0
#define    elementTableLength3      0

int32 elementTable = 0;             //元素顺序表
uint8 elementTableLength = 0;               //元素个数
int32 elementTableA = 0;             //元素顺序表
uint8 drivingDirectionToCam = 0;            //行驶方向

/*
 * page 2 in cam display
 */

/******************Threewayroad*****************************************/
uint8 fixCamThreewayFeatureRow = 20;// +- 1
/******************Crossroad********************************************/
uint8 fixCamDetectCrossroadMissingNumThres = 6;//+- 1
/******************SharpCurve*******************************************/
uint8 fixCamRangeSharpCurveRow = 35;//+- 1
//int16 fixCamSharpCurveJitterThres = unknown;
//int32 fixCamBigCurveThres = unknown;
/******************Else*************************************************/
int8 fixCamGlobalCenterBias = -7;//+- 1
int32 fixCamOutboundAreaThres = 10000;//+- 1000
uint16 fixCamStartlineJumpingPointNumThres = 80;//+- 10
/*
 * page 3 in cam display
 */
/******************Basic************************************************/
uint8 fixCamDetectDistance = 17;//+- 1 实际为float 2.0 +- 0.1
int32 fixCamPixelMeanThres = 100;//+- 10
uint8 fixCamSlopeRowStart = 48;//+- 1
uint8 fixCamSlopeRowEnd = 35;//+- 1
/******************Roundabout*******************************************/
int32 fixCamAreaDetectRoundaboutThresLeft = 370;//+- 10
int32 fixCamAreaDetectRoundaboutThresRight = 370;//+- 10
uint8 fixCamRoundaboutDetectionStartRow = 20;//+- 1
/*******************************************************************************************************/
/*
 * 菜单使用
 */
int8 menuX = 0,menuY = 0;       //菜单左右  菜单上下
uint8 parameterAdjustButton = 0;//左右参数调整响应
uint8 confirmButton = 0;        //确认按键
uint32 servoDuty = 0;           //舵机调参使用
uint8 carStart = 0;             //启动信号
int8 turnpage = 0;              //翻页
uint8 clearCamFlags = 0;        //清理图像核flag
/*
*Pass variable data on the first page
*/
void transfetFunctionFirst(int8 targetRow,char *targetBuff){

    if ((3 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"FlashWrite                     ");
    }
    else if ((4 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"Cargo&Winner                   ");
    }
    else if ((5 - menuY) == targetRow) {                //BLACK
        rt_sprintf(targetBuff,"CarSpeed=%04d                 ",expected_y);
    }
    else if ((6 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"CarDirection=%01d                 ",drivingDirectionToCam);
    }
    else if ((7 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"encoderCountYFlag=%01d            ",encoderCountYFlagMain);
    }
    else if ((8 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"Turn_P=%05d                   ",(int16)(yaw_pid.Kp*1000));
    }
    else if ((9 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"Turn_D=%05d                   ",(int16)(yaw_pid.Kd*1000));
    }
    else if ((10 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"ServoMotor                      ");
    }
    else if ((11 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"ServoChange=%04d               ",servoDuty);
    }
    else if ((12 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"CarSpeeX=%04d                  ",expected_X);
    }
    else if ((13 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"roundIslandMax=%04d            ",roundIslandMax);
    }
    else if ((14 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"testSideFlag=%04d              ",testSideFlag);
    }
    else {
        rt_sprintf(targetBuff,"                               ");
    }
}

/*
*Assign value to data  信号量触发 都只响应一次
*/

void assignValueFirst(void){
    int8 signData;
    if (parameterAdjustButton == 4 && confirmButton == 1) {signData = 1; }//increase
    if (parameterAdjustButton == 1 && confirmButton == 1) {signData = -1;} //decrease

    if (confirmButton == 1 && (menuY + 3) == 3) {
        mainFlashWrite();
    }

    if (confirmButton == 1 && (menuY + 3) == 4) {
        if (car_flag == 0) {
            car_flag = 1;carStart = 1;clearCamFlags = 1;
            sendParameterToCam(8,0xAB,0,carStart,0,0);//启动信号1
            sendParameterToCam(8,0xE2,0,clearCamFlags,0,0);//清空
        }
    }
    else if(confirmButton == 0 && (menuY + 3) == 4 && car_flag == 2){car_flag = 0;}

    if ((parameterAdjustButton == 4 || parameterAdjustButton == 1) && confirmButton == 1) { //按下确认键才响应修改
        switch (menuY + 3) {
            case 5:expected_y = expected_y + 5 * signData;break;
            case 6:
                drivingDirectionToCam = drivingDirectionToCam + 1 * signData;
                if (drivingDirectionToCam <= 0) {drivingDirectionToCam = 0;}
                else{drivingDirectionToCam = 1;}
                sendParameterToCam(8,0xDD,0,drivingDirectionToCam,0,0);break;
            case 7:
                encoderCountYFlagMain = encoderCountYFlagMain + 1 * signData;
                if (encoderCountYFlagMain <= 0) {encoderCountYFlagMain = 0;}
                else{encoderCountYFlagMain = 1;}break;
            case 8:yaw_pid.Kp = yaw_pid.Kp + 0.5 * signData;break;
            case 9:yaw_pid.Kd = yaw_pid.Kd + 0.001 * signData;break;
            case 10:
                if (parameterAdjustButton == 1) {servoDuty = 1000;pwm_duty(PWM1_CH1_A8, servoDuty);}
                else if (parameterAdjustButton == 4){servoDuty = 672;pwm_duty(PWM1_CH1_A8, servoDuty);};break;
            case 11:
                servoDuty = servoDuty + 1 * signData;
                pwm_duty(PWM1_CH1_A8, servoDuty);break;
            case 12:expected_X = expected_X + 500 * signData;break;
            case 13:roundIslandMax = roundIslandMax + 1000 * signData;break;
            case 14:
                testSideFlag = testSideFlag + 1 * signData;
                if (testSideFlag <= 0) {testSideFlag = 0;}
                else{testSideFlag = 1;}break;
            default:break;
        }
    }
    else {return;}
}

/*
*Pass variable data on the second page
*/
void transfetFunctionSecond(int8 targetRow,char *targetBuff){
    if ((3 - menuY) == targetRow) {                                 //BLACK
        rt_sprintf(targetBuff,"ImageFlippingPage1              "); //图像核翻页
    }
    else if ((4 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"clearCamFlags                   ");
    }
    else if ((5 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"ThreewayFeatureRow=%03d          ",fixCamThreewayFeatureRow);
    }
    else if ((6 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"DCrossroadMissNumThres=%03d",fixCamDetectCrossroadMissingNumThres);
    }
    else if ((7 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"RangeSharpCurveRow=%02d           ",fixCamRangeSharpCurveRow);
    }
    else if ((8 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"GlobCenterBias=%03d              ",fixCamGlobalCenterBias);
    }
    else if ((9 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"SLJumpPointNumThres=%03d",fixCamStartlineJumpingPointNumThres);
    }
    else if ((10 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"OutboundAreaThres=%05d         ",fixCamOutboundAreaThres);
    }
    else if ((11 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"                                 ");
    }
    else if ((12 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"                                 ");
    }
    else {
        rt_sprintf(targetBuff,"                                 ");
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
                turnpage = turnpage + 1 * signData;
                if(turnpage < 0){turnpage = 0;}else if(turnpage > 3){turnpage = 3;}
                sendParameterToCam(0,0xE1,turnpage,0,0,0);break;
            case 4:
                clearCamFlags = 1;
                sendParameterToCam(8,0xE2,0,clearCamFlags,0,0);break;
            case 5:
                fixCamThreewayFeatureRow = fixCamThreewayFeatureRow + 1 * signData;
                sendParameterToCam(8,0xE3,0,fixCamThreewayFeatureRow,0,0);break;
            case 6:
                fixCamDetectCrossroadMissingNumThres = fixCamDetectCrossroadMissingNumThres +  1 * signData;
                sendParameterToCam(8,0xE4,0,fixCamDetectCrossroadMissingNumThres,0,0);break;
            case 7:
                fixCamRangeSharpCurveRow = fixCamRangeSharpCurveRow +  1 * signData;
                sendParameterToCam(8,0xE5,0,fixCamRangeSharpCurveRow,0,0);break;
            case 8:
                fixCamGlobalCenterBias = fixCamGlobalCenterBias + 1 * signData;
                sendParameterToCam(0,0xE6,fixCamGlobalCenterBias,0,0,0);break;
            case 9:
                fixCamStartlineJumpingPointNumThres = fixCamStartlineJumpingPointNumThres + 10 * signData;
                sendParameterToCam(16,0xE7,0,0,fixCamStartlineJumpingPointNumThres,0);break;
            case 10:
                fixCamOutboundAreaThres = fixCamOutboundAreaThres + 1000 * signData;
                sendParameterToCam(32,0xBB,0,0,0,fixCamOutboundAreaThres);break;
            case 11:break;
            case 12:break;
            default:break;
        }
    }
    else {return;}
}


/*
*Pass variable data on the Third page
*/
void transfetFunctionThird(int8 targetRow,char *targetBuff){
    if ((3 - menuY) == targetRow) {                //BLACK
        rt_sprintf(targetBuff,"ImageFlippingPage2              "); //图像核翻页
    }
    else if ((4 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"DetectDistance=%03d              ",fixCamDetectDistance);
    }
    else if ((5 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"PixelMeanThres=%03d              ",fixCamPixelMeanThres);
    }
    else if ((6 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"SlopeRowStart =%03d              ",fixCamSlopeRowStart);
    }
    else if ((7 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"SlopeRowEnd   =%03d              ",fixCamSlopeRowEnd );
    }
    else if ((8 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"ADRoundaboutThresLeft=%03d",fixCamAreaDetectRoundaboutThresLeft);
    }
    else if ((9 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"DRAboutThRight=%03d              ",fixCamAreaDetectRoundaboutThresRight);
    }
    else if ((10 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"RADetectionStartRow=%03d ",fixCamRoundaboutDetectionStartRow);
    }
    else if ((11 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"                                 ");
    }
    else if ((12 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"                                 ");
    }
    else {
        rt_sprintf(targetBuff,"                                 ");
    }
}

/*
*Assign value to data on the Third page
*/

void assignValueThird(void){
    int8 signData;
    if (parameterAdjustButton == 4 && confirmButton == 1) {signData = 1; } //increase
    if (parameterAdjustButton == 1 && confirmButton == 1) {signData = -1;} //decrease

    if ((parameterAdjustButton == 4 || parameterAdjustButton == 1) && confirmButton == 1) { //按下确认键才响应修改
        switch (menuY + 3) {
            case 3://图像核翻页
                turnpage = turnpage + 1 * signData;
                if(turnpage < 0){turnpage = 0;}else if(turnpage > 3){turnpage = 3;}
                sendParameterToCam(0,0xE1,turnpage,0,0,0);break;
            case 4:
                fixCamDetectDistance = fixCamDetectDistance + 1 * signData;
                sendParameterToCam(8,0xE8,0,fixCamDetectDistance,0,0);break;
            case 5:
                fixCamPixelMeanThres = fixCamPixelMeanThres + 10 * signData;
                sendParameterToCam(32,0xE9,0,0,0,fixCamPixelMeanThres);break;
            case 6:
                fixCamSlopeRowStart = fixCamSlopeRowStart + 1 * signData;
                sendParameterToCam(8,0xEA,0,fixCamSlopeRowStart,0,0);break;
            case 7:
                fixCamSlopeRowEnd = fixCamSlopeRowEnd + 1 * signData;
                sendParameterToCam(8,0xEB,0,fixCamSlopeRowEnd,0,0);break;
            case 8:
                fixCamAreaDetectRoundaboutThresLeft = fixCamAreaDetectRoundaboutThresLeft + 10 * signData;
                sendParameterToCam(32,0xEC,0,0,0,fixCamAreaDetectRoundaboutThresLeft);break;
            case 9:
                fixCamAreaDetectRoundaboutThresRight = fixCamAreaDetectRoundaboutThresRight + 10 * signData;
                sendParameterToCam(32,0xED,0,0,0,fixCamAreaDetectRoundaboutThresRight);break;
            case 10:
                fixCamRoundaboutDetectionStartRow = fixCamRoundaboutDetectionStartRow + 1 * signData;
                sendParameterToCam(8,0xAA,0,fixCamRoundaboutDetectionStartRow,0,0);break;
            case 11:break;
            case 12:break;
            default:break;
            /*
             * Eg;sendParameterToCam(0,0xE1,abc,0,0,0);break;
             *    sendParameterToCam(8,0xE1,0,abc,0,0);break;
             *    sendParameterToCam(16,0xE1,0,0,abc,0);break;
             *    sendParameterToCam(32,0xE1,0,0,0,abc);break;
             */
        }
    }
    else {return;}
}

/*
*Pass variable data on the Fourth page
*/
uint8 ChooseNumber = 0;
void transfetFunctionFourth(int8 targetRow,char *targetBuff){
    if ((3 - menuY) == targetRow) {                //BLACK
        rt_sprintf(targetBuff,"ClearChoose                     ");
    }
    else if ((4 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"ChooseNumber=%01d                  ",ChooseNumber);
    }
    else if ((5 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"RoundAbout  :a|1                ");
    }
    else if ((6 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"ThreeWay    :b|2                ");
    }
    else if ((7 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"CrossRoad   :c|3                ");
    }
    else if ((8 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"Garage      :e|4                ");
    }
    else if ((9 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"Delay100    :f|5                ");
    }
    else if ((10 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"Delay200    :g|6                ");
    }
    else if ((11 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"Delay300    :h|7                ");
    }
    else if ((12 - menuY) == targetRow) {
        rt_sprintf(targetBuff,"SendElementTable                ");
    }
    else {
        rt_sprintf(targetBuff,"                                 ");
    }
}
/*
 * Create a table related to the order of the elements
 */

void createElementTable(uint8 element){
    if (elementTableLength < 9 && elementTable < 2000000000) {
        elementTable = elementTable * 10 + element;
        elementTableLength = elementTableLength + 1;
    }
    else if (elementTableLength < 18 && elementTableA < 2000000000) {
        elementTableA = elementTableA * 10 + element;
        elementTableLength = elementTableLength + 1;
    }
    else {
        elementTable = 0;elementTableLength = 0;elementTableA = 0;
    }
}

/*
 * Assign value to data on the Fourth page
 */

void assignValueFourth(void){
    if (confirmButton == 1) { //按下确认键才响应修改
        switch (menuY + 3) {
            case 3:elementTable = 0;elementTableLength = 0;elementTableA = 0;break;
            case 4:
                ChooseNumber = ChooseNumber + 1;
                if (ChooseNumber > 3) {ChooseNumber = 0;}
                if (ChooseNumber == 1) {elementTable = elementTable1;elementTableLength = elementTableLength1;}
                else if (ChooseNumber == 2) {elementTable = elementTable2;elementTableLength = elementTableLength2;}
                else if (ChooseNumber == 3) {elementTable = elementTable3;elementTableLength = elementTableLength3;}
                else{return;}break;
            case 5:createElementTable(1);break;
            case 6:createElementTable(2);break;
            case 7:createElementTable(3);break;
            case 8:createElementTable(4);break;
            case 9:createElementTable(5);break;
            case 10:createElementTable(6);break;
            case 11:createElementTable(7);break;
            case 12:
                sendParameterToCam(32,0xDB,0,0,0,elementTable); //告知顺序
                sendParameterToCam(32,0xCC,0,0,0,elementTableA); //告知顺序
                sendParameterToCam(8,0xDC,0,elementTableLength,0,0); //告知元素个数:长度
                break;
            default:break;
        }
    }
    else {return;}
}

/*
*Disaplay Menu
*/

uint8 maxMenuRow = 12;//下滑选择限制 最下面是多少就是多少
uint8 maxMenuPage = 3;//左右选择限制  total: 4 pages
char txt1[39]={0},txt2[39]={0},txt3[39]={0},txt4[39]={0}, //承载数组
                  txt5[39]={0},txt6[39]={0},txt7[39]={0};
void disaplayMenu(void){

/***********************状态栏*******************************************/
    if (menuX < 3) {
        rt_sprintf(txt1,"carF=%01d|Fg=%02d",car_flag,elementFlag);
        rt_sprintf(txt2,"Vc=%03d|AC=%02d                  ",Vc,accelerate);
    }
    else {
        rt_sprintf(txt1,"carF=%01d|Fg=%02d  ",car_flag,elementFlag);
        rt_sprintf(txt2,"N=%02d{%09d,%09d}",elementTableLength,elementTable,elementTableA);//显示传输元素队列 当menuX = 3 时候触发
    }
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
        assignValueThird(); //更改数值
        transfetFunctionThird(3,txt3); transfetFunctionThird(4,txt4);//传递显示
        transfetFunctionThird(5,txt5);transfetFunctionThird(6,txt6);transfetFunctionThird(7,txt7);
    }
    else if (menuX == 3) {
        assignValueFourth(); //更改数值
        transfetFunctionFourth(3,txt3); transfetFunctionFourth(4,txt4);//传递显示
        transfetFunctionFourth(5,txt5);transfetFunctionFourth(6,txt6);transfetFunctionFourth(7,txt7);
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
    switch (key_data) {
        case 1:
            if(confirmButton == 0) {menuX = menuX - 1;menuY = 0;} //无确认按键按下 则可翻页选择
            else if(confirmButton == 1){parameterAdjustButton = 1;}else{return;}break;//left 确认按键按下 进行参数调整
        case 4:
            if(confirmButton == 0) {menuX = menuX + 1;menuY = 0;}
            else if(confirmButton == 1){parameterAdjustButton = 4;}else {return;}break;//right

        case 3:
            if(confirmButton == 0) {menuY = menuY - 1;} //up    保持在按下确定键后不响应上下
            else if(confirmButton == 1){parameterAdjustButton = 0;confirmButton = 0;}else {return;}break;
        case 5:
            if(confirmButton == 0) {menuY = menuY + 1;} //down
            else if(confirmButton == 1){parameterAdjustButton = 0;confirmButton = 0;}else {return;}break;
        case 2://确定键
            if (confirmButton == 0) {confirmButton = 1;} else {confirmButton = 0;parameterAdjustButton = 0;}break;
        default:
            break;
    }
}

char txtcqupt[32]={0};
void display_entry(void *parameter)
{
    rt_sprintf(txtcqupt,"CQUPT                               ");
    ips114_showstrGRAY(0,0,txtcqupt);
    while(1)
    {
        disaplayMenu();
    }
}

rt_thread_t tidDisplay;
void display_init(void)
{
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


