#include "headfile.h"

int main(void)
{
    button_init();                          //������ʼ��
    icm20602_init_spi();                    //�����ǳ�ʼ��
    display_init();                         //��Ļ��ʾ��ʼ��
    encoder_init();                         //�������ɼ�
    motor_init();                           //�����ʼ��
    pwm_init(PWM1_CH1_A8, 50, 338);         //�����ʼ��   Ĭ������λ��       ��� TIMER1  338��ǰ; 672���� ; 1000���
//    esp8266Init();                        //8266�߳�  ʹ�ô���2         <�����ر�>
    PID_Init();                             //PID������ʼ��
    timer_EncoderCounter_init();            //��̼Ʒ��Ͷ�ʱ����ʼ��
    timer_pitAdc_init();                    //��ʼ��ADC��ѹ�ɼ�  ʹ����һ����ʱ�� <�����ر�>

    mainFlashRead();                        //��flash

    //����3��ʼ��
    uart_init(UART_3, 921600, UART3_TX_B10, UART3_RX_B11);      //����3��ʼ�� ������115200
    //������3�ж�
    uart_rx_irq(UART_3, ENABLE);                                //Ĭ����ռ���ȼ�1 �����ȼ�0��
    nvic_init((IRQn_Type)(53 + UART_3), 0, 0, ENABLE);          //������3����ռ���ȼ�����Ϊ��� �����ȼ�����Ϊ��ߡ�
    //���ⲿ�ж�
    gpio_interrupt_init(B2, RISING, GPIO_INT_CONFIG);           //B2 //��ʼ��ΪGPIO //�����ش���
    nvic_init(EXTI2_IRQn, 1, 1, ENABLE);                        //EXTI2  //���ȼ�����  ��ռ���ȼ�1 �����ȼ�1

    while(1)
    {
        rt_thread_mdelay(2);                //new delay ���뱣��
        if (sendMainFlashToCam == 1) {      //�ϵ��cam�˷��ͱ�����flash�����Ϣ
            sendFlashDataToCam();           //communication ����Ԫ��ʱ���flag
        }
    }
}

