#include "headfile.h"

char txt[48]={0};
void show_speed(void)
{
/***************变量名 = 8个字母空缺用空格补齐;数值统一%05d**************************/
/************************************************************************************/
    if (turnPage == 0) {
        rt_sprintf(txt,"CQUPT         |Page         %01d",turnPage);
        ips114_showstrGray(0, 0, txt);
        rt_sprintf(txt,"Error   =%05d|Time    =%05d",cameraError,timeControl);
        ips114_showstr(0, 1, txt);
        rt_sprintf(txt,"FgElemen=%05d|Accelera=%05d",flagCameraElement,accelerateRatio);
        ips114_showstr(0, 2, txt);
        rt_sprintf(txt,"outboSum=%05d|3WayFNum=%05d", outboundAreaSum,detectThreewayFeatureNum);
        ips114_showstr(0, 3, txt);
        rt_sprintf(txt,"areaRoLf=%05d|exRouDey=%05d",areaDetectRoundaboutLeft, exitRoundaboutDelay);
        ips114_showstr(0, 4, txt);
        rt_sprintf(txt,"Test16  =%05d|turnPage=%05d",parameterTest16,turnPage);
        ips114_showstr(0, 5, txt);

    }
/************************************************************************************/
    else if (turnPage == 1) {
        rt_sprintf(txt,"CQUPT         |Page         %01d",turnPage);
        ips114_showstrGray(0, 0, txt);
        rt_sprintf(txt,"Error   =%05d|Time    =%05d",cameraError,timeControl);
        ips114_showstr(0, 1, txt);
        rt_sprintf(txt,"Test16  =%05d|turnPage=%05d",parameterTest16,turnPage);
        ips114_showstr(0, 2, txt);
        rt_sprintf(txt,"flagStaL=%05d|flagOutB=%05d", flagEnterStartLine,flagEnterOutbound);
        ips114_showstr(0, 3, txt);
        rt_sprintf(txt,"3WayFNum=%05d|EntCross=%05d", detectThreewayFeatureNum,flagEnterCrossroad);
        ips114_showstr(0, 4, txt);
        rt_sprintf(txt,"FgentRou=%05d|entThree=%05d", flagEnterRoundabout, flagEnterThreeWay);
        ips114_showstr(0, 5, txt);
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
