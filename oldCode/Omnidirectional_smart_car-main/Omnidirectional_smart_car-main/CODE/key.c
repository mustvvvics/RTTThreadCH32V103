/*
 * key.c
 *
 *  Created on: Jan 13, 2021
 *      Author: xm2000
 */
#include "key.h"
//����״̬����
uint8 key1_status = 1;
uint8 key2_status = 1;
uint8 key3_status = 1;

//���ر�־λ
uint8 key1_flag;
uint8 key2_flag;
uint8 key3_flag;

uint8 key_scan(void)
{

     //��¼���������
     uint8 key_flag=0;

     //��һ�ο���״̬����
     uint8 key1_last_status;
     uint8 key2_last_status;
     uint8 key3_last_status;

     key1_last_status = key1_status;
     key2_last_status = key2_status;
     key3_last_status = key3_status;

     //��ȡ��ǰ����״̬
     key1_status = gpio_get(KEY1);
     key2_status = gpio_get(KEY2);
     key3_status = gpio_get(KEY3);


     //��⵽��������֮��  ���ſ���λ��־λ
     if(key1_status && !key1_last_status)    key1_flag = 1;
     if(key2_status && !key2_last_status)    key2_flag = 1;
     if(key3_status && !key3_last_status)    key3_flag = 1;


     //��־λ��λ֮�󣬿���ʹ�ñ�־λִ���Լ���Ҫ�����¼�
     if(key1_flag)
     {
         key1_flag = 0;//ʹ�ð���֮��Ӧ�������־λ
         key_flag = 1;
     }

     if(key2_flag)
     {
         key2_flag = 0;//ʹ�ð���֮��Ӧ�������־λ
         key_flag = 2;
     }

     if(key3_flag)
     {
         key3_flag = 0;//ʹ�ð���֮��Ӧ�������־λ
         key_flag = 3;
     }

     return key_flag;
}

