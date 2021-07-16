#include "headfile.h"

int16 ICM_FIFO[10];//�洢IMU��Ϣ
//
//
//MPU9250_GyroRead(gyy);//��ȡ�����ǽ��ٶ�ֵ
//
//MPU6050_newValues(gyy);//��z���������� imu������Ϣ��ֵ�˲�
//
int16  ICM_newValues(int16 gyro_z)
{
            unsigned char i ;
            int16 sum=0;

            for(i=1;i<10;i++){  //FIFO ����
            ICM_FIFO[i-1]=ICM_FIFO[i];
            }
            ICM_FIFO[9]=gyro_z;//���µ����ݷ��õ� ���ݵ������
            sum=0;
            for(i=0;i<10;i++){  //��ǰ����ĺϣ���ȡƽ��ֵ
                 sum+=ICM_FIFO[i];
            }

            return sum/10;
}
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
    g_fGyroAngleSpeed_z=ICM_newValues(icm_gyro_z);
}
//return ComplementaryFilter(g_fGravityAngle_z,g_fGyroAngleSpeed_z,0.005);



