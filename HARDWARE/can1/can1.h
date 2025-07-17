#ifndef _CAN_H_
#define _CAN_H_

#include "sys.h"	


#define OpenLoop_Mode                       0x01
#define Current_Mode                        0x02
#define Velocity_Mode                       0x03
#define Position_Mode                       0x04
#define Velocity_Position_Mode              0x05
#define Current_Velocity_Mode               0x06
#define Current_Position_Mode               0x07
#define Current_Velocity_Position_Mode      0x08

void CAN1_Configuration(void);

void CAN_RoboModule_DRV_Reset(unsigned char Group,unsigned char Number);
void CAN_RoboModule_DRV_Mode_Choice(unsigned char Group,unsigned char Number,unsigned char Mode);
void CAN_RoboModule_DRV_OpenLoop_Mode(unsigned char Group,unsigned char Number,short Temp_PWM);
void CAN_RoboModule_DRV_Current_Mode(unsigned char Group,unsigned char Number,short Temp_PWM,short Temp_Current);
void CAN_RoboModule_DRV_Velocity_Mode(unsigned char Group,unsigned char Number,short Temp_PWM,short Temp_Velocity);
void CAN_RoboModule_DRV_Position_Mode(unsigned char Group,unsigned char Number,short Temp_PWM,long Temp_Position);
void CAN_RoboModule_DRV_Velocity_Position_Mode(unsigned char Group,unsigned char Number,short Temp_PWM,short Temp_Velocity,long Temp_Position);
void CAN_RoboModule_DRV_Current_Velocity_Mode(unsigned char Group,unsigned char Number,short Temp_Current,short Temp_Velocity);
void CAN_RoboModule_DRV_Current_Position_Mode(unsigned char Group,unsigned char Number,short Temp_Current,long Temp_Position);
void CAN_RoboModule_DRV_Current_Velocity_Position_Mode(unsigned char Group,unsigned char Number,short Temp_Current,short Temp_Velocity,long Temp_Position);

void CAN_RoboModule_DRV_Config(unsigned char Group,unsigned char Number,unsigned char Temp_Time,unsigned char Ctl1_Ctl2);
void CAN_RoboModule_DRV_Online_Check(unsigned char Group,unsigned char Number);

extern short Real_Current_Value[4];
extern short Real_Velocity_Value[4];
extern long Real_Position_Value[4];
extern char Real_Online[4];
extern char Real_Ctl1_Value[4];
extern char Real_Ctl2_Value[4];

/*3508发送函数*/
void CAN_M3508_Current(unsigned short ID, short Temp_Current_1, short Temp_Current_2, short Temp_Current_3, short Temp_Current_4);

/*CAN发送接收ID*/
typedef enum
{

	CAN_TxPY12V_ID 	= 0x200,		//1-4组3508电机 ID1~4
	CAN_TxPY24V_ID	= 0x1FF,		//5-8组3508电机 ID5~8
	
	CAN_3510Moto_ALL_ID = 0x200,
	CAN_3510Moto1_ID = 0x201,
	CAN_3510Moto2_ID = 0x202,
	CAN_3510Moto3_ID = 0x203,
	CAN_3510Moto4_ID = 0x204,
	
}CAN_Message_ID;

#define FILTER_BUF_LEN		5
/*电机接收参数结构体*/
typedef struct{
		int16_t	 	speed_rpm;
		int16_t  	real_current;
		int16_t  	given_current;
		uint8_t  	hall;
		uint16_t 	angle;				//abs angle range:[0,8191]
		uint16_t 	last_angle;	//abs angle range:[0,8191]
		uint16_t	offset_angle;
		int32_t		round_cnt;
		int32_t		total_angle;
		u8				buf_idx;
		u16				angle_buf[FILTER_BUF_LEN];
		u16				fited_angle;
		u32				msg_cnt;
}moto_measure_t;


/* Extern  ------------------------------------------------------------------*/
extern moto_measure_t  moto_chassis[];
extern moto_measure_t  moto_yaw,moto_pit,moto_poke,moto_info;
extern float real_current_from_judgesys; //unit :mA
extern float dynamic_limit_current;	//unit :mA,;	//from judge_sys
extern float ZGyroModuleAngle,yaw_zgyro_angle;















#endif









