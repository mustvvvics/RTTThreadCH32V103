/*
 * key.c
 *
 *  Created on: Jan 13, 2021
 *      Author: xm2000
 */
#include "key.h"
//开关状态变量
uint8 key1_status = 1;
uint8 key2_status = 1;
uint8 key3_status = 1;

//开关标志位
uint8 key1_flag;
uint8 key2_flag;
uint8 key3_flag;

uint8 key_scan(void)
{

     //记录按键输出号
     uint8 key_flag=0;

     //上一次开关状态变量
     uint8 key1_last_status;
     uint8 key2_last_status;
     uint8 key3_last_status;

     key1_last_status = key1_status;
     key2_last_status = key2_status;
     key3_last_status = key3_status;

     //读取当前按键状态
     key1_status = gpio_get(KEY1);
     key2_status = gpio_get(KEY2);
     key3_status = gpio_get(KEY3);


     //检测到按键按下之后  并放开置位标志位
     if(key1_status && !key1_last_status)    key1_flag = 1;
     if(key2_status && !key2_last_status)    key2_flag = 1;
     if(key3_status && !key3_last_status)    key3_flag = 1;


     //标志位置位之后，可以使用标志位执行自己想要做的事件
     if(key1_flag)
     {
         key1_flag = 0;//使用按键之后，应该清除标志位
         key_flag = 1;
     }

     if(key2_flag)
     {
         key2_flag = 0;//使用按键之后，应该清除标志位
         key_flag = 2;
     }

     if(key3_flag)
     {
         key3_flag = 0;//使用按键之后，应该清除标志位
         key_flag = 3;
     }

     return key_flag;
}

