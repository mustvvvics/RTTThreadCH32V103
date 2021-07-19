#include "headfile.h"

char txt[48]={0};
void show_speed(void)
{
//    rt_sprintf(txt,"l_f=%05d|r_f=%05d",encoder_left_rear,encoder_left_front);
//    ips114_showstr(0, 0, txt);//slopeRowStart
//    rt_sprintf(txt,"Start=%03d|End=%03d|t=%03d",slopeRowStart,slopeRowEnd,(int16)timeControl);
//    ips114_showstr(0, 0, txt);
    rt_sprintf(txt,"time=% 3d", timeControl);
    ips114_showstr(0, 0, txt);
//    rt_sprintf(txt,"coJiBRL=%2d, coJiBRR=%2d",countJitterBreakRowLeft,countJitterBreakRowRight);
    rt_sprintf(txt,"laLoShLo=%4d|laLoShUp=%4d", laneLocationShiftedLower, laneLocationShiftedUpper);
    ips114_showstr(0, 1, txt);
    rt_sprintf(txt,"Err=%05d|Fg=%01d|AC=%02d",cameraError,flagCameraElement,accelerateRatio);
    ips114_showstr(0, 3, txt);
    rt_sprintf(txt, "flagSta=%1d|flagOut=%1d", flagEnterStartLine, flagEnterOutbound);
    ips114_showstr(0, 4, txt);
    rt_sprintf(txt, "threFeaNu=%2d|entCross=%d", detectThreewayFeatureNum, flagEnterCrossroad);
    ips114_showstr(0, 5, txt);
    rt_sprintf(txt, "entRou=%2d|entThree=%1d", flagEnterRoundabout, flagEnterThreeWay);
    ips114_showstr(0, 6, txt);
//    rt_sprintf(txt,"bothMisNum=%2d, flEntRou=%2d",bothMissingNum, flagEnterRoundabout);
//    ips114_showstr(0, 4, txt);
////    rt_sprintf(txt,"pMeanT=%03d,Dis=%03d",pixelMeanThres,(int16)(detectDistance*10));
////    ips114_showstr(0, 2, txt);
////    rt_sprintf(txt,"LJL=%04d|LJR=%4d",laneJitterLeft, laneJitterRight);
////    ips114_showstr(0, 3, txt);
    rt_sprintf(txt,"areaRoL=%4d|exRouDe=%4d    ",areaDetectRoundaboutLeft, exitRoundaboutDelay);
    ips114_showstr(0, 2, txt);

//
//    rt_sprintf(txt,"gyroflag=%05d",gyroRoundFinishFlag);
//    ips114_showstr(0, 2, txt);

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
