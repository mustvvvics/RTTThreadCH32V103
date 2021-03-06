
#ifndef CODE_COMMUNICATIONCAM_H_
#define CODE_COMMUNICATIONCAM_H_

#include "headfile.h"

#define LINE_LEN                17              //发送数据长度
#define receiveBuffLength        8              //接收数据长度

extern uint8 temp_buff[LINE_LEN];
extern int16 encoder_left_front,encoder_left_rear;
extern uint8 receiveMainBuff[receiveBuffLength];
extern uint8 gyroRoundFinishFlag;
extern uint8 steerStatusFromMain;
extern uint8 encoderNumFlag;

extern int32 elementTableFromMain;
extern uint8 elementTableLengthFromMain;
extern int32 elementTableFromMain1;
extern uint8 elementTableLengthFromMain1;
extern char elementQueue[20];
extern char elementQueue0[10];
extern char elementQueue1[10];

extern uint8 drivingDirection;
extern uint8 carStart;
extern uint8 camFlashWriteFlag;
extern int32 encoderCounterNum;

void encoder_init(void);
void get_sensor_data(void);
void process_data(void);
void gyroDataAnalysis(uint8 *line);
void analysisFixParameter(uint8 *line);
void encoderCounterAnalysis(uint8 *line);
char* itoaChar(int32 num,char* str,uint8 radix);
void connectChar(void);

#endif /* CODE_COMMUNICATIONCAM_H_ */
