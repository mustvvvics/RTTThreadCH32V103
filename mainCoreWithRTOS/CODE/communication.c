/*
 * communication.c
 *
 *  Created on: Jun 30, 2021
 *      Author: 29275
 */
#include "headfile.h"

uint8   temp_buff[LINE_LEN];            //�������ڽ������ݵ�BUFF
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
            if (car_flag == 1) {
                ThreeWayIntersectionFlag = 1; //��һ��������ʹ��camera error = 0;
            }
            break;
        case 2://����

            break;
        case 3: //ɲ��
            car_flag = 0;
//            sendFuzzyData();
            break;
        case 4:
            if (car_flag == 1) {
                roundIslandBegin = 1; //�������� �����ǿ�ʼ����
            }
            break;
        case 5:
            sendMainFlashToCam = 1;
            break;
    }

}
void threeWayClearOtherThings(void){
    position_front_delta = 0;position_front = 0;expected_omega = 0;g_fGyroAngleSpeed_z = 0;
}
/*
 * ����·�ھ���
 */
uint8 threeWayIn = 0;
uint8 threeWaySum = 0;
uint8 ThreeWayIntersectionFlagPre = 0; //���������ʹ��
uint8 steerStatus = 0;//0 ����ǰ  1 ������  2�����
void ThreeWayAnalyze(void){

//            ThreeWayIntersectionFlag = 0;//���־  ����cam������
    if (ThreeWayIntersectionFlagPre == 1 && ThreeWayIntersectionFlag == 0) {//����·�λᷢ��flag = 0
        ThreeWayIntersectionFlagPre = 0; //��ԭ�����ش���
    }
    else if (ThreeWayIntersectionFlagPre == 0 && ThreeWayIntersectionFlag == 1 && threeWayIn == 0 && threeWaySum == 0) {//��һ�ν���
        pwm_duty(PWM1_CH1_A8, 672); //�������
        steerStatus = 1;
        sendParameterToCam(8,0xDA,0,steerStatus,0,0);//��֪cam���״̬
        threeWayClearOtherThings(); //������� ��ֹת��
        ThreeWayIntersectionFlagPre = 1;//�����������
        threeWayIn = 1;

//        threeWayClearOtherThings();

      }
    else if (ThreeWayIntersectionFlagPre == 0 && ThreeWayIntersectionFlag == 1 && threeWayIn == 1 && threeWaySum == 0) { //��һ�γ�
        pwm_duty(PWM1_CH1_A8, 1000);     //��һ�γ�ת������
        steerStatus = 2;
        sendParameterToCam(8,0xDA,0,steerStatus,0,0);//��֪cam���״̬
        threeWayClearOtherThings();
        ThreeWayIntersectionFlagPre = 1;//�����������
        threeWayIn = 0;               //��ǰ�������� �˴��ǳ�ȥ
        threeWaySum = 1;                //����һ������

//        threeWayClearOtherThings();
    }
    else if (ThreeWayIntersectionFlagPre == 0 && ThreeWayIntersectionFlag == 1 && threeWayIn == 0 && threeWaySum == 1) { //�ڶ��ν�
        pwm_duty(PWM1_CH1_A8, 672); //�������
        steerStatus = 1;
        sendParameterToCam(8,0xDA,0,steerStatus,0,0);//��֪cam���״̬
        threeWayClearOtherThings();
        ThreeWayIntersectionFlagPre = 1;//�����������
        threeWayIn = 1;

//        threeWayClearOtherThings();
    }
    else if (ThreeWayIntersectionFlagPre == 0 && ThreeWayIntersectionFlag == 1 && threeWayIn == 1 && threeWaySum == 1){ //�ڶ��γ�
        pwm_duty(PWM1_CH1_A8, 338); //�����ԭ
        steerStatus = 0;
        sendParameterToCam(8,0xDA,0,steerStatus,0,0);//��֪cam���״̬
        threeWayClearOtherThings();
        ThreeWayIntersectionFlagPre = 1;//�����������
        threeWayIn = 0;
        threeWaySum = 0;

//        threeWayClearOtherThings();
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
 * ���������ǻ���
 */
uint8 roundIslandBeginPre = 0;
void roundIslandAnalyze(void){

    if (roundIslandBeginPre == 0 && roundIslandBegin == 1) {
        total_z = g_fGyroAngleSpeed_z;
        roundIslandBeginPre = 1;
    }
    else if (roundIslandBeginPre == 1 && roundIslandBegin == 0) {
        roundIslandBeginPre = 0;
    }
    else if (roundIslandBeginPre == 1 && roundIslandBegin == 1){
        total_z += g_fGyroAngleSpeed_z;
        if (total_z > 80000 || total_z < -80000) {
            roundIslandBegin = 0;
            roundFinishFlag = 1;
            total_z = 0;
            send_to_cam();
        }
    }
    else {
        return;
    }
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


