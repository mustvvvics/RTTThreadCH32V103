
#include "headfile.h"

rt_sem_t esp8266_sem;

int32 Int2Float;
int8 display_is_working = 0;
void ESP8266_Clear(void)
{
    memset(esp8266_buf, 0, sizeof(esp8266_buf));
    esp8266_cnt = 0;
}

void Esp_Init(void)
{
    //初始化串口2
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

//    char txt[32]={0};

    while (display_is_working) {rt_thread_mdelay(20);}

    if(strcmp((char *)esp8266_buf,"init\n") == 0){
        uart_putstr(UART_1,"#0007camera\n");
    }
    else if(strcmp((char *)esp8266_buf,"ClearElement\n") == 0){
        flagEnterRoundabout=0;flagEnterThreeWay=0;
        uart_putstr(UART_1,"#0008Cleard!\n");
    }
    else if(strcmp((char *)esp8266_buf,"ShowCamera\n") == 0){
        CameraShow_flag = 1;
    }
    //laTh:19775\n
    //int32 pixelMeanThres
    else if(esp8266_buf[2] == 'T' && esp8266_buf[3] == 'h')
    {
//        sscanf(esp8266_buf,"laTh:%d",&pixelMeanThres);
        pixelMeanThres = 100 * (esp8266_buf[5] - '0') + 10 * (esp8266_buf[6] - '0') + (esp8266_buf[7] - '0');
        uart_putstr(UART_1,"#0025received_pixelMeanThres!\n");
    }
    //laDs:19775\n
    //float detectDistance
    else if(esp8266_buf[2] == 'D' && esp8266_buf[3] == 's')
    {
//        sscanf(esp8266_buf,"laDs:%d",&Int2Float);
        Int2Float = 10 * (esp8266_buf[5] - '0') + (esp8266_buf[6] - '0');
        detectDistance=(float)Int2Float*0.1f;
        uart_putstr(UART_1,"#0025received_detectDistance!\n");
    }
    gpio_set(B15, 1);

    ESP8266_Clear();
}



const char* message0 = ",";
const char* message1 = "\n";
void sendMessage(void) {
    uint16 ii;
    char txtA[6],txtB[6],txtC[3],txtD[40];

    uart_putstr(UART_1,"#0995"); //实际测试得出

    //5+5+5+3+3 * 50 = 1050
    //1+1+1+1+1 = 5
    //1050 + 5 = 1055
    for (ii = 0; ii < imgRow; ++ii) {
        sprintf(txtA,"%04d ",laneLocationLeft[ii]);//5
        uart_putstr(UART_1,txtA);
    }

    uart_putstr(UART_1,message0);//1
    rt_thread_mdelay(20);//new delay

    for (ii = 0; ii < imgRow; ++ii) {
        sprintf(txtA,"%04d ",laneLocationRight[ii]);//5
        uart_putstr(UART_1,txtA);
    }

    uart_putstr(UART_1,message0);//1
    rt_thread_mdelay(20);//new delay

    for (ii = 0; ii < imgRow; ++ii) {
        sprintf(txtB,"%04d ",laneCenter[ii]);//5
        uart_putstr(UART_1,txtB);
    }

    uart_putstr(UART_1,message0);//1
    rt_thread_mdelay(20);//new delay

    for (ii = 0; ii < imgRow; ++ii) {
        sprintf(txtC,"%01d ",flagDetectLeft[ii]);//3
        uart_putstr(UART_1,txtC);
    }

    uart_putstr(UART_1,message0);//1
    rt_thread_mdelay(20);//new delay

    for (ii = 0; ii < imgRow; ++ii) {
        sprintf(txtC,"%01d ",flagDetectRight[ii]);//3
        uart_putstr(UART_1,txtC);
    }

    uart_putstr(UART_1,message0);//1
    rt_thread_mdelay(20);//new delay

    //5+1+5+1+3+1+3+1+2+1+2+1+6+1+6=39
//    sprintf(txtD,"%05d %05d %03d %03d %02d %02d %06d %06d",cameraError,slope,sharpCurveRow
//            ,missCounter,flagEnterRoundabout,flagThreeWay,(int16)(laneJitterLeft*100),(int16)(laneJitterRight*100));   //相关变量
    //5+1+5+1+3+1+3+1+3+1+3+1+2+1+2+1+6+1+6=47
//    uart_putstr(UART_1,txtD);
    sprintf(txtD,"%05d %03d %02d %02d %04d %04d %06d %06d",cameraError, sharpCurveRow,
            flagEnterRoundabout, flagEnterThreeWay, laneJitterLeft, laneJitterRight, areaDetectRoundaboutLeft, areaDetectRoundaboutRight);   //相关变量
    //5+1+3+1+2+1+2+1+4+1+4+1+6+1+6 =39 + 1
    uart_putstr(UART_1,txtD);
    //1
    uart_putstr(UART_1,message1);////结束也算1个

    //39 + 1 = 40
    //1055 + 40 = 1095

    //47+1 = 48
    //1055 + 48 = 1103
}

void esp8266Entry(void *parameter)
{
    rt_sem_take(esp8266_sem, RT_WAITING_FOREVER);
    display_is_working = 1;
//    esp8266_buf[esp8266_cnt-3] = '\0';//消除显示乱码
    esp8266_buf[esp8266_cnt-2] = '\0';//消除显示乱码
//    esp8266_buf[esp8266_cnt-1] = '\0';//消除显示乱码
    oled_p6x8str(0,7,esp8266_buf);
    display_is_working = 0;
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
    tidEsp8266 = rt_thread_create("esp8266", esp8266Entry, RT_NULL, 1024, 2, 200);//倒数 心跳 优先级 stack_size

    //启动线程
    if(RT_NULL != tidEsp8266)
    {
        rt_thread_startup(tidEsp8266);
    }
}

