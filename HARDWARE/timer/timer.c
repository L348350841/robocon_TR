#include "timer.h"
#include "public.h"
extern char a;
//��ʱ��7�жϷ������	 
void TIM7_IRQHandler(void)
{ 		    		  			    
		if(TIM7->SR&0X0001)//����ж�
		{
			#ifdef ReceiveDataFromNRF
				//���߱���
				if(NRF24L01_RxPacket(tmp_buf)==0){//һ�����յ���Ϣ,����ʾ����.
					ReceiveDataFormNRF();			
					tmp_buf[32]=0;//�����ַ���������
				}		
			#endif	
				//if(ActInitFlag){								//ȫ����λ��־λ��1Ϊ��ʼ�����
					
					//sheshoujian++;
					Motor_Control();							//�������
					
//					printf("X_ActPos=%f/r/n", X_ActPos);
//					printf("Y_ActPos=%f/r/n", Y_ActPos);
					if (direction == 0)								//��ü��⴫������ֵ
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
			/////////////����Y�Ƕȼ��㹫ʽ(0-45)///////
//					if(Y_LeftDis>Y_RightDis)
//					  JGY_Angle=lookup_arctan((float)(Y_LeftDis-Y_RightDis)/38);
//					else
//						JGY_Angle=lookup_arctan((Y_RightDis-Y_LeftDis)/38);
//					
//						/////ͬһ��������������ȡƽ��ֵ//////////
//						X_AveDis=(X_RightDis+X_LeftDis)/2;
//						Y_AveDis=(Y_RightDis+Y_LeftDis)/2;

					
//				sprintf(txt,"X_ActPos=%f,\r\n",X_ActPos);	//��λ����-��λ����ʾ���겨��
//				Usart_SendString(USART1,txt);
				//}
										
//				if(T_init<=1100){				//ȫ����λ��ʼ��10s�ȴ�			//question����һ����ʲô��˼��
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
////							GPIO_ToggleBits(GPIOB,GPIO_Pin_9);//�����˸
//							GPIO_ToggleBits(GPIOB,GPIO_Pin_8);	//������˸
//							Ct=0;											//question����ʱ��������;
//						}
//				}
				//Ct++;
		}				   
		TIM7->SR&=~(1<<0);//����жϱ�־λ									//question��������    
}

//ͨ�ö�ʱ��7�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ42M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��3!
void TIM7_Int_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<5;	//TIM3ʱ��ʹ��    
 	TIM7->ARR=arr;  	//�趨�������Զ���װֵ 
	TIM7->PSC=psc;  	//Ԥ��Ƶ��	  
	TIM7->DIER|=1<<0;   //��������ж�	  
	TIM7->CR1|=0x01;    //ʹ�ܶ�ʱ��3
	MY_NVIC_Init(4,0,TIM7_IRQn,3);	//��ռ3�������ȼ�3����2									 
}

/* sbus���߽������ö�ʱ�� */
void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///ʹ��TIM3ʱ��
	
	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//��ʼ��TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	TIM_Cmd(TIM3,ENABLE); //ʹ�ܶ�ʱ��3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

