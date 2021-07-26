#include "headfile.h"
#include "MyFuzzy.h"

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

void motor1_ctl(int32 speed1_power)
{
    if(0<=speed1_power) //���1    ����ռ�ձ�Ϊ �ٷ�֮ (1000/TIMER1_PWM_DUTY_MAX*100)
    {
        gpio_set(MOTOR1_A, 1);
        pwm_duty(MOTOR1_B, speed1_power);
    }
    else                //���1
    {
        gpio_set(MOTOR1_A, 0);
        pwm_duty(MOTOR1_B, -speed1_power);
    }
}

//>0��ת
void motor2_ctl(int32 speed2_power)
{
    if(0<=speed2_power) //���2
    {
        gpio_set(MOTOR2_A, 0);
        pwm_duty(MOTOR2_B, speed2_power);
    }
    else                //���2
    {
        gpio_set(MOTOR2_A, 1);
        pwm_duty(MOTOR2_B, -speed2_power);
    }
}

void motor3_ctl(int32 speed3_power)
{
    if(0<=speed3_power) //���3
    {
        gpio_set(MOTOR3_A, 0);
        pwm_duty(MOTOR3_B, speed3_power);
    }
    else                //���3
    {
        gpio_set(MOTOR3_A, 1);
        pwm_duty(MOTOR3_B, -speed3_power);
    }
}

void motor4_ctl(int32 speed4_power)
{
    if(0<=speed4_power) //���3
    {
        gpio_set(MOTOR4_A, 1);
        pwm_duty(MOTOR4_B, speed4_power);
    }
    else                //���3
    {
        gpio_set(MOTOR4_A, 0);
        pwm_duty(MOTOR4_B, -speed4_power);
    }
}
/*******************************************************************************************************/
/*
 * �ٶȽ���
 */
/*******************************************************************************************************/
void speed_conversion(float Vx, float Vy,float Vz)
{
    Left_front= (int16)(+Vx + Vy - Vz);
    Right_front = (int16)(-Vx + Vy + Vz);
    Right_rear = (int16)(+Vx + Vy + Vz);
    Left_rear = (int16)(-Vx + Vy - Vz);
}

/*******************************************************************************************************/
/*
 * ���Բ����н�����
 */
/*******************************************************************************************************/
uint8 car_flagPre = 0;
void testSideAdvancementAbility(void){
    if (car_flag == 1 || car_flag == 2) {
        car_flag = 2;
        if (carFlagPre == 0 && car_flag == 2) {
            clearCamFlags = 1;confirmButton = 0;carStart = 2;
            sendParameterToCam(8,0xAB,0,carStart,0,0);//�����ź�2
            sendParameterToCam(8,0xE2,0,clearCamFlags,0,0);//���
            carFlagPre = 1;
        }
        expected_omega = Fuzzy((position_front),(position_front_delta)); //ģ��PID
//        expected_omega = PID_Loc(0,-position_front,&yaw_pid);
        if (accelerate == 0) {
            clearError();
            speed_conversion(0,0,0);
        }
        else {
            speed_conversion((-expected_y * accelerate) / 10,0, PID_Angle(expected_omega,g_fGyroAngleSpeed_z,&yaw_w_pid)+(expected_omega/11) );
        }
    }
    else {
        carStart = 0;
        carFlagPre = 0;
        clearError();clearFlags();
        speed_conversion(0,0,0);
    }
}
