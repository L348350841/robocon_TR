#ifndef _ALLPARA_H
#define _ALLPARA_H

#include "sys.h"
#include "pid.h"
#include "can1.h"
#define ReceiveDataFromNRF //如果使用自制遥控器，必须使用这个宏定义，如果使用飞控遥控器，请注释掉该宏定义

#define PWM_LIMIT	4750		//RoboModule驱动 PWM限幅


typedef struct PID_d
{
  float Kp;
  float Ki;
  float Kd;
} PID_d;

/*****************************************************变量声明***********************************************/
//上位机调试数组
extern char txt[50];
//定位
extern float 	X_ActPos, Y_ActPos,															//目标位置
							Z_Angle, X_Angle, Y_Angle, Z_W, X_Pos, Y_Pos,		//定位回传参数
							X_PosLast, Y_PosLast, Z_AngleLast, 							//上一次的x,y位置和z轴角度
							X_TmpPos, Y_TmpPos;
extern u8			ActInitFlag;
//角度
extern float 	TargetAngle, AngleErr,dj_angle;


////m2006

extern moto_measure_t  moto_chassis[4];  //四组电机反馈数据

//PID结构体
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


//速度
extern	s32		Vx,Vy,Vz;	//车体运动速度
extern	s32	TargetSpeed1,	TargetSpeed2,	TargetSpeed3,	TargetSpeed4 ,TargetSpeedKick;		//RE40目标速度
extern	s32		Real_Vx, Real_Vy, Real_Vz;//车体实际运动速度
extern	s32		real_Vx[5], real_Vy[5], real_Vz[5];//车体实际运动速度数组，用来就是那平均速度

extern double Total_Angle;

//extern	s32		set_V;
//extern short Ie;

extern s32 expect_angle , set_angle,expect_angle2 , set_angle2;	//设定角度，给定值，角度环PID入口参数
extern s16 set_V,set_V2;			//设定速度，由角度环PID算得，速度环PID入口参数
extern short Ie,Ie2;	
//位置
extern	int32_t X_SetPos, Y_SetPos, set_ypos2,y_err;
//激光
extern	u32 x_LeftDis[5], x_RightDis[5], y_LeftDis[5],y_RightDis[5];
extern	u32 X_LeftDis, X_RightDis, Y_LeftDis, Y_RightDis;

extern	u8 X_LeftDis_T,X_RightDis_T,Y_LeftDis_T,Y_RightDis_T;

extern float JGY_Angle;

//遥控器
enum{THROTTLE,YAW,PITCH,ROLL,
YAW_control,aa,bb,loca,dd,Mode_control,ee,ff,
KEY,ii};
//电磁阀
extern u16 DCF_t;
extern u16 DCF_temp1;
extern u16 DCF_temp2;

extern	s32 TD1;//不能太小，溢出会再归零
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

extern s32 times ;//一整套操作
extern s32 times2 ;//局部试验

 extern s32 times3 ;
 extern s32 shejian;
 extern	s32 shoujian;
 extern	s32 sheshoujian;
extern	float buChangX, buChangY;
//NRF接收数组
extern u8 tmp_buf[33];
extern u16 rcData[14];

//状态机初值
extern char CompState;
extern char ChassisMotor;	//底盘电机
extern char KickMotor;		//踢球电机

//延时参数
extern u16 Tt;
extern u16 T_init;
extern u16 Vt;
extern u16 Ct;

//激光方向
extern u8 simple;
extern u8 direction;
//2006数值
extern float Dj_angle1, Dj_angle2,	Dj_angle3;	//2006角度

////////////激光
extern float Pos_Error;
extern float JG_Error;
extern float X_Set_JGPosL,X_Set_JGPosR;
extern float X_AveDis;
extern float Y_AveDis;
extern float Y_Set_JGPosL,Y_Set_JGPosR;




//射击频率
extern u32 Shoot_Fre;
//射击距离
extern u32 juli;



//尝试次数
enum
{
	FIRST_TRY,
	RETRY
};

//新遥控器接收数组
//extern uint8_t USART1_RX_BUF[26];

extern uint16_t sbus_ch[16];  // 通道值
//extern uint8_t  sbus_rc_flag;

/*****************************************************宏定义***********************************************/

#ifdef ReceiveDataFromNRF

//遥控器数值死区
#define ROLL_deadzone_upperlimit     1610 //此方向误差较大
#define ROLL_deadzone_lowerlimit     1430
#define THROTTLE_deadzone_upperlimit 1500
#define THROTTLE_deadzone_lowerlimit 1400
#define YAW_deadzone_upperlimit      1600			//X2的最高偏差
#define YAW_deadzone_lowerlimit      1400			//X2最低偏差

#else

/* 新遥控器的死区 */
#define LX_DeadZone_LowerLimit 988
#define LX_DeadZone_UpperLimit 998


#define RY_DeadZone_LowerLimit 985
#define RY_DeadZone_UpperLimit 998

#define RX_DeadZone_LowerLimit 985
#define RX_DeadZone_UpperLimit 998

#endif

//过杆曲线的参数
#define A 650
#define K 2*3.141592f/3000.0f
 
#define CAR_RADIUS  325  
//+320

//定位中心和三轮中心的距离
#define CENTER_DIS  220

#ifndef ReceiveDataFromNRF	

#endif

#endif
