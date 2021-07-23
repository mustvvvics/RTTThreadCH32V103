#ifndef CODE_COMMUNICATION_H_
#define CODE_COMMUNICATION_H_

#include "headfile.h"

#define LINE_LEN                15      //���ݳ���

#define E_START                 0       //׼��״̬
#define E_OK                    1       //�ɹ�
#define E_FRAME_HEADER_ERROR    2       //֡ͷ����
#define E_FRAME_RTAIL_ERROR     3       //֡β����

extern vuint8  uart_flag;                   //�������ݱ�־λ
extern uint8   temp_buff[LINE_LEN];         //�������ڽ������ݵ�BUFF
extern uint8 threeWayIn;
extern uint8 threeWaySum;
extern int16 threeWayOutAngle;
extern int8 ThreeWayDirection;
extern uint8 sendMainFlashToCam;

void ThreeWayAnalyze(void);
void get_slave_data(uint8 data);
void data_analysis(uint8 *line);
void roundIslandAnalyze(void);
void send_to_cam(void);
void sendParameterToCam(uint8 parameterBit,uint8 featuresWord,int8 parameterData_0,uint8 parameterData8,int16 parameterData16,int32 parameterData32);
#endif /* CODE_COMMUNICATION_H_ */
