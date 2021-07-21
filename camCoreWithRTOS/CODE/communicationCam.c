#include "headfile.h"

uint8 temp_buff[LINE_LEN];                      //�ӻ���������������BUFF
int16 encoder_left_front,encoder_left_rear;     //ǰ�����ұ�������ֵ
uint8 receiveMainBuff[receiveBuffLength];      //������������buff

uint8 gyroRoundFinishFlag = 0;                 //����������Flag
int16 slave_position=0;                         //�������

int16 ABS(int16 x){                             //����ֵ
    return x>0?x:-x;
}

void encoder_init(void)
{
    timer_quad_init(TIMER_2, TIMER2_CHA_A15, TIMER2_CHB_B3);
    timer_quad_init(TIMER_3, TIMER3_CHA_B4, TIMER3_CHB_B5);
}

void get_sensor_data(void)
{
    if(gpio_get(B5))                            //��ȡ��������ֵ
        encoder_left_front = ABS(timer_quad_get(TIMER_3));
    else
        encoder_left_front = -ABS(timer_quad_get(TIMER_3));
    timer_quad_clear(TIMER_3);                  //��ռ�����

    if(gpio_get(B3))
        encoder_left_rear = -ABS(timer_quad_get(TIMER_2));
    else
        encoder_left_rear = ABS(timer_quad_get(TIMER_2));
    timer_quad_clear(TIMER_2);
}

void process_data(void)                         //����Э�鴦��Ҫ���������͵�����
{
    slave_position = cameraError;//�������

    temp_buff[0] = 0xD8;                         //֡ͷ

    temp_buff[1] = 0xB0;                         //������
    temp_buff[2] = encoder_left_front>>8;        //���ݸ�8λ
    temp_buff[3] = encoder_left_front&0xFF;      //���ݵ�8λ

    temp_buff[4] = 0xB1;                         //������
    temp_buff[5] = encoder_left_rear>>8;         //���ݸ�8λ
    temp_buff[6] = encoder_left_rear&0xFF;       //���ݵ�8λ

    temp_buff[7] = 0xB2;                         //������
    temp_buff[8] = slave_position>>8;            //���ݸ�8λ
    temp_buff[9] = slave_position&0xFF;          //���ݵ�8λ

    temp_buff[10] = 0xB3;
    temp_buff[11] = accelerateRatio;            //���ٲ���

    temp_buff[12] = 0xB4;
    temp_buff[13] = flagCameraElement;          //Ԫ��falg

    temp_buff[14] = 0xEE;                       //֡β
}


void gyroDataAnalysis(uint8 *line)
{
    if (line[0] != 0xD8) {return;}
    else if(line[0] == 0xD8 && line[1] == 0xB0 && line[4] == 0xB3){ //����ɸѡ
        gyroRoundFinishFlag = line[5];
    }
    else {return;}
}

/*
 * �����޸�����
 */
int8 parameterTest;
uint8 parameterTest8;
int16 parameterTest16;
int32 parameterTest32;

int8 negativeNumber = 0;
void analysisFixParameter(uint8 *line){
    if (line[0] != 0xDE) { return ;} //��Ϊ�޸Ĳ�����֡ͷֱ�ӷ���
    else if (line[0] == 0xDE) {//֡β֡ͷУ��ɹ�
        if (line[1] == 0xA0 && line[3] == 0xA8) { //int8
            if (line[5] == 1 ) {negativeNumber = -1;}else if (line[5] == 0) {negativeNumber = 1;}
            switch (line[4]) { //������ѡ��
//Eg:                case 0xE1: parameterTest = negativeNumber * line[6];break;
                case 0xE6:globalCenterBias = negativeNumber * line[6];break;
                default:
                    break;
            }
        }
        else if (line[1] == 0xA8 && line[4] == 0xA0) { //uint8
            switch (line[5]) {
//Eg:                case 0xE2:parameterTest8 = line[6];break;
                case 0xE1:turnPage = line[6];break;
                case 0xE2:
                    clearCamFlags = line[6];
                    if (clearCamFlags == 1) {
                        flagEnterRoundabout=0;flagEnterThreeWay=0;gyroRoundFinishFlag = 0;
                        clearCamFlags = 0;}
                    break;
                case 0xE3:threewayFeatureRow = line[6];break;
                case 0xE4:detectCrossroadMissingNumThres = line[6];break;
                case 0xE5:rangeSharpCurveRow = line[6];break;
                case 0xE8:detectDistance = (line[6] / 10) + ((line[6] % 10) * 0.1) ;break; //ʵ��Ϊfloat
                case 0xEA:slopeRowStart = line[6];break;
                case 0xEB:slopeRowEnd = line[6];break;
                case 0xEE:roundaboutDetectionStartRow = line[6];break;
                default:
                    break;
            }
        }
        else if (line[2] == 0xA1 && line[3] == 0xA6) { //int16
            switch (line[4]) {
//Eg:                case 0xE3:parameterTest16 = ((int16)line[5] << 8) | line[6];break;
                case 0xE7: startlineJumpingPointNumThres = ((int16)line[5] << 8) | line[6];break;
                default:
                    break;
            }
        }
        else if (line[1] == 0xA3) {                    //int32
            switch (line[2]) {
//Eg:                case 0xE4:parameterTest32 = (line[3]<<24)|(line[4]<<16)|(line[5]<<8)| line[6];break;
                case 0xE9:pixelMeanThres = (line[3]<<24)|(line[4]<<16)|(line[5]<<8)| line[6];break;
                case 0xEC:areaDetectRoundaboutThresLeft = (line[3]<<24)|(line[4]<<16)|(line[5]<<8)| line[6];break;
                case 0xED:areaDetectRoundaboutThresRight = (line[3]<<24)|(line[4]<<16)|(line[5]<<8)| line[6];break;
                default:
                    break;
            }
        }
    }

    else {return;}
}














