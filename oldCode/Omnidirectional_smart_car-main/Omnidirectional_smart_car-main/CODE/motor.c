/*
 * motor.c
 *
 *  Created on: Jan 13, 2021
 *      Author: xm2000
 */
#include "motor.h"

//�����������У�����������Ƶ��ѡ��13K-17K
//���ռ�ձ�ֵPWM_DUTY_MAX ������fsl_pwm.h�ļ����޸� Ĭ��Ϊ50000
//����һ��PWMģ�� ����������ͨ��ֻ�����Ƶ��һ�� ռ�ձȲ�һ���� PWM CH32V103R8T6ֻ���ĸ�PWMģ�� ÿ��ģ����4��ͨ��
//ÿ������������������źţ���ʵ���ǿ��������������źŵ�ռ�ձȣ����ߵ�ƽʱ�䣩
//ʹ�ö�ʱ��1��0��1ͨ����������������0ռ�ձ�Ϊ50%��1ͨ��Ϊ%0,��ô�����ת
//����0ռ�ձ�Ϊ0%��1ͨ��Ϊ%50,��ô�����ת�������Ϊ0������ֹͣ
//��Χ+5000~-5000
void motor1_ctl(int32 speed1_power)
{
    if(0<=speed1_power) //���1   ��ת ����ռ�ձ�Ϊ �ٷ�֮ (1000/TIMER1_PWM_DUTY_MAX*100)
    {
        gpio_set(MOTOR1_A, 0);
        pwm_duty(MOTOR1_B, speed1_power);
    }
    else                //���1   ��ת
    {
        gpio_set(MOTOR1_A, 1);
        pwm_duty(MOTOR1_B, -speed1_power);
    }
}

//>0��ת
void motor2_ctl(int32 speed2_power)
{
    if(0<=speed2_power) //���2   ��ת
    {
        gpio_set(MOTOR2_A, 0);
        pwm_duty(MOTOR2_B, speed2_power);
    }
    else                //���2   ��ת
    {
        gpio_set(MOTOR2_A, 1);
        pwm_duty(MOTOR2_B, -speed2_power);
    }
}

void motor3_ctl(int32 speed3_power)
{
    if(0<=speed3_power) //���3   ��ת
    {
        gpio_set(MOTOR3_A, 1);
        pwm_duty(MOTOR3_B, speed3_power);
    }
    else                //���3   ��ת
    {
        gpio_set(MOTOR3_A, 0);
        pwm_duty(MOTOR3_B, -speed3_power);
    }
}

void motor4_ctl(int32 speed4_power)
{
    if(0<=speed4_power) //���3   ��ת
    {
        gpio_set(MOTOR4_A, 1);
        pwm_duty(MOTOR4_B, speed4_power);
    }
    else                //���3   ��ת
    {
        gpio_set(MOTOR4_A, 0);
        pwm_duty(MOTOR4_B, -speed4_power);
    }
}


//�ٶȽ���
void speed_conversion(double Vx, double Vy, int Vz) {
    Left_front= (int)(+Vx + Vy - Vz * 0.18);
    Right_front = (int)(-Vx + Vy + Vz * 0.18);
    Right_rear = (int)(+Vx + Vy + Vz * 0.18);
    Left_rear = (int)(-Vx + Vy - Vz * 0.18);
}


