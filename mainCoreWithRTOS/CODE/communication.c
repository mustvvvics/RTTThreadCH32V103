#include "headfile.h"

uint8   temp_buff[LINE_LEN];         //�������ڽ������ݵ�BUFF
vuint8  uart_flag;                   //�������ݱ�־λ
uint8 sendMainFlashToCam = 0;

void get_slave_data(uint8 data)
{
    static uint8 uart_num = 0;
    temp_buff[uart_num++] = data;

    if(1 == uart_num)
    {
        //֡ͷ���� ���յ��ĵ�һ���ַ���Ϊ0xD8
        if(0xD8 != temp_buff[0]){
            uart_num = 0;
            uart_flag = E_FRAME_HEADER_ERROR;
        }
    }
    if(LINE_LEN == uart_num)
    {
        uart_flag = E_OK;
        //���յ����һ���ֽ�Ϊ0xEE
        if(0xEE == temp_buff[LINE_LEN - 1]){
            uart_flag = E_OK;
        }
        else{    //֡β���� ���յ����һ���ֽڲ���0xEE
            uart_flag = E_FRAME_RTAIL_ERROR;
        }
        uart_num = 0;
    }
}
/*
 * ���ݽ���  �����ӻ�����������   ƫ��    Ԫ��   �Լ���������
 */
void data_analysis(uint8 *line)
{
    if(line[1] == 0xB0)    encoder_data[2] = ((int16)line[2] << 8) | line[3];
    if(line[4] == 0xB1)    encoder_data[3] = ((int16)line[5] << 8) | line[6];
    if(line[7] == 0xB2)    position_front  = ((int16)line[8] << 8) | line[9];
    if(line[10] == 0xB3)   accelerate  = line[11];
    if(line[12] == 0xB4)   elementFlag  = line[13];

    switch (elementFlag) {
        case 0://��Ԫ��
            roundIslandBegin = 0;
            ThreeWayIntersectionFlag = 0;
            sendMainFlashToCam = 0;
            break;
        case 1://����
            if (car_flag == 2) {
                ThreeWayIntersectionFlag = 1;  //�������濪ʼ����
            }
            break;
        case 2://����

            break;
        case 3: //ɲ��
            if (car_flag == 2) {
                //            sendFuzzyData(); //����ģ��pid
                car_flag = 0;
            }
            break;
        case 4:
            if (car_flag == 2) {
                roundIslandBegin = 1;         //�������� �����ǿ�ʼ����
            }
            break;
        case 5:
            sendMainFlashToCam = 1;          //�ϵ緢��flash����
            break;
    }

}

uint8 Gyro_buff[8];
void send_to_cam(void)
{
    //��������������
    Gyro_buff[0] = 0xD8;                            //֡ͷ

    Gyro_buff[1] = 0xB0;                            //������
    Gyro_buff[2] = 0xB1;                            //������
    Gyro_buff[3] = 0xB2;                            //������
    Gyro_buff[4] = 0xB3;                            //������
    Gyro_buff[5] = roundFinishFlag;                 //��������
    Gyro_buff[6] = 0xB3;                            //������

    Gyro_buff[7] = 0xEE;                            //֡β
    uart_putbuff(UART_3, Gyro_buff, 8);             //ͨ������3�����ݷ��ͳ�ȥ��
}
/*
 * ������̼�����  use in adc.c ��ʱ��
 */
uint8 encoderCounterNumBuff[8];
void sendEncoderCounterNum(void)
{
    //��������������
    encoderCounterNumBuff[0] = 0xCE;                            //֡ͷ

    encoderCounterNumBuff[1] = 0xC6;                            //������
    encoderCounterNumBuff[2] = 0xC9;                            //������
    encoderCounterNumBuff[3] = (encoder_y>>24)&0xFF;          //����
    encoderCounterNumBuff[4] = (encoder_y>>16)&0xFF;
    encoderCounterNumBuff[5] = (encoder_y>>8)&0xFF;
    encoderCounterNumBuff[6] = encoder_y&0xFF;

    encoderCounterNumBuff[7] = 0xEE;                            //֡β
    uart_putbuff(UART_3, encoderCounterNumBuff, 8);             //ͨ������3�����ݷ��ͳ�ȥ��
}

int8 absInt8(int8 x){return x>0?x:-x;}
uint8 parameterBuff[8];
uint8 negativeNumber = 0;
void sendParameterToCam(uint8 parameterBit,uint8 featuresWord,int8 parameterData_0,uint8 parameterData8,int16 parameterData16,int32 parameterData32){
    /* do:��ͼ����ķ����������
     * in:���˰�����������޸�  �� display.c ��ʹ��  Լ��20������
     * parameterBit:����λ��           0:int8  ; 8: uint8  ; 16: int16 ;  32 int32
     * featuresWord:������               0xE1 0xE2 ...
     *         sendParameterToCam(����λ��,������,int8,uint8,int16,int32);
     *         sendParameterToCam(0,0xE1,-12,0,0,0);//����  int8��-12
     *         sendParameterToCam(8,0xE2,0,13,0,0);//���� uint8��13
     *         sendParameterToCam(16,0xE3,0,0,-14,0);//���� int16��-14
     *         sendParameterToCam(32,0xE4,0,0,0,-15);//���� int32��-15
     */
    if (parameterBit == 0) {
        if (parameterData_0 < 0) {negativeNumber = 1;}
        else {negativeNumber = 0;}
        parameterBuff[0] = 0xDE;                            //֡ͷ

        parameterBuff[1] = 0xA0;                            //������ռλ
        parameterBuff[2] = 0xA0;                            //������ռλ
        parameterBuff[3] = 0xA8;                            //������ռλ
        parameterBuff[4] = featuresWord;                    //������
        parameterBuff[5] = negativeNumber;                  //int8 ��������

        parameterBuff[6] = (uint8)absInt8(parameterData_0); //����

        parameterBuff[7] = 0xEE;                            //֡β
        uart_putbuff(UART_3, parameterBuff, 8);             //ͨ������3�����ݷ��ͳ�ȥ��
    }
    else if (parameterBit == 8) {
        parameterBuff[0] = 0xDE;                            //֡ͷ

        parameterBuff[1] = 0xA8;                            //������ռλ
        parameterBuff[2] = 0xA0;                            //������ռλ
        parameterBuff[3] = 0xA0;                            //������ռλ
        parameterBuff[4] = 0xA0;                            //������ռλ
        parameterBuff[5] = featuresWord;                    //������

        parameterBuff[6] = parameterData8;                 //����

        parameterBuff[7] = 0xEE;                            //֡β
        uart_putbuff(UART_3, parameterBuff, 8);             //ͨ������3�����ݷ��ͳ�ȥ��
    }
    else if (parameterBit == 16) { //parameter>>8;parameter&0xFF
        parameterBuff[0] = 0xDE;                            //֡ͷ

        parameterBuff[1] = 0xA0;                            //������ռλ
        parameterBuff[2] = 0xA1;                            //������ռλ
        parameterBuff[3] = 0xA6;                            //������ռλ
        parameterBuff[4] = featuresWord;                    //������

        parameterBuff[5] = (parameterData16>>8)&0xFF;       //���ݸ߰�λ
        parameterBuff[6] = parameterData16&0xFF;            //���ݵڰ�λ

        parameterBuff[7] = 0xEE;                            //֡β
        uart_putbuff(UART_3, parameterBuff, 8);             //ͨ������3�����ݷ��ͳ�ȥ��
    }
    else if (parameterBit == 32) {
        parameterBuff[0] = 0xDE;                            //֡ͷ

        parameterBuff[1] = 0xA3;                            //������ռλ
        parameterBuff[2] = featuresWord;                    //������

        parameterBuff[3] = (parameterData32>>24)&0xFF;       //����
        parameterBuff[4] = (parameterData32>>16)&0xFF;
        parameterBuff[5] = (parameterData32>>8)&0xFF;
        parameterBuff[6] = parameterData32&0xFF;

        parameterBuff[7] = 0xEE;                            //֡β
        uart_putbuff(UART_3, parameterBuff, 8);             //ͨ������3�����ݷ��ͳ�ȥ��
    }
    else {
        return;
    }

}


