#include "timer.h"
#include "public.h"
extern char a;
//定时器7中断服务程序	 
void TIM7_IRQHandler(void)
{ 		    		  			    
		if(TIM7->SR&0X0001)//溢出中断
		{
			#ifdef ReceiveDataFromNRF
				//无线备用
				if(NRF24L01_RxPacket(tmp_buf)==0){//一旦接收到信息,则显示出来.
					ReceiveDataFormNRF();			
					tmp_buf[32]=0;//加入字符串结束符
				}		
			#endif	
				//if(ActInitFlag){								//全场定位标志位：1为初始化完成
					
					//sheshoujian++;
					Motor_Control();							//电机任务
					
//					printf("X_ActPos=%f/r/n", X_ActPos);
//					printf("Y_ActPos=%f/r/n", Y_ActPos);
					if (direction == 0)								//获得激光传感器数值
					{
						Get_Distance(X_RIGHTDIR);
						direction = 1;
					}
					else if (direction == 1)
					{
						Get_Distance(Y_LEFTDIR);
						direction = 2;
					}
					else if (direction == 2)
					{
						Get_Distance(X_LEfTDIR);
						direction = 3;
					}
					else if (direction == 3)
					{
						Get_Distance(Y_RIGHTDIR);
						direction = 0;
					}
			/////////////激光Y角度计算公式(0-45)///////
//					if(Y_LeftDis>Y_RightDis)
//					  JGY_Angle=lookup_arctan((float)(Y_LeftDis-Y_RightDis)/38);
//					else
//						JGY_Angle=lookup_arctan((Y_RightDis-Y_LeftDis)/38);
//					
//						/////同一侧两个激光数据取平均值//////////
//						X_AveDis=(X_RightDis+X_LeftDis)/2;
//						Y_AveDis=(Y_RightDis+Y_LeftDis)/2;

					
//				sprintf(txt,"X_ActPos=%f,\r\n",X_ActPos);	//定位数据-上位机显示坐标波形
//				Usart_SendString(USART1,txt);
				//}
										
//				if(T_init<=1100){				//全场定位初始化10s等待			//question：这一段是什么意思？
//					if(CompState == RETRY){
//								 if(T_init==100)	beep_on;
//						else if(T_init==200)	beep_off;
//						else if(T_init==300)	beep_on;
//						else if(T_init==400)	beep_off;
//						else if(T_init==500)	beep_on;
//						else if(T_init==600)	beep_off;
//						}
//						
//					if(T_init==1000)		beep_on;
//						else if (T_init==1100){
//								beep_off;
//								ActInitFlag=1;
//					}
//						T_init++;
//				}	
//				else{
//						if(Ct>=50){
////							GPIO_ToggleBits(GPIOB,GPIO_Pin_9);//红灯闪烁
//							GPIO_ToggleBits(GPIOB,GPIO_Pin_8);	//蓝灯闪烁
//							Ct=0;											//question：延时参数的用途
//						}
//				}
				//Ct++;
		}				   
		TIM7->SR&=~(1<<0);//清除中断标志位									//question：看不懂    
}

//通用定时器7中断初始化
//这里时钟选择为APB1的2倍，而APB1为42M
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!
void TIM7_Int_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<5;	//TIM3时钟使能    
 	TIM7->ARR=arr;  	//设定计数器自动重装值 
	TIM7->PSC=psc;  	//预分频器	  
	TIM7->DIER|=1<<0;   //允许更新中断	  
	TIM7->CR1|=0x01;    //使能定时器3
	MY_NVIC_Init(4,0,TIM7_IRQn,3);	//抢占3，子优先级3，组2									 
}

/* sbus总线接收所用定时器 */
void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///使能TIM3时钟
	
	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//初始化TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM3,ENABLE); //使能定时器3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

