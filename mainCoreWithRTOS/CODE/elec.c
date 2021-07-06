#include "elec.h"

#define ADC_CH1 ADC_IN4_A4
#define ADC_CH2 ADC_IN6_A6
#define ADC_CH3 ADC_IN8_B0
#define ADC_CH4 ADC_IN9_B1

void elec_init(void)
{
    adc_init(ADC_CH1);
    adc_init(ADC_CH2);
    adc_init(ADC_CH3);
    adc_init(ADC_CH4);
}

uint16 elec_data[4];
void elec_get(void)
{
    elec_data[0] = adc_mean_filter(ADC_CH1, ADC_8BIT, 10);
    elec_data[1] = adc_mean_filter(ADC_CH2, ADC_8BIT, 10);
    elec_data[2] = adc_mean_filter(ADC_CH3, ADC_8BIT, 10);
    elec_data[3] = adc_mean_filter(ADC_CH4, ADC_8BIT, 10);
    
}

void elec_calculate(void)
{
    
}
