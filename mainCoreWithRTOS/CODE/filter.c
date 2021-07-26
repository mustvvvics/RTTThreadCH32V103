#include "headfile.h"

int16 ICM_FIFO[10];//存储ICM信息

int16  ICM_newValues(int16 gyro_z)
{
    uint8 i ;
    int16 sum=0;

    for(i=1;i<10;i++){  //FIFO 操作
        ICM_FIFO[i-1]=ICM_FIFO[i];
    }
    ICM_FIFO[9]=gyro_z;//将新的数据放置到 数据的最后面
    sum=0;
    for(i=0;i<10;i++){  //求当前数组的合，再取平均值
        sum+=ICM_FIFO[i];
    }

    return sum/10;
}

void AngleZ_Get(void)
{
    //获取原始数据
//    get_icm20602_accdata_spi();
    get_icm20602_gyro_spi();

    //计算角速度
    icm_gyro_z  = icm_gyro_z / 16.4;  //计算角速度值
    g_fGyroAngleSpeed_z=ICM_newValues(icm_gyro_z);
//    icm_gyro_y = icm_gyro_y /16.4;  //俯仰
//    icm_gyro_x = icm_gyro_x /16.4;  //横滚
}
