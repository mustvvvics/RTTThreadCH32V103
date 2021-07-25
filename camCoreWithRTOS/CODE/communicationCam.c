#include "headfile.h"

uint8 temp_buff[LINE_LEN];                      //从机向主机发送数据BUFF
int16 encoder_left_front,encoder_left_rear;     //前轮左右编码器数值
uint8 receiveMainBuff[receiveBuffLength];      //接收主机数据buff

uint8 gyroRoundFinishFlag = 0;                 //接收陀螺仪Flag
int16 slave_position=0;                         //传递误差

/*
 * 1环岛     2三叉      3十字     4斜坡   5车库   6延时
 */
int32 elementTableFromMain = 11325;         //元素顺序
uint8 elementTableLengthFromMain = 5;       //元素个数
char elementQueue[16] = {0};                //转成字符串
uint8 carStart = 0;                         //启动信号

/*
 * 0 left  1 right
 */
uint8 drivingDirection = 1;                     //行驶方向
int16 ABS(int16 x){                             //绝对值
    return x>0?x:-x;
}

/*
 * change int to char
 * itoaChar(elementTableFromMain,elementTableChar,10;
 */
char* itoaChar(int32 num,char* str,uint8 radix)//value: 要转换的整数，string: 转换后的字符串,radix: 转换进制数
{
    char index[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";//索引表
    uint32 unum;//存放要转换的整数的绝对值,转换的整数可能是负数
    int16 i=0,j,k;//i用来指示设置字符串相应位，转换之后i其实就是字符串的长度；转换后顺序是逆序的，有正负的情况，k用来指示调整顺序的开始位置;j用来指示调整顺序时的交换。
    if ( num >= 0) {unum=num;}//若是num为正，直接赋值给unum
    else{unum = 0;}
    //转换部分，注意转换后是逆序的
    do
    {
        str[i++]=index[unum%radix];//取unum的最后一位，并设置为str对应位，指示索引加1
        unum/=radix;//unum去掉最后一位

    }while(unum);//直至unum为0退出循环

    str[i]='\0';//在字符串最后添加'\0'字符，c语言字符串以'\0'结束。

    //将顺序调整过来
    if(str[0]=='-') k=1;//如果是负数，符号不用调整，从符号后面开始调整
    else k=0;//不是负数，全部都要调整

    char temp;//临时变量，交换两个值时用到
    for(j=k;j<=(i-1)/2;j++)//头尾一一对称交换，i其实就是字符串的长度，索引最大值比长度少1
    {
        temp=str[j];//头部赋值给临时变量
        str[j]=str[i-1+k-j];//尾部赋值给头部
        str[i-1+k-j]=temp;//将临时变量的值(其实就是之前的头部值)赋给尾部
    }
    return str;//返回转换后的字符串

}

void encoder_init(void)
{
    timer_quad_init(TIMER_2, TIMER2_CHA_A15, TIMER2_CHB_B3);
    timer_quad_init(TIMER_3, TIMER3_CHA_B4, TIMER3_CHB_B5);
}

void get_sensor_data(void)
{
    if(gpio_get(B5))                            //读取编码器数值
        encoder_left_front = ABS(timer_quad_get(TIMER_3));
    else
        encoder_left_front = -ABS(timer_quad_get(TIMER_3));
    timer_quad_clear(TIMER_3);                  //清空计数器

    if(gpio_get(B3))
        encoder_left_rear = -ABS(timer_quad_get(TIMER_2));
    else
        encoder_left_rear = ABS(timer_quad_get(TIMER_2));
    timer_quad_clear(TIMER_2);
}

void process_data(void)                         //根据协议处理要向主机发送的数据
{
    slave_position = cameraError;//传递误差

    temp_buff[0] = 0xD8;                         //帧头

    temp_buff[1] = 0xB0;                         //功能字
    temp_buff[2] = encoder_left_front>>8;        //数据高8位
    temp_buff[3] = encoder_left_front&0xFF;      //数据低8位

    temp_buff[4] = 0xB1;                         //功能字
    temp_buff[5] = encoder_left_rear>>8;         //数据高8位
    temp_buff[6] = encoder_left_rear&0xFF;       //数据低8位

    temp_buff[7] = 0xB2;                         //功能字
    temp_buff[8] = slave_position>>8;            //数据高8位
    temp_buff[9] = slave_position&0xFF;          //数据低8位

    temp_buff[10] = 0xB3;
    temp_buff[11] = accelerateRatio;            //变速参数

    temp_buff[12] = 0xB4;
    temp_buff[13] = flagCameraElement;          //元素falg

    temp_buff[14] = 0xEE;                       //帧尾
}


void gyroDataAnalysis(uint8 *line)
{
    if (line[0] != 0xD8) {return;}
    else if(line[0] == 0xD8 && line[1] == 0xB0 && line[4] == 0xB3){ //条件筛选
        gyroRoundFinishFlag = line[5];
    }
    else {return;}
}

/*
 * 分析修改数据
 */
uint8 steerStatusFromMain = 0;
int8 negativeNumber = 0;
uint8 camFlashWriteFlag = 0;
void analysisFixParameter(uint8 *line){
    if (line[0] != 0xDE) { return ;} //不为修改参数的帧头直接返回
    else if (line[0] == 0xDE) {//帧尾帧头校验成功
        if (line[1] == 0xA0 && line[3] == 0xA8) { //int8
            if (line[5] == 1 ) {negativeNumber = -1;}else if (line[5] == 0) {negativeNumber = 1;}
            switch (line[4]) { //功能字选择
                case 0xE1:turnPage = negativeNumber * line[6];break;
                case 0xE6:globalCenterBias = negativeNumber * line[6];break;
                default:
                    break;
            }
        }
        else if (line[1] == 0xA8 && line[4] == 0xA0) { //uint8
            switch (line[5]) {
                case 0xE2:
                    clearCamFlags = line[6];
                    if (clearCamFlags == 1) {
                        flagEnterRoundabout=0;flagEnterThreeWay=0;gyroRoundFinishFlag = 0;
                        clearCamFlags = 0;}
                    break;
                case 0xE4:detectCrossroadMissingNumThres = line[6];break;
                case 0xE5:rangeSharpCurveRow = line[6];break;
                case 0xE8:detectDistance = (line[6] / 10) + ((line[6] % 10) * 0.1) ;break; //实际为float
                case 0xEA:slopeRowStart = line[6];break;
                case 0xEB:slopeRowEnd = line[6];break;
                case 0xAA:roundaboutDetectionStartRow = line[6];break;
                case 0xDA:steerStatusFromMain = line[6];break;
                case 0xDC:elementTableLengthFromMain = line[6];break;
                case 0xDD:drivingDirection = line[6];break;
                case 0xDF:
                    if (camFlashWriteFlag == 0) {camFlashWriteFlag = line[6];} break;//从机接受主机数据
                case 0xAB:carStart = line[6];break;
                default:
                    break;
            }
        }
        else if (line[2] == 0xA1 && line[3] == 0xA6) { //int16
            switch (line[4]) {
                case 0xE7: startlineJumpingPointNumThres = ((int16)line[5] << 8) | line[6];break;
                default:
                    break;
            }
        }
        else if (line[1] == 0xA3) {                    //int32
            switch (line[2]) {
                case 0xE9:pixelMeanThres = (line[3]<<24)|(line[4]<<16)|(line[5]<<8)| line[6];break;
                case 0xEC:areaDetectRoundaboutThresLeft = (line[3]<<24)|(line[4]<<16)|(line[5]<<8)| line[6];break;
                case 0xED:areaDetectRoundaboutThresRight = (line[3]<<24)|(line[4]<<16)|(line[5]<<8)| line[6];break;
                case 0xDB:
                    elementTableFromMain = (line[3]<<24)|(line[4]<<16)|(line[5]<<8)| line[6];
                    itoaChar(elementTableFromMain,elementQueue,10);break;//int to char
                case 0xBB:outboundAreaThres = (line[3]<<24)|(line[4]<<16)|(line[5]<<8)| line[6];break;
                default:
                    break;
            }
        }
    }
    else {return;}
}
