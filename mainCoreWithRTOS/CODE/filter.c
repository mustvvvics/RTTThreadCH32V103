#include "headfile.h"

int16 ICM_FIFO[10];//�洢ICM��Ϣ

int16  ICM_newValues(int16 gyro_z)
{
    uint8 i ;
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

void AngleZ_Get(void)
{
    //��ȡԭʼ����
//    get_icm20602_accdata_spi();
    get_icm20602_gyro_spi();

    //������ٶ�
    icm_gyro_z  = icm_gyro_z / 16.4;  //������ٶ�ֵ
    g_fGyroAngleSpeed_z=ICM_newValues(icm_gyro_z);
//    icm_gyro_y = icm_gyro_y /16.4;  //����
//    icm_gyro_x = icm_gyro_x /16.4;  //���
}
