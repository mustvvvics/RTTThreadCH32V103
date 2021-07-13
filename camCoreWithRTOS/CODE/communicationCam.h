
#ifndef CODE_COMMUNICATIONCAM_H_
#define CODE_COMMUNICATIONCAM_H_

#include "headfile.h"

#define LINE_LEN                15              //数据长度
#define GyroLINE_LEN             5

extern uint8 temp_buff[LINE_LEN];
extern int16 encoder_left_front,encoder_left_rear;
extern uint8 Gyro_buff[5];
extern int16 gyroData;

void encoder_init(void);
void get_sensor_data(void);
void process_data(void);
void gyroData_analysis(uint8 *line);

#endif /* CODE_COMMUNICATIONCAM_H_ */
