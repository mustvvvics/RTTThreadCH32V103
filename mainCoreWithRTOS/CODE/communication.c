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

uint8 Gyro_buff[8];
void send_to_cam(void)
{
    //发送陀螺仪数据
    Gyro_buff[0] = 0xD8;                            //帧头

    Gyro_buff[1] = 0xB0;                            //功能字
    Gyro_buff[2] = 0xB1;                            //功能字
    Gyro_buff[3] = 0xB2;                            //功能字
    Gyro_buff[4] = 0xB3;                            //功能字
    Gyro_buff[5] = roundFinishFlag;                 //发送数据
    Gyro_buff[6] = 0xB3;                            //功能字

    Gyro_buff[7] = 0xEE;                            //帧尾
    uart_putbuff(UART_3, Gyro_buff, 8);             //通过串口3将数据发送出去。
}

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

int8 absInt8(int8 x){return x>0?x:-x;}
/*
 * 在主核按键触后进行修改
 * 约有20个变量
 * parameterBit:数据位数           0:int8  ; 8: uint8  ; 16: int16 ;  32 int32
 * featuresWord:功能字               0xE1 0xE2 ...
 * 数据位数,功能字,  其后是传输的数据格式
 */

uint8 parameterBuff[8];
uint8 negativeNumber = 0;
void sendParameterToCam(uint8 parameterBit,uint8 featuresWord,int8 parameterData_0,uint8 parameterData8,int16 parameterData16,int32 parameterData32){
    //向图像核心发送相关数据
    if (parameterBit == 0) {
        if (parameterData_0 < 0) {negativeNumber = 1;}
        else {negativeNumber = 0;}
        parameterBuff[0] = 0xDE;                            //帧头

        parameterBuff[1] = 0xA0;                            //功能字占位省开数组
        parameterBuff[2] = 0xA0;                            //功能字占位省开数组
        parameterBuff[3] = 0xA8;                            //功能字占位省开数组
        parameterBuff[4] = featuresWord;                    //功能字
        parameterBuff[5] = negativeNumber;                  //int8 正负传递

        parameterBuff[6] = (uint8)absInt8(parameterData_0); //数据

        parameterBuff[7] = 0xEE;                            //帧尾
        uart_putbuff(UART_3, parameterBuff, 8);             //通过串口3将数据发送出去。
    }
    else if (parameterBit == 8) {
        parameterBuff[0] = 0xDE;                            //帧头

        parameterBuff[1] = 0xA8;                            //功能字占位省开数组
        parameterBuff[2] = 0xA0;                            //功能字占位省开数组
        parameterBuff[3] = 0xA0;                            //功能字占位省开数组
        parameterBuff[4] = 0xA0;                            //功能字占位省开数组
        parameterBuff[5] = featuresWord;                    //功能字

        parameterBuff[6] = parameterData8;                 //数据

        parameterBuff[7] = 0xEE;                            //帧尾
        uart_putbuff(UART_3, parameterBuff, 8);             //通过串口3将数据发送出去。
    }
    else if (parameterBit == 16) { //parameter>>8;parameter&0xFF
        parameterBuff[0] = 0xDE;                            //帧头

        parameterBuff[1] = 0xA0;                            //功能字占位省开数组
        parameterBuff[2] = 0xA1;                            //功能字占位省开数组
        parameterBuff[3] = 0xA6;                            //功能字占位省开数组
        parameterBuff[4] = featuresWord;                    //功能字

        parameterBuff[5] = (parameterData16>>8)&0xFF;       //数据高八位
        parameterBuff[6] = parameterData16&0xFF;            //数据第八位

        parameterBuff[7] = 0xEE;                            //帧尾
        uart_putbuff(UART_3, parameterBuff, 8);             //通过串口3将数据发送出去。
    }
    else if (parameterBit == 32) { ////Parameter>>8;Parameter>>16;Parameter>>24;Parameter&0xFF;
        parameterBuff[0] = 0xDE;                            //帧头

        parameterBuff[1] = 0xA3;                            //功能字占位省开数组
        parameterBuff[2] = featuresWord;                    //功能字

        parameterBuff[3] = (parameterData32>>24)&0xFF;       //数据
        parameterBuff[4] = (parameterData32>>16)&0xFF;      //其反解析：u32 = (u8[0]<<24)|(u8[1]<<16)|(u8[2]<<8)| u8[3];
        parameterBuff[5] = (parameterData32>>8)&0xFF;
        parameterBuff[6] = parameterData32&0xFF;

        parameterBuff[7] = 0xEE;                            //帧尾
        uart_putbuff(UART_3, parameterBuff, 8);             //通过串口3将数据发送出去。
    }
    else {
        return;
    }

}


