#ifndef CODE_COMMUNICATION_H_
#define CODE_COMMUNICATION_H_

#include "headfile.h"

#define LINE_LEN                17      //数据长度

#define E_START                 0       //准备状态
#define E_OK                    1       //成功
#define E_FRAME_HEADER_ERROR    2       //帧头错误
#define E_FRAME_RTAIL_ERROR     3       //帧尾错误

extern vuint8  uart_flag;                   //接收数据标志位
extern uint8   temp_buff[LINE_LEN];         //主机用于接收数据的BUFF
extern uint8 sendMainFlashToCam;            //发送flash标志

void get_slave_data(uint8 data);
void data_analysis(uint8 *line);
void send_to_cam(void);                 //发送陀螺仪flag
//主机向从机发送数据函数
void sendParameterToCam(uint8 parameterBit,uint8 featuresWord,int8 parameterData_0,uint8 parameterData8,int16 parameterData16,int32 parameterData32);
void sendEncoderCounterNum(void);

#endif /* CODE_COMMUNICATION_H_ */
