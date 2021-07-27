#include "headfile.h"

/************************************************************************************************************/
/*
 * 清空误差 && 清空标志位
 */
/************************************************************************************************************/
void clearError(void){
    expected_omega = 0;position_front = 0;g_fGyroAngleSpeed_z = 0;
    yaw_pid.LocSum = 0;yaw_pid.Ek = 0;yaw_pid.Ek1 = 0;                  //角速度环
    yaw_w_pid.target_val = 0;yaw_w_pid.err_next = 0;yaw_w_pid.err = 0;  //转向环
    yaw_w_pid.err_last = 0;yaw_w_pid.actual_val = 0;
}

void clearFlags(void){
    go_forward=0;go_backward=0;go_left=0;go_right=0;
    elementFlag = 0;roundIslandBegin = 0;ThreeWayIntersectionFlag = 0;
}

/************************************************************************************************************/
/*
 * 环岛陀螺仪积分
 */
/************************************************************************************************************/
uint8 roundIslandBeginPre = 0;
int32 roundIslandMax = 80000;          //出环岛最大积分
void roundIslandAnalyze(void){

    if (roundIslandBeginPre == 0 && roundIslandBegin == 1) {
        total_z = g_fGyroAngleSpeed_z;
        roundIslandBeginPre = 1;
    }
    else if (roundIslandBeginPre == 1 && roundIslandBegin == 0) {
        roundIslandBeginPre = 0;
    }
    else if (roundIslandBeginPre == 1 && roundIslandBegin == 1){
        total_z += g_fGyroAngleSpeed_z;
        if (total_z > 2000000000 || total_z < -2000000000) {total_z = 0;}//限幅防止越界
        if (total_z > roundIslandMax || total_z < -roundIslandMax + 2000) {
            roundIslandBegin = 0;
            roundFinishFlag = 1;
            total_z = 0;
            send_to_cam();
        }
    }
    else {
        return;
    }
}

/************************************************************************************************************/
/*
 * 三叉路口决策
 */
/************************************************************************************************************/
uint8 threeWayIn = 0;                       //进入三叉
uint8 threeWaySum = 0;                      //三叉计数
uint8 ThreeWayIntersectionFlagPre = 0;      //检测上升沿使用
uint8 steerStatus = 0;                      //0 朝向前  1 朝向左  2朝向后
void ThreeWayAnalyze(void){
    if (ThreeWayIntersectionFlagPre == 1 && ThreeWayIntersectionFlag == 0) {//正常路段会发送flag = 0
        ThreeWayIntersectionFlagPre = 0; //还原上升沿触发
    }
    else if (ThreeWayIntersectionFlagPre == 0 && ThreeWayIntersectionFlag == 1 && threeWayIn == 0 && threeWaySum == 0) {//第一次进入
        pwm_duty(PWM1_CH1_A8, 672); //舵机向左
        steerStatus = 1;
        sendParameterToCam(8,0xDA,0,steerStatus,0,0);//告知cam舵机状态
        ThreeWayIntersectionFlagPre = 1;//已完成上升沿
        threeWayIn = 1;
      }
    //第一次出三叉
    else if (ThreeWayIntersectionFlagPre == 0 && ThreeWayIntersectionFlag == 1 && threeWayIn == 1 && threeWaySum == 0) { //第一次出
        pwm_duty(PWM1_CH1_A8, 1000);     //第一次出转舵机向后
        steerStatus = 2;
        sendParameterToCam(8,0xDA,0,steerStatus,0,0);//告知cam舵机状态
        ThreeWayIntersectionFlagPre = 1;//已完成上升沿
        threeWayIn = 0;               //先前进入三叉 此次是出去
        threeWaySum = 1;                //进入一次三叉

    }
    else if (ThreeWayIntersectionFlagPre == 0 && ThreeWayIntersectionFlag == 1 && threeWayIn == 0 && threeWaySum == 1) { //第二次进
        pwm_duty(PWM1_CH1_A8, 672); //舵机向左
        steerStatus = 1;
        sendParameterToCam(8,0xDA,0,steerStatus,0,0);//告知cam舵机状态
        ThreeWayIntersectionFlagPre = 1;//已完成上升沿
        threeWayIn = 1;
    }

    //第二次出三叉
    else if (ThreeWayIntersectionFlagPre == 0 && ThreeWayIntersectionFlag == 1 && threeWayIn == 1 && threeWaySum == 1){ //第二次出
        pwm_duty(PWM1_CH1_A8, 338); //舵机还原
        steerStatus = 0;
        sendParameterToCam(8,0xDA,0,steerStatus,0,0);//告知cam舵机状态
        ThreeWayIntersectionFlagPre = 1;//已完成上升沿
        threeWayIn = 0;
        threeWaySum = 0;
    }
}
/*******************************************************************************************************/
/*
 * 里程计  encoder_x 侧面行驶路程      expected_X 期待值
 */
/*******************************************************************************************************/
int32 encoder_x = 0,encoder_y = 0;                //里程计
uint8 encoderCountYFlag = 0;
uint8 encoderCountYFlagPre = 0;
uint8 encoderCountYFlagMain = 0;
void encoderCountX(void){
    encoder_x +=  -(-encoder_data[3] + encoder_data[2] - encoder_data[0] + encoder_data[1])/4;
}
/*
 * 在元素之间积分  遇到元素置零  元素结束开始
 */
void encoderCountY(void){ //use in isr
    if (encoderCountYFlagMain == 0) {
        if (encoderCountYFlagPre == 0 && encoderCountYFlag == 1) { //上升沿
            encoderCountYFlagPre = 1;
        }
        else if (encoderCountYFlagPre == 1 && encoderCountYFlag == 0) {//下降沿
            encoderCountYFlagPre = 0;
        }
        else if (encoderCountYFlagPre == 1 && encoderCountYFlag == 1) {//进入上升期 开始积分
            encoder_y += -(encoder_data[3] + encoder_data[2] + encoder_data[1] + encoder_data[0])/4;
            if (encoder_y > 2000000000 || encoder_y < -2000000000) {encoder_y = 0;} //限幅防止越界
        }
        else {//都为零时清空
            encoder_y = 0;
        }
    }
    else {
        if (encoderCountYFlagMain == 1) {
            encoder_y += -(encoder_data[3] + encoder_data[2] + encoder_data[1] + encoder_data[0])/4;
            if (encoder_y > 2000000000 || encoder_y < -2000000000) {encoder_y = 0;} //限幅防止越界
        }
        else {
            encoder_y = 0;
        }
    }
}

/************************************************************************************************************/
/*
 * 控制方式选择
 */
/************************************************************************************************************/
uint8 testSideFlag = 0;
void motor_conversion(void)
{
    if (testSideFlag == 1) {
        testSideAdvancementAbility();
    }   //测试侧面行进能力
    else {
        if (car_flag == 1) {
            if (drivingDirectionToCam == 1) {           //向右出库
                speed_conversion(-expected_y, 0, 0);
            }
            else {
                speed_conversion(expected_y, 0, 0);     //向左出库
            }
            encoderCountX();             //编码器计数
            if (encoder_x >= expected_X || encoder_x <= -expected_X) {
                car_flag = 2;                                       //开始行驶
                clearCamFlags = 1;confirmButton = 0;
//                carStart = 2;   //清理;取消按键;发送信号2;
//                sendParameterToCam(8,0xAB,0,carStart,0,0);          //发送启动信号2
                sendParameterToCam(8,0xE2,0,clearCamFlags,0,0);     //清空cam flag
//                carStart = 0;
                encoder_x = 0;                         //使得停下后可以再次启动
            }
        }
    /************************************************************************************************************/
        //正向行驶
        else if (car_flag == 2 && threeWayIn == 0 && threeWaySum == 0)
        {
            carFlagPre = 0;
            if(roundIslandBegin)                        //环岛PID
            {
                expected_y = 55;
//                yaw_pid.Kp = 8;yaw_pid.Kd = 0.005;   //全局可控
                expected_omega = PID_Loc(0,-position_front,&yaw_pid);//环岛 P:8.0 D:0.005
            }
            else
            {
                expected_omega = Fuzzy((position_front),(position_front_delta)); //模糊PID
            }
            if (accelerate == 0) {clearError();speed_conversion(0,0,0);}    //转完舵机停一会
            else {
    //            speed_conversion(0,dynamic_programming(-position_front,position_front_delta), \           //带主核加速
    //                    PID_Angle(expected_omega,g_fGyroAngleSpeed_z,&yaw_w_pid)+(expected_omega/11));
                speed_conversion(0,(expected_y * accelerate) / 10,    \
                        PID_Angle(expected_omega,g_fGyroAngleSpeed_z,&yaw_w_pid)+(expected_omega/11));
            }

        }
    /************************************************************************************************************/
        //进三叉行驶              //p22.0 d:0.005 sp:50   // p:16.5   d:0.005   sp=45    // p:11.0 d:0.005 sp:40
        else if (car_flag == 2 && threeWayIn == 1) {
    //        yaw_pid.Kp = 26;yaw_pid.Kd = 0.005;
            expected_y = 40;
            if (accelerate == 0) {clearError();speed_conversion(0,0,0);}    //转完舵机停一会
            else {
    //            expected_omega = PID_Loc(0,-position_front,&yaw_pid);     //位置PID
                expected_omega = Fuzzy((position_front),(position_front_delta)); //模糊PID
                speed_conversion((-expected_y * accelerate) / 10, 0, \
                        PID_Angle(expected_omega,g_fGyroAngleSpeed_z,&yaw_w_pid)+(expected_omega/11) );
            }
        }
    /************************************************************************************************************/
        //退出三叉行驶
        else if (car_flag == 2  && threeWayIn == 0 && threeWaySum == 1) {
            if(roundIslandBegin)                      //环岛PID
            {
                expected_y = 55;
//                yaw_pid.Kp = 8;yaw_pid.Kd = 0.005;  //全局可控
                expected_omega = PID_Loc(0,-position_front,&yaw_pid);
            }
            else
            {

                expected_omega = Fuzzy((position_front),(position_front_delta)); //模糊PID
            }
            if (accelerate == 0) {clearError();speed_conversion(0,0,0);}         //转完舵机停一会
            else {
    //            speed_conversion(0,dynamic_programming(-position_front,position_front_delta), \            //带主核加速
    //                    PID_Angle(expected_omega,g_fGyroAngleSpeed_z,&yaw_w_pid)+(expected_omega/11));
                speed_conversion(0,(-expected_y * accelerate) / 10 , \
                        PID_Angle(expected_omega,g_fGyroAngleSpeed_z,&yaw_w_pid)+(expected_omega/11));
            }


        }
    /************************************************************************************************************/
//        else if (carFlagPre == 1 && car_flag == 0) {//停下来给一次信号1
//            carStart = 1;
//            sendParameterToCam(8,0xAB,0,carStart,0,0);//启动信号1
//            carFlagPre = 0;
//        }
        else
        {
            clearError();clearFlags();
            speed_conversion(0,0,0);
        }
    }
}
