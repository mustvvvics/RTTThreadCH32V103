#include "headfile.h"

int16 g_fGravityAngle_z,g_fGyroAngleSpeed_z;
int16 position_front; //偏差
int16 position_front_last,position_front_delta;//上一次偏差，偏差的变化率

//期望直线速度和角速度
int16 expected_omega=0;
int32 expected_y=40;
int16 speed_low=50;
int16 expected_X = 3000;

//速度环PID //参数 8000限幅
float S_P=136;
float S_I=33;
float S_D=45;

//角速度环参数
float yaw_w_P=0.03;//0.83;//0.67;
float yaw_w_I=0;//0.002;//0.039;//0.1;//0.06;//  0.02
float yaw_w_D=0.002;//6.389;//3.7;////0.26;//0.15   0

//转向环参数
float yaw_P=8.0;//8.0 50速度的环岛
float yaw_I=0;
float yaw_D=0.005;

//启停标志位
uint8 car_flag=0;
uint8 carFlagPre = 0;

//电机PWM限幅
uint16 limit_pwm=8000;

//速度环PID //参数
PID motor1_pid;
PID motor2_pid;
PID motor3_pid;
PID motor4_pid;

//角速度环参数
PID yaw_w_pid;
PID_LocTypeDef yaw_pid;

//四个轮子的目标速度值
int16 Left_rear;
int16 Left_front;
int16 Right_front;
int16 Right_rear;

//陀螺仪角速率积分
int32 total_z=0;

//串口接收缓冲区
uint8 esp8266_buf[64];
//串口接收计数变量
uint16 esp8266_cnt=0;

uint8 CameraShow_flag=0;

uint8 go_forward=0;
uint8 go_backward=0;
uint8 go_right=0;
uint8 go_left=0;

uint8 elementFlag = 0;          //0:无元素 1:三叉  2:左环岛 3:右环岛 4:车库 5:刹车
uint8 accelerate = 10;          //加速因子  两位数 eg:23   ------>  2 . 3
uint8 ThreeWayIntersectionFlag;   //三叉路口
uint8 garage;                   //车库
uint8 roundFinishFlag = 0;
uint8 roundIslandBegin = 0;

uint8 pidModel;                 //pid //调试
rt_tick_t timet1 = 0;           //计算运行时间
rt_tick_t timet2 = 0;
rt_tick_t timeControl = 0;

int16 motor1_pwm,motor2_pwm,motor3_pwm,motor4_pwm;
int fuzzy_txt[10];
