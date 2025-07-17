#ifndef _TASK_H
#define _TASK_H
#include "public.h"

#define X_LASER_RIGHT_DISTANCE 150
#define Y_LASER_LEFT_DISTANCE 150

enum{				//ÈÎÎñ×´Ì¬»ú
	READY,
	START,
	READY2,
	READY3,
	READY4,
	READY5,
	READY6,
	READY7,
	READY8,
	READY9,
	READY10,
	READY11,
	RESTART,
	READY13,
};


void Motor_Task(void);
void Start_lian(void);


void MissionReady(void);
void MissionStart(void);
void MissionStart2(void);
void MissionStart3(void);
void MissionStart4(void);
void MissionStart5(void);
void MissionStart6(void);
void MissionStart7(void);
void MissionStart8(void);
void MissionStart9(void);
void MissionStart10(void);
void MissionStart11(void);
void MissionStart13(void);
//void Shoot_Drive(pid_t *pid1,pid_t *pid2,s32 expect_angle);

void MissionReset(void);
//void Close_Loop(float dj_angle1,float dj_angle2,float dj_angle3);
void Close_Loop(float dj_angle1,float dj_angle2,float dj_angle3, u8 index);
short Motor_M3508_Con(float expect_angle, u8 index);
//short Motor_M2006_Con(float expect_angle, u8 index);
short Motor_M2006_Con(float expect_angle, u8 index_1, u8 index_2);
void Back();
void Index1_Shoot();
void Index2_Shoot();
#endif

