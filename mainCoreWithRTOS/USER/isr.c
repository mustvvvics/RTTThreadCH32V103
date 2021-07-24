/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file            isr
 * @company         成都逐飞科技有限公司
 * @author          逐飞科技(QQ790875685)
 * @version         查看doc内version文件 版本说明
 * @Software        MounRiver Studio V1.3
 * @Target core     CH32V103R8T6
 * @Taobao          https://seekfree.taobao.com/
 * @date            2020-12-04
 ********************************************************************************************************************/

#include "headfile.h"

void WWDG_IRQHandler(void) __attribute__((interrupt()));
void HardFault_Handler(void) __attribute__((interrupt()));
void NMI_Handler(void) __attribute__((interrupt()));
void EXTI0_IRQHandler(void) __attribute__((interrupt()));
void EXTI1_IRQHandler(void) __attribute__((interrupt()));
void EXTI2_IRQHandler(void) __attribute__((interrupt()));
void EXTI3_IRQHandler(void) __attribute__((interrupt()));
void EXTI4_IRQHandler(void) __attribute__((interrupt()));
void DMA1_Channel1_IRQHandler(void) __attribute__((interrupt()));
void DMA1_Channel2_IRQHandler(void) __attribute__((interrupt()));
void DMA1_Channel3_IRQHandler(void) __attribute__((interrupt()));
void DMA1_Channel4_IRQHandler(void) __attribute__((interrupt()));
void DMA1_Channel5_IRQHandler(void) __attribute__((interrupt()));
void DMA1_Channel6_IRQHandler(void) __attribute__((interrupt()));
void DMA1_Channel7_IRQHandler(void) __attribute__((interrupt()));
void ADC1_2_IRQHandler(void) __attribute__((interrupt()));
void EXTI9_5_IRQHandler(void) __attribute__((interrupt()));
void TIM1_BRK_IRQHandler(void) __attribute__((interrupt()));
void TIM1_UP_IRQHandler(void) __attribute__((interrupt()));
void TIM1_TRG_COM_IRQHandler(void) __attribute__((interrupt()));
void TIM1_CC_IRQHandler(void) __attribute__((interrupt()));
void TIM3_IRQHandler(void) __attribute__((interrupt()));
void TIM4_IRQHandler(void) __attribute__((interrupt()));
void USART2_IRQHandler(void) __attribute__((interrupt()));
void USART3_IRQHandler(void) __attribute__((interrupt()));
void EXTI15_10_IRQHandler(void) __attribute__((interrupt()));

void EXTI0_IRQHandler(void)
{
    rt_interrupt_enter();   //进入中断

    if(SET == EXTI_GetITStatus(EXTI_Line0))
    {

        EXTI_ClearITPendingBit(EXTI_Line0);
    }

    rt_interrupt_leave();   //退出中断
}

void EXTI1_IRQHandler(void)
{
    rt_interrupt_enter();   //进入中断

    if(SET == EXTI_GetITStatus(EXTI_Line1))
    {
        EXTI_ClearITPendingBit(EXTI_Line1);
    }

    rt_interrupt_leave();   //退出中断
}
/************************************************************************************************/
/*
 * 外部中断 同步引脚拉起
 */
/************************************************************************************************/
void EXTI2_IRQHandler(void)
{
    rt_interrupt_enter();    //进入中断
/*
 * 计时器:计算运行时间
 */
//    timet1 = rt_tick_get();
//    timeControl = timet1 - timet2;
//    timet2 = timet1;

    if(SET == EXTI_GetITStatus(EXTI_Line2))
    {

        while (uart_flag != E_OK);                          //等待接收数据
        {
            AngleZ_Get();
            data_analysis(temp_buff);                       //数据解析 图像核编码器  偏差数据
            uart_flag = E_START;
            encoder_get();                                  //主核编码器
        }
        //计算偏差变化
        position_front_delta = position_front - position_front_last;
        position_front_last = -position_front;
        ThreeWayAnalyze();  //三叉解析
        roundIslandAnalyze();//环岛通讯flag
        motor_conversion();//控制在最后，时序问题

        EXTI_ClearITPendingBit(EXTI_Line2);
    }
    rt_interrupt_leave();    //退出中断
}

void EXTI3_IRQHandler(void)
{
    rt_interrupt_enter();   //进入中断

    if(SET == EXTI_GetITStatus(EXTI_Line3))
    {

        EXTI_ClearITPendingBit(EXTI_Line3);
    }

    rt_interrupt_leave();   //退出中断
}

void EXTI4_IRQHandler(void)
{
    rt_interrupt_enter();    //进入中断

    if(SET == EXTI_GetITStatus(EXTI_Line4))
    {

        EXTI_ClearITPendingBit(EXTI_Line4);
    }

    rt_interrupt_leave();    //退出中断
}

void EXTI9_5_IRQHandler(void)
{
    rt_interrupt_enter();   //进入中断

    if(SET == EXTI_GetITStatus(EXTI_Line9))
    {

        EXTI_ClearITPendingBit(EXTI_Line9);
    }
    else if(SET == EXTI_GetITStatus(EXTI_Line8))
    {

        EXTI_ClearITPendingBit(EXTI_Line8);
    }
    else if(SET == EXTI_GetITStatus(EXTI_Line7))
    {

        EXTI_ClearITPendingBit(EXTI_Line7);
    }
    else if(SET == EXTI_GetITStatus(EXTI_Line6))
    {

        EXTI_ClearITPendingBit(EXTI_Line6);
    }
    else if(SET == EXTI_GetITStatus(EXTI_Line5))
    {

        EXTI_ClearITPendingBit(EXTI_Line5);
    }

    rt_interrupt_leave();       //退出中断
}

void EXTI15_10_IRQHandler(void)
{
    rt_interrupt_enter();   //进入中断

    if(SET == EXTI_GetITStatus(EXTI_Line15))
    {

        EXTI_ClearITPendingBit(EXTI_Line15);
    }
    else if(SET == EXTI_GetITStatus(EXTI_Line14))
    {

        EXTI_ClearITPendingBit(EXTI_Line14);
    }
    else if(SET == EXTI_GetITStatus(EXTI_Line13))
    {

        EXTI_ClearITPendingBit(EXTI_Line13);
    }
    else if(SET == EXTI_GetITStatus(EXTI_Line12))
    {

        EXTI_ClearITPendingBit(EXTI_Line12);
    }
    else if(SET == EXTI_GetITStatus(EXTI_Line11))
    {

        EXTI_ClearITPendingBit(EXTI_Line11);
    }
    else if(SET == EXTI_GetITStatus(EXTI_Line10))
    {

        EXTI_ClearITPendingBit(EXTI_Line10);
    }

    rt_interrupt_leave();       //退出中断
}

void ADC1_2_IRQHandler(void)
{
    rt_interrupt_enter();       //进入中断

    //获取中断标志位
    //清除中断标志位

    rt_interrupt_leave();       //退出中断
}

void TIM1_BRK_IRQHandler(void)
{
    rt_interrupt_enter();       //进入中断

    if(TIM_GetITStatus(TIM1, TIM_IT_Break) != RESET)
    {
        TIM_ClearITPendingBit(TIM1, TIM_IT_Break);

    }

    rt_interrupt_leave();       //退出中断
}

void TIM1_UP_IRQHandler(void)
{
    rt_interrupt_enter();       //进入中断


    if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);

    }

    rt_interrupt_leave();       //退出中断
}

void TIM1_TRG_COM_IRQHandler(void)
{
    rt_interrupt_enter();       //进入中断

    if(TIM_GetITStatus(TIM1, TIM_IT_Trigger) != RESET)
    {
        TIM_ClearITPendingBit(TIM1, TIM_IT_Trigger);

    }
    if(TIM_GetITStatus(TIM1, TIM_IT_COM) != RESET)
    {
        TIM_ClearITPendingBit(TIM1, TIM_IT_COM);

    }

    rt_interrupt_leave();       //退出中断
}

void TIM1_CC_IRQHandler(void)
{
    rt_interrupt_enter();       //进入中断

    if(TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET)
    {
        TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);

    }
    if(TIM_GetITStatus(TIM1, TIM_IT_CC2) != RESET)
    {
        TIM_ClearITPendingBit(TIM1, TIM_IT_CC2);

    }
    if(TIM_GetITStatus(TIM1, TIM_IT_CC3) != RESET)
    {
        TIM_ClearITPendingBit(TIM1, TIM_IT_CC3);

    }
    if(TIM_GetITStatus(TIM1, TIM_IT_CC4) != RESET)
    {
        TIM_ClearITPendingBit(TIM1, TIM_IT_CC4);

    }

    rt_interrupt_leave();       //退出中断
}

void TIM2_IRQHandler(void)
{
    rt_interrupt_enter();       //进入中断

    if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update );
    }

    rt_interrupt_leave();       //退出中断
}

void TIM3_IRQHandler(void)
{
    rt_interrupt_enter();       //进入中断

    if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update );

    }

    rt_interrupt_leave();       //退出中断
}

void TIM4_IRQHandler(void)
{
    rt_interrupt_enter();       //进入中断

    if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update );

    }

    rt_interrupt_leave();       //退出中断
}

//串口1 定义在了board.c //中用于接收串口内容并通过邮件发送给finsh

/**************************************************************************************************************************/
/*
//串口2接收中断  用于接收8266信号
 */
/**************************************************************************************************************************/
void USART2_IRQHandler(void)
{
    rt_interrupt_enter();       //进入中断
    uint8 data_temp;
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
        data_temp = (uint8)USART_ReceiveData(USART2);

        esp8266_buf[esp8266_cnt++] = data_temp;
        if (esp8266_cnt > 63) { //防止溢出
            esp8266_cnt = 0;
        }
        if (data_temp == 0x0A) { //接收到了换行符
            rt_sem_release(esp8266_sem); //释放信号量
        }
    }

    rt_interrupt_leave();       //退出中断
}

/**************************************************************************************************************************/
/*
 //串口3 接收中断 接收图像和数据
 */
/**************************************************************************************************************************/
void USART3_IRQHandler(void)
{
    rt_interrupt_enter();       //进入中断

    uint8 dat_USART3;
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(USART3, USART_IT_RXNE);
        dat_USART3 = (uint8)USART_ReceiveData(USART3);                //获取串口数据
        get_slave_data(dat_USART3);                                   //将每一个字节的串口数据存入temp_buff
    }

    rt_interrupt_leave();       //退出中断
}

void DMA1_Channel4_IRQHandler(void)
{
    rt_interrupt_enter();       //进入中断

    if(SET == DMA_GetFlagStatus(DMA1_FLAG_TC4))
    {
        DMA_ClearFlag(DMA1_FLAG_TC4);
    }

    rt_interrupt_leave();       //退出中断
}

/*******************************************************************************
* Function Name  : NMI_Handler
* Description    : This function handles NMI exception.
* Input          : None
* Return         : None
*******************************************************************************/
void NMI_Handler(void)
{
    rt_interrupt_enter();       //进入中断

    //获取中断标志位
    //清除中断标志位

    rt_interrupt_leave();       //退出中断
}

void HardFault_Handler(void)
{
    rt_interrupt_enter();       //进入中断

    while(1);

    rt_interrupt_leave();       //退出中断
}



void WWDG_IRQHandler(void)
{
    rt_interrupt_enter();       //进入中断

    //获取中断标志位
    //清除中断标志位

    rt_interrupt_leave();       //退出中断
}

void TAMPER_IRQHandler(void)
{
    rt_interrupt_enter();       //进入中断

    //获取中断标志位
    //清除中断标志位

    rt_interrupt_leave();       //退出中断
}
void RTC_IRQHandler(void)
{
    rt_interrupt_enter();       //进入中断

    //获取中断标志位
    //清除中断标志位

    rt_interrupt_leave();       //退出中断
}
void FLASH_IRQHandler(void)
{
    rt_interrupt_enter();       //进入中断

    //获取中断标志位
    //清除中断标志位

    rt_interrupt_leave();       //退出中断
}
void RCC_IRQHandler(void)
{
    rt_interrupt_enter();       //进入中断

    //获取中断标志位
    //清除中断标志位

    rt_interrupt_leave();       //退出中断
}
void DMA1_Channel1_IRQHandler(void)
{
    rt_interrupt_enter();       //进入中断

    //获取中断标志位
    //清除中断标志位

    rt_interrupt_leave();       //退出中断
}
void DMA1_Channel2_IRQHandler(void)
{
    rt_interrupt_enter();       //进入中断

    //获取中断标志位
    //清除中断标志位

    rt_interrupt_leave();       //退出中断
}
void DMA1_Channel3_IRQHandler(void)
{
    rt_interrupt_enter();       //进入中断

    //获取中断标志位
    //清除中断标志位

    rt_interrupt_leave();       //退出中断
}
void DMA1_Channel5_IRQHandler(void)
{
    rt_interrupt_enter();       //进入中断

    //获取中断标志位
    //清除中断标志位

    rt_interrupt_leave();       //退出中断
}
void DMA1_Channel6_IRQHandler(void)
{
    rt_interrupt_enter();       //进入中断

    //获取中断标志位
    //清除中断标志位

    rt_interrupt_leave();       //退出中断
}
void DMA1_Channel7_IRQHandler(void)
{
    rt_interrupt_enter();       //进入中断

    //获取中断标志位
    //清除中断标志位

    rt_interrupt_leave();       //退出中断
}


