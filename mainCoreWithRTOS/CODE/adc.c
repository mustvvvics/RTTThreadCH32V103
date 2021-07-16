#include "headfile.h"

uint16_t temp_ADC_Value;//暂存在DMA从ADC通道转移来的数值

float vbat_ad;//电池电压采样

char txt[32];
int16 Vc;
int16 VcPreviously = 0;
int16 VcDifference;
void getAdc(void){
    u8 i;
    u32 sum;
    static uint16_t adc[10];//如果定义在函数里面，这里需要改成静态局部变量static uint16_t adc[10];
    for(i=1;i<10;i++){  //FIFO 操作
        adc[i-1]=adc[i];
    }

    adc[9]=adc_convert(ADC_IN9_B1,ADC_12BIT);;//将新的数据放置到 数据的最后面

    sum=0;
    for(i=0;i<10;i++){
        sum+=adc[i];
    }
    vbat_ad=(float)(sum/10) / 4096 * 3.3f * 6.07f ;//具体方法，先不乘倍数，显示ADC采样的原始电压(0~3.3V) 然后用电表测量电池电压，再用电表测得的电压除去ADC实际电压得到倍数，就可以得到最接近真实值的放大倍数
    //测量原理 使用电阻对电池电压进行分压，降低到可以用单片机ADC采样，再乘以倍数放大回真实电压，所以一定有误差，但误差很小
    //8.328
    Vc = (int16)(vbat_ad * 100);
    VcDifference = VcPreviously - Vc;
    VcPreviously = Vc;
    if (VcPreviously > 0) {
        if (Vc >= 710 && Vc <= 750) {
            ips114_clear(BLACK);
        }
    }

}
