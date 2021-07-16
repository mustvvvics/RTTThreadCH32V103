#include "headfile.h"

int16 ICM_FIFO[10];//存储IMU信息
//
//
//MPU9250_GyroRead(gyy);//读取陀螺仪角速度值
//
//MPU6050_newValues(gyy);//将z轴数据送入 imu采样信息均值滤波
//
int16  ICM_newValues(int16 gyro_z)
{
            unsigned char i ;
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
//float angle_z;
//float a;
//float dt = 0.005;//互补滤波器控制周期 5ms测一次
//float ComplementaryFilter(float acc, float gyro, float dt)
//{
//    a = 0.98;
//    angle_z = a * (angle_z + gyro * dt) + (1 - a) * (acc);
//    return angle_z;
//}
//对原始数据处理   得到3个轴的加速度数据 和 3个轴的角速度数据 这里只用到了Z
//所以写一个函数,只对该轴加速度,角速度,最终角度进行姿态结算
void AngleZ_Get(void)
{
    //获取原始数据
    get_icm20602_accdata_spi();
    get_icm20602_gyro_spi();

    //计算角速度
    icm_gyro_z  = icm_gyro_z / 16.4;  //计算角速度值
    g_fGyroAngleSpeed_z=ICM_newValues(icm_gyro_z);
}
//return ComplementaryFilter(g_fGravityAngle_z,g_fGyroAngleSpeed_z,0.005);



