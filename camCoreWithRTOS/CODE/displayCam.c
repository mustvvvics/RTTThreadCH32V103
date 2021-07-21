#include "headfile.h"

char txt[48]={0};
void show_speed(void)
{
/***************������ = 8����ĸ��ȱ�ÿո���;��ֵͳһ%05d**************************/
/************************************************************************************/
    if (turnPage == 0) {
        rt_sprintf(txt,"CQUPT         |Page         %01d",turnPage);
        ips114_showstrGray(0, 0, txt);
        rt_sprintf(txt,"Error   =%05d|Time    =%05d",cameraError,timeControl);
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
    }
/************************************************************************************/
    else if (turnPage == 1) {
        rt_sprintf(txt,"CQUPT         |Page         %01d",turnPage);
        ips114_showstrGray(0, 0, txt);
        rt_sprintf(txt,"Error   =%05d|Time    =%05d",cameraError,timeControl);
        ips114_showstr(0, 1, txt);

        rt_sprintf(txt,"outboSum=%05d|              ",outboundAreaSum);
        ips114_showstr(0, 2, txt);
        rt_sprintf(txt,"areaRoLf=%05d|areaRoRi=%05d",areaDetectRoundaboutLeft,areaDetectRoundaboutRight);
        ips114_showstr(0, 3, txt);

        rt_sprintf(txt,"DCRMissNum=%03d|SpCurveRow=%03d",detectCrossroadMissingNumThres,rangeSharpCurveRow);
        ips114_showstr(0, 4, txt);
        rt_sprintf(txt,"CenterBias=%03d|JpPointNum=%03d",globalCenterBias,startlineJumpingPointNumThres);
        ips114_showstr(0, 5, txt);
        rt_sprintf(txt,"              |              ");
        ips114_showstr(0, 6, txt);
    }
    else if (turnPage == 2) {
        rt_sprintf(txt,"CQUPT         |Page         %01d",turnPage);
        ips114_showstrGray(0, 0, txt);
        rt_sprintf(txt,"Error   =%05d|Time    =%05d",cameraError,timeControl);
        ips114_showstr(0, 1, txt);

        rt_sprintf(txt,"RAStartRow=%03d|              ",roundaboutDetectionStartRow);
        ips114_showstr(0, 2, txt);
        rt_sprintf(txt,"DtDistance=%03d|pixelMeanT=%03d",(int16)(detectDistance * 10),pixelMeanThres);
        ips114_showstr(0, 3, txt);

        rt_sprintf(txt,"SpRowStart=%03d|SlopRowEnd=%03d",slopeRowStart,slopeRowEnd);
        ips114_showstr(0, 4, txt);
        rt_sprintf(txt,"RAboutLeft=%03d|RAboutRigh=%03d",areaDetectRoundaboutThresLeft,areaDetectRoundaboutThresRight);
        ips114_showstr(0, 5, txt);
        rt_sprintf(txt,"              |              ");
        ips114_showstr(0, 6, txt);
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

    //��ʼ����Ļ
    ips114_init();
    
    //������ʾ�߳� ���ȼ�����Ϊ5
    tid1 = rt_thread_create("display", display_entry, RT_NULL, 1024, 5, 50);
    
    //������ʾ�߳�
    if(RT_NULL != tid1)
    {
        rt_thread_startup(tid1);
    }
}
