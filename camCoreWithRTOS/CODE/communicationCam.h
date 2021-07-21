
#ifndef CODE_COMMUNICATIONCAM_H_
#define CODE_COMMUNICATIONCAM_H_

#include "headfile.h"

#define LINE_LEN                15              //�������ݳ���
#define receiveBuffLength        8              //�������ݳ���

extern uint8 temp_buff[LINE_LEN];
extern int16 encoder_left_front,encoder_left_rear;
extern uint8 receiveMainBuff[receiveBuffLength];
extern uint8 gyroRoundFinishFlag;
extern uint8 steerStatusFromMain;

void encoder_init(void);
void get_sensor_data(void);
void process_data(void);
void gyroDataAnalysis(uint8 *line);
void analysisFixParameter(uint8 *line);

#endif /* CODE_COMMUNICATIONCAM_H_ */
