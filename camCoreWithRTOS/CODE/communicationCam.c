#include "headfile.h"

uint8 temp_buff[LINE_LEN];                      //从机向主机发送数据BUFF
int16 encoder_left_front,encoder_left_rear;     //前轮左右编码器数值
uint8 Gyro_buff[GyroLINE_LEN];                  //接收陀螺仪数据buff
int16 gyroData;                                 //接收陀螺仪数据
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


void gyroData_analysis(uint8 *line)
{
    if(line[1] == 0xB0)    gyroData = ((int16)line[2] << 8) | line[3];
}
