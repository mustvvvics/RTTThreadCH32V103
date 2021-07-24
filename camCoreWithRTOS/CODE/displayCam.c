#include "headfile.h"

char txt[48]={0};
uint16 showVar;//遍历边线数组
uint16 showline = 0;//显示图像行
void show_speed(void)
{
/***************变量名 = 8个字母空缺用空格补齐;数值统一%05d**************************/
/************************************************************************************/
    if (turnPage == 0) {
        if (camFlashWriteFlag == 1) {
                 itoaChar(elementTableFromMain,elementQueue,10);
                 camFlashWriteFlag = 0;
             }
        rt_sprintf(txt,"CQUPT         |Page         %01d",turnPage);
        ips114_showstrGray(0, 0, txt);
//        ips114_showstrGray(50,0, elementTableChar);
        rt_sprintf(txt,"Error   =%05d|Direction=%04d",cameraError,drivingDirection);
        ips114_showstr(0, 1, txt);

        rt_sprintf(txt,"FgCElement=%03d|Accelerate=%03d",flagCameraElement,accelerateRatio);
        ips114_showstr(0, 2, txt);
        rt_sprintf(txt,"FgEntRound=%03d|FgEntThree=%03d", flagEnterRoundabout, flagEnterThreeWay);
        ips114_showstr(0, 3, txt);
        rt_sprintf(txt,"FgEntCross=%03d|FgRoudFini=%03d",flagEnterCrossroad,gyroRoundFinishFlag);
        ips114_showstr(0, 4, txt);
        rt_sprintf(txt,"FgEnStartL=%03d|FgOutBound=%03d", flagEnterStartLine,flagEnterOutbound);
        ips114_showstr(0, 5, txt);
        rt_sprintf(txt,"3WayFeaRow=%03d|3WayFeaNum=%03d",threewayFeatureRow,detectThreewayFeatureNum);
        ips114_showstr(0, 6, txt);
        rt_sprintf(txt,"Num=%02d|ElementTable=%09d ",elementTableLengthFromMain,elementTableFromMain);
        ips114_showstr(0, 7, txt);
    }
/************************************************************************************/
    else if (turnPage == 1) {
        rt_sprintf(txt,"CQUPT         |Page         %01d",turnPage);
        ips114_showstrGray(0, 0, txt);
//        ips114_showstrGray(50,0, elementTableChar);
        rt_sprintf(txt,"Error   =%05d|Time    =%05d",cameraError,timeControl);
        ips114_showstr(0, 1, txt);

        rt_sprintf(txt,"outboSum=%05d|StStatus=%05d",outboundAreaSum,steerStatusFromMain);
        ips114_showstr(0, 2, txt);
        rt_sprintf(txt,"areaRoLf=%05d|areaRoRi=%05d",areaDetectRoundaboutLeft,areaDetectRoundaboutRight);
        ips114_showstr(0, 3, txt);

        rt_sprintf(txt,"DCRMissNum=%03d|SpCurveRow=%03d",detectCrossroadMissingNumThres,rangeSharpCurveRow);
        ips114_showstr(0, 4, txt);
        rt_sprintf(txt,"CenterBias=%03d|JpPointNum=%03d",globalCenterBias,startlineJumpingPointNumThres);
        ips114_showstr(0, 5, txt);
        rt_sprintf(txt,"jitterLef=%04d|jitterRig=%04d",laneJitterLeft,laneJitterRight);
        ips114_showstr(0, 6, txt);
        rt_sprintf(txt,"                             ");
        ips114_showstr(0, 7, txt);
    }
    else if (turnPage == 2) {
        rt_sprintf(txt,"CQUPT         |Page         %01d",turnPage);
        ips114_showstrGray(0, 0, txt);
//        ips114_showstrGray(50,0, elementTableChar);
        rt_sprintf(txt,"Error   =%05d|carStart=%05d",cameraError,carStart);
        ips114_showstr(0, 1, txt);

        rt_sprintf(txt,"RAStartRow=%03d|              ",roundaboutDetectionStartRow);
        ips114_showstr(0, 2, txt);
        rt_sprintf(txt,"DtDistance=%03d|pixelMeanT=%03d",(int16)(detectDistance * 10),pixelMeanThres);
        ips114_showstr(0, 3, txt);

        rt_sprintf(txt,"SpRowStart=%03d|SlopRowEnd=%03d",slopeRowStart,slopeRowEnd);
        ips114_showstr(0, 4, txt);
        rt_sprintf(txt,"AboutLeft=%04d|AboutRigh=%04d",areaDetectRoundaboutThresLeft,areaDetectRoundaboutThresRight);
        ips114_showstr(0, 5, txt);
        rt_sprintf(txt,"              |               ");
        ips114_showstr(0, 6, txt);
//        ips114_showstr(0, 6, elementTableChar);
        rt_sprintf(txt,"                             ");
        ips114_showstr(0, 7, txt);
    }
    else if (turnPage == 3) {

        ips114_clear(BLACK);
        rt_sem_take(camera_sem, RT_WAITING_FOREVER);
        for (showVar = 0; showVar < imgRow; ++showVar) {
            ips114_drawpoint( (uint16)laneLocationLeft[showVar]+32,showline,YELLOW);
            ips114_drawpoint( (uint16)laneLocationLeft[showVar]+32,showline + 1,YELLOW);
            ips114_drawpoint((uint16)laneCenter[showVar]+32,showline,WHITE);
            ips114_drawpoint((uint16)laneCenter[showVar]+32,showline + 1,WHITE);
            ips114_drawpoint((uint16)laneLocationRight[showVar]+32,showline,YELLOW);
            ips114_drawpoint((uint16)laneLocationRight[showVar]+32,showline + 1,YELLOW);
            if (showline > 96) {showline = 0;showVar = 0;}
            else {showline = showline + 2;}
        }
    }
    else{return;}
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
