#include "headfile.h"

uint16_t temp_ADC_Value;//�ݴ���DMA��ADCͨ��ת��������ֵ

float vbat_ad;//��ص�ѹ����

char txt[32];
int16 Vc = 0;
int16 VcPreviously = 0;
int16 VcDifference;
void getAdc(void){
    u8 i;
    u32 sum;
    static uint16_t adc[10];//��������ں������棬������Ҫ�ĳɾ�̬�ֲ�����static uint16_t adc[10];
    for(i=1;i<10;i++){  //FIFO ����
        adc[i-1]=adc[i];
    }
    adc[9]=adc_convert(ADC_IN9_B1,ADC_12BIT);;//���µ����ݷ��õ� ���ݵ������
    sum=0;
    for(i=0;i<10;i++){
        sum+=adc[i];
    }
    vbat_ad=(float)(sum/10) / 4096 * 3.3f * 6.07f ;
    //���巽�����Ȳ��˱�������ʾADC������ԭʼ��ѹ(0~3.3V) Ȼ���õ�������ص�ѹ�����õ���õĵ�ѹ��ȥADCʵ�ʵ�ѹ�õ��������Ϳ��Եõ���ӽ���ʵֵ�ķŴ���
    Vc = (int16)(vbat_ad * 100);
    VcDifference = VcPreviously - Vc;
    VcPreviously = Vc;
    if (VcDifference > 0) {
        if (Vc >= 710 && Vc <= 750) {
            ips114_clear(RED);
        }
    }
}

void timer1_pitAdc_entry(void *parameter)
{
    getAdc();
}
rt_timer_t timerAdc;

void timer_pitAdc_init(void)
{

    adc_init(ADC_IN9_B1); //��ʼ��ADC����

    //����һ����ʱ�� ��������
    timerAdc = rt_timer_create("timerAdc", timer1_pitAdc_entry, RT_NULL, 2000, RT_TIMER_FLAG_PERIODIC);

    //������ʱ��
    if(RT_NULL != timerAdc)
    {
        rt_timer_start(timerAdc);
    }
}

void timer1_EncoderCounter_entry(void *parameter)
{
    sendEncoderCounterNum();//������̼�����
}
rt_timer_t timerEncoderCounter;

void timer_EncoderCounter_init(void)
{
    //����һ����ʱ�� ��������
    timerEncoderCounter = rt_timer_create("timerEncoderCounter", timer1_EncoderCounter_entry, RT_NULL, 4, RT_TIMER_FLAG_PERIODIC);

    //������ʱ��
    if(RT_NULL != timerEncoderCounter)
    {
        rt_timer_start(timerEncoderCounter);
    }
}
