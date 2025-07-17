/***********************************************************************************************************
 *  @file      AllPara.c
 *  @version   1.0
 *  @date      2020.8.15
 *  @author    xb
 *  @brief     所有的参数的定义
 *  @copyright 2019 SDUST Robocon. All rights reserved.
 ***********************************************************************************************************/
#include "AllPara.h"
#include "task.h"

//全场定位数据，右手系坐标系，踢球方向为X轴正方向，左向为Y轴正方向
float X_ActPos, Y_ActPos,															//车体实际位置，补正定位安装和车体中心偏差
			X_Angle, Y_Angle, Z_Angle, X_Pos, Y_Pos, Z_W,		//全场定位回传参数：X、Y、Z轴角度；X、Y坐标；Z轴角速度
			X_PosLast, Y_PosLast, Z_AngleLast, 							//上一次的x,y位置和z轴角度
			X_TmpPos, Y_TmpPos;
			
//目标位置
int32_t X_SetPos, Y_SetPos;

//角度
float TargetAngle, 		//目标角度
			AngleErr,				//角度偏差
			dj_angle;				

u8 		ActInitFlag=0;	//全场定位标志位：1为初始化完成
////m2006
moto_measure_t  moto_chassis[4];  //四组电机反馈数据

//PID结构体  pid_t 与pid_d要分开-注意pid计算函数
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

//速度
s32		Vx,Vy,Vz;										//车体的目标运动速度
s32		Real_Vx, Real_Vy, Real_Vz;	//车体实际运动速度
s32		TargetSpeed1,	TargetSpeed2,	TargetSpeed3,	TargetSpeed4 ,TargetSpeedKick;		//RE40电机目标速度，从左前逆时针编号1-4
s32		real_Vx[5], real_Vy[5], real_Vz[5];//车体实际运动速度数组，用来就求平均速度

s32 expect_angle , set_angle,expect_angle2 , set_angle2;	//设定角度，给定值，角度环PID入口参数
s16 set_V,set_V2;			//设定速度，由角度环PID算得，速度环PID入口参数
short Ie,Ie2;				//输出电流，控制量，由速度环PID算得

double Total_Angle;

//s32		set_V;//设定速度，由角度环PID算得，速度环PID入口参数
//short Ie;				//输出电流，控制量，由速度环PID算得

//激光

u32 	x_LeftDis[5], x_RightDis[5], y_LeftDis[5],	y_RightDis[5];	//激光数据滤波数组
u32	 	X_LeftDis, 		X_RightDis, 	 Y_LeftDis, 		Y_RightDis;			//激光最终数据
u8 		X_LeftDis_T=0,X_RightDis_T=0,Y_LeftDis_T=0,	Y_RightDis_T=0;	//计数量，用于激光不同通道读数，暂时没用！

float JGY_Angle=0;

//电磁阀
	s32 TD1=0;//不能太小，溢出会再归零
	s32 TD2=0;
	s32 TD3=0;
	u16 DCF_t1=400;
	u16 DCF_t2=500;
	u16 DCF_t3=500;
	u16 DCF_temp1;
	u16 DCF_temp2;
	
	//箭的位置
	u16 Ar=66;
	u16 Ar1=500;
	u16 Ar2=300;
	u16 Ar3=0;
	s16 Ar4=-300;
	s16 Ar5=-500;
	
	
	s32 times = 0;//一整套操作
	s32 times2 = 0;//局部试验
	s32 times3 = 0;//局部试验
	
	s32 shejian = 0;
	s32 shoujian = 0;
	s32 sheshoujian = 0;

	float buChangX = 0, buChangY = 0;
//NRF接收数组
u8 tmp_buf[33];

//状态机
char CompState;							//尝试次数


//延时参数
u16 Tt=0;
u16 T_init=0;	//全场定位初始化延时
u16 Vt=0;
u16 Ct=0;

//激光方向
u8 simple = 0;
u8 direction = 0;

//sbus总线接受后处理的数据
uint16_t sbus_ch[16];  // 通道值
uint8_t  sbus_rc_flag = 0;

//遥控器接收数组
uint8_t USART1_RX_BUF[26];
////sbus接收状态机
//static u32 recvStat = COM_STOP_BIT;     // 定义状态机
//static u16 recvData = 0;                 // 定义一个接收数据变量

u8 USART_buf[25]={0};
			
float Dj_angle1, Dj_angle2,	Dj_angle3;	//2006角度


////////激光
float Pos_Error;//全场定位的差值
float JG_Error;//激光的差值
float X_Set_JGPosL,X_Set_JGPosR;
float Y_Set_JGPosL,Y_Set_JGPosR;
float X_AveDis;

float Y_AveDis;


//射击频率
u32 Shoot_Fre;
//射击距离
u32 juli;



