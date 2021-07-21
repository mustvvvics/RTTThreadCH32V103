#include "headfile.h"

rt_sem_t esp8266_sem;

void ESP8266_Clear(void)
{
    memset(esp8266_buf, 0, sizeof(esp8266_buf));
    esp8266_cnt = 0;
}

void Esp_Init(void)
{
    //初始化串口2
    uart_init(UART_2,921600,UART2_TX_A2,UART2_RX_A3);//    UART2_TX_A2 = 0x10,     UART2_RX_A3,    //默认引脚 原921600

    //使能串口接收中断
    uart_rx_irq(UART_2,ENABLE);

    //初始化复位引脚
    gpio_init(A4, GPO, 1, GPIO_PIN_CONFIG);

    gpio_set(A4,0);
    systick_delay_ms(250);
    gpio_set(A4,1);
    systick_delay_ms(500);

    ESP8266_Clear();
}

int32 recevePidData(int16 receiveWay){
    int32 pidData = 0;
    if (receiveWay == 100) {
        pidData = 100 * (esp8266_buf[9] - '0') + 10 * (esp8266_buf[10] - '0')
                        + (esp8266_buf[11] - '0');
    }
    else if (receiveWay == 1000) {
        pidData = 1000 * (esp8266_buf[9] - '0') + 100 * (esp8266_buf[10] - '0')
                + 10 * (esp8266_buf[11] - '0') + (esp8266_buf[12] - '0');
    }
    return pidData;
}

void Tcp_Decode(void)
{
    if(esp8266_buf[esp8266_cnt-1] != 0x0A)return;

    if(strcmp((char *)esp8266_buf,"init\r\n") == 0) { //
        uart_putstr(UART_2,"#0008control\n");
    }
    /**************************************************************************/
    //启停
    else if(strcmp((char *)esp8266_buf,"car_go\r\n") == 0){
        car_flag = 1;
    }

    else if (strcmp((char *)esp8266_buf,"car_stop\r\n") == 0) {
        car_flag =0;
        clearError();//清除累计误差
    }

    else if (strcmp((char *)esp8266_buf,"fuzzy\r\n") == 0) {
        sendFuzzyData();
    }
    /**************************************************************************/
    //参数修改 调速
    else if(esp8266_buf[4] == 'S' && esp8266_buf[0] == 'C')
    {
        expected_y = 100 * (esp8266_buf[10] - '0') + 10 * (esp8266_buf[11] - '0') + (esp8266_buf[12] - '0');
    }

    else if(strcmp((char *)esp8266_buf,"ThreeWay\r\n") == 0) //三叉测试
    {ThreeWayIntersectionFlag = 1;uart_putstr(UART_2,"#ThreeWay!\n");}

    ESP8266_Clear();
}

const char* message0 = ",";
const char* message1 = "\n";

void sendFuzzyData(void){
  char txt[10];
  uart_putstr(UART_2,"-");
  for(int i = 0;i<8;i++){
    rt_sprintf(txt,"%d",fuzzy_txt[i]);
    if(i==0 || i == 2 || i == 4 || i == 6){
        uart_putstr(UART_2,"[");
        uart_putstr(UART_2,txt);
        uart_putstr(UART_2,".");
    }
    else{
        uart_putstr(UART_2,txt);
        uart_putstr(UART_2,"]");
    }
  }
}

void esp8266Entry(void *parameter)
{
    rt_sem_take(esp8266_sem, RT_WAITING_FOREVER);
    esp8266_buf[esp8266_cnt-2] = '\0'; //消除显示乱码
    ips114_showstrGRAY(45,0,esp8266_buf); //显示在标题栏目
    ESP8266_Clear();
    while(1)
    {
        rt_sem_take(esp8266_sem, RT_WAITING_FOREVER);
        Tcp_Decode();
    }
}
rt_thread_t tidEsp8266;
void esp8266Init(void)
{
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
