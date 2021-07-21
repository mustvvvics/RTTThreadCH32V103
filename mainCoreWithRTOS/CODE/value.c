#include "headfile.h"

int16 g_fGravityAngle_z,g_fGyroAngleSpeed_z;
int16 position_front; //ƫ��
int16 position_front_last,position_front_delta;//��һ��ƫ�ƫ��ı仯��
//����ֱ���ٶȺͽ��ٶ�
int16 expected_omega=0;
int32 expected_y=30;//ת����������28 53cm OMEGA�ٶ� 80    OMEGA 195 Y�� 90
int16 speed_low=45;
int16 manual_y=0; //ң����ʻ
int16 manual_z=0;
//int32 dx=0,dy=0,dz=0,dist;//·���ۼ�
//uint8 feedBadDog=1;

//�ٶȻ�PID //���� 8000�޷�
float S_P=136;//5;//4;//0.5;//136;
float S_I=33;//12.1;//4.9;//6.1;//33;
float S_D=45;//28.8;//5.8;//45;

//���ٶȻ�����
float yaw_w_P=0.03;//0.83;//0.67;
float yaw_w_I=0;//0.002;//0.039;//0.1;//0.06;//  0.02
float yaw_w_D=0.002;//6.389;//3.7;////0.26;//0.15   0

//������ ת�򻷲���
float yaw_P=7;//2.9->50;
float yaw_I=0;
float yaw_D=0.005;

//��ͣ��־λ
uint8 car_flag=0;
uint8 carFlagPre = 0;

//���PWM //�޷�
uint16 limit_pwm=8000;

//�ٶȻ�PID //����
PID motor1_pid;
PID motor2_pid;
PID motor3_pid;
PID motor4_pid;

//���ٶȻ�����
PID yaw_w_pid;
PID_LocTypeDef yaw_pid;

//�ĸ����ӵ�Ŀ���ٶ�ֵ
int16 Left_rear;
int16 Left_front;
int16 Right_front;
int16 Right_rear;

//�����ǽ����ʻ���
int32 total_z=0;

//���ڽ��ջ�����
uint8 esp8266_buf[64];
//���ڽ��ռ�������
uint16 esp8266_cnt=0;
//IP��ַ���ձ�־λ
//uint8 IP_Start_Rec=0;
uint8 CameraShow_flag=0;

uint8 count_en=0;
uint8 go_forward=0;
uint8 go_backward=0;
uint8 go_right=0;
uint8 go_left=0;

uint8 elementFlag = 0;          //0:��Ԫ�� 1:����  2:�󻷵� 3:�һ��� 4:���� 5:ɲ��
uint8 accelerate = 10;          //��������  ��λ�� eg:23   ------>  2 . 3
uint8 ThreeWayIntersectionFlag;     //����·��
uint8 garage;                   //����
uint8 roundFinishFlag = 0;
uint8 roundIslandBegin = 0;

uint8 pidModel;                 //pid //����
rt_tick_t timet1 = 0;           //��������ʱ��
rt_tick_t timet2 = 0;
rt_tick_t timeControl = 0;

int16 motor1_pwm,motor2_pwm,motor3_pwm,motor4_pwm;
int fuzzy_txt[10];
