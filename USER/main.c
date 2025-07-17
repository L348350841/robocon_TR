#include "public.h"

int main(void)
{ 
		
		Bsp_Init();			//所有参数初始化
		
		LCD_ShowString(10,20,90,24,24,"Vx:");
		LCD_ShowString(10,50,90,24,24,"Vy:");
		LCD_ShowString(10,80,90,24,24,"Vz:");
		
		LCD_ShowString(10,120,90,24,24,"X_Pos:");
		LCD_ShowString(10,150,90,24,24,"Y_Pos:");
		LCD_ShowString(10,180,90,24,24,"Z_Angle:");
		LCD_ShowString(250,180,90,24,24,"TargetAngle:");
		LCD_ShowString(10,210,90,16,16,"X_Angle:");
		LCD_ShowString(10,240,90,16,16,"Y_Angle:");
		LCD_ShowString(10,270,90,16,16,"Z_W:");
		
		LCD_ShowString(10,510,90,24,24,"mode:");
		LCD_ShowString(10,540,90,24,24,"pos_flag:");
		
		LCD_ShowString(10,600,90,24,24,"X_LeftDis:");
		LCD_ShowString(10,630,90,24,24,"X_RightDis:");
		LCD_ShowString(10,660,90,24,24,"Y_LeftDis:");
		LCD_ShowString(10,690,90,24,24,"Y_RightDis:");
		
		LCD_ShowString(10,690 + 24,90,24,24,"sbus:");
		LCD_ShowString(10,690 + 48,90,24,24,"DCF:     s");

		while(1)
		{
			//分辨率800*480
			POINT_COLOR=BLACK;
			BACK_COLOR =WHITE;
			
			RpiLidar_Task();
//			USART_SendData(USART3,0x01);
//			delay_ms(5);
			
			//vx vy vw
			LED_PrintValueF(50,20,Vx,24,0)	;			
			LED_PrintValueF(200,20,AngleErr,24,4)	;
			LED_PrintValueF(50,50,Vy,24,0)	;
			LED_PrintValueF(50,80,Vz,24,0)	;
//		LCD_ShowNum(50,20, LX, 4, 24);
//		LCD_ShowNum(50,50, RY, 4, 24);
//		LCD_ShowNum(50,80, RX, 4, 24);
		
		

			//陀螺仪数据
			//LED_PrintValueF(100, 120, X_Pos,24, 4);
			//LED_PrintValueF(100, 150, Y_Pos,24, 4);
			LED_PrintValueF(100, 120,X_Pos ,24, 4);
			LED_PrintValueF(100, 150, Y_Pos,24, 4);
		
			LED_PrintValueF(100, 180, Z_Angle,24, 4);
			LED_PrintValueF(340, 180, TargetAngle,24, 4);
			LED_PrintValueF(100, 210, X_Angle,24, 4);
			LED_PrintValueF(100, 240, Y_Angle,24, 4);
			LED_PrintValueF(100, 270, Z_W,24, 4);
		
			LED_PrintValueF(100, 310, X_SetPos,24, 4);
			LED_PrintValueF(100, 340, Y_SetPos,24, 4);

			LCD_ShowNum(300,470,rcData[THROTTLE],16,16);
			LCD_ShowNum(300,490,rcData[ROLL],16,16);
			LCD_ShowNum(300,510,rcData[PITCH],16,16);				
			LCD_ShowNum(300,530,rcData[YAW],16,16); 

			LCD_ShowNum(300,550,rcData[12],16,16); 
			
			
			
			LED_PrintValueF(150,600,X_LeftDis,24,0);//激光数据
			LED_PrintValueF(150,630,X_RightDis,24,0);
			LED_PrintValueF(150,660,Y_LeftDis,24,0);
			LED_PrintValueF(150,690,Y_RightDis,24,0);
			
			
			
			
			
			
			
			
			//LCD_ShowNum(300,730,rcData[11],16,16); 
#ifndef ReceiveDataFromNRF
			LCD_ShowNum(100,730 + 16 *1, LX, 4, 16);
			LCD_ShowNum(100 + 16 * 2,730 + 16 *1, RY, 4, 16);
			LCD_ShowNum(100 + 16 * 4,730 + 16 *1, RX, 4, 16);
			LCD_ShowNum(100 + 16 * 6,730 + 16 *1, SA, 4, 16);
#endif	

#ifdef ReceiveDataFromNRF
			if (rcData[11]==3)	LCD_ShowString(110,510,90,24,24,"manl");
			else LCD_ShowString(110,510,90,24,24,"auto");
#else
			if (SF == 1)	LCD_ShowString(110,510,90,24,24,"manl");
			else LCD_ShowString(110,510,90,24,24,"auto");
#endif	
	}
	
}








 
