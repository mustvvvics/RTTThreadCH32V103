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
//void speed_conversion(float Vx, float Vy, float Vz) {
//    Left_front= (int16)(+Vx + Vy - Vz * 0.18);//*0.18 削弱z轴作用
//    Right_front = (int16)(-Vx + Vy + Vz * 0.18);
//    Right_rear = (int16)(+Vx + Vy + Vz * 0.18);
//    Left_rear = (int16)(-Vx + Vy - Vz * 0.18);
//}


//速度解算
void speed_conversion(float Vx, float Vy,float Vz)
{
    Left_front= (int16)(+Vx + Vy - Vz);
    Right_front = (int16)(-Vx + Vy + Vz);
    Right_rear = (int16)(+Vx + Vy + Vz);
    Left_rear = (int16)(-Vx + Vy - Vz);
}

//void enc_conversion(float Vx, float Vy)
//{
//    encoder_data[3]= (int16)(+Vx + Vy);
//    encoder_data[2] = (int16)(-Vx + Vy);
//    encoder_data[0] = (int16)(+Vx + Vy);
//    encoder_data[1] = (int16)(-Vx + Vy);
//}
//
//void conversion_speed(void)
//{
//    real_x=-(-encoder_data[3] + encoder_data[2] - encoder_data[0] + encoder_data[1])/4;
//    real_y=-(encoder_data[3] + encoder_data[2] + encoder_data[1] + encoder_data[0])/4;
//}

//
void motor_conversion(void)
{

    if (car_flag == 1 && threeWayIn == 0 && threeWaySum == 0 )//正向行驶
    {
        if(roundIslandBegin)
        {
            expected_omega = PID_Loc(0,-position_front,&yaw_pid);
        }
        else
        {
            expected_omega = Fuzzy((position_front),(position_front_delta)); //模糊PID
        }
        //x,y,z轴的期望速度输入
//        speed_conversion(0,dynamic_programming(-position_front,position_front_delta),PID_Angle(expected_omega,g_fGyroAngleSpeed_z,&yaw_w_pid)+(expected_omega/11));
        speed_conversion(0,(expected_y * accelerate) / 10,PID_Angle(expected_omega,g_fGyroAngleSpeed_z,&yaw_w_pid)+(expected_omega/11));
    }
    else if (car_flag == 1 && threeWayIn == 1) { //变形
        expected_omega = Fuzzy((position_front),(position_front_delta)); //模糊PID
        speed_conversion((-expected_y * accelerate) / 10,0,PID_Angle(expected_omega,g_fGyroAngleSpeed_z,&yaw_w_pid)+(expected_omega/11));
    }
    else if (car_flag == 1 && threeWayIn == 0 && threeWaySum == 1 ) {//逆向行驶
        expected_omega = Fuzzy((position_front),(position_front_delta)); //模糊PID
                //x,y,z轴的期望速度输入
        //        speed_conversion(0,dynamic_programming(-position_front,position_front_delta),PID_Angle(expected_omega,g_fGyroAngleSpeed_z,&yaw_w_pid)+(expected_omega/11));
        speed_conversion(0,(-expected_y * accelerate) / 10 ,PID_Angle(expected_omega,g_fGyroAngleSpeed_z,&yaw_w_pid)+(expected_omega/11));
    }
    else
    {
        clearError();
        speed_conversion(0,0,0);
    }

    //输出PWM  ADRC模型+速度环
    motor1_pwm=PID_Speed(Left_front,-encoder_data[3],&motor1_pid)+(Left_front*37+419);
    motor2_pwm=PID_Speed(Right_front,-encoder_data[2],&motor2_pid)+(Right_front*38+198);
    motor3_pwm=PID_Speed(Right_rear,-encoder_data[0],&motor3_pid)+(Right_rear*41+287);
    motor4_pwm=PID_Speed(Left_rear,-encoder_data[1],&motor4_pid)+(Left_rear*40+379);

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
//舵机朝向与行驶在中线的镜头偏移量
//    if (steerStatusFromMain == 0) {
//        globalCenterBias = -7;    //速度45
//    } else if (steerStatusFromMain == 1){
//        globalCenterBias = -13;   //速度40
//    } else if (steerStatusFromMain == 2) {
//        globalCenterBias = -5;    //速度45

//ENC3 y = 0.0264x - 11.085
//y37.878+419=x;
//ENC2 y = 0.0259x - 5.1331
//198.2+38.6y=x;
//ENC0 y = 0.0241x - 6.9405
//287+41y=x;
//ENC1 y = 0.0246x - 9.3414
// 379+40.65y=x;


//    }
