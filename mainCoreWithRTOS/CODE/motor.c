#include "headfile.h"


void motor_init(void)
{
    //初始化电机PWM引脚和方向引脚

    //桌大大的推文中，建议电磁组电机频率选用13K-17K
    //最大占空比值PWM_DUTY_MAX 可以在fsl_pwm.h文件中修改 默认为50000
    //对于一个PWM模块 包含的所有通道只能输出频率一样 占空比不一样的 PWM CH32V103R8T6只有四个PWM模块 每个模块有4个通道
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
void speed_conversion(double Vx, double Vy, int Vz) {
    Left_front= (int)(+Vx + Vy - Vz * 0.18);
    Right_front = (int)(-Vx + Vy + Vz * 0.18);
    Right_rear = (int)(+Vx + Vy + Vz * 0.18);
    Left_rear = (int)(-Vx + Vy - Vz * 0.18);
}
