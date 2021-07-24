/*
 * communication.c
 *
 *  Created on: Jun 30, 2021
 *      Author: 29275
 */
#include "headfile.h"

uint8   temp_buff[LINE_LEN];            //主机用于接收数据的BUFF
vuint8  uart_flag;                   //接收数据标志位

uint8 sendMainFlashToCam = 0;
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
            ThreeWayIntersectionFlag = 0;
            sendMainFlashToCam = 0;
            break;
        case 1://三叉
            if (car_flag == 1) {
                ThreeWayIntersectionFlag = 1; //在一个周期内使得camera error = 0;
            }
            break;
        case 2://车库

            break;
        case 3: //刹车
            car_flag = 0;
//            sendFuzzyData();
            break;
        case 4:
            if (car_flag == 1) {
                roundIslandBegin = 1; //遇见环岛 陀螺仪开始积分
            }
            break;
        case 5:
            sendMainFlashToCam = 1;
            break;
    }

}
void threeWayClearOtherThings(void){
    position_front_delta = 0;position_front = 0;expected_omega = 0;g_fGyroAngleSpeed_z = 0;
}
/*
 * 三叉路口决策
 */
uint8 threeWayIn = 0;
uint8 threeWaySum = 0;
uint8 ThreeWayIntersectionFlagPre = 0; //检测上升沿使用
uint8 steerStatus = 0;//0 朝向前  1 朝向左  2朝向后
void ThreeWayAnalyze(void){

//            ThreeWayIntersectionFlag = 0;//清标志  是由cam核清理
    if (ThreeWayIntersectionFlagPre == 1 && ThreeWayIntersectionFlag == 0) {//正常路段会发送flag = 0
        ThreeWayIntersectionFlagPre = 0; //还原上升沿触发
    }
    else if (ThreeWayIntersectionFlagPre == 0 && ThreeWayIntersectionFlag == 1 && threeWayIn == 0 && threeWaySum == 0) {//第一次进入
        pwm_duty(PWM1_CH1_A8, 672); //舵机向左
        steerStatus = 1;
        sendParameterToCam(8,0xDA,0,steerStatus,0,0);//告知cam舵机状态
        threeWayClearOtherThings(); //清理误差 防止转动
        ThreeWayIntersectionFlagPre = 1;//已完成上升沿
        threeWayIn = 1;

//        threeWayClearOtherThings();

      }
    else if (ThreeWayIntersectionFlagPre == 0 && ThreeWayIntersectionFlag == 1 && threeWayIn == 1 && threeWaySum == 0) { //第一次出
        pwm_duty(PWM1_CH1_A8, 1000);     //第一次出转舵机向后
        steerStatus = 2;
        sendParameterToCam(8,0xDA,0,steerStatus,0,0);//告知cam舵机状态
        threeWayClearOtherThings();
        ThreeWayIntersectionFlagPre = 1;//已完成上升沿
        threeWayIn = 0;               //先前进入三叉 此次是出去
        threeWaySum = 1;                //进入一次三叉

//        threeWayClearOtherThings();
    }
    else if (ThreeWayIntersectionFlagPre == 0 && ThreeWayIntersectionFlag == 1 && threeWayIn == 0 && threeWaySum == 1) { //第二次进
        pwm_duty(PWM1_CH1_A8, 672); //舵机向左
        steerStatus = 1;
        sendParameterToCam(8,0xDA,0,steerStatus,0,0);//告知cam舵机状态
        threeWayClearOtherThings();
        ThreeWayIntersectionFlagPre = 1;//已完成上升沿
        threeWayIn = 1;

//        threeWayClearOtherThings();
    }
    else if (ThreeWayIntersectionFlagPre == 0 && ThreeWayIntersectionFlag == 1 && threeWayIn == 1 && threeWaySum == 1){ //第二次出
        pwm_duty(PWM1_CH1_A8, 338); //舵机还原
        steerStatus = 0;
        sendParameterToCam(8,0xDA,0,steerStatus,0,0);//告知cam舵机状态
        threeWayClearOtherThings();
        ThreeWayIntersectionFlagPre = 1;//已完成上升沿
        threeWayIn = 0;
        threeWaySum = 0;

//        threeWayClearOtherThings();
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
        if (total_z > 80000 || total_z < -80000) {
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

uint8 parameterBuff[8];
uint8 negativeNumber = 0;
void sendParameterToCam(uint8 parameterBit,uint8 featuresWord,int8 parameterData_0,uint8 parameterData8,int16 parameterData16,int32 parameterData32){
    /* do:向图像核心发送相关数据
     * in:主核按键触后进行修改  在 display.c 中使用  约有20个变量
     * parameterBit:数据位数           0:int8  ; 8: uint8  ; 16: int16 ;  32 int32
     * featuresWord:功能字               0xE1 0xE2 ...
     *         sendParameterToCam(数据位数,功能字,int8,uint8,int16,int32);
     *         sendParameterToCam(0,0xE1,-12,0,0,0);//发送  int8：-12
     *         sendParameterToCam(8,0xE2,0,13,0,0);//发送 uint8：13
     *         sendParameterToCam(16,0xE3,0,0,-14,0);//发送 int16：-14
     *         sendParameterToCam(32,0xE4,0,0,0,-15);//发送 int32：-15
     */
    if (parameterBit == 0) {
        if (parameterData_0 < 0) {negativeNumber = 1;}
        else {negativeNumber = 0;}
        parameterBuff[0] = 0xDE;                            //帧头

        parameterBuff[1] = 0xA0;                            //功能字占位
        parameterBuff[2] = 0xA0;                            //功能字占位
        parameterBuff[3] = 0xA8;                            //功能字占位
        parameterBuff[4] = featuresWord;                    //功能字
        parameterBuff[5] = negativeNumber;                  //int8 正负传递

        parameterBuff[6] = (uint8)absInt8(parameterData_0); //数据

        parameterBuff[7] = 0xEE;                            //帧尾
        uart_putbuff(UART_3, parameterBuff, 8);             //通过串口3将数据发送出去。
    }
    else if (parameterBit == 8) {
        parameterBuff[0] = 0xDE;                            //帧头

        parameterBuff[1] = 0xA8;                            //功能字占位
        parameterBuff[2] = 0xA0;                            //功能字占位
        parameterBuff[3] = 0xA0;                            //功能字占位
        parameterBuff[4] = 0xA0;                            //功能字占位
        parameterBuff[5] = featuresWord;                    //功能字

        parameterBuff[6] = parameterData8;                 //数据

        parameterBuff[7] = 0xEE;                            //帧尾
        uart_putbuff(UART_3, parameterBuff, 8);             //通过串口3将数据发送出去。
    }
    else if (parameterBit == 16) { //parameter>>8;parameter&0xFF
        parameterBuff[0] = 0xDE;                            //帧头

        parameterBuff[1] = 0xA0;                            //功能字占位
        parameterBuff[2] = 0xA1;                            //功能字占位
        parameterBuff[3] = 0xA6;                            //功能字占位
        parameterBuff[4] = featuresWord;                    //功能字

        parameterBuff[5] = (parameterData16>>8)&0xFF;       //数据高八位
        parameterBuff[6] = parameterData16&0xFF;            //数据第八位

        parameterBuff[7] = 0xEE;                            //帧尾
        uart_putbuff(UART_3, parameterBuff, 8);             //通过串口3将数据发送出去。
    }
    else if (parameterBit == 32) {
        parameterBuff[0] = 0xDE;                            //帧头

        parameterBuff[1] = 0xA3;                            //功能字占位
        parameterBuff[2] = featuresWord;                    //功能字

        parameterBuff[3] = (parameterData32>>24)&0xFF;       //数据
        parameterBuff[4] = (parameterData32>>16)&0xFF;
        parameterBuff[5] = (parameterData32>>8)&0xFF;
        parameterBuff[6] = parameterData32&0xFF;

        parameterBuff[7] = 0xEE;                            //帧尾
        uart_putbuff(UART_3, parameterBuff, 8);             //通过串口3将数据发送出去。
    }
    else {
        return;
    }

}


