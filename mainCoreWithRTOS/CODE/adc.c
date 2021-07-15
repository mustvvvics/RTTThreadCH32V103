#include "headfile.h"

uint16_t temp_ADC_Value;//�ݴ���DMA��ADCͨ��ת��������ֵ

float vbat_ad;//��ص�ѹ����

void DMA_Tx_Init( DMA_Channel_TypeDef* DMA_CHx, u32 ppadr, u32 memadr, u16 bufsize)
{
    DMA_InitTypeDef DMA_InitStructure;

    //ʹ��DMA1����ʱ��
    RCC_AHBPeriphClockCmd( RCC_AHBPeriph_DMA1, ENABLE );

    //DMA������ȱʡֵ
    DMA_DeInit(DMA_CHx);
    //ADC��ַ
    DMA_InitStructure.DMA_PeripheralBaseAddr = ppadr;
    //�ڴ��ַ
    DMA_InitStructure.DMA_MemoryBaseAddr = memadr;
    //�����赽�ڴ�
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    //�������ݵĴ�С
    DMA_InitStructure.DMA_BufferSize = bufsize;
    //�����ַ�̶�ѡDisable ����ж�����裬ʹ��Enable������ֻ��ADC1������ϵ
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    //DMA���ʶ���ڴ����
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    //�������ݵ�λ
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    //�ڴ����ݵ�λ
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    //DMAģʽ �������������ϵ�ѭ��ģʽ����ֻ�����һ�κ�Ͳ�Ҫ������
    //����ָ����������ʣ�Ҳ������Ҫ�����ʵ�ʱ��ͷ��ʣ���Ҫ�����ʵ�ʱ���ֹͣ�����������ó�ͨ��ģʽ
    //����Ϊѭ������
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    //����DMA���ȼ� ���ȼ���
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    //��ֹ�ڴ浽�ڴ洫��
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    //����DMAx ͨ��x
    DMA_Init( DMA_CHx, &DMA_InitStructure );
}

void ADC_Function_Init(void)
{
    ADC_InitTypeDef ADC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    //����ʱ��ʹ��
    //B1
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE );
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE );
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);//��Ƶ80Mhz 8��Ƶ�൱��10Mhz ADC������������

    //ADC������������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    ADC_DeInit(ADC1);//��дȱʡֵ
    //����ADCģʽ,�����ģʽ�£�˫ADC����ͬ����ÿ��ADC�ӿڶ��������������������ҪADCͬ������ֻ������һ��ADC��ʱ�򣬾�Ӧ����ɶ���ģʽ�ˡ�
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    //ֻ������һ��ͨ���Ļ���DISABLE�Ϳ����ˣ����ʹ���˶��ͨ���Ļ�������뽫������ΪENABLE
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    //����ת��ֱ�����е�����ת����ɺ��ֹͣת����������ת����ֻת��һ�����ݾ�ֹͣ��Ҫ�ٴδ���ת���ſ��ԡ����������Ҫһ���Բɼ�1024�����ݻ��߸��࣬���������ת��
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;

    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//������� ���ж�ʱ���������ⲿ�жϴ���
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���
    //ͨ����������Ҫ�ǵ����ͨ���ɼ����ݵĻ��͵�����һ����������������ڹ���ͨ��������ú�����Ҳ������ͨ����˳����һ��
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);

    //ʹ��ADC��DMA
    ADC_DMACmd(ADC1, ENABLE);
    ADC_Cmd(ADC1, ENABLE);

    //�Զ�ת��У׼
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));

    //����DMAת������
    DMA_Tx_Init(DMA1_Channel1,(u32)&ADC1->RDATAR,(u32)temp_ADC_Value,1);//���ڷ��ý��ձ���
    //ʹ��DMAͨ��1
    DMA_Cmd( DMA1_Channel1, ENABLE );
    //ADC1 ͨ��2 rank 1 ������������� 239.5 cycles
    //��Ϊ��2��ͨ��������rank = 2
    //��ADC
    ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 1, ADC_SampleTime_239Cycles5 );
    //����û���ⲿ������ʱ��������ʹ���������ADCת��
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}


char txt[32];
void getAdc(void){
    u8 i;
    u32 sum;
    static uint16_t adc[10];//��������ں������棬������Ҫ�ĳɾ�̬�ֲ�����static uint16_t adc[10];
//    for(;;){
    for(i=1;i<10;i++){  //FIFO ����
        adc[i-1]=adc[i];
    }
    adc[9]=temp_ADC_Value;//���µ����ݷ��õ� ���ݵ������

    sum=0;
    for(i=0;i<10;i++){
        sum+=adc[i];
    }
    vbat_ad=(float)(sum/10) / 4096 * 3.3f * 1
            ;//���巽�����Ȳ��˱�������ʾADC������ԭʼ��ѹ(0~3.3V) Ȼ���õ�������ص�ѹ�����õ���õĵ�ѹ��ȥADCʵ�ʵ�ѹ�õ��������Ϳ��Եõ���ӽ���ʵֵ�ķŴ���
    //����ԭ�� ʹ�õ���Ե�ص�ѹ���з�ѹ�����͵������õ�Ƭ��ADC�������ٳ��Ա����Ŵ����ʵ��ѹ������һ������������С
    rt_thread_mdelay(20);
    sprintf(txt,"%04d",(int16)(vbat_ad*10));uart_putstr(UART_2,txt);
    uart_putstr(UART_2,"\n");
    //        OsDelay(100);//������Ըģ���Ϊ��ص�ѹ��ⲻ��Ҫ��ʵʱ
//    }
}
