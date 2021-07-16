#include "headfile.h"

void motor_init(void)
{
    gpio_init(MOTOR1_A, GPO, 0, GPIO_PIN_CONFIG);
    pwm_init(MOTOR1_B,17000,0);
    gpio_init(MOTOR2_A, GPO, 0, GPIO_PIN_CONFIG);
    pwm_init(MOTOR2_B,17000,0);
    gpio_init(MOTOR3_A, GPO, 0, GPIO_PIN_CONFIG);
    pwm_init(MOTOR3_B,17000,0);
    gpio_init(MOTOR4_A, GPO, 0, GPIO_PIN_CONFIG);
    pwm_init(MOTOR4_B,17000,0);
}

//void motor_pid(int16 expect_speed)
//{
//
//}

void motor1_ctl(int32 speed1_power)
{
    if(0<=speed1_power) //电机1    设置占空比为 百分之 (1000/TIMER1_PWM_DUTY_MAX*100)
    {
        gpio_set(MOTOR1_A, 1);
        pwm_duty(MOTOR1_B, speed1_power);
    }
    else                //电机1
    {
        gpio_set(MOTOR1_A, 0);
        pwm_duty(MOTOR1_B, -speed1_power);
    }
}

//>0正转
void motor2_ctl(int32 speed2_power)
{
    if(0<=speed2_power) //电机2
    {
        gpio_set(MOTOR2_A, 0);
        pwm_duty(MOTOR2_B, speed2_power);
    }
    else                //电机2
    {
        gpio_set(MOTOR2_A, 1);
        pwm_duty(MOTOR2_B, -speed2_power);
    }
}

void motor3_ctl(int32 speed3_power)
{
    if(0<=speed3_power) //电机3
    {
        gpio_set(MOTOR3_A, 0);
        pwm_duty(MOTOR3_B, speed3_power);
    }
    else                //电机3
    {
        gpio_set(MOTOR3_A, 1);
        pwm_duty(MOTOR3_B, -speed3_power);
    }
}

void motor4_ctl(int32 speed4_power)
{
    if(0<=speed4_power) //电机3
    {
        gpio_set(MOTOR4_A, 1);
        pwm_duty(MOTOR4_B, speed4_power);
    }
    else                //电机3
    {
        gpio_set(MOTOR4_A, 0);
        pwm_duty(MOTOR4_B, -speed4_power);
    }
}

//速度解算
void speed_conversion(float Vx, float Vy, float Vz) {
    Left_front= (int16)(+Vx + Vy - Vz * 0.18);
    Right_front = (int16)(-Vx + Vy + Vz * 0.18);
    Right_rear = (int16)(+Vx + Vy + Vz * 0.18);
    Left_rear = (int16)(-Vx + Vy - Vz * 0.18);

//    leftFrontADRC = 31.804 * Left_front + 211.95;
//    rightFrontADRC = 26.742 * Right_front + 538.58;
//    rightRearADRC = 26.742 * Right_rear + 538.58 ;
//    leftRearADRC = 26.870 * Left_rear + 551.53;
    leftFrontADRC = 29.769 * Left_front + 293.13;
    rightFrontADRC = 27.239 * Right_front + 512.17;
    rightRearADRC = 26.549 * Right_rear + 519.87 ;
    leftRearADRC = 23.851 * Left_rear + 668.88;

}
