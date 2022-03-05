/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897(����)  ��Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file            isr
 * @company         �ɶ���ɿƼ����޹�˾
 * @author          ��ɿƼ�(QQ790875685)
 * @version         �鿴doc��version�ļ� �汾˵��
 * @Software        MounRiver Studio V1.3
 * @Target core     CH32V103R8T6
 * @Taobao          https://seekfree.taobao.com/
 * @date            2020-12-04
 ********************************************************************************************************************/


#include "headfile.h"



void NMI_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void HardFault_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void EXTI0_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void EXTI1_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void EXTI2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void EXTI3_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void EXTI4_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
//void DMA1_Channel1_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
//void DMA1_Channel2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
//void DMA1_Channel3_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void DMA1_Channel4_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
//void DMA1_Channel5_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
//void DMA1_Channel6_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
//void DMA1_Channel7_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void ADC1_2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void EXTI9_5_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM1_BRK_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM1_UP_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM1_TRG_COM_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM1_CC_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM3_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM4_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
//void I2C1_EV_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
//void I2C1_ER_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
//void I2C2_EV_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
//void I2C2_ER_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
//void SPI1_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
//void SPI2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void USART1_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void USART2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void USART3_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void EXTI15_10_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
//void RTCAlarm_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
//void USBWakeUp_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
//void USBHD_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

void EXTI0_IRQHandler(void)
{


}

void EXTI1_IRQHandler(void)
{
    if(SET == EXTI_GetITStatus(EXTI_Line1))
    {
        enconder_input++;
        EXTI_ClearITPendingBit(EXTI_Line1);
    }

}

void EXTI2_IRQHandler(void)
{
    EXTI_ClearITPendingBit(EXTI_Line2);
}

void EXTI3_IRQHandler(void)
{


}

void EXTI4_IRQHandler(void)
{


}

void EXTI9_5_IRQHandler(void)
{


}

void EXTI15_10_IRQHandler(void)
{

}

void ADC1_2_IRQHandler(void)
{


}

void TIM1_BRK_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM1, TIM_IT_Break) != RESET)
    {
        TIM_ClearITPendingBit(TIM1, TIM_IT_Break);

    }
}

void TIM1_UP_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);

    }
}

void TIM1_TRG_COM_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM1, TIM_IT_Trigger) != RESET)
    {
        TIM_ClearITPendingBit(TIM1, TIM_IT_Trigger);
    }
    if(TIM_GetITStatus(TIM1, TIM_IT_COM) != RESET)
    {
        TIM_ClearITPendingBit(TIM1, TIM_IT_COM);

    }
}

void TIM1_CC_IRQHandler(void)
{
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
}

void TIM2_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update );
    }
}

void TIM3_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update );
        five_mstick++;
        key_tick++;
        encoder_data[0] = encoder1_speed_spi(ABS_ENCODER_SPI_PC1_PIN);
        encoder_data[1] = -encoder2_speed_spi(ABS_ENCODER_SPI_PC2_PIN);          //��ȡ������2������
        if(gpio_get(A2))
        encoder_data[2] = enconder_input;          //��ȡ������3������
        else {
            encoder_data[2] = -enconder_input;          //��ȡ������3������
        }
        enconder_input=0;
        encoder_data[3] = encoder4_speed_spi(ABS_ENCODER_SPI_PC4_PIN);          //��ȡ������4������

        if(five_mstick >=3)
        {
            five_mstick=0;
            if(car_flag == 1)
            {
                if(position[0]>5)
                {
                    //pid����һ��0~128��ֵ
                    speed_conversion(0,basic_speed,direction_pid(position[0],&turn_pid,1));
                }
                else
                {
                    speed_conversion(0,basic_speed,direction_pid(position[0],&turn_pid,0));
                }
//                if(position[0]>5)
//                {
//                    speed_conversion(0,basic_speed*1.4,direction_pid(position[0],&turn_pid,1));
//                }
//                else
//                {
//                    speed_conversion(0,basic_speed*0.7,direction_pid(position[0],&turn_pid,0));
//                }
            }
            else {
                speed_conversion(0,0,0);
            }

            motor1_ctl(PID_Speed(Right_rear,encoder_data[3],&motor1_pid));
            motor2_ctl(PID_Speed(Left_rear,encoder_data[2],&motor2_pid));
            motor3_ctl(PID_Speed(Left_front,encoder_data[1],&motor3_pid));
            motor4_ctl(PID_Speed(Right_front,encoder_data[0],&motor4_pid));

            dx += abs(encoder_data[0]-encoder_data[3])/2;
            dy += abs(encoder_data[2]+encoder_data[3]+encoder_data[1]+encoder_data[0])/4;
            dz += abs(encoder_data[0]+encoder_data[3]-encoder_data[1]-encoder_data[2]);
            dist = sqrt(dx*dx+dy*dy);
            distance_real = dist;
        }
    }
}

void TIM4_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update );

    }
}

void USART1_IRQHandler(void)
{
    //uint8 data;
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
        //data = USART_ReceiveData(USART1);
//
//        rx_buf[rx_cnt++] = data;
//        if(rx_buf[0] != '{')
//        {
//            rx_cnt=0;
//        }
//        if(rx_buf[0] == '{'&& rx_buf[rx_cnt-1] == '}')
//        {
//            for(uint8 i=1;i<rx_cnt-1;i++)
//            {
//                rx_num[i-1]=rx_buf[i];
//            }
//            angle = atoi(rx_num);
//            rx_cnt=0;
//            memset(rx_buf,0,100);
//        }
    }
}



void USART2_IRQHandler(void)
{
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
            mt9v03x_uart_callback();
    }
}

void USART3_IRQHandler(void)
{
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(USART3, USART_IT_RXNE);
    }
}

void DMA1_Channel4_IRQHandler(void)
{
    if(SET == DMA_GetFlagStatus(DMA1_FLAG_TC4))
    {
        DMA_ClearFlag(DMA1_FLAG_TC4);
            mt9v03x_dma();
    }
}

/*******************************************************************************
* Function Name  : NMI_Handler
* Description    : This function handles NMI exception.
* Input          : None
* Return         : None
*******************************************************************************/
void NMI_Handler(void)
{

}

/*******************************************************************************
* Function Name  : HardFault_Handler
* Description    : This function handles Hard Fault exception.
* Input          : None
* Return         : None
*******************************************************************************/
void HardFault_Handler(void)
{

  while (1)
  {
  }
}



