/***********************************************************************************************************
 *  @file      AllPara.c
 *  @version   1.0
 *  @date      2020.8.15
 *  @author    xb
 *  @brief     ���еĲ����Ķ���
 *  @copyright 2019 SDUST Robocon. All rights reserved.
 ***********************************************************************************************************/
#include "AllPara.h"
#include "task.h"

//ȫ����λ���ݣ�����ϵ����ϵ��������ΪX������������ΪY��������
float X_ActPos, Y_ActPos,															//����ʵ��λ�ã�������λ��װ�ͳ�������ƫ��
			X_Angle, Y_Angle, Z_Angle, X_Pos, Y_Pos, Z_W,		//ȫ����λ�ش�������X��Y��Z��Ƕȣ�X��Y���ꣻZ����ٶ�
			X_PosLast, Y_PosLast, Z_AngleLast, 							//��һ�ε�x,yλ�ú�z��Ƕ�
			X_TmpPos, Y_TmpPos;
			
//Ŀ��λ��
int32_t X_SetPos, Y_SetPos;

//�Ƕ�
float TargetAngle, 		//Ŀ��Ƕ�
			AngleErr,				//�Ƕ�ƫ��
			dj_angle;				

u8 		ActInitFlag=0;	//ȫ����λ��־λ��1Ϊ��ʼ�����
////m2006
moto_measure_t  moto_chassis[4];  //��������������

//PID�ṹ��  pid_t ��pid_dҪ�ֿ�-ע��pid���㺯��
pid_t posx_PID, posy_PID,angle_PID, 
			angle_PID_2006_1,speed_PID_2006_1,
			angle_PID_2006_2,speed_PID_2006_2,
			angle_PID_2006_3,speed_PID_2006_3,
			angle_PID_3508_1,speed_PID_3508_1,
			angle_PID_3508_2,speed_PID_3508_2,
			angle_PID_3508_3,speed_PID_3508_3,
			angle_PID_3508_4,speed_PID_3508_4,
			posX_PID, posY1_PID, posY2_PID;


PID_d pos_pid;//, angle_pid  angle_pid,

//�ٶ�
s32		Vx,Vy,Vz;										//�����Ŀ���˶��ٶ�
s32		Real_Vx, Real_Vy, Real_Vz;	//����ʵ���˶��ٶ�
s32		TargetSpeed1,	TargetSpeed2,	TargetSpeed3,	TargetSpeed4 ,TargetSpeedKick;		//RE40���Ŀ���ٶȣ�����ǰ��ʱ����1-4
s32		real_Vx[5], real_Vy[5], real_Vz[5];//����ʵ���˶��ٶ����飬��������ƽ���ٶ�

s32 expect_angle , set_angle,expect_angle2 , set_angle2;	//�趨�Ƕȣ�����ֵ���ǶȻ�PID��ڲ���
s16 set_V,set_V2;			//�趨�ٶȣ��ɽǶȻ�PID��ã��ٶȻ�PID��ڲ���
short Ie,Ie2;				//��������������������ٶȻ�PID���

double Total_Angle;

//s32		set_V;//�趨�ٶȣ��ɽǶȻ�PID��ã��ٶȻ�PID��ڲ���
//short Ie;				//��������������������ٶȻ�PID���

//����

u32 	x_LeftDis[5], x_RightDis[5], y_LeftDis[5],	y_RightDis[5];	//���������˲�����
u32	 	X_LeftDis, 		X_RightDis, 	 Y_LeftDis, 		Y_RightDis;			//������������
u8 		X_LeftDis_T=0,X_RightDis_T=0,Y_LeftDis_T=0,	Y_RightDis_T=0;	//�����������ڼ��ⲻͬͨ����������ʱû�ã�

float JGY_Angle=0;

//��ŷ�
	s32 TD1=0;//����̫С��������ٹ���
	s32 TD2=0;
	s32 TD3=0;
	u16 DCF_t1=400;
	u16 DCF_t2=500;
	u16 DCF_t3=500;
	u16 DCF_temp1;
	u16 DCF_temp2;
	
	//����λ��
	u16 Ar=66;
	u16 Ar1=500;
	u16 Ar2=300;
	u16 Ar3=0;
	s16 Ar4=-300;
	s16 Ar5=-500;
	
	
	s32 times = 0;//һ���ײ���
	s32 times2 = 0;//�ֲ�����
	s32 times3 = 0;//�ֲ�����
	
	s32 shejian = 0;
	s32 shoujian = 0;
	s32 sheshoujian = 0;

	float buChangX = 0, buChangY = 0;
//NRF��������
u8 tmp_buf[33];

//״̬��
char CompState;							//���Դ���


//��ʱ����
u16 Tt=0;
u16 T_init=0;	//ȫ����λ��ʼ����ʱ
u16 Vt=0;
u16 Ct=0;

//���ⷽ��
u8 simple = 0;
u8 direction = 0;

//sbus���߽��ܺ��������
uint16_t sbus_ch[16];  // ͨ��ֵ
uint8_t  sbus_rc_flag = 0;

//ң������������
uint8_t USART1_RX_BUF[26];
////sbus����״̬��
//static u32 recvStat = COM_STOP_BIT;     // ����״̬��
//static u16 recvData = 0;                 // ����һ���������ݱ���

u8 USART_buf[25]={0};
			
float Dj_angle1, Dj_angle2,	Dj_angle3;	//2006�Ƕ�


////////����
float Pos_Error;//ȫ����λ�Ĳ�ֵ
float JG_Error;//����Ĳ�ֵ
float X_Set_JGPosL,X_Set_JGPosR;
float Y_Set_JGPosL,Y_Set_JGPosR;
float X_AveDis;

float Y_AveDis;


//���Ƶ��
u32 Shoot_Fre;
//�������
u32 juli;



