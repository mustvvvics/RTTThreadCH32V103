/*
 * function.c
 *
 *  Created on: Jan 23, 2021
 *      Author: xm2000
 */
#include "headfile.h"

int16 ad_diff;
//int16 ad_dif0;//���м����� ��value.c �Ѿ�����
//int16 ad_dif1;
//int16 ad_dif2;//�������
//int16 ad_dif3;
//int16 ad_sum2;//�������
//int16 ad_sum3;

//uint8 island_flag = 0;
//int16 island_sum;
int16 threeWay_flag = 0;
int16 threeWay_sum = 0;
//uint8 island_finish = 0;
void Peripherals_Init(void)
{
    uint8 i;
    DisableGlobalIRQ();
    board_init();           //��ر��������������ڳ�ʼ��MPU ʱ�� ���Դ���
    systick_delay_ms(300);

    //��ʼ������������
    gpio_init(BEEP_PIN,GPO,0,GPIO_PIN_CONFIG);

    //������ʼ��
    gpio_init(KEY1, GPI, 0, GPIO_INT_CONFIG);
    gpio_init(KEY2, GPI, 0, GPIO_INT_CONFIG);
    gpio_init(KEY3, GPI, 0, GPIO_INT_CONFIG);

    //���������ʼ�� TIMER4    ��ʼ��Ƶ��17K
    gpio_init(MOTOR1_A, GPO, 0, GPIO_PIN_CONFIG);
    pwm_init(MOTOR1_B,17000,0);
    gpio_init(MOTOR2_A, GPO, 0, GPIO_PIN_CONFIG);
    pwm_init(MOTOR2_B,17000,0);
    gpio_init(MOTOR3_A, GPO, 0, GPIO_PIN_CONFIG);
    pwm_init(MOTOR3_B,17000,0);
    gpio_init(MOTOR4_A, GPO, 0, GPIO_PIN_CONFIG);
    pwm_init(MOTOR4_B,17000,0);

    //��������ʼ��
    encoder_init_spi(ABS_ENCODER_SPI_PC1_PIN);      //������1��ʼ����
    encoder_init_spi(ABS_ENCODER_SPI_PC2_PIN);      //������2��ʼ����
    gpio_interrupt_init(A1,BOTH, GPIO_INT_CONFIG);//D0�������ж�
    nvic_init(EXTI1_IRQn, 1, 1, ENABLE);                    //EXTI0���ȼ����ã���ռ���ȼ�1�������ȼ�1
    gpio_init(A2, GPI, 0, GPIO_INT_CONFIG);
    encoder_init_spi(ABS_ENCODER_SPI_PC4_PIN);      //������4��ʼ����

    //PID������ʼ��
    PID_Init();

    ADC_Function_Init();

    //lcd��Ļ��ʼ��
    ips114_init();

    //���ߴ��ڳ�ʼ��
    //����3
    seekfree_wireless_init();
    uart_rx_irq(UART_3,ENABLE);

    //����15�������ĵ�0ҳ ��ȡ��read_buff��
    flash_page_read(FLASH_SECTION_15, FLASH_PAGE_0, read_buff,8);

    for(i=0;i<4;i++)
    {
       ad_max[i]=read_buff[i];
       ad_min[i]=read_buff[i+4];
    }

    //��ʱ���жϳ�ʼ��
    timer_pit_interrupt_ms(TIMER_3,1);

    //uart_init(UART_1, 115200, UART1_TX_A9, UART1_RX_A10);

    //���ж������
    EnableGlobalIRQ(0);
}


void ESP8266_Init()
{
    gpio_set(A11,0);
    systick_delay_ms(250);
    gpio_set(A11,1);
    systick_delay_ms(500);
}

void vision_pc(void)
{
//    uint8 i;
    uart_putchar(UART_3,0x03);
    uart_putchar(UART_3,0xFC);
//    for(i=0;i<4;i++)
//    {
//        uart_putchar(UART_3,(encoder_data[i]&0xff00)>>8);
//        uart_putchar(UART_3,encoder_data[i]&0xff);
//    }
//    uart_putchar(UART_3,(target_speed&0xff00)>>8);
//    uart_putchar(UART_3,target_speed&0xff);

    uart_putchar(UART_3,0XFC);
    uart_putchar(UART_3,0X03);
}

void scan_key(void)
{
    uint8 key_num=0,key1=0,key2=0,key3=0;
    //�������
    key_num = key_scan();
    if(key_num == 1)
        key1++;
    else if(key_num == 2)
        key2++;
    else if(key_num == 3)
        key3++;
}


void show_speed(void)
{
    char txt[32]={0};

    sprintf(txt,"left_front = %05d",encoder_data[1]);
    ips114_showstr(0,0,txt);
    sprintf(txt,"right_front = %05d",encoder_data[0]);
    ips114_showstr(0,1,txt);
    sprintf(txt,"left_rear = %05d",encoder_data[2]);
    ips114_showstr(0,2,txt);
    sprintf(txt,"right_rear = %05d",encoder_data[3]);
    ips114_showstr(0,3,txt);

}

void show_ad(void)
{
    char txt[32]={0};

    sprintf(txt,"ad_0=%03d|ad_1=%03d",ad[0],ad[1]);
    ips114_showstr(0,0,txt);
    sprintf(txt,"ad_2=%03d|ad_3=%03d",ad[2],ad[3]);
    ips114_showstr(0,1,txt);
    sprintf(txt,"max_0=%03d|min_0=%03d",ad_max[0],ad_min[0]);
    ips114_showstr(0,2,txt);
    sprintf(txt,"max_1=%03d|min_1=%03d",ad_max[1],ad_min[1]);
    ips114_showstr(0,3,txt);
    sprintf(txt,"max_2=%03d|min_2=%03d",ad_max[2],ad_min[2]);
    ips114_showstr(0,4,txt);
    sprintf(txt,"max_3=%03d|min_3=%03d",ad_max[3],ad_min[3]);
    ips114_showstr(0,5,txt);
    sprintf(txt,"ad0_real=%03d|ad1_real=%03d",ad_real[0],ad_real[1]);
    ips114_showstr(0,6,txt);
    sprintf(txt,"data=%03d|dist=%03d",basic_data,dist);
    ips114_showstr(0,7,txt);

}


void show_simple(void)
{
    char txt[32]={0};

    sprintf(txt,"ad_0 = %03d",ad[0]);
    ips114_showstr(0,0,txt);
    sprintf(txt,"ad_1 = %03d",ad[1]);
    ips114_showstr(0,1,txt);
    sprintf(txt,"ad_2 = %03d",ad[2]);
    ips114_showstr(0,0,txt);
    sprintf(txt,"ad_3 = %03d",ad[3]);
    ips114_showstr(0,1,txt);
    if(key_cnt == 1)
    {
        sprintf(txt,"simpling max");
        ips114_showstr(0,4,txt);
    }
    else if (key_cnt == 2)
    {
        sprintf(txt,"simpling min0");
        ips114_showstr(0,4,txt);
    }
    else if (key_cnt == 3)
    {
        sprintf(txt,"simpling min1");
        ips114_showstr(0,4,txt);
    }
    else if (key_cnt == 4)
    {
        sprintf(txt,"simpling min2");
        ips114_showstr(0,4,txt);
    }
    else if (key_cnt == 5)
    {
        sprintf(txt,"simpling min3");
        ips114_showstr(0,4,txt);
    }
    sprintf(txt,"simpling...");
    ips114_showstr(0,5,txt);


}

void show_car(void)
{
    char txt[32]={0};

    sprintf(txt,"direction_pid_1=%05d",direction_pid(position[0],&turn_pid,1));
    ips114_showstr(0,0,txt);
//    sprintf(txt,"direction_pid_0=%05d",(int16)direction_pid(position[0],&turn_pid,0));
//    ips114_showstr(0,1,txt);
    sprintf(txt,"island_flag=%02d|three_flag=%02d",island_flag,threeWay_flag);
    ips114_showstr(0,1,txt);
    sprintf(txt,"key_cnt=%03d|car_flag=%03d",key_cnt,car_flag);
    ips114_showstr(0,2,txt);
    sprintf(txt,"island_finish=%03d",island_finish);
    ips114_showstr(0,3,txt);

//    sprintf(txt,"ad_1-2=%03d|ad_0-3=%03d",ad_dif0,ad_dif1);
//    ips114_showstr(0,3,txt);
//    sprintf(txt,"ad_0-1=%03d|ad_2-3=%03d",ad_dif2,ad_dif3);
//    ips114_showstr(0,4,txt);
//    sprintf(txt,"ad_0+1=%03d|ad_2+3=%03d",ad_sum2,ad_sum3);
//    ips114_showstr(0,5,txt);
    sprintf(txt,"speed=%03d",basic_speed);
    ips114_showstr(0,4,txt);
    sprintf(txt,"island_need=%03d",island_need);
    ips114_showstr(0,5,txt);
    sprintf(txt,"ad_sum=%03d|pos[1]=%05d",ad_sum,position[1]);
    ips114_showstr(0,6,txt);
    sprintf(txt,"p[0]=%05d|island_sum=%05d",position[0],island_sum);
    ips114_showstr(0,7,txt);


}

void show_time(void)
{
    char txt[32]={0};

    sprintf(txt,"time = %.3f",(float)runtime*0.001f);
    ips114_showstr(0,0,txt);


}

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
    //C0~C5
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE );
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE );
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE );
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);//��Ƶ80Mhz 8��Ƶ�൱��10Mhz ADC������������

    //ADC������������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

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
    ADC_InitStructure.ADC_NbrOfChannel = 6;
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
    DMA_Tx_Init(DMA1_Channel1,(u32)&ADC1->RDATAR,(u32)adc_value,30);
    //ʹ��DMAͨ��1
    DMA_Cmd( DMA1_Channel1, ENABLE );
    //ADC1 ͨ��2 rank 1 ������������� 239.5 cycles
    //��Ϊ��2��ͨ��������rank = 2
    //��ADC
    ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_239Cycles5 );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 2, ADC_SampleTime_239Cycles5 );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 3, ADC_SampleTime_239Cycles5 );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 4, ADC_SampleTime_239Cycles5 );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 5, ADC_SampleTime_239Cycles5 );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 6, ADC_SampleTime_239Cycles5 );
    //����û���ⲿ������ʱ��������ʹ���������ADCת��
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

int16 adc_mean(uint8 n)
{
    uint8 i;
    uint32 sum=0;

    for(i=0; i<5; i++)
    {
        sum+=adc_value[i*6+n];
    }
    sum = sum/5;
    return sum;
}

void backtoreal(void)
{
    uint8 i;
    float sensor_to_on[6];
    for(i=0;i<6;i++)
    {
        sensor_to_on[i]=(float)(ad[i]-ad_min[i])/(float)(ad_max[i]-ad_min[i]);
        if(sensor_to_on[i]<=0.0)sensor_to_on[i]=0.001;
        if(sensor_to_on[i]>1.0)sensor_to_on[i]=1.0;
        ad_real[i]=100*sensor_to_on[i];
    }
}

void ad_calculate(void)//��м���
{
    for(uint8 i=0;i<6;i++)
    {
       //���ÿ����е��˲���ֵ �ֶ�8λ
        ad[i]=adc_mean(i)>>4;
    }

    //Խ������ ֵԽС
    //A4 ���� ad[0]
    //A6 ���� ad[1]
    //B0 ���� ad[2]
    //B1 ���� ad[3]
    //ȷ��һ�����ֵ��ȷ��һ����Сֵ����ô������ʱ �󲿷ֲ���ֵӦ���������Χ֮��
    if(key_cnt >= 5 || car_flag ==1)
    {
        backtoreal();


        ad_dif0 = (int16)ad[1]-ad[2]; //��м��һ��
        ad_dif1 = (int16)ad[0]-ad[3];
        ad_dif2 = (int16)ad[0]-ad[1]; //�������
        ad_dif3 = (int16)ad[2]-ad[3];
        ad_sum = ad[0]+ad[3];
        ad_sum2 = ad[0] + ad[1];
        ad_sum3 = ad[2] + ad[3];

        ad_diff=(int16)ad_real[1]-ad_real[0];//�������ҵ��֮��
        position[0] =(int16)(ad_dif0<<7)/(ad_sum+1);
        position[1] =(int16)(ad_dif1<<7)/(ad_sum+1);//�����ȺͲ��Ŵ�128��:<<7�Ŵ�2^7

/*
 **********************************�߽籣�� **********************************
*/
        if(abs(ad_sum) < 40)//�������
        {
            car_flag = 0;
        }
/*
 **********************************ʶ������ **********************************
*/
        if(abs(ad_dif2) < 40 && abs(ad_dif3) < 40 && ad_sum2 > 180
                && ad_sum3 > 180 && ad_sum2 < 240 && ad_sum3 < 240){ //����·��
            threeWay_flag = 1;
        }
        else {
            threeWay_flag = 0;
        }
/*
 ********************************** ʶ���� **********************************
*/
        if (ad_dif2 < 0 && ad_sum2 > 300) { //�󻷵� ���������в����
            island_flag = 1;
            island_sum++;
        }
        else {
            island_flag = 0;
        }
/*
 ********************************** ����·�� **********************************
*/
        if (threeWay_flag == 1 && island_flag == 0 && threeWay_sum == 0) {//��һ��
            position[0] = -101;//����ת
            systick_delay_ms(500);
            threeWay_flag = 0;//���־
            threeWay_sum = 1;
        }
        if (threeWay_flag == 1 && island_flag == 0 && threeWay_sum == 1) { //�ڶ��ξ�������·��
            position[0] = 102;//����ת
            systick_delay_ms(300);
            threeWay_sum = 0;
            threeWay_flag = 0;
        }
/*
 ********************************** �󻷵� **********************************
*/
        if (island_flag == 1 && threeWay_flag == 0 && island_sum > island_need) { //�뻷
            position[0] = 80; //����ת�뻷
            systick_delay_ms(300);
            island_flag = 0;//�����־/
            island_finish = 1;
            island_sum = 0;
        }
        if (island_finish == 1 && island_flag == 1 && threeWay_flag == 0) {
            position[0] = 50;//ֱ�г���
            systick_delay_ms(500);
            island_flag = 0;
            island_finish = 2;
            island_sum = 0;
        }
        if (island_finish == 2 && island_flag == 1 && threeWay_flag == 0){
            position[0] = 0;//ֱ�г���
            systick_delay_ms(1000);
            island_finish = 0;
            island_flag = 0;
            island_sum = 0;
        }

    }
}






