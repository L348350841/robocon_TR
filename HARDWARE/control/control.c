/***********************************************************************************************************
 *  @file      control.c
 *  @version   1.0
 *  @date      2020.9.9
 *  @author    xb
 *  @brief     所有的电机控制函数
 *  @copyright 2019 SDUST Robocon. All rights reserved.
 ***********************************************************************************************************/
#include "control.h"
#include "task.h"
#include "can1.h"
#include "AllPara.h"
#include "math.h"
#include "ReceiveData.h"
#include "arm_math.h"
#include "lcd.h"
#include "ADS1115.h"
/*************************************************************************************
 * @brief   底盘电机 RE40 控制
 * @version 1.0
 * @date    2020.8.15
 * @author  xb
 * @param		none
 * @retval  none
 *************************************************************************************/
void Motor_Control()
{
#ifdef ReceiveDataFromNRF
		if((rcData[9] == 1) || (rcData[12] == 14))				//急停
		{      
			
//			CAN_RoboModule_DRV_Velocity_Mode(0,1,PWM_LIMIT,0);
//			CAN_RoboModule_DRV_Velocity_Mode(0,2,PWM_LIMIT,0);
//			CAN_RoboModule_DRV_Velocity_Mode(0,3,PWM_LIMIT,0);		
//			CAN_RoboModule_DRV_Velocity_Mode(0,4,PWM_LIMIT,0);
			
			//void CAN_RoboModule_DRV_Current_Velocity_Mode(unsigned char Group,unsigned char Number,short Temp_Current,short Temp_Velocity)
//			CAN_RoboModule_DRV_Current_Velocity_Mode(0, 1, 1500, 0);
//			CAN_RoboModule_DRV_Current_Velocity_Mode(0, 2, 1500, 0);
//			CAN_RoboModule_DRV_Current_Velocity_Mode(0, 3, 1500, 0);
//			CAN_RoboModule_DRV_Current_Velocity_Mode(0, 4, 1500, 0);
			
			CAN_RoboModule_DRV_Current_Velocity_Mode(0, 1, 2000, 0);
			CAN_RoboModule_DRV_Current_Velocity_Mode(0, 2, 2000, 0);
			CAN_RoboModule_DRV_Current_Velocity_Mode(0, 3, 2000, 0);
			CAN_RoboModule_DRV_Current_Velocity_Mode(0, 4, 2000, 0);
			
			///暂时用于运行一次任务（例：电磁阀）	
			TD1=0;
			TD2=0;
			TD3=0;
			
		//状态机
			times=0;
			times2=0;
			times3=0;
			
			
			shejian = 0;
			shoujian = 0;
			sheshoujian = 0;

		}
#else 
		
		if(SE == 1 || SE == 2 || SA == 0) 				//急停
		{
			//void CAN_RoboModule_DRV_Current_Velocity_Mode(unsigned char Group,unsigned char Number,short Temp_Current,short Temp_Velocity)
			CAN_RoboModule_DRV_Current_Velocity_Mode(0, 1, 1500, 0);
			CAN_RoboModule_DRV_Current_Velocity_Mode(0, 2, 1500, 0);
			CAN_RoboModule_DRV_Current_Velocity_Mode(0, 3, 1500, 0);
			CAN_RoboModule_DRV_Current_Velocity_Mode(0, 4, 1500, 0);
		}
#endif		
		else
		{                                               //正常
			Speed_Cal();//计算速度
			
//			CAN_RoboModule_DRV_Velocity_Mode(0,1,PWM_LIMIT,TargetSpeed1);
//			CAN_RoboModule_DRV_Velocity_Mode(0,2,PWM_LIMIT,TargetSpeed2);
//			CAN_RoboModule_DRV_Velocity_Mode(0,3,PWM_LIMIT,TargetSpeed3);
//			CAN_RoboModule_DRV_Velocity_Mode(0,4,PWM_LIMIT,TargetSpeed4);
			
			CAN_RoboModule_DRV_Current_Velocity_Mode(0, 1, 2050, TargetSpeed1);//2050  3200  4000   6000
			CAN_RoboModule_DRV_Current_Velocity_Mode(0, 2, 2150, TargetSpeed2);//2150  3300  4100   6100
			CAN_RoboModule_DRV_Current_Velocity_Mode(0, 3, 2100, TargetSpeed3);//2100	 3300  4100   6100
			CAN_RoboModule_DRV_Current_Velocity_Mode(0, 4, 2200, TargetSpeed4);//2200  3400  4200   6200
			
		}
}

/*************************************************************************************************
 * @brief   底盘电机速度计算
 * @version 1.0
 * @date    March 2019
 * @param		none
 * @retval  none
 * @author  Mebius
 *************************************************************************************************/
void Speed_Cal()
{
#ifdef ReceiveDataFromNRF
		if (rcData[11]==3)
		{   //手动开环
			Get_Manual_Speed();
			if(rcData[KEY]==1)
			{
				MissionStart();
			}
			if(rcData[KEY]==2)
			{
				MissionStart2();
			}
			if(rcData[KEY]==3)
			{
				MissionStart3();
			}
			if(rcData[KEY]==6)
			{
				MissionStart6();
			}
			if(rcData[KEY]==7)
			{
				MissionStart7();
			}
			if(rcData[KEY]==8)
			{
				MissionStart8();
			}
			if(rcData[KEY]==9)
			{
				MissionStart9();
			}
			if(rcData[KEY]==10)
			{
				MissionStart10();
			}
			if(rcData[KEY]==11)
			{
				MissionStart11();
			}
			if(rcData[KEY]==13)
			{
				MissionStart13();
			}
			
		}
		else
		{  								
			Motor_Task();			//半自动模式
		}
#else	
		if (SF == 1)
		{   //手动开环
				Get_Manual_Speed();
		
		else
		{  								
				Motor_Task();			//半自动模式
		}
#endif		
		/************由于角度控制的特殊性 需要特殊处理************/
		AngleErr=TargetAngle-Z_Angle;
		if(AngleErr>=180)//角度正负180度交界处
			TargetAngle=TargetAngle-360;
		if(AngleErr<-180)
			TargetAngle=TargetAngle+360;
		
		TargetSpeed1 = -Vx+ Vy + Vz;
		TargetSpeed2 = (-Vx- Vy + Vz)*0.8;
		TargetSpeed3 = (+Vx- Vy + Vz)*0.7;
		TargetSpeed4 = +Vx+ Vy + Vz;
		
		
		
//		TargetSpeed1 = 10 * (-Vx * arm_sin_f32((45 + Z_Angle) / 180.0f * 3.141592f) + Vy * arm_cos_f32((45 + Z_Angle) / 180.0f * 3.141592f) + Vz/2);
//		TargetSpeed2 = 10 * (-Vx * arm_sin_f32((45 - Z_Angle) / 180.0f * 3.141592f) - Vy * arm_cos_f32((45 - Z_Angle) / 180.0f * 3.141592f) + Vz/2);
//		TargetSpeed3 = 10 * (+Vx * arm_sin_f32((45 + Z_Angle) / 180.0f * 3.141592f) - Vy * arm_cos_f32((45 + Z_Angle) / 180.0f * 3.141592f) + Vz/2);
//		TargetSpeed4 = 10 * (+Vx * arm_sin_f32((45 - Z_Angle) / 180.0f * 3.141592f) + Vy * arm_cos_f32((45 - Z_Angle) / 180.0f * 3.141592f) + Vz/2);
}
/*************************************************************************************************
 * @brief   手动遥控电机速度获取
 * @version 2.0
 * @date    2020.9.9
 * @param		none
 * @retval  none
 * @author  xb
 *************************************************************************************************/
void Get_Manual_Speed()
{
#ifdef ReceiveDataFromNRF	//2
		if 			(rcData[ROLL]>ROLL_deadzone_upperlimit) 				Vx=(rcData[ROLL]-ROLL_deadzone_upperlimit)*15;
		else if (rcData[ROLL]<ROLL_deadzone_lowerlimit) 				Vx=(rcData[ROLL]-ROLL_deadzone_lowerlimit)*15;
		else	  Vx=0;
		
		if 			(rcData[THROTTLE]>THROTTLE_deadzone_upperlimit) Vy=(rcData[THROTTLE]-THROTTLE_deadzone_upperlimit)*15;
		else if (rcData[THROTTLE]<THROTTLE_deadzone_lowerlimit) Vy=(rcData[THROTTLE]-THROTTLE_deadzone_lowerlimit)*15  ;
		else 		Vy=0;
		
		if (rcData[YAW]>YAW_deadzone_upperlimit)
		{
			Vz = -(rcData[YAW]-YAW_deadzone_upperlimit);
		}
			
		else if (rcData[YAW]<YAW_deadzone_lowerlimit)
		{
			Vz = -(rcData[YAW]-YAW_deadzone_lowerlimit);
		}
		else
			Vz=0;
		if 	   (rcData[12] == 3)  Vy=0;  
		else if(rcData[12] == 4)  Vx=0;  
		else if(rcData[12] == 5)	TargetAngle=0;
		
#else
		/* 新遥控器手动模式计算 */
		if (LX > LX_DeadZone_UpperLimit)		Vz = LX - LX_DeadZone_UpperLimit;
		
		else if (LX < LX_DeadZone_LowerLimit)	Vz = LX - LX_DeadZone_LowerLimit;
		else Vz = 0;
		
		if (RX > RX_DeadZone_UpperLimit)		Vx = RX - RX_DeadZone_UpperLimit;
		else if (RX < RX_DeadZone_LowerLimit)	Vx = RX - RX_DeadZone_LowerLimit;
		else Vx = 0;
		
		if (RY > RY_DeadZone_UpperLimit)		Vy = RY - RY_DeadZone_UpperLimit;
		else if (RY < RY_DeadZone_LowerLimit)	Vy = RY - RY_DeadZone_LowerLimit;
		else Vy = 0;
#endif		
		
		//限幅
		if(Vx>8000)Vx=8000;
		if(Vx<-8000)Vx=-8000;
		
		if(Vy>8000)Vy=8000;
		if(Vy<-8000)Vy=-8000;
		
//		if(Vx>1500)Vx=1500;
//		if(Vx<-1500)Vx=-1500;
//		
//		if(Vy>1500)Vy=1500;
//		if(Vy<-1500)Vy=-1500;
		
		if(Vz>400)Vz=400;
		if(Vz<-400)Vz=-400;
		
		//死区
#ifdef ReceiveDataFromNRF
		if(( rcData[ROLL]     >= ROLL_deadzone_lowerlimit     && rcData[ROLL]     < ROLL_deadzone_upperlimit)
			&&(rcData[THROTTLE] >  THROTTLE_deadzone_lowerlimit && rcData[THROTTLE] < THROTTLE_deadzone_upperlimit)
			&&(rcData[YAW]      >  YAW_deadzone_lowerlimit      && rcData[YAW]      < YAW_deadzone_upperlimit))
		{
				Vx=0;
				Vy=0;
				//自己加的代码
				Vz = 0;
		}
#else
		if (LX >= LX_DeadZone_LowerLimit && LX < LX_DeadZone_UpperLimit && 
			RX >= RX_DeadZone_LowerLimit && RX < RX_DeadZone_UpperLimit &&
			RY >= RY_DeadZone_LowerLimit && RY < RY_DeadZone_UpperLimit)
		{
			Vx = 0;
			Vy = 0;
			Vz = 0;
		}
#endif


}
/*************************************************************************************************
 * @brief     启动防打滑
 * @param[in] V:		目标速度
 * @param[in]	err: 	目标速度偏差
 * @retval    处理后的速度
 * @Sample    usage:Vx=ABS_lim(Vx, x_err);
 * @author    Mebius
 *************************************************************************************************/
void ABS(s32 *V, int32_t err)
{
	if (err>0)//正方向运动
	{
		*V=*V+15;//逐渐加速  6
	}
	else//负方向运动
	{
		*V=*V-15;
	}
}
/*************************************************************************************************
 * @brief    计算速度与限幅
 * @param[in] V:				目标速度
 * @param[in]	Kp:  			比例系数
 * @param[in] Kd: 			微分系数
 * @param[in]	dir: 			方向：0为x方向，1为y方向
 * @param[in]	err_last: 上一个偏差值
 * @param[in]	Vm:  			限幅大小
 * @param[in]	value:		防打滑的临界值
 * @retval    处理后的速度
 * @see       none
 * @Sample    usage:Vx=speed_cal_lim(Vx, posPID[P], posPID[D], x_err, x_err_last, 200, 1000);
 * @author    Mebius
 *************************************************************************************************/
void Speed_Cal_Lim(s32 *V, u8 dir, float get, float set, s32 Vm, u16 value)
{
		s32 err=set-get;
		if (abs(err)>value){//偏差大于临界值，速度缓慢增加
				ABS(V, err);
				if(dir==X_DIR) {
						posx_PID.err[LAST]=0;
						posx_PID.err[LLAST]=0;
				}
				else if(dir==Y_DIR){
						posy_PID.err[LAST]=0;
						posy_PID.err[LLAST]=0;
				}
				else{
						angle_PID.err[LAST]=0;
						angle_PID.err[LLAST]=0;
				}
		}
		else{//偏差较小  通过pid调节
				if		 (dir==X_DIR) {
						*V=pid_calc(&posx_PID , get, set);
				}
				else if(dir==Y_DIR){
						*V=pid_calc(&posy_PID , get, set);
				}
				else if(dir==Z_DIR){
						*V=pid_calc(&angle_PID , get, set);
					//*V=pid_calcJG(&angle_PID , get, set);
				}
		}
		
		if		 (*V>Vm)   *V=Vm;		//输出速度限幅
		else if(*V<-Vm)  *V=-Vm;
}



/*************************************************************************************************
 * @brief    计算速度与限幅
 * @param[in] V:				目标速度
 * @param[in]	Kp:  			比例系数
 * @param[in] Kd: 			微分系数
 * @param[in]	dir: 			方向：0为x方向，1为y方向
 * @param[in]	err_last: 上一个偏差值
 * @param[in]	Vm:  			限幅大小
 * @param[in]	value:		防打滑的临界值
 * @retval    处理后的速度
 * @see       none
 * @Sample    usage:Vx=speed_cal_lim(Vx, posPID[P], posPID[D], x_err, x_err_last, 200, 1000);
 * @author    Mebius
 *************************************************************************************************/
void SpeedJ_Cal_Lim(s32 *V, u8 dir, float get, float set, s32 Vm, u16 value)
{
		s32 err=(set-get)/10;//转化为毫米为单位
		if (abs(err)>value){//偏差大于临界值，速度缓慢增加
				ABS(V, err);
				if(dir==X_DIR) {
						posx_PID.err[LAST]=0;
						posx_PID.err[LLAST]=0;
				}
				else if(dir==Y_DIR){
						posy_PID.err[LAST]=0;
						posy_PID.err[LLAST]=0;
				}
				else{
						angle_PID.err[LAST]=0;
						angle_PID.err[LLAST]=0;
				}
		}
		else{//偏差较小  通过pid调节
				if		 (dir==X_DIR) {
						*V=pid_calc(&posx_PID , get, set);
				}
				else if(dir==Y_DIR){
						*V=pid_calc(&posy_PID , get, set);
				}
				else if(dir==Z_DIR){
						*V=pid_calc(&angle_PID , get, set);
				}
		}
		
		if		 (*V>Vm)   *V=Vm;		//输出速度限幅
		else if(*V<-Vm)  *V=-Vm;
}
/*************************************************************************************************
 * @brief   获取某一方向上激光传感器数据
 * @version 1.0
 * @date    March 2019
 * @param		dir：	X_LEfTDIR	：X轴左侧激光
									X_RIGHTDIR：X轴右侧激光
									Y_LEFTDIR	：Y轴左侧激光
									Y_RIGHTDIR：Y轴右侧激光
 * @retval  none
 * @author  Mebius
 *************************************************************************************************/

void Get_Distance(u8 dir)
{
		if(dir==X_LEfTDIR)
		{
				x_LeftDis[0]=x_LeftDis[1];
				x_LeftDis[1]=x_LeftDis[2];
				x_LeftDis[2]=x_LeftDis[3];
				x_LeftDis[3]=x_LeftDis[4];
				x_LeftDis[4]=getadc(ch0); 	//读取ADC的值
			
				X_LeftDis=(x_LeftDis[0]+x_LeftDis[1]+x_LeftDis[2]+x_LeftDis[3]+x_LeftDis[4])/5;		//滑动滤波
				X_LeftDis = X_LeftDis/100;// 
		}
		else if(dir==X_RIGHTDIR)
		{			
				x_RightDis[0]=x_RightDis[1];
				x_RightDis[1]=x_RightDis[2];
				x_RightDis[2]=x_RightDis[3];
				x_RightDis[3]=x_RightDis[4];
				x_RightDis[4]=getadc(ch1); 
		
				X_RightDis=(x_RightDis[0]+x_RightDis[1]+x_RightDis[2]+x_RightDis[3]+x_RightDis[4])/5;
				X_RightDis=X_RightDis/40;//100 
		}
		else if(dir==Y_LEFTDIR)
		{		
				y_LeftDis[0]=y_LeftDis[1];
				y_LeftDis[1]=y_LeftDis[2];
				y_LeftDis[2]=y_LeftDis[3];
				y_LeftDis[3]=y_LeftDis[4];
				y_LeftDis[4]=getadc(ch2); //2本来用的2通道
			
				Y_LeftDis=(y_LeftDis[0]+y_LeftDis[1]+y_LeftDis[2]+y_LeftDis[3]+y_LeftDis[4])/5;
				Y_LeftDis=Y_LeftDis/100;// 104
		}
		else if(dir==Y_RIGHTDIR)
		{		
				y_RightDis[0]=y_RightDis[1];
				y_RightDis[1]=y_RightDis[2];
				y_RightDis[2]=y_RightDis[3];
				y_RightDis[3]=y_RightDis[4];
				y_RightDis[4]=getadc(ch3); 
				Y_RightDis=(y_RightDis[0]+y_RightDis[1]+y_RightDis[2]+y_RightDis[3]+y_RightDis[4])/5;
				Y_RightDis=Y_RightDis/40;// 
		}
}

