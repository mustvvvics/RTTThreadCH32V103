/*
 * filter.c
 *
 *  Created on: Jun 9, 2021
 *      Author: 29275
 */
#include "headfile.h"

//float angle_z;
//float a;
//float dt = 0.005;//�����˲����������� 5ms��һ��
//float ComplementaryFilter(float acc, float gyro, float dt)
//{
//    a = 0.98;
//    angle_z = a * (angle_z + gyro * dt) + (1 - a) * (acc);
//    return angle_z;
//}
//��ԭʼ���ݴ���   �õ�3����ļ��ٶ����� �� 3����Ľ��ٶ����� ����ֻ�õ���Z�ᣬ����дһ��������ֻ��Z����ٶȣ����ٶȣ����սǶȽ�����̬����
void AngleZ_Get(void)
{
    //��ȡԭʼ����
    get_icm20602_accdata_spi();
    get_icm20602_gyro_spi();

    //������ٶ�
    //g_fGravityAngle_z = atan2(icm_acc_x/16384.0,icm_acc_y/16384.0) * 180.0 / 3.14;
    //������ٶ�
    icm_gyro_z  = icm_gyro_z / 16.4;  //������ٶ�ֵ
    g_fGyroAngleSpeed_z = icm_gyro_z;

    //Kalman_Filter(g_fGravityAngle_z,g_fGyroAngleSpeed_z);
    //������սǶ�
    //return angle;
    //return 0.01;
    //return ComplementaryFilter(g_fGravityAngle_z,g_fGyroAngleSpeed_z,0.005);
}

//void show_angle(void)
//{
//    char txt[32];
//
//    sprintf(txt,"omega=%05d",g_fGyroAngleSpeed_z);
//    ips114_showstr(0,0,txt);
//
//}


