#include "task.h"
#include "AllPara.h"

/*********************************3508  motor_drive*****************************/
void Shoot_Drive(pid_t *pid1,pid_t *pid2,s32 expect_angle)
{
		set_angle = 8191.0*(3591.0 / 187.0)/360.0*expect_angle;	//78646		
		set_V = pid_calc(pid1, moto_chassis[3].total_angle, set_angle);
		Ie    = pid_calc(pid2, moto_chassis[3].speed_rpm  , set_V);
		CAN_M3508_Current(0x200, 0,0,0,Ie);
}
//void Pick_Drive


void Motor_Task(){
	static char MissionState = READY;
#ifdef ReceiveDataFromNRF
		if(rcData[KEY]==0)	MissionState=READY;
		else if(rcData[KEY]==1)								MissionState=START;
		else if(rcData[KEY]==2)								MissionState=READY2;
		else if(rcData[KEY]==3)								MissionState=READY3;
		else if(rcData[KEY]==4)								MissionState=READY4;
	                        ///////��ѹ///////
		else if(rcData[KEY]==5)								MissionState=READY5;
	  else if(rcData[KEY]==6)								MissionState=READY6;
	  else if(rcData[KEY]==7)								MissionState=READY7;
							////////��̨////////
		else if(rcData[KEY]==8)								MissionState=READY8;
		else if(rcData[KEY]==9)								MissionState=READY9;
		else if(rcData[KEY]==10)							MissionState=READY10;
		else if(rcData[KEY]==11)							MissionState=READY11;
		else if(rcData[KEY]==12)							MissionState=RESTART;
		else if(rcData[KEY]==13)							MissionState=READY13;
#else	
	switch(SA)                 //��ʱ�޸İ���
	{
		case 1:
			//MissionState=READY;
			MissionState=START;
//			MissionState=READY2;
			break;
		case 3:
			MissionState=READY2;
//			MissionState=START;
			break;
		default :
			break;
		
	}	
	switch (SB)
	{
		case 1:
			MissionState=READY3;
			break;
		case 3:
			MissionState=RESTART;
			break;
		default :
			
			break;
	}
	switch (SC)
	{
		case 1:
			MissionState=READY4;
			break;
		case 3:
			MissionState=READY5;
			break;
		default :
			
			break;
	}
	
	switch (SD)
	{
		case 1:
			MissionState=READY6;
			break;
		case 3:
			MissionState=READY7;
			break;
		default :
			
			break;
	}
#endif	
	
	switch(MissionState)
	{
		case READY:	MissionReady();break;								
		case READY2:MissionStart2();break;
		case START : MissionStart();break;
		case READY3 : MissionStart3();break;
		case READY4 : MissionStart4();break;
		case READY5 : MissionStart5();break;
		case READY6 : MissionStart6();break;
		case READY7 : MissionStart7();break;
		case READY8 : MissionStart8();break;
		case READY9 : MissionStart9();break;
		case READY10 : MissionStart10();break;
		case READY11 : MissionStart11();break;
		case RESTART : MissionReset();break;
		case READY13 : MissionStart13();break;
	}
}

void MissionReady()
{
	Close_Loop(0,0,0,1);//�����ޱ�Ҫ��
	
	X_SetPos=0;
	Y_SetPos=0;

	Pos_Error=X_SetPos-X_ActPos;

	posx_PID.p=0.6;  
	posx_PID.i=0;  
	posx_PID.d=0;		
	
	posy_PID.p=0.6;  	
	posy_PID.i=0;  
	posy_PID.d=0;
	
	if(Pos_Error<100){
		Speed_Cal_Lim(&Vx, X_DIR, X_ActPos, X_SetPos, 400, 1200);
		Speed_Cal_Lim(&Vy, Y_DIR, Y_ActPos, Y_SetPos, 400, 1200);
		//Speed_Cal_Lim(&Vz, Z_DIR, Z_Angle, -3, 200, 130);
	}
	else{
		SpeedJ_Cal_Lim(&Vx, X_DIR, X_AveDis,X_Set_JGPosL, 400, 1200);
		SpeedJ_Cal_Lim(&Vy, Y_DIR, Y_AveDis,Y_Set_JGPosL, 400, 1200);
	}
}
/*************************************************************************************
 * @brief   ����2---����PID����
 * @version 1.0
 * @date    2020.7.18
 * @author  
 * @param		none
 * @retval  none
 *************************************************************************************/
void MissionStart()
{  
/*
	X_SetPos=-9500;//-2200  1200(�����Ĳ���)
	Y_SetPos=880;//900
	TargetAngle=0;//0.3

	
	posy_PID.p=0.2;  //0.6	0.3
	posy_PID.i=0;
	posy_PID.d=0;
	
	angle_PID.p = 15;//19
	angle_PID.i = 0;
	angle_PID.d = 0;
	
	if (X_ActPos > -8000)
	{
		posx_PID.p=0.7;
		posx_PID.i=0.3;
		posx_PID.d=1;
		Speed_Cal_Lim(&Vx, X_DIR, X_ActPos, X_SetPos, 400, 8000);//500

	}
	else
	{
		posx_PID.p=0.7;
		posx_PID.i=0.1;
		posx_PID.d=0;
		Speed_Cal_Lim(&Vx, X_DIR, X_ActPos, X_SetPos, 100, 1200);//500
		}
	Speed_Cal_Lim(&Vy, Y_DIR, Y_ActPos, Y_SetPos, 100,  1500);
	Speed_Cal_Lim(&Vz, Z_DIR, Z_Angle, TargetAngle, 200, 130);
	
	times++;
		if (times < 200)///
	{
		Close_Loop(8000,0,0,1);//7800
		
	}
	else if (times > 200 && times <= 400)
	{
		Close_Loop(8000,0,0,1);//7800
		GPIO_SetBits(GPIOF,GPIO_Pin_0);	//���ӿ�	
	}
	else if (times > 400 && times <= 1200)
	{
		Close_Loop(8000,0,0,1);//7800
		GPIO_SetBits(GPIOF,GPIO_Pin_0);	//���ӿ�	
		GPIO_SetBits(GPIOF,GPIO_Pin_1);	
	}
		*/
		
		Close_Loop(5000,0,0,1);//8000
		GPIO_SetBits(GPIOF,GPIO_Pin_0);	//���ӿ�	
		GPIO_SetBits(GPIOF,GPIO_Pin_1);	

}


/*************************************************************************************
 * @brief   ����2---ȡ��
 * @version 1.0
 * @date    2020.7.18
 * @author  
 * @param		none
 * @retval  none
 *************************************************************************************/
void MissionStart2()
{	
	
	//GPIO_ResetBits(GPIOF,GPIO_Pin_0);	//�պ�
	
	
	
	
	
	times2++;
	if (times2 < 200)///
	{
		GPIO_ResetBits(GPIOF,GPIO_Pin_0);	//�պ�
		Back();
	}
	else if (times2 > 200 && times2 <= 800)
	{
		Vx=Vy=Vz=0;
		GPIO_ResetBits(GPIOF,GPIO_Pin_1);		//�ջ�
		Close_Loop(0,0,0,1);
	}
	
	
	if(times2>1000)
	{
		times2=900;
	}
	
	
	
//	else if (times2 > 500 && times2 <= 1200)
//	{
//		//Vx=Vy=Vz=0;
//		Close_Loop(2500,0,0,1);
//		//GPIO_SetBits(GPIOF,GPIO_Pin_0);
//	}
//	else if (times2 > 1200 && times2 <= 1400)
//	{
//		GPIO_SetBits(GPIOF,GPIO_Pin_0);	//���ӿ�	
//	}
//	else if (times2 > 1400 && times2 <= 2500)
//	{
//		//Vx=Vy=Vz=0;//
//		//GPIO_ResetBits(GPIOF,GPIO_Pin_1);		//�ؼ���
//		Close_Loop(2500,0,0,1);//7800
//		GPIO_SetBits(GPIOF,GPIO_Pin_0);	//���ӿ�	
//		GPIO_SetBits(GPIOF,GPIO_Pin_1);	
//	}
	
	
	
		
}


/*************************************************************************************
 * @brief   ȡ�������ף�PF0���ӿ��ϣ�PF1���ӷ�ת   
 * @version 1.0   ���---3000
 * @date    2020.7.18
 * @author  
 * @param		none
 * @retval  none
 *************************************************************************************/

void MissionStart3()
{
	
	Close_Loop(0,0,0,1);//720+180+200=1100
	GPIO_SetBits(GPIOF,GPIO_Pin_0);	//���ӿ�	
	GPIO_ResetBits(GPIOF,GPIO_Pin_1);
	
}
/*************************************************************************************
 * @brief   ����4---����ѹ��   �������λ��  ����������
 * @version 1.0
 * @date    2020.7.18
 * @author  lg
 * @param		none
 * @retval  none
 *************************************************************************************/
void MissionStart4()
{
	Close_Loop(3000,0,0,1);//720+180+200=1100
	GPIO_ResetBits(GPIOF,GPIO_Pin_0);	//���ӿ�	
	GPIO_ResetBits(GPIOF,GPIO_Pin_1);
	
}
/*************************************************************************************
 * @brief   ����5---���Դ��� ---����--�����ͼ�  
 * @version 1.0
 * @date    2020.7.18
 * @author  lg
 * @param		none
 * @retval  none
 *************************************************************************************/

void MissionStart5()
{
	
	Close_Loop(-5000,0,0,1);
	GPIO_ResetBits(GPIOF,GPIO_Pin_0);
	GPIO_ResetBits(GPIOF,GPIO_Pin_1);
}

/*************************************************************************************
 * @brief   ����6---���
 * @version 1.0
 * @date    2020.7.18
 * @author  lg
 * @param		none
 * @retval  none
 *************************************************************************************/

void MissionStart6()
{
	
	times3++;
	if (times3 < 100)///
	{
		GPIO_SetBits(GPIOF,GPIO_Pin_2);	//�պ�
	}
	else if (times3 > 100 && times3 <= 800)
	{
		GPIO_ResetBits(GPIOF,GPIO_Pin_2);		//�ջ�
	}
	if(times3>1000)
	{
		times3=900;
	}
	
	
	
	
	////////////////////////////////
//	Index1_Shoot();
//	shejian++;
//	if (shejian == 2)
//	{
//		Locate_Rle(2450-1,Shoot_Fre, CW);//1��Ͳ   18181  4680
//		shoujian = 0;
//	}
//	if (shejian == 100)
//	{
//		Locate_Rle(2450-1, 8000, CCW);//2450
//	}
//	if(shejian>500)
//	{
//		shejian = 101;
//	}
//	Vx = Vy = Vz = 0;	//���ڵ��������
//	
}

/*************************************************************************************
 * @brief   ����7---ȡ�������ջ�  ��Ŷ�����
 * @version 1.0
 * @date    2020.7.18
 * @author  lg
 * @param		none
 * @retval  none
 *************************************************************************************/
void MissionStart7()
{
	
	Vx = Vy = Vz = 0;
	//times3++;
	
	//Close_Loop(3000,1080,0,1);//720+180+200=1100
	Close_Loop(0,1080,0,1);
	GPIO_ResetBits(GPIOF,GPIO_Pin_0);	//���ӿ�	
	GPIO_ResetBits(GPIOF,GPIO_Pin_1);	
	
//	Close_Loop(0,0,0,2);
//	GPIO_ResetBits(GPIOF,GPIO_Pin_0);	//���ӿ�	
//	GPIO_ResetBits(GPIOF,GPIO_Pin_1);
	
}
/*************************************************************************************
 * @brief   ����8---�����
 * @version 1.0
 * @date    2021.7.18
 * @author  lg
 * @param		none
 * @retval  none
 *************************************************************************************/
void MissionStart8()//2006����Ƕ�
{
	
	
	Vx = Vy = Vz = 0;
	
	//Close_Loop(3000,2160,0,1);//720+180+200=1100
	Close_Loop(0,2160,0,1);
	GPIO_ResetBits(GPIOF,GPIO_Pin_0);	//���ӿ�	
	GPIO_ResetBits(GPIOF,GPIO_Pin_1);	
	
	
//	Close_Loop(0,3633,0,1);//720+180+200=1100
//	GPIO_ResetBits(GPIOF,GPIO_Pin_0);	//���ӿ�	
//	GPIO_ResetBits(GPIOF,GPIO_Pin_1);
	


}
/*************************************************************************************
 * @brief   ����9---ԭ��ת�Ƕȣ����
 * @version 1.0
 * @date    2020.7.18
 * @author  lg
 * @param		none
 * @retval  none
 *************************************************************************************/

void MissionStart9()
{
	Vx = Vy = Vz = 0;
	//Close_Loop(3000,3240,0,1);
	Close_Loop(0,3240,0,1);//720+180+200=1100
	GPIO_ResetBits(GPIOF,GPIO_Pin_0);	//���ӿ�	
	GPIO_ResetBits(GPIOF,GPIO_Pin_1);	
	
}

void MissionStart10()
{
	Vx = Vy = Vz = 0;
	//Close_Loop(3000,0,0,1);
	Close_Loop(0,0,0,1);//720+180+200=1100
	GPIO_ResetBits(GPIOF,GPIO_Pin_0);	//���ӿ�	
	GPIO_ResetBits(GPIOF,GPIO_Pin_1);
}




void MissionStart11()
{
		shejian++;
	if (shejian == 2)
	{
		Locate_Rle(2450-1,20800, CW);//1��Ͳ 6480mm  21000
		shoujian = 0;
	}
	if (shejian == 100)
	{
		Locate_Rle(2450-1, 8000, CCW);//2450
	}
	if(shejian>500)
	{
		shejian = 101;
	}
	Vx = Vy = Vz = 0;	//���ڵ��������
}

void MissionStart13()
{
		shejian++;
	if (shejian == 2)
	{
		Locate_Rle(2450-1,19000, CW);//1��Ͳ 6480mm
		shoujian = 0;
	}
	if (shejian == 100)
	{
		Locate_Rle(2450-1, 8000, CCW);//2450
	}
	if(shejian>500)
	{
		shejian = 101;
	}
	Vx = Vy = Vz = 0;	//���ڵ��������
}
/*************************************************************************************
 * @brief   ����---�ص�ԭ��_�о�ǽ��һ���ľ���
 * @version 1.0
 * @date    2020.7.18
 * @author  lg
 * @param		none
 * @retval  none
 *************************************************************************************/
void MissionReset()
{
	//X_SetPos=-20;//-30--100
	//Y_SetPos=30;
	Y_Set_JGPosR=17;
	Y_Set_JGPosL=22;//5
	TargetAngle=0;//0.3
	
	posx_PID.p=0.6;	//0.5
	posx_PID.i=0.1;//	0	0.05,�ͻ��һ���ٻ�������ʱҲ��ƫһ��
	posx_PID.d=0;	//0.35
	
	posy_PID.p=0.7;//	0.5	0.45
	posy_PID.i=0.1;
	posy_PID.d=0;//	0.3	0.03
	
	angle_PID.p = 19;
	angle_PID.i = 0;
	angle_PID.d = 0;

	
//	if(X_ActPos<-1000)
//	{
//	Speed_Cal_Lim(&Vx, X_DIR, X_ActPos, X_SetPos, 200, 1200);//400
//	}
//	else
//	{
//		Speed_Cal_Lim(&Vx, X_DIR, X_ActPos, X_SetPos, 100, 1200);//400
//	}
//	Speed_Cal_Lim(&Vy, Y_DIR, Y_ActPos, Y_SetPos, 100,  1200);

	Speed_Cal_Lim(&Vx, X_DIR, Y_RightDis, Y_Set_JGPosR, 400,  10000);
//	if (Y_LeftDis > 100)
//	{
		Speed_Cal_Lim(&Vy, Y_DIR, Y_LeftDis, Y_Set_JGPosL, 100,  1200);
//	}
//	else
//	{
//		Speed_Cal_Lim(&Vy, Y_DIR, Y_LeftDis, Y_Set_JGPosL, 100,  1200);
//	}
	Vx = -Vx;
	Speed_Cal_Lim(&Vz, Z_DIR, Z_Angle, TargetAngle, 200, 130);	
	
	
}
/*************************************************************************************
 * @brief   �Զ��庯��---2006�ջ�����
 * @version 1.0
 * @date    2020.7.18
 * @author  lg
 * @param		none
 * @retval  none
 *************************************************************************************/

void Close_Loop(float dj_angle1,float dj_angle2,float dj_angle3, u8 index)//1--ƽ�ƣ�2---�����3--��ת
{
	short Ie1, Ie2, Ie3;
	Ie1 = Motor_M2006_Con(dj_angle1, 1,index);
	Ie2 = Motor_M2006_Con(dj_angle2, 2,3);
	Ie3 = Motor_M2006_Con(dj_angle3, 3,index);
	CAN_M3508_Current(0x200, Ie1,Ie2,Ie3,0);
}

/*index---1:��ת2006_pid1
					2:��ת2006_pid2					
										*/
/*************************************************************************************
 * @brief   �Զ��庯��---2006�ǶȻ����ٶȻ�
 * @version 1.0
 * @date    2020.7.18
 * @author  lg
 * @param		none
 * @retval  index---1:��ת2006_pid1
				 ---2:��ת2006_pid2	
 *************************************************************************************/

short Motor_M2006_Con(float expect_angle, u8 index_1, u8 index_2) //�ļ��ٱȵ�Ϊ2006 2006���ٱ�Լ39/1
{
	s32 set_angle, set_V;
	short Ie;
	
	//ת�ӻ�е�Ƕ�ֵ0~8190��0~360�ȣ�3508������ٱ� 3591/187  
	set_angle = 8191.0*(36.0 / 1.0)/360.0*expect_angle;	//78646		
	//�⻷���ǶȻ�
	if(index_2==1)
		{
			set_V = pid_calc(&angle_PID_2006_1, moto_chassis[index_1 - 1].total_angle, set_angle);
			Ie    = pid_calc(&speed_PID_2006_1, moto_chassis[index_1 - 1].speed_rpm  , set_V);
	
		}
	if(index_2==2)
		{
			set_V = pid_calc(&angle_PID_2006_2, moto_chassis[index_1 - 1].total_angle, set_angle);
			Ie    = pid_calc(&speed_PID_2006_2, moto_chassis[index_1 - 1].speed_rpm  , set_V);
		}	
	if(index_2==3)
		{
			set_V = pid_calc(&angle_PID_2006_3, moto_chassis[index_1 - 1].total_angle, set_angle);
			Ie    = pid_calc(&speed_PID_2006_3, moto_chassis[index_1 - 1].speed_rpm  , set_V);
		}	
	//�ڻ����ٶȻ�
	
	return Ie;
}
/*************************************************************************************
 * @brief   �Զ��庯��---���ռ�ס����������
 * @version 1.0
 * @date    2020.7.18
 * @author  lg
 * @param		none
 * @retval  index---1:��ת2006_pid1
				 ---2:��ת2006_pid2	
 *************************************************************************************/
void Back()
{
//	X_SetPos=-1000;//-2200  1200(�����Ĳ���)
//	Y_SetPos=1200;//900
//	TargetAngle=0;//0.3   XR
	
	X_Set_JGPosR=15;
	Y_Set_JGPosL=320;//5

	posx_PID.p=0.9;  //0.5
	posx_PID.i=0;  
	posx_PID.d=5;		
//	
	posy_PID.p=0.2;  //0.6	0.3
	posy_PID.i=0;
	posy_PID.d=0;
	
//	angle_PID.p = 19;
//	angle_PID.i = 0;
//	angle_PID.d = 0;
	
//	if (X_ActPos > -8000)
//	{
//		posx_PID.p=0.7;
//		posx_PID.i=0.3;
//		posx_PID.d=10;
//		Speed_Cal_Lim(&Vx, X_DIR, X_ActPos, X_SetPos, 500, 8000);//400

//	}
//	else
//	{
//		posx_PID.p=0.4;
//		posx_PID.i=0.08;
//		posx_PID.d=0.1;
//		Speed_Cal_Lim(&Vx, X_DIR, X_ActPos, X_SetPos, 200, 1200);//400

//	}
	Speed_Cal_Lim(&Vx, X_DIR, X_RightDis, X_Set_JGPosR, 200,  1200);
	
	Vx=Vx*10;
	Speed_Cal_Lim(&Vy, Y_DIR, Y_LeftDis, Y_Set_JGPosL, 200,  1200);
	//Speed_Cal_Lim(&Vz, Z_DIR, Z_Angle, TargetAngle, 200, 130);
	
}

/*************************************************************************************
 * @brief   �Զ��庯��---������ݾ���ȷ���������
 * @version 1.0
 * @date    2020.7.18
 * @author  lg
 * @param		none
 * @retval  index---1:��ת2006_pid1
				 ---2:��ת2006_pid2	
 *************************************************************************************/
void Index2_Shoot()
{
	if(juli > 4588 && juli <= 4596)
	{
		Shoot_Fre = 17555;
	}
	if (juli > 4600 && juli <= 4700)
	{
		Shoot_Fre = 17870;
	}
	if (juli > 4700 && juli <= 4800)
	{
//		Shoot_Fre = 17545;//��������
	}
	if (juli > 4800 && juli <= 4900)
	{
		Shoot_Fre = 18359;
	}
	if (juli > 4900 && juli <= 5000)
	{
//		Shoot_Fre = 18359;//��������
	}
	if (juli > 5000 && juli <= 5100)
	{
		
	}
	if (juli > 5100 && juli <= 5200)
	{
		
	}
	if (juli > 5200 && juli <= 5300)
	{
		Shoot_Fre = 18558;
	}
	if (juli > 5300 && juli <= 5400)
	{
		
	}
	if (juli > 5000 && juli <= 5100)
	{
		
	}
	//Զ�Ķ���Ͱ
	if (juli > 6650 && juli <= 6750)
	{
		Shoot_Fre = 21300;
	}
	if (juli > 6750 && juli <= 6850)
	{
		//��������
	}
	if (juli > 6850 && juli <= 6950)
	{
//		Shoot_Fre = 21300;
	}
	if (juli > 6950 && juli <= 7050)
	{
		Shoot_Fre = 21750;
	}
	if (juli > 7050 && juli <= 7150)
	{
//		Shoot_Fre = 21300;
	}
	//Shoot_Fre=
}

void Index1_Shoot()
{
	//1��Ͳ
	if (juli > 6000 && juli <= 6100)
	{
		Shoot_Fre = 20420;
	}
	if (juli > 6100 && juli <= 6200)
	{
		Shoot_Fre = 20430;
	}
	
}
