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
//��ԭʼ���ݴ���   �õ�3����ļ��ٶ����� �� 3����Ľ��ٶ����� ����ֻ�õ���Z
//����дһ������,ֻ�Ը�����ٶ�,���ٶ�,���սǶȽ�����̬����
void AngleZ_Get(void)
{
    //��ȡԭʼ����
    get_icm20602_accdata_spi();
    get_icm20602_gyro_spi();

    //������ٶ�
    icm_gyro_z  = icm_gyro_z / 16.4;  //������ٶ�ֵ
    g_fGyroAngleSpeed_z = icm_gyro_z;
}
//return ComplementaryFilter(g_fGravityAngle_z,g_fGyroAngleSpeed_z,0.005);


