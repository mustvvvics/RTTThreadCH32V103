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
void conversion_speed(void)
{
    //整车编码器
//    real_x=-(-encoder_data[3] + encoder_data[2] - encoder_data[0] + encoder_data[1])/4;
//    real_y=-(encoder_data[3] + encoder_data[2] + encoder_data[1] + encoder_data[0])/4;
    //路程计
    real_x +=  -(-encoder_data[3] + encoder_data[2] - encoder_data[0] + encoder_data[1])/4;

}
void speedPidConversion(void){
    if ((expected_y * accelerate) / 10 == 40) {
        yaw_pid.Kp = 11;yaw_pid.Kd = 0.005;
    }
    else if ((expected_y * accelerate) / 10 == 45) {
        yaw_pid.Kp = 16.5;yaw_pid.Kd = 0.005;
    }
    else if ((expected_y * accelerate) / 10 == 50) {
        yaw_pid.Kp = 22;yaw_pid.Kd = 0.005;
    }
    else {
        yaw_pid.Kp = 8;yaw_pid.Kd = 0.005;
    }
}
uint8 car_flagPre = 0;
void motor_conversion(void)
{
    //for test
/**************************************************************************************************/
//    if (car_flag == 1 || car_flag == 2) {
//        car_flag = 2;
////        speedPidConversion();//分速度段PID
//
//        if (carFlagPre == 0 && car_flag == 2) {
//            clearCamFlags = 1;confirmButton = 0;carStart = 2;
//            sendParameterToCam(8,0xAB,0,carStart,0,0);//启动信号2
//            sendParameterToCam(8,0xE2,0,clearCamFlags,0,0);//清空
//            carFlagPre = 1;
//        }
//        expected_omega = Fuzzy((position_front),(position_front_delta)); //模糊PID
////        expected_omega = PID_Loc(0,-position_front,&yaw_pid);
//        if (accelerate == 0) {
//            clearError();
//            speed_conversion(0,0,0);
//        }
//        else {
//            speed_conversion((-expected_y * accelerate) / 10,0, PID_Angle(expected_omega,g_fGyroAngleSpeed_z,&yaw_w_pid)+(expected_omega/11) );
//        }
//    }
//    else {
//        carStart = 0;
//        carFlagPre = 0;
//        clearError();clearFlags();
//        speed_conversion(0,0,0);
//    }
/**************************************************************************************************/
    //use this
    if (car_flag == 1) {
        speed_conversion(-expected_y, 0, 0);
        conversion_speed();//编码器计数
        if (real_x >= expected_X || real_x <= -expected_X) {
            car_flag = 2;
            clearCamFlags = 1;confirmButton = 0;carStart = 2;
            sendParameterToCam(8,0xAB,0,carStart,0,0);//启动信号2
            sendParameterToCam(8,0xE2,0,clearCamFlags,0,0);//清空
            carStart = 0;
//            real_x = 0;
        }
    }
    else if (car_flag == 2 && threeWayIn == 0 && threeWaySum == 0)//正向行驶
    {
        carFlagPre = 0;
        if(roundIslandBegin)
        {
            yaw_pid.Kp = 8;yaw_pid.Kd = 0.005;
            expected_omega = PID_Loc(0,-position_front,&yaw_pid);//环岛 P:8.0 D:0.005
        }
        else
        {
            expected_omega = Fuzzy((position_front),(position_front_delta)); //模糊PID
        }
        //x,y,z轴的期望速度输入
//        speed_conversion(0,dynamic_programming(-position_front,position_front_delta),PID_Angle(expected_omega,g_fGyroAngleSpeed_z,&yaw_w_pid)+(expected_omega/11));
        speed_conversion(0,(expected_y * accelerate) / 10,PID_Angle(expected_omega,g_fGyroAngleSpeed_z,&yaw_w_pid)+(expected_omega/11));
    }
        //p22.0 d:0.005 sp:50   // p:16.5   d:0.005   sp=45    // p:11.0 d:0.005 sp:40
    else if (car_flag == 2 && threeWayIn == 1) {
//        speedPidConversion();//分速度段PID
        yaw_pid.Kp = 26;yaw_pid.Kd = 0.005;

        if (accelerate == 0) {
            clearError();
            speed_conversion(0,0,0);
        }
        else {
//            expected_omega = PID_Loc(0,-position_front,&yaw_pid);
            expected_omega = Fuzzy((position_front),(position_front_delta)); //模糊PID
            speed_conversion((-expected_y * accelerate) / 10,0, PID_Angle(expected_omega,g_fGyroAngleSpeed_z,&yaw_w_pid)+(expected_omega/11) );
        }

    }
    else if (car_flag == 2  && threeWayIn == 0 && threeWaySum == 1) {
        if(roundIslandBegin)
        {
            yaw_pid.Kp = 8;yaw_pid.Kd = 0.005;
            expected_omega = PID_Loc(0,-position_front,&yaw_pid);
        }
        else
        {
            expected_omega = Fuzzy((position_front),(position_front_delta)); //模糊PID
        }
        if (accelerate == 0) {
            clearError();
            speed_conversion(0,0,0);
        }
        else {
            speed_conversion(0,(-expected_y * accelerate) / 10 ,PID_Angle(expected_omega,g_fGyroAngleSpeed_z,&yaw_w_pid)+(expected_omega/11));
        }
                //x,y,z轴的期望速度输入
//        speed_conversion(0,dynamic_programming(-position_front,position_front_delta),PID_Angle(expected_omega,g_fGyroAngleSpeed_z,&yaw_w_pid)+(expected_omega/11));

    }
    else if (carFlagPre == 1 && car_flag == 0) {//停下来给一次信号1
        carStart = 1;
        sendParameterToCam(8,0xAB,0,carStart,0,0);//启动信号2
        carFlagPre = 0;
    }
    else
    {

        clearError();clearFlags();
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
