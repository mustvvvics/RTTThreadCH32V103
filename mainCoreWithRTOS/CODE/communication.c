/*
 * communication.c
 *
 *  Created on: Jun 30, 2021
 *      Author: 29275
 */
#include "headfile.h"

uint8   temp_buff[LINE_LEN];            //主机用于接收数据的BUFF
vuint8  uart_flag;                   //接收数据标志位

void get_slave_data(uint8 data)
{
    static uint8 uart_num = 0;
    temp_buff[uart_num++] = data;

    if(1 == uart_num)
    {
        //帧头错误 接收到的第一个字符不为0xD8
        if(0xD8 != temp_buff[0]){
            uart_num = 0;
            uart_flag = E_FRAME_HEADER_ERROR;
        }
    }

    if(LINE_LEN == uart_num)
    {
        uart_flag = E_OK;
        //接收到最后一个字节为0xEE
        if(0xEE == temp_buff[LINE_LEN - 1]){
            uart_flag = E_OK;
        }
        else{    //帧尾错误 接收到最后一个字节不是0xEE
            uart_flag = E_FRAME_RTAIL_ERROR;
        }
        uart_num = 0;
    }
}

void data_analysis(uint8 *line)
{
    if(line[1] == 0xB0)    encoder_data[2] = ((int16)line[2] << 8) | line[3];
    if(line[4] == 0xB1)    encoder_data[3] = ((int16)line[5] << 8) | line[6];
    if(line[7] == 0xB2)    position_front  = ((int16)line[8] << 8) | line[9];
    if(line[10] == 0xB3)   accelerate  = line[11];
    if(line[12] == 0xB4)   elementFlag  = line[13];

    switch (elementFlag) {
        case 0://无元素
            roundIslandBegin = 0;
            break;
        case 1://三叉
//            ThreeWayIntersectionFlag = 1; //在一个周期内使得camera error = 0;
            car_flag = 0;
            break;
        case 2://车库

            break;
        case 3: //刹车
            car_flag = 0;
            break;
        case 4:
            roundIslandBegin = 1; //遇见环岛 陀螺仪开始积分
            break;
    }

}
/*
 * 三叉路口决策
 */
int8 ThreeWayDirection = 0;
int16 threeWayOutAngle = 0;
uint8 threeWayIn = 0;
uint8 threeWayOut = 0;
uint8 threeWaySum = 0;
void ThreeWayAnalyze(void){
    if (ThreeWayIntersectionFlag == 1 && threeWayIn == 0 && threeWaySum == 0) {//第一次进入
        ThreeWayIntersectionFlag = 0;//清标志
        ThreeWayDirection = -1; //才能按照摄像头朝向行驶  中断中行驶决策
        threeWayIn = 1;
        pwm_duty(PWM1_CH1_A8, 990); //舵机左转
      }
    else if (ThreeWayIntersectionFlag == 1 && threeWayIn == 1 && threeWaySum == 0) { //第一次出
        ThreeWayIntersectionFlag = 0;
//        threeWayIn = 0;               //在外部中断中,当完成姿态变形出三叉后 释放flag
        threeWayOut = 1;                //用于在中断中变形
        threeWayOutAngle = 1500;          //变形力度
        threeWaySum = 1;                //进入一次三叉
        pwm_duty(PWM1_CH1_A8, 670);
    }
    else if (ThreeWayIntersectionFlag == 1 && threeWayIn == 0 && threeWaySum == 1) { //第二次进
        ThreeWayIntersectionFlag = 0; //才能按照摄像头朝向行驶
        ThreeWayDirection = 1;
        threeWayIn = 1;
        pwm_duty(PWM1_CH1_A8, 338); //舵机右转
    }
    else if (ThreeWayIntersectionFlag == 1 && threeWayIn == 1 && threeWaySum == 1){ //第二次出
        ThreeWayIntersectionFlag = 0;
//        threeWayIn = 0;
        threeWayOut = 1;  //用于在中断中变形
        threeWayOutAngle = -1500;
        threeWaySum = 0;
        pwm_duty(PWM1_CH1_A8, 670);
    }
}

uint8 Gyro_buff[4];

void send_to_cam(void)
{
    //发送陀螺仪数据
    Gyro_buff[0] = 0xD8;                            //帧头

    Gyro_buff[1] = 0xB0;                            //功能字
    Gyro_buff[2] = roundFinishFlag;                 //发送数据

    Gyro_buff[3] = 0xEE;                            //帧尾
    uart_putbuff(UART_3, Gyro_buff, 4);             //通过串口3将数据发送出去。
}

//            if(count_en == 1)
//            {
//                //里程计
//                    左边两个向内,右边两个向外 ->向左行进
//                dx += (-encoder_data[3] + encoder_data[0] + encoder_data[2] - encoder_data[1])/4;
//                    四个轮子正值相加
//                dy += (encoder_data[3] + encoder_data[2] + encoder_data[1] + encoder_data[0])/4;
//                dz += (-encoder_data[3] - encoder_data[0] + encoder_data[2] + encoder_data[1])/4;
//                //dist = sqrt(dx*dx+dy*dy);
//                //total_z += (int16)g_fGyroAngleSpeed_z;
//                //dx=0;dy=0;dz=0;dist=0;total_z=0;count_en=0;
//            }//清空并关闭里程计

/*
 * 环岛陀螺仪积分
 */
uint8 roundIslandBeginPre = 0;
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
        if (total_z > 84000 || total_z < -84000) {
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
