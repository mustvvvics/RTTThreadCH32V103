#include "headfile.h"

rt_sem_t esp8266_sem;

int32 Int2Float;                    //传递更改变量的数值
void ESP8266_Clear(void)            //清空缓冲区
{
    memset(esp8266_buf, 0, sizeof(esp8266_buf));
    esp8266_cnt = 0;
}

void Esp_Init(void)
{
    //初始化串口1 将原先给tshell 以及debug的串口更改
//    uart_init(UART_1,921600,UART1_TX_A9,UART1_RX_A10);//    UART2_TX_A2 = 0x10,     UART2_RX_A3,    //默认引脚 原921600
//
//    //使能串口接收中断
//    uart_rx_irq(UART_1,ENABLE);

    //初始化复位引脚
    gpio_init(A11, GPO, 1, GPIO_PIN_CONFIG);

    gpio_set(A11,0);
    systick_delay_ms(250);
    gpio_set(A11,1);
    systick_delay_ms(500);

    ESP8266_Clear();
}

void Tcp_Decode(void)
{

    if(esp8266_buf[esp8266_cnt-1] != 0x0A)return;

    if(strcmp((char *)esp8266_buf,"init\r\n") == 0){
        uart_putstr(UART_1,"#0007camera\n");
    }
    else if(strcmp((char *)esp8266_buf,"ClearElement\r\n") == 0){
        flagEnterRoundabout=0;flagEnterThreeWay=0;flagEnterOutbound=0;
        uart_putstr(UART_1,"#0008Cleard!\n");
    }
    else if(strcmp((char *)esp8266_buf,"ShowCamera\r\n") == 0){
        CameraShow_flag = 1;
    }
    //int32 pixelMeanThres "laTh:%d"
    else if(esp8266_buf[2] == 'T' && esp8266_buf[3] == 'h')
    {
        pixelMeanThres = 100 * (esp8266_buf[5] - '0') + 10 * (esp8266_buf[6] - '0') + (esp8266_buf[7] - '0');
        uart_putstr(UART_1,"#0025received_pixelMeanThres!\n");
    }
    //float detectDistance   "laDs:%d"
    else if(esp8266_buf[2] == 'D' && esp8266_buf[3] == 's')
    {
        Int2Float = 10 * (esp8266_buf[5] - '0') + (esp8266_buf[6] - '0');
        detectDistance=(float)Int2Float*0.1f;
        uart_putstr(UART_1,"#0025received_detectDistance!\n");
    }
    //"slopeRow:11-22"
    else if(esp8266_buf[2] == 'o' && esp8266_buf[3] == 'p')
    {
        slopeRowStart =(uint8)(10 * (esp8266_buf[9] - '0') + (esp8266_buf[10] - '0'));
        slopeRowEnd =(uint8)(10 * (esp8266_buf[12] - '0') + (esp8266_buf[13] - '0'));
        uart_putstr(UART_1,"#0010slopeRow!\n");
    }
    ESP8266_Clear();
}

const char* message0 = ",";
const char* message1 = "\n";
void sendMessage(void) {
    uint16 ii;
    char txtA[6],txtB[6],txtC[3],txtD[40];
    uart_putstr(UART_1,"#0995");                    //帧头 实际测试得出

    for (ii = 0; ii < imgRow; ++ii) {
        rt_sprintf(txtA,"%04d ",laneLocationLeft[ii]);//5
        uart_putstr(UART_1,txtA);
    }
    uart_putstr(UART_1,message0);//1

    for (ii = 0; ii < imgRow; ++ii) {
        rt_sprintf(txtA,"%04d ",laneLocationRight[ii]);//5
        uart_putstr(UART_1,txtA);
    }
    uart_putstr(UART_1,message0);//1

    for (ii = 0; ii < imgRow; ++ii) {
        rt_sprintf(txtB,"%04d ",laneCenter[ii]);//5
        uart_putstr(UART_1,txtB);
    }
    uart_putstr(UART_1,message0);//1

    for (ii = 0; ii < imgRow; ++ii) {
        rt_sprintf(txtC,"%01d ",flagDetectLeft[ii]);//3
        uart_putstr(UART_1,txtC);
    }
    uart_putstr(UART_1,message0);//1

    for (ii = 0; ii < imgRow; ++ii) {
        rt_sprintf(txtC,"%01d ",flagDetectRight[ii]);//3
        uart_putstr(UART_1,txtC);
    }
    uart_putstr(UART_1,message0);//1
//    rt_thread_mdelay(50);//new delay

    rt_sprintf(txtD,"%05d %03d %02d %02d %04d %04d %06d %06d",cameraError, sharpCurveRow,
            flagEnterRoundabout, flagEnterThreeWay, laneJitterLeft, laneJitterRight,
            areaDetectRoundaboutLeft, areaDetectRoundaboutRight);   //相关变量
    uart_putstr(UART_1,txtD);   //1
    uart_putstr(UART_1,message1);//1
}

void esp8266Entry(void *parameter)
{
    rt_sem_take(esp8266_sem, RT_WAITING_FOREVER);
//    esp8266_buf[esp8266_cnt-3] = '\0';//消除显示乱码
    esp8266_buf[esp8266_cnt-2] = '\0';//消除显示乱码
//    esp8266_buf[esp8266_cnt-1] = '\0';//消除显示乱码
    ips114_showstr(0,7,esp8266_buf);
    ESP8266_Clear();
    while(1)
    {
        rt_sem_take(esp8266_sem, RT_WAITING_FOREVER);
        Tcp_Decode();
    }

}

void esp8266Init(void)
{
    rt_thread_t tidEsp8266;

    //初始化
    Esp_Init();

    esp8266_sem = rt_sem_create("esp8266", 0, RT_IPC_FLAG_FIFO);

    //创建线程
    tidEsp8266 = rt_thread_create("esp8266", esp8266Entry, RT_NULL, 1024, 2, 200);//stack_size 优先级     心跳

    //启动线程
    if(RT_NULL != tidEsp8266)
    {
        rt_thread_startup(tidEsp8266);
    }
}

//const char* message0 = ",";
//const char* message1 = "\n";

void sendMessageData(void) {                //发送数据曲线进行分析
    char txtA[64];
//    rt_sprintf(txtA,"ThreeFeaN:%d",detectThreewayFeatureNum );
//    uart_putstr(UART_1,txtA);uart_putstr(UART_1,message0);
//
//    rt_sprintf(txtA,"flagEnt:%d",flagEnterRoundabout );
//    uart_putstr(UART_1,txtA);uart_putstr(UART_1,message0);
//
//    rt_sprintf(txtA,"flagEntTH:%d",flagEnterThreeWay );
//    uart_putstr(UART_1,txtA);uart_putstr(UART_1,message0);

//    rt_sprintf(txtA,"flIRo:%d, 3Xsum:% 2d, width:% 3d, ratio:%d",flagEnterRoundabout, detectThreewayFeatureNum, detectThreewayFeatureNearestRowWidth, (int16)(detectThreewayFeatureNearestRowRatio * 100));
//    if (leftStartFlagThreewayFeatureFound && rightStartFlagThreewayFeatureFound) {
//        if (leftStartThreewayFeatureJumpPointLeft-rightStartThreewayFeatureJumpPointLeft < 5 && \
//                leftStartThreewayFeatureJumpPointLeft-rightStartThreewayFeatureJumpPointLeft >-5) {
//            if(leftStartThreewayFeatureJumpPointRight-rightStartThreewayFeatureJumpPointRight < 5 && \
//                    leftStartThreewayFeatureJumpPointRight-rightStartThreewayFeatureJumpPointRight > -5) {
//                rt_sprintf(txtA,"Ro:%2d,LBi:%3d, RBi:%3d, LWi:%3d, RWi:%3d,sum:%d",threewayFeatureRow, leftStartThreewayFeatureJumpPointLeft-rightStartThreewayFeatureJumpPointLeft,leftStartThreewayFeatureJumpPointRight-rightStartThreewayFeatureJumpPointRight,
//                        leftStartThreewayFeatureJumpPointLeft-leftStartThreewayFeatureJumpPointRight,rightStartThreewayFeatureJumpPointLeft-rightStartThreewayFeatureJumpPointRight, threewayFeatureWidthSum);
//                uart_putstr(UART_1,txtA);
//                uart_putstr(UART_1,message1);
//            }
//        }
//    }
    if (laneWidthSlopeLeft > 0 && laneWidthSlopeRight < 0) {
        rt_sprintf(txtA,"widLef:%03d, widRig:%03d", laneWidthSlopeLeft, laneWidthSlopeRight);
        uart_putstr(UART_1,txtA);
        uart_putstr(UART_1,message1);
    }

}
