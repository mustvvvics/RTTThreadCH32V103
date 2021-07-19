/*
 * communication.c
 *
 *  Created on: Jun 30, 2021
 *      Author: 29275
 */
#include "headfile.h"

uint8   temp_buff[LINE_LEN];            //�������ڽ������ݵ�BUFF
vuint8  uart_flag;                   //�������ݱ�־λ

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
            break;
        case 1://����
//            ThreeWayIntersectionFlag = 1; //��һ��������ʹ��camera error = 0;
            car_flag = 0;
            break;
        case 2://����

            break;
        case 3: //ɲ��
            car_flag = 0;
            break;
        case 4:
            roundIslandBegin = 1; //�������� �����ǿ�ʼ����
            break;
    }

}
/*
 * ����·�ھ���
 */
int8 ThreeWayDirection = 0;
int16 threeWayOutAngle = 0;
uint8 threeWayIn = 0;
uint8 threeWayOut = 0;
uint8 threeWaySum = 0;
void ThreeWayAnalyze(void){
    if (ThreeWayIntersectionFlag == 1 && threeWayIn == 0 && threeWaySum == 0) {//��һ�ν���
        ThreeWayIntersectionFlag = 0;//���־
        ThreeWayDirection = -1; //���ܰ�������ͷ������ʻ  �ж�����ʻ����
        threeWayIn = 1;
        pwm_duty(PWM1_CH1_A8, 990); //�����ת
      }
    else if (ThreeWayIntersectionFlag == 1 && threeWayIn == 1 && threeWaySum == 0) { //��һ�γ�
        ThreeWayIntersectionFlag = 0;
//        threeWayIn = 0;               //���ⲿ�ж���,�������̬���γ������ �ͷ�flag
        threeWayOut = 1;                //�������ж��б���
        threeWayOutAngle = 1500;          //��������
        threeWaySum = 1;                //����һ������
        pwm_duty(PWM1_CH1_A8, 670);
    }
    else if (ThreeWayIntersectionFlag == 1 && threeWayIn == 0 && threeWaySum == 1) { //�ڶ��ν�
        ThreeWayIntersectionFlag = 0; //���ܰ�������ͷ������ʻ
        ThreeWayDirection = 1;
        threeWayIn = 1;
        pwm_duty(PWM1_CH1_A8, 338); //�����ת
    }
    else if (ThreeWayIntersectionFlag == 1 && threeWayIn == 1 && threeWaySum == 1){ //�ڶ��γ�
        ThreeWayIntersectionFlag = 0;
//        threeWayIn = 0;
        threeWayOut = 1;  //�������ж��б���
        threeWayOutAngle = -1500;
        threeWaySum = 0;
        pwm_duty(PWM1_CH1_A8, 670);
    }
}

uint8 Gyro_buff[4];


void send_to_cam(void)
{
    //��������������
    Gyro_buff[0] = 0xD8;                            //֡ͷ

    Gyro_buff[1] = 0xB0;                            //������
    Gyro_buff[2] = roundFinishFlag;                 //��������

    Gyro_buff[3] = 0xEE;                            //֡β
    uart_putbuff(UART_3, Gyro_buff, 4);             //ͨ������3�����ݷ��ͳ�ȥ��
}

/*
//�����˰�����������޸�
//Լ��20������
 */
uint8 parameterBuff[5];
void sendParameterToCam(uint8 parameterBit,){
    //��ͼ����ķ����������
    if (parameterBit == 8) {
        parameterBuff[0] = 0xDB;                            //֡ͷ

        parameterBuff[1] = 0x90;                            //������
        parameterBuff[2] = 0x91;                            //������
        parameterBuff[3] = roundFinishFlag;                 //��������

        parameterBuff[4] = 0xEE;                            //֡β

    }
    else if (parameterBit == 16) { //parameter>>8;parameter&0xFF
        parameterBuff[0] = 0xDB;                            //֡ͷ

        parameterBuff[1] = 0xE1;                            //������
//        parameterBuff[2] = parameter>>8;                            //������
//        parameterBuff[3] = parameter&0xFF;                 //��������

        parameterBuff[4] = 0xEE;                            //֡β
    }
    else if (parameterBit == 32) { ////Parameter>>8;Parameter>>16;Parameter>>24;Parameter&0xFF;

    }
    else {
        return;
    }
    uart_putbuff(UART_3, parameterBuff, 5);             //ͨ������3�����ݷ��ͳ�ȥ��
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
        if (total_z > 84000 || total_z < -84000) {
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
