#include "headfile.h"

uint8   temp_buff[LINE_LEN];         //主机用于接收数据的BUFF
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
/*
 * 数据解析  包含从机陀螺仪数据   偏差    元素   以及加速因子
 */
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
            if (car_flag == 2) {
                ThreeWayIntersectionFlag = 1;  //遇见三叉开始解析
            }
            break;
        case 2://车库

            break;
        case 3: //刹车
            if (car_flag == 2) {
                //            sendFuzzyData(); //测试模糊pid
                car_flag = 0;
            }
            break;
        case 4:
            if (car_flag == 2) {
                roundIslandBegin = 1;         //遇见环岛 陀螺仪开始积分
            }
            break;
        case 5:
            sendMainFlashToCam = 1;          //上电发送flash数据
            break;
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
 * 发送里程计数据  use in adc.c 定时器
 */
uint8 encoderCounterNumBuff[8];
void sendEncoderCounterNum(void)
{
    //发送陀螺仪数据
    encoderCounterNumBuff[0] = 0xCE;                            //帧头

    encoderCounterNumBuff[1] = 0xC6;                            //功能字
    encoderCounterNumBuff[2] = 0xC9;                            //功能字
    encoderCounterNumBuff[3] = (encoder_y>>24)&0xFF;          //数据
    encoderCounterNumBuff[4] = (encoder_y>>16)&0xFF;
    encoderCounterNumBuff[5] = (encoder_y>>8)&0xFF;
    encoderCounterNumBuff[6] = encoder_y&0xFF;

    encoderCounterNumBuff[7] = 0xEE;                            //帧尾
    uart_putbuff(UART_3, encoderCounterNumBuff, 8);             //通过串口3将数据发送出去。
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


