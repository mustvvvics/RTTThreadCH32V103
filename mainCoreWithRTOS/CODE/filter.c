/*
 * filter.c
 *
 *  Created on: Jun 9, 2021
 *      Author: 29275
 */
#include "headfile.h"

//float angle_z;
//float a;
//float dt = 0.005;//互补滤波器控制周期 5ms测一次
//float ComplementaryFilter(float acc, float gyro, float dt)
//{
//    a = 0.98;
//    angle_z = a * (angle_z + gyro * dt) + (1 - a) * (acc);
//    return angle_z;
//}
//对原始数据处理   得到3个轴的加速度数据 和 3个轴的角速度数据 这里只用到了Z轴，所以写一个函数，只对Z轴加速度，角速度，最终角度进行姿态结算
void AngleZ_Get(void)
{
    //获取原始数据
    get_icm20602_accdata_spi();
    get_icm20602_gyro_spi();

    //计算加速度
    //g_fGravityAngle_z = atan2(icm_acc_x/16384.0,icm_acc_y/16384.0) * 180.0 / 3.14;
    //计算角速度
    icm_gyro_z  = icm_gyro_z / 16.4;  //计算角速度值
    g_fGyroAngleSpeed_z = icm_gyro_z;

    //Kalman_Filter(g_fGravityAngle_z,g_fGyroAngleSpeed_z);
    //算得最终角度
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


