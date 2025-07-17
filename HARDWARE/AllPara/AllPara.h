#ifndef _ALLPARA_H
#define _ALLPARA_H

#include "sys.h"
#include "pid.h"
#include "can1.h"
#define ReceiveDataFromNRF //���ʹ������ң����������ʹ������궨�壬���ʹ�÷ɿ�ң��������ע�͵��ú궨��

#define PWM_LIMIT	4750		//RoboModule���� PWM�޷�


typedef struct PID_d
{
  float Kp;
  float Ki;
  float Kd;
} PID_d;

/*****************************************************��������***********************************************/
//��λ����������
extern char txt[50];
//��λ
extern float 	X_ActPos, Y_ActPos,															//Ŀ��λ��
							Z_Angle, X_Angle, Y_Angle, Z_W, X_Pos, Y_Pos,		//��λ�ش�����
							X_PosLast, Y_PosLast, Z_AngleLast, 							//��һ�ε�x,yλ�ú�z��Ƕ�
							X_TmpPos, Y_TmpPos;
extern u8			ActInitFlag;
//�Ƕ�
extern float 	TargetAngle, AngleErr,dj_angle;


////m2006

extern moto_measure_t  moto_chassis[4];  //��������������

//PID�ṹ��
extern  pid_t posx_PID, posy_PID, angle_PID,
				angle_PID_2006_1,speed_PID_2006_1,
				angle_PID_2006_2,speed_PID_2006_2,
				angle_PID_2006_3,speed_PID_2006_3,
				angle_PID_3508_1,speed_PID_3508_1,
				angle_PID_3508_2,speed_PID_3508_2,
				angle_PID_3508_3,speed_PID_3508_3,
				angle_PID_3508_4,speed_PID_3508_4,
				posX_PID, posY1_PID, posY2_PID;
extern PID_d pos_pid;//, angle_pid,angle_pid


//�ٶ�
extern	s32		Vx,Vy,Vz;	//�����˶��ٶ�
extern	s32	TargetSpeed1,	TargetSpeed2,	TargetSpeed3,	TargetSpeed4 ,TargetSpeedKick;		//RE40Ŀ���ٶ�
extern	s32		Real_Vx, Real_Vy, Real_Vz;//����ʵ���˶��ٶ�
extern	s32		real_Vx[5], real_Vy[5], real_Vz[5];//����ʵ���˶��ٶ����飬����������ƽ���ٶ�

extern double Total_Angle;

//extern	s32		set_V;
//extern short Ie;

extern s32 expect_angle , set_angle,expect_angle2 , set_angle2;	//�趨�Ƕȣ�����ֵ���ǶȻ�PID��ڲ���
extern s16 set_V,set_V2;			//�趨�ٶȣ��ɽǶȻ�PID��ã��ٶȻ�PID��ڲ���
extern short Ie,Ie2;	
//λ��
extern	int32_t X_SetPos, Y_SetPos, set_ypos2,y_err;
//����
extern	u32 x_LeftDis[5], x_RightDis[5], y_LeftDis[5],y_RightDis[5];
extern	u32 X_LeftDis, X_RightDis, Y_LeftDis, Y_RightDis;

extern	u8 X_LeftDis_T,X_RightDis_T,Y_LeftDis_T,Y_RightDis_T;

extern float JGY_Angle;

//ң����
enum{THROTTLE,YAW,PITCH,ROLL,
YAW_control,aa,bb,loca,dd,Mode_control,ee,ff,
KEY,ii};
//��ŷ�
extern u16 DCF_t;
extern u16 DCF_temp1;
extern u16 DCF_temp2;

extern	s32 TD1;//����̫С��������ٹ���
extern	s32 TD2;
extern	s32 TD3;
extern	u16 DCF_t1;
extern	u16 DCF_t2;
extern	u16 DCF_t3;


extern  u16 Ar;
extern  u16 Ar1;
extern	u16 Ar2;
extern	u16 Ar3;
extern	s16 Ar4;
extern	s16 Ar5;

extern s32 times ;//һ���ײ���
extern s32 times2 ;//�ֲ�����

 extern s32 times3 ;
 extern s32 shejian;
 extern	s32 shoujian;
 extern	s32 sheshoujian;
extern	float buChangX, buChangY;
//NRF��������
extern u8 tmp_buf[33];
extern u16 rcData[14];

//״̬����ֵ
extern char CompState;
extern char ChassisMotor;	//���̵��
extern char KickMotor;		//������

//��ʱ����
extern u16 Tt;
extern u16 T_init;
extern u16 Vt;
extern u16 Ct;

//���ⷽ��
extern u8 simple;
extern u8 direction;
//2006��ֵ
extern float Dj_angle1, Dj_angle2,	Dj_angle3;	//2006�Ƕ�

////////////����
extern float Pos_Error;
extern float JG_Error;
extern float X_Set_JGPosL,X_Set_JGPosR;
extern float X_AveDis;
extern float Y_AveDis;
extern float Y_Set_JGPosL,Y_Set_JGPosR;




//���Ƶ��
extern u32 Shoot_Fre;
//�������
extern u32 juli;



//���Դ���
enum
{
	FIRST_TRY,
	RETRY
};

//��ң������������
//extern uint8_t USART1_RX_BUF[26];

extern uint16_t sbus_ch[16];  // ͨ��ֵ
//extern uint8_t  sbus_rc_flag;

/*****************************************************�궨��***********************************************/

#ifdef ReceiveDataFromNRF

//ң������ֵ����
#define ROLL_deadzone_upperlimit     1610 //�˷������ϴ�
#define ROLL_deadzone_lowerlimit     1430
#define THROTTLE_deadzone_upperlimit 1500
#define THROTTLE_deadzone_lowerlimit 1400
#define YAW_deadzone_upperlimit      1600			//X2�����ƫ��
#define YAW_deadzone_lowerlimit      1400			//X2���ƫ��

#else

/* ��ң���������� */
#define LX_DeadZone_LowerLimit 988
#define LX_DeadZone_UpperLimit 998


#define RY_DeadZone_LowerLimit 985
#define RY_DeadZone_UpperLimit 998

#define RX_DeadZone_LowerLimit 985
#define RX_DeadZone_UpperLimit 998

#endif

//�������ߵĲ���
#define A 650
#define K 2*3.141592f/3000.0f
 
#define CAR_RADIUS  325  
//+320

//��λ���ĺ��������ĵľ���
#define CENTER_DIS  220

#ifndef ReceiveDataFromNRF	

#endif

#endif
