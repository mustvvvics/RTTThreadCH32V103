#include "headfile.h"

/************************************************************************************************************/
/*
 * ������ && ��ձ�־λ
 */
/************************************************************************************************************/
void clearError(void){
    expected_omega = 0;position_front = 0;g_fGyroAngleSpeed_z = 0;
    yaw_pid.LocSum = 0;yaw_pid.Ek = 0;yaw_pid.Ek1 = 0;                  //���ٶȻ�
    yaw_w_pid.target_val = 0;yaw_w_pid.err_next = 0;yaw_w_pid.err = 0;  //ת��
    yaw_w_pid.err_last = 0;yaw_w_pid.actual_val = 0;
}

void clearFlags(void){
    go_forward=0;go_backward=0;go_left=0;go_right=0;
    elementFlag = 0;roundIslandBegin = 0;ThreeWayIntersectionFlag = 0;
}

/************************************************************************************************************/
/*
 * ���������ǻ���
 */
/************************************************************************************************************/
uint8 roundIslandBeginPre = 0;
int32 roundIslandMax = 80000;          //������������
void roundIslandAnalyze(void){

    if (roundIslandBeginPre == 0 && roundIslandBegin == 1) {
        total_z = g_fGyroAngleSpeed_z;
        roundIslandBeginPre = 1;
    }
    else if (roundIslandBeginPre == 1 && roundIslandBegin == 0) {
        roundIslandBeginPre = 0;
    }
    else if (roundIslandBeginPre == 1 && roundIslandBegin == 1){
        total_z += g_fGyroAngleSpeed_z;
        if (total_z > 2000000000 || total_z < -2000000000) {total_z = 0;}//�޷���ֹԽ��
        if (total_z > roundIslandMax || total_z < -roundIslandMax + 2000) {
            roundIslandBegin = 0;
            roundFinishFlag = 1;
            total_z = 0;
            send_to_cam();
        }
    }
    else {
        return;
    }
}

/************************************************************************************************************/
/*
 * ����·�ھ���
 */
/************************************************************************************************************/
uint8 threeWayIn = 0;                       //��������
uint8 threeWaySum = 0;                      //�������
uint8 ThreeWayIntersectionFlagPre = 0;      //���������ʹ��
uint8 steerStatus = 0;                      //0 ����ǰ  1 ������  2�����
void ThreeWayAnalyze(void){
    if (ThreeWayIntersectionFlagPre == 1 && ThreeWayIntersectionFlag == 0) {//����·�λᷢ��flag = 0
        ThreeWayIntersectionFlagPre = 0; //��ԭ�����ش���
    }
    else if (ThreeWayIntersectionFlagPre == 0 && ThreeWayIntersectionFlag == 1 && threeWayIn == 0 && threeWaySum == 0) {//��һ�ν���
        pwm_duty(PWM1_CH1_A8, 672); //�������
        steerStatus = 1;
        sendParameterToCam(8,0xDA,0,steerStatus,0,0);//��֪cam���״̬
        ThreeWayIntersectionFlagPre = 1;//�����������
        threeWayIn = 1;
      }
    //��һ�γ�����
    else if (ThreeWayIntersectionFlagPre == 0 && ThreeWayIntersectionFlag == 1 && threeWayIn == 1 && threeWaySum == 0) { //��һ�γ�
        pwm_duty(PWM1_CH1_A8, 1000);     //��һ�γ�ת������
        steerStatus = 2;
        sendParameterToCam(8,0xDA,0,steerStatus,0,0);//��֪cam���״̬
        ThreeWayIntersectionFlagPre = 1;//�����������
        threeWayIn = 0;               //��ǰ�������� �˴��ǳ�ȥ
        threeWaySum = 1;                //����һ������

    }
    else if (ThreeWayIntersectionFlagPre == 0 && ThreeWayIntersectionFlag == 1 && threeWayIn == 0 && threeWaySum == 1) { //�ڶ��ν�
        pwm_duty(PWM1_CH1_A8, 672); //�������
        steerStatus = 1;
        sendParameterToCam(8,0xDA,0,steerStatus,0,0);//��֪cam���״̬
        ThreeWayIntersectionFlagPre = 1;//�����������
        threeWayIn = 1;
    }

    //�ڶ��γ�����
    else if (ThreeWayIntersectionFlagPre == 0 && ThreeWayIntersectionFlag == 1 && threeWayIn == 1 && threeWaySum == 1){ //�ڶ��γ�
        pwm_duty(PWM1_CH1_A8, 338); //�����ԭ
        steerStatus = 0;
        sendParameterToCam(8,0xDA,0,steerStatus,0,0);//��֪cam���״̬
        ThreeWayIntersectionFlagPre = 1;//�����������
        threeWayIn = 0;
        threeWaySum = 0;
    }
}
/*******************************************************************************************************/
/*
 * ��̼�  encoder_x ������ʻ·��      expected_X �ڴ�ֵ
 */
/*******************************************************************************************************/
int32 encoder_x = 0,encoder_y = 0;                //��̼�
uint8 encoderCountYFlag = 0;
uint8 encoderCountYFlagPre = 0;
uint8 encoderCountYFlagMain = 0;
void encoderCountX(void){
    encoder_x +=  -(-encoder_data[3] + encoder_data[2] - encoder_data[0] + encoder_data[1])/4;
}
/*
 * ��Ԫ��֮�����  ����Ԫ������  Ԫ�ؽ�����ʼ
 */
void encoderCountY(void){ //use in isr
    if (encoderCountYFlagMain == 0) {
        if (encoderCountYFlagPre == 0 && encoderCountYFlag == 1) { //������
            encoderCountYFlagPre = 1;
        }
        else if (encoderCountYFlagPre == 1 && encoderCountYFlag == 0) {//�½���
            encoderCountYFlagPre = 0;
        }
        else if (encoderCountYFlagPre == 1 && encoderCountYFlag == 1) {//���������� ��ʼ����
            encoder_y += -(encoder_data[3] + encoder_data[2] + encoder_data[1] + encoder_data[0])/4;
            if (encoder_y > 2000000000 || encoder_y < -2000000000) {encoder_y = 0;} //�޷���ֹԽ��
        }
        else {//��Ϊ��ʱ���
            encoder_y = 0;
        }
    }
    else {
        if (encoderCountYFlagMain == 1) {
            encoder_y += -(encoder_data[3] + encoder_data[2] + encoder_data[1] + encoder_data[0])/4;
            if (encoder_y > 2000000000 || encoder_y < -2000000000) {encoder_y = 0;} //�޷���ֹԽ��
        }
        else {
            encoder_y = 0;
        }
    }
}

/************************************************************************************************************/
/*
 * ���Ʒ�ʽѡ��
 */
/************************************************************************************************************/
uint8 testSideFlag = 0;
void motor_conversion(void)
{
    if (testSideFlag == 1) {
        testSideAdvancementAbility();
    }   //���Բ����н�����
    else {
        if (car_flag == 1) {
            if (drivingDirectionToCam == 1) {           //���ҳ���
                speed_conversion(-expected_y, 0, 0);
            }
            else {
                speed_conversion(expected_y, 0, 0);     //�������
            }
            encoderCountX();             //����������
            if (encoder_x >= expected_X || encoder_x <= -expected_X) {
                car_flag = 2;                                       //��ʼ��ʻ
                clearCamFlags = 1;confirmButton = 0;
//                carStart = 2;   //����;ȡ������;�����ź�2;
//                sendParameterToCam(8,0xAB,0,carStart,0,0);          //���������ź�2
                sendParameterToCam(8,0xE2,0,clearCamFlags,0,0);     //���cam flag
//                carStart = 0;
                encoder_x = 0;                         //ʹ��ͣ�º�����ٴ�����
            }
        }
    /************************************************************************************************************/
        //������ʻ
        else if (car_flag == 2 && threeWayIn == 0 && threeWaySum == 0)
        {
            carFlagPre = 0;
            if(roundIslandBegin)                        //����PID
            {
                expected_y = 55;
//                yaw_pid.Kp = 8;yaw_pid.Kd = 0.005;   //ȫ�ֿɿ�
                expected_omega = PID_Loc(0,-position_front,&yaw_pid);//���� P:8.0 D:0.005
            }
            else
            {
                expected_omega = Fuzzy((position_front),(position_front_delta)); //ģ��PID
            }
            if (accelerate == 0) {clearError();speed_conversion(0,0,0);}    //ת����ͣһ��
            else {
    //            speed_conversion(0,dynamic_programming(-position_front,position_front_delta), \           //�����˼���
    //                    PID_Angle(expected_omega,g_fGyroAngleSpeed_z,&yaw_w_pid)+(expected_omega/11));
                speed_conversion(0,(expected_y * accelerate) / 10,    \
                        PID_Angle(expected_omega,g_fGyroAngleSpeed_z,&yaw_w_pid)+(expected_omega/11));
            }

        }
    /************************************************************************************************************/
        //��������ʻ              //p22.0 d:0.005 sp:50   // p:16.5   d:0.005   sp=45    // p:11.0 d:0.005 sp:40
        else if (car_flag == 2 && threeWayIn == 1) {
    //        yaw_pid.Kp = 26;yaw_pid.Kd = 0.005;
            expected_y = 40;
            if (accelerate == 0) {clearError();speed_conversion(0,0,0);}    //ת����ͣһ��
            else {
    //            expected_omega = PID_Loc(0,-position_front,&yaw_pid);     //λ��PID
                expected_omega = Fuzzy((position_front),(position_front_delta)); //ģ��PID
                speed_conversion((-expected_y * accelerate) / 10, 0, \
                        PID_Angle(expected_omega,g_fGyroAngleSpeed_z,&yaw_w_pid)+(expected_omega/11) );
            }
        }
    /************************************************************************************************************/
        //�˳�������ʻ
        else if (car_flag == 2  && threeWayIn == 0 && threeWaySum == 1) {
            if(roundIslandBegin)                      //����PID
            {
                expected_y = 55;
//                yaw_pid.Kp = 8;yaw_pid.Kd = 0.005;  //ȫ�ֿɿ�
                expected_omega = PID_Loc(0,-position_front,&yaw_pid);
            }
            else
            {

                expected_omega = Fuzzy((position_front),(position_front_delta)); //ģ��PID
            }
            if (accelerate == 0) {clearError();speed_conversion(0,0,0);}         //ת����ͣһ��
            else {
    //            speed_conversion(0,dynamic_programming(-position_front,position_front_delta), \            //�����˼���
    //                    PID_Angle(expected_omega,g_fGyroAngleSpeed_z,&yaw_w_pid)+(expected_omega/11));
                speed_conversion(0,(-expected_y * accelerate) / 10 , \
                        PID_Angle(expected_omega,g_fGyroAngleSpeed_z,&yaw_w_pid)+(expected_omega/11));
            }


        }
    /************************************************************************************************************/
//        else if (carFlagPre == 1 && car_flag == 0) {//ͣ������һ���ź�1
//            carStart = 1;
//            sendParameterToCam(8,0xAB,0,carStart,0,0);//�����ź�1
//            carFlagPre = 0;
//        }
        else
        {
            clearError();clearFlags();
            speed_conversion(0,0,0);
        }
    }
}
