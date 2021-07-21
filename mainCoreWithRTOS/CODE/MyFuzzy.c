/*
 * MyFuzzy.c
 *
 *  Created on: Jul 20, 2021
 *      Author: 29275
 */
#include "headfile.h"
/**********Z�������ر���***************************/
/************************ ģ���������� ************************/
#define PMAX  100
#define PMIN -100
#define DMAX  100
#define DMIN -100
#define FMAX  100       //����ֵ������ֵ
int PFF[4] = {0,20,40,60};//0,6,14,28
/*������D����ֵ������*/
int DFF[4] = {0,15,30,50};
/*�����U����ֵ������*/
int UFF[7] = {0,50,150,200,300,350,400};//���������Χ

//��ǿ
//int rule[7][7]={//���仯�� -3,-2,-1, 0, 1, 2, 3                //   ���
//                {-12,-12,-12,-12,-10,-10,-8,},                        //   -3      0
//                {-10,-10,-10,-8,-8,-6,-2,},                        //   -2      1
//                {-8,-8,-6,-2, 0, 2, 4,},                        //   -1      2
//                {-8,-6,-2, 0, 2, 6, 8,},                        //    0      3
//                {-4,-2, 0, 2, 6, 8, 8,},                        //    1      4
//                { 2, 6, 8, 8, 10, 10, 10,},                        //    2      5
//                { 8, 10, 10, 12, 12, 12, 12} };

////����
//int rule[7][7]={//���仯�� -3,-2,-1, 0, 1, 2, 3                //   ���
//                {-12,-12,-12,-12,-10,-9,-9,},                        //   -3      0
//                {-10,-10,-10,-6,-6,-5,-2,},                        //   -2      1
//                {-6,-6,-4,-2, 0, 2, 4,},                        //   -1      2
//                {-9,-5,-2, 0, 2, 5, 9,},                        //    0      3
//                {-5,-2, 0, 2, 4, 9, 9,},                        //    1      4
//                { 2, 3, 9, 9, 10, 10, 10,},                        //    2      5
//                { 9, 10, 10, 12, 12, 12, 12}};                       //    3     6  5.94

//����
int rule[7][7]={//���仯�� -3,-2,-1, 0, 1, 2, 3                //   ���
                {-6,-6,-6,-6,-5,-5,-4,},                        //   -3      0
                {-5,-5,-5,-4,-4,-3,-1,},                        //   -2      1
                {-4,-4,-3,-1, 0, 1, 2,},                        //   -1      2
                {-4,-3,-1, 0, 1, 3, 4,},                        //    0      3
                {-2,-1, 0, 1, 3, 4, 4,},                        //    1      4
                { 1, 3, 4, 4, 5, 5, 5,},                        //    2      5
                { 4, 5, 5, 6, 6, 6, 6} };                        //    3     6  5.94

float Fuzzy(float P, float D)
{
    float U;                //ƫ��,ƫ��΢���Լ����ֵ�ľ�ȷ��
    float PF[2]={0}, DF[2]={0}, UF[4]={0};//ƫ��,ƫ��΢���Լ����ֵ��������
    int Pn=0, Dn=0, Un[4]={0};
    float temp1, temp2;

    /*�����ȵ�ȷ��*/
    /*����PD��ָ������ֵ�����Ч������*/
    if (P > -PFF[3] && P < PFF[3])
    {
        if (P <= -PFF[2])
        {
            Pn = -2;
            PF[0] = FMAX * ((float)(-PFF[2] - P) / (PFF[3] - PFF[2]));
        }
        else if (P <= -PFF[1])
        {
            Pn = -1;
            PF[0] = FMAX * ((float)(-PFF[1] - P) / (PFF[2] - PFF[1]));
        }
        else if (P <= PFF[0])
        {
            Pn = 0;
            PF[0] = FMAX * ((float)(-PFF[0] - P) / (PFF[1] - PFF[0]));
        }
        else if (P <= PFF[1])
        {
            Pn = 1; PF[0] = FMAX * ((float)(PFF[1] - P) / (PFF[1] - PFF[0]));
        }
        else if (P <= PFF[2])
        {
            Pn = 2; PF[0] = FMAX * ((float)(PFF[2] - P) / (PFF[2] - PFF[1]));
        }
        else if (P <= PFF[3])
        {
            Pn = 3; PF[0] = FMAX * ((float)(PFF[3] - P) / (PFF[3] - PFF[2]));
        }
    }
    else if (P <= -PFF[3])
    {
        Pn = -2; PF[0] = FMAX;
    }
    else if (P >= PFF[3])
    {
        Pn = 3; PF[0] = 0;
    }
    PF[1] = FMAX - PF[0];

    if (D > -DFF[3] && D < DFF[3])
    {
        if (D <= -DFF[2])
        {
            Dn = -2; DF[0] = FMAX * ((float)(-DFF[2] - D) / (DFF[3] - DFF[2]));
        }
        else if (D <= -DFF[1])
        {
            Dn = -1;
            DF[0] = FMAX * ((float)(-DFF[1] - D) / (DFF[2] - DFF[1]));
        }
        else if (D <= DFF[0])
        {
            Dn = 0;
            DF[0] = FMAX * ((float)(-DFF[0] - D) / (DFF[1] - DFF[0]));
        }
        else if (D <= DFF[1])
        {
            Dn = 1;
            DF[0] = FMAX * ((float)(DFF[1] - D) / (DFF[1] - DFF[0]));
        }
        else if (D <= DFF[2])
        {
            Dn = 2; DF[0] = FMAX * ((float)(DFF[2] - D) / (DFF[2] - DFF[1]));
        }
        else if (D <= DFF[3])
        {
            Dn = 3; DF[0] = FMAX * ((float)(DFF[3] - D) / (DFF[3] - DFF[2]));
        }
    }
    else if (D <= -DFF[3])
    {
        Dn = -2;
        DF[0] = FMAX;
    }
    else if (D >= DFF[3])
    {
        Dn = 3;
        DF[0] = 0;
    }
    DF[1] = FMAX - DF[0];

    /*ʹ����Χ�Ż���Ĺ����rule[7][7]*/
    /*���ֵʹ��13����������,����ֵ��UFF[7]ָ��*/
    /*һ�㶼���ĸ�������Ч*/
    fuzzy_txt[0]=Pn - 1 + 3;
    fuzzy_txt[1]=Dn - 1 + 3;
    fuzzy_txt[2]=Pn + 3;
    fuzzy_txt[3]=Dn - 1 + 3;
    fuzzy_txt[4]=Pn - 1 + 3;
    fuzzy_txt[5]=Dn + 3;
    fuzzy_txt[6]=Pn + 3;
    fuzzy_txt[7]=Dn + 3;

    Un[0] = rule[Pn - 1 + 3][Dn - 1 + 3];
    Un[1] = rule[Pn + 3][Dn - 1 + 3];
    Un[2] = rule[Pn - 1 + 3][Dn + 3];
    Un[3] = rule[Pn + 3][Dn + 3];
    if (PF[0] <= DF[0]) UF[0] = PF[0];
    else    UF[0] = DF[0];
    if (PF[1] <= DF[0]) UF[1] = PF[1];
    else    UF[1] = DF[0];
    if (PF[0] <= DF[1]) UF[2] = PF[0];
    else    UF[2] = DF[1];
    if (PF[1] <= DF[1]) UF[3] = PF[1];
    else    UF[3] = DF[1];

    /*ͬ���������������ֵ���*/
    if (Un[0] == Un[1])
    {
        if (UF[0] > UF[1])  UF[1] = 0;
        else    UF[0] = 0;
    }
    if (Un[0] == Un[2])
    {
        if (UF[0] > UF[2])  UF[2] = 0;
        else    UF[0] = 0;
    }
    if (Un[0] == Un[3])
    {
        if (UF[0] > UF[3])  UF[3] = 0;
        else    UF[0] = 0;
    }
    if (Un[1] == Un[2])
    {
        if (UF[1] > UF[2])  UF[2] = 0;
        else    UF[1] = 0;
    }
    if (Un[1] == Un[3])
    {
        if (UF[1] > UF[3])  UF[3] = 0;
        else    UF[1] = 0;
    }
    if (Un[2] == Un[3])
    {
        if (UF[2] > UF[3])  UF[3] = 0;
        else    UF[2] = 0;
    }
//
    /*���ķ���ģ��*/
    /*Un[]ԭֵΪ�������������ţ�ת��Ϊ��������ֵ*/
    if (Un[0] >= 0) Un[0] = UFF[Un[0]];
    else            Un[0] = -UFF[-Un[0]];
    if (Un[1] >= 0) Un[1] = UFF[Un[1]];
    else            Un[1] = -UFF[-Un[1]];
    if (Un[2] >= 0) Un[2] = UFF[Un[2]];
    else            Un[2] = -UFF[-Un[2]];
    if (Un[3] >= 0) Un[3] = UFF[Un[3]];
    else            Un[3] = -UFF[-Un[3]];

    temp1 = UF[0] * Un[0] + UF[1] * Un[1] + UF[2] * Un[2] + UF[3] * Un[3];
    temp2 = UF[0] + UF[1] + UF[2] + UF[3];
    U = temp1 / temp2;

    return U;
}

//position_front_last
//previously on:
//This is a algorithm to help smart car finish the speed dynamic_programming
//�ڱ���ʱ���������ļ���·����
//ֱ��
//���
//S��
//����
//����
//90�ȼ���
//��Բ��
int16 dynamic_programming(int16 error,int16 delta)
{
    int16 output_speed;

    if(roundIslandBegin)//����
    {
       output_speed=speed_low;//�����ٶȹ̶�
    }
    else//�ǻ�������
    {
       if(error<=5 && delta<=5)//ƫ��ͱ仯�ʶ��Ƚ�С �ȶ�ֱ��
       {
           output_speed=expected_y;
       }
       else
       {
           //output_speed=speed_low*accelerate/10;//��ͨ���
           //if(output_speed<speed_low)output_speed=speed_low;
           //if(output_speed>expected_y)output_speed=expected_y;
           output_speed=speed_low;
       }
    }
    return output_speed;
}

