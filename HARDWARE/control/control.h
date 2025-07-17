#ifndef _CONTROL_H
#define _CONTROL_H
#include "sys.h"
/*****************************************************枚举变量声明***********************************************/
//激光定位方向
enum
{
	X_LEfTDIR,
	X_RIGHTDIR,
	Y_LEFTDIR,
	Y_RIGHTDIR,
};

enum
{
	X_DIR,
	Y_DIR,
	Z_DIR,	
};
/*****************************************************函数声明***********************************************/
void Motor_Control(void);
void Speed_Cal(void);
void ABS(s32 *V, int32_t err);
void Speed_Cal_Lim(s32 *V, u8 dir, float get, float set, s32 Vm, u16 value);

void SpeedJ_Cal_Lim(s32 *V, u8 dir, float get, float set, s32 Vm, u16 value);
void Get_Manual_Speed(void);
//void Get_All_Distance(void);
void Get_Distance(u8);
#endif



