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

////速度解算
//void speed_conversion(float Vx, float Vy, float Vz) {
//    Left_front= (int16)(+Vx + Vy - Vz * 0.18);
//    Right_front = (int16)(-Vx + Vy + Vz * 0.18);
//    Right_rear = (int16)(+Vx + Vy + Vz * 0.18);
//    Left_rear = (int16)(-Vx + Vy - Vz * 0.18);
//
////    leftFrontADRC = 35.333 * Left_front + 50.834;
////    rightFrontADRC = 36.246 * Right_front + 152.89;
////    rightRearADRC = 40.119 * Right_rear + 204.74 ;
////    leftRearADRC = 51.127 * Left_rear + 285.22;
//
//
//}


//速度解算
void speed_conversion(float Vx, float Vy,float Vz)
{
    Left_front= (int16)(+Vx + Vy - Vz);
    Right_front = (int16)(-Vx + Vy + Vz);
    Right_rear = (int16)(+Vx + Vy + Vz);
    Left_rear = (int16)(-Vx + Vy - Vz);
}

void enc_conversion(float Vx, float Vy)
{
    encoder_data[3]= (int16)(+Vx + Vy);
    encoder_data[2] = (int16)(-Vx + Vy);
    encoder_data[0] = (int16)(+Vx + Vy);
    encoder_data[1] = (int16)(-Vx + Vy);
}

void conversion_speed(void)
{
    //static int16 Encoder_Least;
    //Encoder_Least =(encoder_left+encoder_right)-0;                    //===获取最新速度偏差==测量速度（左右编码器之和）-目标速度（此处为零）
    //Encoder *= 0.8;                                                     //===一阶低通滤波器
    //Encoder += Encoder_Least*0.2;                                       //===一阶低通滤波器

    real_x=-(-encoder_data[3] + encoder_data[2] - encoder_data[0] + encoder_data[1])/4;
    real_y=-(encoder_data[3] + encoder_data[2] + encoder_data[1] + encoder_data[0])/4;
}

//
void motor_conversion(void)
{

    if (car_flag == 1)
    {
        expected_omega = PID_Loc(0,-position_front,&yaw_pid);
        //x和y轴的期望速度输入
        speed_conversion(0,expected_y*accelerate/10,PID_Angle(expected_omega,g_fGyroAngleSpeed_z,&yaw_w_pid)+(expected_omega/11));
    }
    else
    {
        speed_conversion(0,0,0);
    }

    conversion_speed();//先正
    //输出PWM  ADRC模型+速度环
    motor1_pwm=PID_Speed(Left_front,-encoder_data[3],&motor1_pid)+(Left_front*37+419);
    motor2_pwm=PID_Speed(Right_front,-encoder_data[2],&motor2_pid)+(Right_front*38+198);
    motor3_pwm=PID_Speed(Right_rear,-encoder_data[0],&motor3_pid)+(Right_rear*41+287);
    motor4_pwm=PID_Speed(Left_rear,-encoder_data[1],&motor4_pid)+(Left_rear*40+379);

    //ENC3 y = 0.0264x - 11.085
    //y37.878+419=x;
    //ENC2 y = 0.0259x - 5.1331
    //198.2+38.6y=x;
    //ENC0 y = 0.0241x - 6.9405
    //287+41y=x;
    //ENC1 y = 0.0246x - 9.3414
    // 379+40.65y=x;
    //限幅输出
    if(motor1_pwm>limit_pwm)motor1_pwm=limit_pwm;
    if(motor1_pwm<-limit_pwm)motor1_pwm=-limit_pwm;

    if(motor2_pwm>limit_pwm)motor2_pwm=limit_pwm;
    if(motor2_pwm<-limit_pwm)motor2_pwm=-limit_pwm;

    if(motor3_pwm>limit_pwm)motor3_pwm=limit_pwm;
    if(motor3_pwm<-limit_pwm)motor3_pwm=-limit_pwm;

    if(motor4_pwm>limit_pwm)motor4_pwm=limit_pwm;
    if(motor4_pwm<-limit_pwm)motor4_pwm=-limit_pwm;

    motor1_ctl(motor1_pwm);
    motor2_ctl(motor2_pwm);
    motor3_ctl(motor3_pwm);
    motor4_ctl(motor4_pwm);

}
//motor1_ctl(leftFrontADRC + );
//motor2_ctl(rightFrontADRC + );
//motor3_ctl(rightRearADRC + );
//motor4_ctl(leftRearADRC + );
//leftFrontADRC = 35.333 * Left_front + 50.834;
//rightFrontADRC = 36.246 * Right_front + 152.89;
//rightRearADRC = 40.119 * Right_rear + 204.74 ;
//leftRearADRC = 51.127 * Left_rear + 285.22;

