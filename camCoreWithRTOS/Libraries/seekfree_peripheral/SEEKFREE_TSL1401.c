/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file            main
 * @company         成都逐飞科技有限公司
 * @author          逐飞科技(QQ790875685)
 * @version         查看doc内version文件 版本说明
 * @Software        MDK FOR C51 V9.60
 * @Target core     STC8A8K64S4
 * @Taobao          https://seekfree.taobao.com/
 * @date            2020-4-14
 * @note
                    TSL1401接线定义：
                    ------------------------------------
                                                模块管脚                                     单片机管脚
                        CCD_AO          查看SEEKFREE_TSL1401.h文件中的AD_CHANNEL 宏定义
                        CCD_CLK         查看SEEKFREE_TSL1401.h文件中的CCD_CLK_PIN宏定义
                        CCD_SI          查看SEEKFREE_TSL1401.h文件中的CCD_SI_PIN 宏定义
                    ------------------------------------
 ********************************************************************************************************************/




#include "zf_adc.h"
#include "zf_pit.h"
#include "SEEKFREE_TSL1401.h"
uint8 tsl1401_finish_flag;
uint16 ccd_data[128];                           //CCD数据

//-------------------------------------------------------------------------------------------------------------------
//  @brief      TSL1401线阵CCD初始化
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void ccd_init(void)     
{       
    gpio_init(CCD_CLK_PIN, GPO, 1, GPIO_PIN_CONFIG);
    gpio_init(CCD_SI_PIN, GPO, 1, GPIO_PIN_CONFIG);
    adc_init(AD_CHANNEL);
    timer_pit_interrupt_ms(PIT_TIMER_CH, EXPOSURE_TIME);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      TSL1401线阵CCD图像发送至上位机查看图像
//  @param      uart_n          串口号
//  @param      uart_n          线性CCD数据指针
//  @return     void
//  @since      v1.0
//  Sample usage:               调用该函数前请先初始化串口
//-------------------------------------------------------------------------------------------------------------------
void ccd_send_data(UARTN_enum uart_n ,uint16 *dat)
{
   uint8 i = 0;
   uart_putchar(uart_n, 0x00); 
   uart_putchar(uart_n, 0xff);
   uart_putchar(uart_n, 0x01);
   uart_putchar(uart_n, 0x00);
   
    for(i=0; i<128; i++)
    {
        uart_putchar(uart_n, dat[i]>>8);   //发送高8位
        uart_putchar(uart_n, dat[i]&0XFF); //发送低8位
    }
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      TSL1401线阵CCD数据采集
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:               在isr.c里面先创建对应的中断函数，然后调用该函数(之后别忘记清除中断标志位)
//-------------------------------------------------------------------------------------------------------------------
void ccd_collect(void)
{
    uint8 i = 0;

    CCD_CLK(1);
    CCD_SI(0);
    CCD_CLK(0);
    CCD_SI(1);
    CCD_CLK(1);
    CCD_SI(0);

    for(i=0;i<128;i++)
    {
        CCD_CLK(0);
        //这里可以同时采集两个CCD数据
        ccd_data[i] = adc_convert(AD_CHANNEL, AD_RESOLUTION);
        //ccd_data_two[i] = adc_convert(AD_CHANNEL, AD_RESOLUTION);
        CCD_CLK(1);
    }

    //采集完成标志位置1
    tsl1401_finish_flag = 1;

}
