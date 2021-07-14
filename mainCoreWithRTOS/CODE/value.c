#include "headfile.h"

int16 g_fGravityAngle_z,g_fGyroAngleSpeed_z;
int16 position_front; //偏差

//期望直线速度和角速度
int16 expected_omega=0;
int32 expected_y=0;//转个环岛试试28 53cm OMEGA速度 80    OMEGA 195 Y轴 90
int16 manual_y=0; //遥控行驶
int16 manual_z=0;
//int32 dx=0,dy=0,dz=0,dist;//路径累计
//uint8 feedBadDog=1;

//速度环PID参数 8000限幅
float S_P=136;
float S_I=33;
float S_D=45;

//角速度环参数
float yaw_w_P=1;
float yaw_w_I=0.01;//0.06;//  0.02
float yaw_w_D=0.15;//0.26;//0.15   0

//待整定 转向环参数
float yaw_P=5;//2.9->50;
float yaw_I=0;
float yaw_D=0;

//启停标志位
uint8 car_flag=0;
uint8 carFlagPre = 0;

//电机PWM限幅
uint16 limit_pwm=10000;

//速度环PID参数
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

float leftRearADRC;
float leftFrontADRC;
float rightFrontADRC;
float rightRearADRC;

//陀螺仪角速率积分
int32 total_z=0;

//串口接收缓冲区
uint8 esp8266_buf[64];
//串口接收计数变量
uint16 esp8266_cnt=0;
//IP地址接收标志位
//uint8 IP_Start_Rec=0;
uint8 CameraShow_flag=0;

uint8 count_en=0;
uint8 go_forward=0;
uint8 go_backward=0;
uint8 go_right=0;
uint8 go_left=0;

uint8 elementFlag = 0;          //0:无元素 1:三叉  2:左环岛 3:右环岛 4:车库 5:刹车
uint8 accelerate = 10;          //加速因子  两位数 eg：23   ------>  2 . 3
uint8 ThreeWayIntersection;     //三叉路口
uint8 garage;                   //车库

uint8 pidModel;                 //pid 调试
rt_tick_t timet1 = 0;           //计算运行时间
rt_tick_t timet2 = 0;
rt_tick_t timeControl = 0;
