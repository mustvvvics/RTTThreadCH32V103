/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897(����)  ��Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file            zf_systick
 * @company         �ɶ���ɿƼ����޹�˾
 * @author          ��ɿƼ�(QQ790875685)
 * @version         �鿴doc��version�ļ� �汾˵��
 * @Software        MounRiver Studio V1.3
 * @Target core     CH32V103R8T6
 * @Taobao          https://seekfree.taobao.com/
 * @date            2020-12-04
 ********************************************************************************************************************/
#include "zf_systick.h"
#include "core_riscv.h"

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��શ�ʱ����ʱ
//  @param      time                us
//  @return     void
//  Sample usage:                   �ڲ�ʹ�ã��û��������
//-------------------------------------------------------------------------------------------------------------------
void systick_delay(uint64 time)
{
    SysTick->CTLR = 0;
    SysTick->CNTL0 = 0;
    SysTick->CNTL1 = 0;
    SysTick->CNTL2 = 0;
    SysTick->CNTL3 = 0;
    SysTick->CNTH0 = 0;
    SysTick->CNTH1 = 0;
    SysTick->CNTH2 = 0;
    SysTick->CNTH3 = 0;
    SysTick->CTLR = 1;          //����ϵͳ������ systick��HCLK/8 ʱ���� us

    while((*(volatile uint32*)0xE000F004) <= time);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      systick��ʱ������
//  @param      void
//  @return     void
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void systick_start(void)
{
    SysTick->CTLR = 0;
    SysTick->CNTL0 = 0;
    SysTick->CNTL1 = 0;
    SysTick->CNTL2 = 0;
    SysTick->CNTL3 = 0;
    SysTick->CTLR = 1;          //����ϵͳ������ systick��HCLK/8 ʱ���� us
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��õ�ǰSystem tick timer��ֵ
//  @return     ���ص�ǰSystem tick timer��ֵ
//  Sample usage:               uint32 tim = systick_getval();
//-------------------------------------------------------------------------------------------------------------------
uint32 systick_getval(void)
{
    return (*(volatile uint32*)0xE000F004);
}



//vuint16 zf_delay_ms = sys_clk / 8000;
////----------------------------------------------
////�����ʱ
////-------------------------------------------
//void delay_ms(uint32 count)
//{
//    uint16 i;
//    do {
//            i = zf_delay_ms;
//            //i = sys_clk/6000;//����: ms,Ҫ��ʱ��ms��, ����ֻ֧��1~255ms. �Զ���Ӧ��ʱ��.
//            while(--i);
//       }while(--count);
//
//}
