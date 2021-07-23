
#ifndef CODE_COMMUNICATIONCAM_H_
#define CODE_COMMUNICATIONCAM_H_

#include "headfile.h"

#define LINE_LEN                15              //发送数据长度
#define receiveBuffLength        8              //接收数据长度

extern uint8 temp_buff[LINE_LEN];
extern int16 encoder_left_front,encoder_left_rear;
extern uint8 receiveMainBuff[receiveBuffLength];
extern uint8 gyroRoundFinishFlag;
extern uint8 steerStatusFromMain;
extern int32 elementTableFromMain;
extern uint8 elementTableLengthFromMain;
extern char elementQueue[16];
extern uint8 drivingDirection;
extern uint8 carStart;

void encoder_init(void);
void get_sensor_data(void);
void process_data(void);
void gyroDataAnalysis(uint8 *line);
void analysisFixParameter(uint8 *line);
char* itoaChar(int num,char* str,int radix);

#endif /* CODE_COMMUNICATIONCAM_H_ */
