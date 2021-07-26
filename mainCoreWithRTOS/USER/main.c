#include "headfile.h"

int main(void)
{
    button_init();                          //按键初始化
    icm20602_init_spi();                    //陀螺仪初始化
    display_init();                         //屏幕显示初始化
    encoder_init();                         //编码器采集
    motor_init();                           //电机初始化
    pwm_init(PWM1_CH1_A8, 50, 338);         //舵机初始化   默认在中位上       舵机 TIMER1  338向前; 672向左 ; 1000向后
//    esp8266Init();                        //8266线程  使用串口2         <比赛关闭>
    PID_Init();                             //PID参数初始化
    timer_EncoderCounter_init();            //里程计发送定时器初始化
    timer_pitAdc_init();                    //初始化ADC电压采集  使用了一个定时器 <比赛关闭>

    mainFlashRead();                        //读flash

    //串口3初始化
    uart_init(UART_3, 921600, UART3_TX_B10, UART3_RX_B11);      //串口3初始化 波特率115200
    //开串口3中断
    uart_rx_irq(UART_3, ENABLE);                                //默认抢占优先级1 次优先级0。
    nvic_init((IRQn_Type)(53 + UART_3), 0, 0, ENABLE);          //将串口3的抢占优先级设置为最高 次优先级设置为最高。
    //开外部中断
    gpio_interrupt_init(B2, RISING, GPIO_INT_CONFIG);           //B2 //初始化为GPIO //上升沿触发
    nvic_init(EXTI2_IRQn, 1, 1, ENABLE);                        //EXTI2  //优先级配置  抢占优先级1 次优先级1

    while(1)
    {
        rt_thread_mdelay(2);                //new delay 必须保留
        if (sendMainFlashToCam == 1) {      //上电给cam核发送保存在flash里的信息
            sendFlashDataToCam();           //communication 中无元素时清空flag
        }
    }
}

