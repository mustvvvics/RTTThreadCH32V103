
#ifndef CODE_COMMUNICATIONCAM_H_
#define CODE_COMMUNICATIONCAM_H_

#include "headfile.h"

#define LINE_LEN                15              //发送数据长度
#define receiveBuffLength        8              //接收数据长度

extern uint8 temp_buff[LINE_LEN];
extern int16 encoder_left_front,encoder_left_rear;
extern uint8 receiveMainBuff[receiveBuffLength];
extern uint8 gyroRoundFinishFlag;

extern int8 parameterTest;
extern uint8 parameterTest8;
extern int16 parameterTest16;
extern int32 parameterTest32;


void encoder_init(void);
void get_sensor_data(void);
void process_data(void);
void gyroDataAnalysis(uint8 *line);
void analysisFixParameter(uint8 *line);

#endif /* CODE_COMMUNICATIONCAM_H_ */
