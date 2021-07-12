/*
 * communication.c 主核心
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
        //接收到的第一个字符不为0xD8，帧头错误
        if(0xD8 != temp_buff[0])
        {
            uart_num = 0;
            uart_flag = E_FRAME_HEADER_ERROR;
//            rt_kprintf("HEADERR\n");
        }
    }

    if(LINE_LEN == uart_num)
    {
        uart_flag = E_OK;
        //接收到最后一个字节为0xEE
        if(0xEE == temp_buff[LINE_LEN - 1])
        {
            uart_flag = E_OK;
//            rt_kprintf("ok\n");
        }
        else    //接收到最后一个字节不是0xEE，帧尾错误
        {
            uart_flag = E_FRAME_RTAIL_ERROR;
//            rt_kprintf("ERROR\n");
        }
        uart_num = 0;
    }
}


void data_analysis(uint8 *line)
{
    if(line[1] == 0xB0)    encoder_data[2] = ((int16)line[2] << 8) | line[3];
    if(line[4] == 0xB1)    encoder_data[3] = ((int16)line[5] << 8) | line[6];
    if(line[7] == 0xB2)    position_front  = ((int16)line[8] << 8) | line[9];
    if(line[10] == 0xB3)   accelerate  = (line[11] << 8) | line[12];
    if(line[13] == 0xB4)   elementFlag  = line[14];

    switch (elementFlag) {
        case '0'://无元素

            break;
        case '1'://三叉

            break;
        case '2':// 车库

            break;
        case '3': //刹车
            car_flag = 0;
            break;
        default:
            break;
    }

}
