#include "headfile.h"

uint16_t temp_ADC_Value;//暂存在DMA从ADC通道转移来的数值

float vbat_ad;//电池电压采样

void DMA_Tx_Init( DMA_Channel_TypeDef* DMA_CHx, u32 ppadr, u32 memadr, u16 bufsize)
{
    DMA_InitTypeDef DMA_InitStructure;

    //使能DMA1外设时钟
    RCC_AHBPeriphClockCmd( RCC_AHBPeriph_DMA1, ENABLE );

    //DMA中填入缺省值
    DMA_DeInit(DMA_CHx);
    //ADC地址
    DMA_InitStructure.DMA_PeripheralBaseAddr = ppadr;
    //内存地址
    DMA_InitStructure.DMA_MemoryBaseAddr = memadr;
    //从外设到内存
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    //传输内容的大小
    DMA_InitStructure.DMA_BufferSize = bufsize;
    //外设地址固定选Disable 如果有多个外设，使用Enable，这里只与ADC1建立联系
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    //DMA访问多个内存参数
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    //外设数据单位
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    //内存数据单位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    //DMA模式 正常，连续不断的循环模式，若只想访问一次后就不要访问了
    //（或按指令操作来反问，也就是想要它访问的时候就访问，不要它访问的时候就停止），可以设置成通用模式
    //设置为循环传输
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    //配置DMA优先级 优先级高
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    //禁止内存到内存传输
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    //配置DMAx 通道x
    DMA_Init( DMA_CHx, &DMA_InitStructure );
}

void ADC_Function_Init(void)
{
    ADC_InitTypeDef ADC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    //外设时钟使能
    //B1
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE );
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE );
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);//主频80Mhz 8分频相当于10Mhz ADC工作周期配置

    //ADC采样引脚设置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    ADC_DeInit(ADC1);//填写缺省值
    //独立ADC模式,在这个模式下，双ADC不能同步，每个ADC接口独立工作。所以如果不需要ADC同步或者只是用了一个ADC的时候，就应该设成独立模式了。
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    //只是用了一个通道的话，DISABLE就可以了，如果使用了多个通道的话，则必须将其设置为ENABLE
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    //连续转换直到所有的数据转换完成后才停止转换，而单次转换则只转换一次数据就停止，要再次触发转换才可以。所以如果需要一次性采集1024个数据或者更多，则采用连续转换
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;

    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//软件触发 还有定时器触发和外部中断触发
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐
    //通道的数量。要是到多个通道采集数据的话就得设置一下这个参数。此外在规则通道组的配置函数中也许将各个通道的顺序定义一下
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);

    //使能ADC和DMA
    ADC_DMACmd(ADC1, ENABLE);
    ADC_Cmd(ADC1, ENABLE);

    //自动转换校准
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));

    //配置DMA转换参数
    DMA_Tx_Init(DMA1_Channel1,(u32)&ADC1->RDATAR,(u32)temp_ADC_Value,1);//用于放置接收变量
    //使能DMA通道1
    DMA_Cmd( DMA1_Channel1, ENABLE );
    //ADC1 通道2 rank 1 精度最高拉满了 239.5 cycles
    //因为有2个通道，所以rank = 2
    //开ADC
    ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 1, ADC_SampleTime_239Cycles5 );
    //由于没有外部触发或定时器触发，使用软件触发ADC转换
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}


char txt[32];
void getAdc(void){
    u8 i;
    u32 sum;
    static uint16_t adc[10];//如果定义在函数里面，这里需要改成静态局部变量static uint16_t adc[10];
//    for(;;){
    for(i=1;i<10;i++){  //FIFO 操作
        adc[i-1]=adc[i];
    }
    adc[9]=temp_ADC_Value;//将新的数据放置到 数据的最后面

    sum=0;
    for(i=0;i<10;i++){
        sum+=adc[i];
    }
    vbat_ad=(float)(sum/10) / 4096 * 3.3f * 1
            ;//具体方法，先不乘倍数，显示ADC采样的原始电压(0~3.3V) 然后用电表测量电池电压，再用电表测得的电压除去ADC实际电压得到倍数，就可以得到最接近真实值的放大倍数
    //测量原理 使用电阻对电池电压进行分压，降低到可以用单片机ADC采样，再乘以倍数放大回真实电压，所以一定有误差，但误差很小
    rt_thread_mdelay(20);
    sprintf(txt,"%04d",(int16)(vbat_ad*10));uart_putstr(UART_2,txt);
    uart_putstr(UART_2,"\n");
    //        OsDelay(100);//这里可以改，因为电池电压监测不需要很实时
//    }
}
