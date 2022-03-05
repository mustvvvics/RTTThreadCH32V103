/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file            main
 * @company         成都逐飞科技有限公司
 * @author          逐飞科技(QQ790875685)
 * @version         查看doc内version文件 版本说明
 * @Software        MounRiver Studio V1.3
 * @Target core     CH32V103R8T6
 * @Taobao          https://seekfree.taobao.com/
 * @date            2020-12-04
 ********************************************************************************************************************/
//整套推荐IO查看Projecct文件夹下的TXT文本
//打开新的工程或者工程移动了位置务必执行以下操作
//右键单击工程，选择刷新
#include "headfile.h"

uint8 message[] = "\n";
int main(void)
{
    char txt[128]={0};
    uint8 key_key,simple_cnt=0,simple_flag=0,i;
    int32 simple[4]={0};
    //float turn_speed;
    Peripherals_Init();
    //z<0 右转
    //z>0 左转
    //pos值判断
    //负的话向左 正的话向右
    speed_conversion(0,0,0);
    while(1)
    {
        ad_calculate();

//        if(simple_flag == 0 && car_flag ==0 && start_flag ==0)
//        {
//            show_ad();
//        }
//        else if(simple_flag == 1&& car_flag ==0&& start_flag ==0)
//        {
//            show_simple();
        show_car();
        if(key_tick>=10)
        {
            sprintf(txt,"island_flag=%d | island_finish=%d|island_sum=%d|"
                    "ad_1-2=%03d|ad_0-3=%03d|ad_0-1=%03d|"
                    "ad_2-3=%03d|ad_0+1=%03d|ad_2+3=%03d"
                    ,island_flag,island_finish,island_sum,
                    ad_dif0,ad_dif1,ad_dif2,ad_dif3,ad_sum2,ad_sum3);

            seekfree_wireless_send_buff(txt,sizeof(txt)-1);

            seekfree_wireless_send_buff(message,sizeof(message)-1); //换行符号
            key_key = key_scan();//默认返回0
            if(key_key == 1)
                {key_cnt++;simple_flag=1;ips114_clear(WHITE);}
            else if(key_key == 2)
            {
                if(car_flag == 1)
                    car_flag =0;
                else if(car_flag == 0)
                    car_flag=1;
                ips114_clear(WHITE);
            }
            else if(key_key == 3)
            {
//                basic_data++;
                if (key_cnt == 1) {
                    island_need++;
                }
                else if (key_cnt == 2) {
                    island_need--;
                }
                else if (key_cnt == 0) {
                    basic_speed++;
                }
            }
            //ad_max校正
            if(key_cnt == 1 && simple_flag==1)
            {
                for(i=0;i<4;i++)
                {
                   simple[i]+=ad[i];
                }
                simple_cnt++;
                if(simple_cnt>=200)
                {
                    for(i=0;i<4;i++)
                    {
                       ad_max[i]=simple[i]/200;
                       simple[i]=0;
                    }
                    simple_flag =0;
                    simple_cnt=0;
                    ips114_clear(WHITE);
                }
            }
            //ad[1]校正
            else if(key_cnt == 2&& simple_flag==1)
            {
                simple[0]+=ad[0];
                simple_cnt++;
                if(simple_cnt>=200)
                {
                    ad_min[0]=simple[0]/200;
                    simple[0]=0;
                    simple_flag =0;
                    simple_cnt=0;
                    ips114_clear(WHITE);
                }
            }
            else if(key_cnt == 3&& simple_flag==1)
            {
                simple[1]+=ad[1];
                simple_cnt++;
                if(simple_cnt>=200)
                {
                    ad_min[1]=simple[1]/200;
                    simple[1]=0;
                    simple_flag =0;
                    simple_cnt=0;
                    ips114_clear(WHITE);
                }
            }
            else if(key_cnt == 4&& simple_flag==1)
            {
                simple[2]+=ad[2];
                simple_cnt++;
                if(simple_cnt>=200)
                {
                    ad_min[2]=simple[2]/200;
                    simple[2]=0;
                    simple_flag =0;
                    simple_cnt=0;
                    ips114_clear(WHITE);
                }
            }
            else if(key_cnt == 5&& simple_flag==1)
            {
                simple[3]+=ad[3];
                simple_cnt++;
                if(simple_cnt>=200)
                {
                    ad_min[3]=simple[3]/200;
                    simple[3]=0;
                    simple_flag =0;
                    simple_cnt=0;
                    ips114_clear(WHITE);
                }
            }
            else if(key_cnt == 6&& simple_flag==1)
            {
                for(i=0;i<4;i++)
                {
                   write_buff[i]=ad_max[i];
                   write_buff[i+4]=ad_min[i];
                }
                //将write_buff中的10个数据写入第15个扇区的第0页
                flash_page_program(FLASH_SECTION_15, FLASH_PAGE_0, write_buff, 8);
                simple_flag =0;
            }
            key_tick=0;
        }

    }
}

//if(abs(angle - last_angle)>30)position[0] = last_angle;
//else position[0] = angle;
//
//last_angle = angle;
//if(position[0]>55)position[0]=55;
//else if(position[0]<-55)position[0]=-55;


