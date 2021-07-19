#include "headfile.h"

uint8 temp_buff[LINE_LEN];                      //从机向主机发送数据BUFF
int16 encoder_left_front,encoder_left_rear;     //前轮左右编码器数值
uint8 receiveMainBuff[receiveBuffLength];      //接收主机数据buff

uint8 gyroRoundFinishFlag = 0;                 //接收陀螺仪Flag
int16 slave_position=0;                         //传递误差

int16 ABS(int16 x){                             //绝对值
    return x>0?x:-x;
}

void encoder_init(void)
{
    timer_quad_init(TIMER_2, TIMER2_CHA_A15, TIMER2_CHB_B3);
    timer_quad_init(TIMER_3, TIMER3_CHA_B4, TIMER3_CHB_B5);
}

void get_sensor_data(void)
{
    if(gpio_get(B5))                            //读取编码器数值
        encoder_left_front = ABS(timer_quad_get(TIMER_3));
    else
        encoder_left_front = -ABS(timer_quad_get(TIMER_3));
    timer_quad_clear(TIMER_3);                  //清空计数器

    if(gpio_get(B3))
        encoder_left_rear = -ABS(timer_quad_get(TIMER_2));
    else
        encoder_left_rear = ABS(timer_quad_get(TIMER_2));
    timer_quad_clear(TIMER_2);
}

void process_data(void)                         //根据协议处理要向主机发送的数据
{
    slave_position = cameraError;//传递误差

    temp_buff[0] = 0xD8;                         //帧头

    temp_buff[1] = 0xB0;                         //功能字
    temp_buff[2] = encoder_left_front>>8;        //数据高8位
    temp_buff[3] = encoder_left_front&0xFF;      //数据低8位

    temp_buff[4] = 0xB1;                         //功能字
    temp_buff[5] = encoder_left_rear>>8;         //数据高8位
    temp_buff[6] = encoder_left_rear&0xFF;       //数据低8位

    temp_buff[7] = 0xB2;                         //功能字
    temp_buff[8] = slave_position>>8;            //数据高8位
    temp_buff[9] = slave_position&0xFF;          //数据低8位

    temp_buff[10] = 0xB3;
    temp_buff[11] = accelerateRatio;            //变速参数

    temp_buff[12] = 0xB4;
    temp_buff[13] = flagCameraElement;          //元素falg

    temp_buff[14] = 0xEE;                       //帧尾
}


void gyroDataAnalysis(uint8 *line)
{
    if (line[0] != 0xD8) {return;}
    else if(line[0] == 0xD8 && line[1] == 0xB0 && line[4] == 0xB3){ //条件筛选
        gyroRoundFinishFlag = line[5];
    }
    else {return;}
}

/*
 * 分析修改数据
 */
int8 parameterTest;
uint8 parameterTest8;
int16 parameterTest16;
int32 parameterTest32;

int8 negativeNumber = 0;
void analysisFixParameter(uint8 *line){
    if (line[0] != 0xDE) { return ;} //不为修改参数的帧头直接返回
    else if (line[0] == 0xDE) {//帧尾帧头校验成功
        if (line[1] == 0xA0 && line[3] == 0xA8) { //int8
            if (line[5] == 1 ) {negativeNumber = -1;}else if (line[5] == 0) {negativeNumber = 1;}
            switch (line[4]) { //功能字选择
                case 0xE1: parameterTest = negativeNumber * line[6];break;
                default:
                    break;
            }
        }
        else if (line[1] == 0xA8 && line[4] == 0xA0) { //uint8
            switch (line[5]) {
                case 0xE2:parameterTest8 = line[6];break;
                default:
                    break;
            }
        }
        else if (line[2] == 0xA1 && line[3] == 0xA6) { //int16
            switch (line[4]) {
                case 0xE3:parameterTest16 = ((int16)line[5] << 8) | line[6];break;
                default:
                    break;
            }
        }
        else if (line[1] == 0xA3) {                    //int32
            switch (line[2]) {
                case 0xE4:parameterTest32 = (line[3]<<24)|(line[4]<<16)|(line[5]<<8)| line[6];break;
                default:
                    break;
            }
        }
    }

    else {return;}
}














