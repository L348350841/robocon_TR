/***********************************************************************************************************
 *  @file      control.c
 *  @version   1.0
 *  @date      2020.9.9
 *  @author    xb
 *  @brief     ���еĵ�����ƺ���
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
 * @brief   ���̵�� RE40 ����
 * @version 1.0
 * @date    2020.8.15
 * @author  xb
 * @param		none
 * @retval  none
 *************************************************************************************/
void Motor_Control()
{
#ifdef ReceiveDataFromNRF
		if((rcData[9] == 1) || (rcData[12] == 14))				//��ͣ
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
			
			///��ʱ��������һ������������ŷ���	
			TD1=0;
			TD2=0;
			TD3=0;
			
		//״̬��
			times=0;
			times2=0;
			times3=0;
			
			
			shejian = 0;
			shoujian = 0;
			sheshoujian = 0;

		}
#else 
		
		if(SE == 1 || SE == 2 || SA == 0) 				//��ͣ
		{
			//void CAN_RoboModule_DRV_Current_Velocity_Mode(unsigned char Group,unsigned char Number,short Temp_Current,short Temp_Velocity)
			CAN_RoboModule_DRV_Current_Velocity_Mode(0, 1, 1500, 0);
			CAN_RoboModule_DRV_Current_Velocity_Mode(0, 2, 1500, 0);
			CAN_RoboModule_DRV_Current_Velocity_Mode(0, 3, 1500, 0);
			CAN_RoboModule_DRV_Current_Velocity_Mode(0, 4, 1500, 0);
		}
#endif		
		else
		{                                               //����
			Speed_Cal();//�����ٶ�
			
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
 * @brief   ���̵���ٶȼ���
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
		{   //�ֶ�����
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
			Motor_Task();			//���Զ�ģʽ
		}
#else	
		if (SF == 1)
		{   //�ֶ�����
				Get_Manual_Speed();
		
		else
		{  								
				Motor_Task();			//���Զ�ģʽ
		}
#endif		
		/************���ڽǶȿ��Ƶ������� ��Ҫ���⴦��************/
		AngleErr=TargetAngle-Z_Angle;
		if(AngleErr>=180)//�Ƕ�����180�Ƚ��紦
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
 * @brief   �ֶ�ң�ص���ٶȻ�ȡ
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
		/* ��ң�����ֶ�ģʽ���� */
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
		
		//�޷�
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
		
		//����
#ifdef ReceiveDataFromNRF
		if(( rcData[ROLL]     >= ROLL_deadzone_lowerlimit     && rcData[ROLL]     < ROLL_deadzone_upperlimit)
			&&(rcData[THROTTLE] >  THROTTLE_deadzone_lowerlimit && rcData[THROTTLE] < THROTTLE_deadzone_upperlimit)
			&&(rcData[YAW]      >  YAW_deadzone_lowerlimit      && rcData[YAW]      < YAW_deadzone_upperlimit))
		{
				Vx=0;
				Vy=0;
				//�Լ��ӵĴ���
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
 * @brief     ��������
 * @param[in] V:		Ŀ���ٶ�
 * @param[in]	err: 	Ŀ���ٶ�ƫ��
 * @retval    �������ٶ�
 * @Sample    usage:Vx=ABS_lim(Vx, x_err);
 * @author    Mebius
 *************************************************************************************************/
void ABS(s32 *V, int32_t err)
{
	if (err>0)//�������˶�
	{
		*V=*V+15;//�𽥼���  6
	}
	else//�������˶�
	{
		*V=*V-15;
	}
}
/*************************************************************************************************
 * @brief    �����ٶ����޷�
 * @param[in] V:				Ŀ���ٶ�
 * @param[in]	Kp:  			����ϵ��
 * @param[in] Kd: 			΢��ϵ��
 * @param[in]	dir: 			����0Ϊx����1Ϊy����
 * @param[in]	err_last: ��һ��ƫ��ֵ
 * @param[in]	Vm:  			�޷���С
 * @param[in]	value:		���򻬵��ٽ�ֵ
 * @retval    �������ٶ�
 * @see       none
 * @Sample    usage:Vx=speed_cal_lim(Vx, posPID[P], posPID[D], x_err, x_err_last, 200, 1000);
 * @author    Mebius
 *************************************************************************************************/
void Speed_Cal_Lim(s32 *V, u8 dir, float get, float set, s32 Vm, u16 value)
{
		s32 err=set-get;
		if (abs(err)>value){//ƫ������ٽ�ֵ���ٶȻ�������
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
		else{//ƫ���С  ͨ��pid����
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
		
		if		 (*V>Vm)   *V=Vm;		//����ٶ��޷�
		else if(*V<-Vm)  *V=-Vm;
}



/*************************************************************************************************
 * @brief    �����ٶ����޷�
 * @param[in] V:				Ŀ���ٶ�
 * @param[in]	Kp:  			����ϵ��
 * @param[in] Kd: 			΢��ϵ��
 * @param[in]	dir: 			����0Ϊx����1Ϊy����
 * @param[in]	err_last: ��һ��ƫ��ֵ
 * @param[in]	Vm:  			�޷���С
 * @param[in]	value:		���򻬵��ٽ�ֵ
 * @retval    �������ٶ�
 * @see       none
 * @Sample    usage:Vx=speed_cal_lim(Vx, posPID[P], posPID[D], x_err, x_err_last, 200, 1000);
 * @author    Mebius
 *************************************************************************************************/
void SpeedJ_Cal_Lim(s32 *V, u8 dir, float get, float set, s32 Vm, u16 value)
{
		s32 err=(set-get)/10;//ת��Ϊ����Ϊ��λ
		if (abs(err)>value){//ƫ������ٽ�ֵ���ٶȻ�������
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
		else{//ƫ���С  ͨ��pid����
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
		
		if		 (*V>Vm)   *V=Vm;		//����ٶ��޷�
		else if(*V<-Vm)  *V=-Vm;
}
/*************************************************************************************************
 * @brief   ��ȡĳһ�����ϼ��⴫��������
 * @version 1.0
 * @date    March 2019
 * @param		dir��	X_LEfTDIR	��X����༤��
									X_RIGHTDIR��X���Ҳ༤��
									Y_LEFTDIR	��Y����༤��
									Y_RIGHTDIR��Y���Ҳ༤��
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
				x_LeftDis[4]=getadc(ch0); 	//��ȡADC��ֵ
			
				X_LeftDis=(x_LeftDis[0]+x_LeftDis[1]+x_LeftDis[2]+x_LeftDis[3]+x_LeftDis[4])/5;		//�����˲�
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
				y_LeftDis[4]=getadc(ch2); //2�����õ�2ͨ��
			
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

