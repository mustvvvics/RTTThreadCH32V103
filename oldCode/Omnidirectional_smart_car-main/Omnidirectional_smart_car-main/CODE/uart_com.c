/*
 * spi_glu.c
 *
 *  Created on: Jan 24, 2021
 *      Author: xm2000
 */

#include "headfile.h"
#include "stdlib.h"
/* Global define */

uint8   temp_buff[LINE_LEN];            //�������ڽ������ݵ�BUFF
vuint8  uart_flag;                   //�������ݱ�־λ
vuint8  hmi_flag;
uint8   num_flag=0;
uint8   receiveBuffer[10];


void analysis_hmi_data(uint8 data)
{
    char txt[32]={0};
    static uint8 uart_num = 0;
    if(num_flag == 0)
    {
        get_hmi_data(data);
    }
    else
    {
        if(0x32 == data)
        {
            switch(num_flag)
            {
            //�ٶȻ�����
            case 1://Kp
                S_P = atof(txt);
                motor1_pid.Kp=S_P;
                motor2_pid.Kp=S_P;
                motor3_pid.Kp=S_P;
                motor4_pid.Kp=S_P;
                break;
            case 2://Ki
                S_I = atof(txt);
                motor1_pid.Ki=S_I;
                motor2_pid.Ki=S_I;
                motor3_pid.Ki=S_I;
                motor4_pid.Ki=S_I;
                break;
            case 3://Kd
                S_D = atof(txt);
                motor1_pid.Kd=S_D;
                motor2_pid.Kd=S_D;
                motor3_pid.Kd=S_D;
                motor4_pid.Kd=S_D;
                break;
            default:
                break;
            }
            uart_num=0;
            num_flag = 0;
        }
        else
        {
            txt[uart_num++] = data;
        }
    }
}


void get_hmi_data(uint8 data)
{
    static uint8 uart_num = 0;

    receiveBuffer[uart_num++] = data;

    if(1 == uart_num)
    {
        //���յ��ĵ�һ���ַ���Ϊ0xD8��֡ͷ����
        if(0x65 != receiveBuffer[0])
        {
           uart_num = 0;
           hmi_flag = E_FRAME_HEADER_ERROR;
        }

    }

    if(7 == uart_num)
    {
        hmi_flag = E_OK;
        //���յ����һ���ֽ�Ϊ0xEE
        if(0xFF == receiveBuffer[7 - 1])
        {
            hmi_flag = E_OK;
        }
        else    //���յ����һ���ֽڲ���0xEE��֡β����
        {
            hmi_flag = E_FRAME_RTAIL_ERROR;
        }
        uart_num = 0;

        switch(receiveBuffer[2])
        {
        //�ٶȻ���������
        case 0x02://Kp
            num_flag=1;
            break;
        case 0x04://Ki
            num_flag=2;
            break;
        case 0x06://Kd
            num_flag=3;
            break;
        case 0x07://�ϴ�����
            break;
        case 0x08://ʹ�ܵ��
            gpio_set(MOTOR1_A, 1);
            gpio_set(MOTOR2_A, 1);
            gpio_set(MOTOR3_A, 1);
            gpio_set(MOTOR4_A, 1);
            break;
        case 0x09://ʧ�ܵ��
            gpio_set(MOTOR1_A, 0);
            gpio_set(MOTOR2_A, 0);
            gpio_set(MOTOR3_A, 0);
            gpio_set(MOTOR4_A, 0);
            break;
        case 0x0A://���ٶ�
            //target_speed++;
            break;
        case 0x0B://���ٶ�
            //target_speed--;
            break;
        case 0x0C://�ٶ�����
            //target_speed=0;
            break;
        default:
            break;
        }
    }

}



void HMI_Send_Data(char* txt)
{
    uart_putstr(UART_1,txt);
    uart_putchar(UART_1,0XFF);
    uart_putchar(UART_1,0XFF);
    uart_putchar(UART_1,0XFF);
}





