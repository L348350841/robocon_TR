#include "sys.h"
#include "AllPara.h"
#include "usart.h"	
#include "arm_math.h"
#include "math.h"
#include "lcd.h"
float tmpPos_x = 0;
float tmpPos_y = 0;

////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif

//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif
 
#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���





void USART1_Init(u32 bound)
{
		//GPIO�˿�����
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART2ʱ��

		//����2��Ӧ���Ÿ���ӳ��
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); 
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); 

		//USART2�˿�����
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9| GPIO_Pin_10; 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
		GPIO_Init(GPIOA,&GPIO_InitStructure); 

		//USART2 ��ʼ������
		USART_InitStructure.USART_BaudRate = bound;//����������
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
		USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
		USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
		USART_InitStructure.USART_Mode =USART_Mode_Tx;	//�շ�ģʽ
		USART_Init(USART1, &USART_InitStructure); //��ʼ������1

		USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1

		USART_ClearFlag(USART1, USART_FLAG_TC);
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�

		//Usart1 NVIC ����
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�3
		NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�3
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
		NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
}







/*************************************************************************************
 * @brief   ���� 3 ��ʼ���� ������λ�����Ժͺ����Ӿ��ӿ�
 * @version 1.0
 * @date    2020.9.10
 * @author  xb
* @param		bound:������
 * @retval  none
 *************************************************************************************/
void USART3_Init(u32 bound)
{
		//GPIO�˿�����
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); 
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//ʹ��USART2ʱ��

		//����2��Ӧ���Ÿ���ӳ��
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); 
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); 

		//USART2�˿�����
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10| GPIO_Pin_11; 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
		GPIO_Init(GPIOB,&GPIO_InitStructure); 

		//USART2 ��ʼ������
		USART_InitStructure.USART_BaudRate = bound;//����������
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
		USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
		USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
		USART_InitStructure.USART_Mode =USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
		USART_Init(USART3, &USART_InitStructure); //��ʼ������1

		USART_Cmd(USART3, ENABLE);  //ʹ�ܴ���1

		USART_ClearFlag(USART3, USART_FLAG_TC);
		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//��������ж�

		//Usart1 NVIC ����
		NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//����1�ж�ͨ��
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�3
		NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�3
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
		NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
}

/*************************************************************************************
 * @brief   ���� 2 ��ʼ���� ���ڰ����궨λͨ��
 * @version 1.0
 * @date    2020.5.15
 * @author  xb
 * @param		bound:������
 * @retval  none
 *************************************************************************************/

void USART2_Init(u32 bound)
{
		//GPIO�˿�����
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); 
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ��USART2ʱ��

		//����2��Ӧ���Ÿ���ӳ��
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); 
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); 

		//USART2�˿�����
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
		GPIO_Init(GPIOA,&GPIO_InitStructure); 

		//USART2 ��ʼ������
		USART_InitStructure.USART_BaudRate = bound;//����������
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
		USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
		USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
		USART_Init(USART2, &USART_InitStructure); //��ʼ������2

		USART_Cmd(USART2, ENABLE);  //ʹ�ܴ���2
		
		USART_ClearFlag(USART2, USART_FLAG_TC);
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//��������ж�

		//Usart2 NVIC ����
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;				//����2�ж�ͨ��
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;	//��ռ���ȼ�0
		NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;				//�����ȼ�0
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQͨ��ʹ��
		NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
}



/*************************************************************************************
 * @brief   ���� 1 �жϺ����� �Ӿ��״�
 * @version 1.0
 * @date    2020.7.17
 * @author  
 * @param		none
 * @retval  none
 *************************************************************************************/
u8 t;
u8 len;
u8 Res;
//u8 Receive[8];
void USART3_IRQHandler(void)                	//����1�жϷ������
{
	
#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(USART3);//(USART1->DR);	//��ȡ���յ�������
		
		if((USART_RX_STA&0x8000)==0)//����δ���
		{
			if(USART_RX_STA&0x4000)//���յ���0x0d
			{
				if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
				else USART_RX_STA|=0x8000;	//��������� 
			}
			else //��û�յ�0X0D
			{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}   		 
  } 
#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntExit();  											 
#endif
} 




void USART1_IRQHandler(void)                	//����1�жϷ������
{
	
#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
		
		if((USART_RX_STA&0x8000)==0)//����δ���
		{
			if(USART_RX_STA&0x4000)//���յ���0x0d
			{
				if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
				else USART_RX_STA|=0x8000;	//��������� 
			}
			else //��û�յ�0X0D
			{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}   		 
  } 
#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntExit();  											 
#endif
} 
/*************************************************************************************
 * @brief   ���� 1 ������--�״�����Ͱ�ľ���
 * @version 1.0
 * @date    2020.5.15
 * @author  xb
 * @param		none
 * @retval  none
 *************************************************************************************/
void RpiLidar_Task()
{
	
	if(USART_RX_STA&0x8000)              //һ�ν������  USART_RX_STA&0x8000
		{		
				//LED0=!LED0;
//			len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
			//printf("\r\n�����͵���ϢΪ:\r\n");
//			for(t=0;t<len;t++)
//			{
//				USART_SendData(USART3, USART_RX_BUF[t]);         //�򴮿�1��������
//				while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
//			}
			
			
			//USART_SendData(USART1, USART_RX_BUF[3]);
//			150,600,X_LeftDis,24,0);//��������
//			LED_PrintValueF(150,630,X_RightDis,24,0);
//			LED_PrintValueF(150,660,Y_LeftDis,24,0);
//			LED_PrintValueF(150,690,Y_RightDis,24,0);
			
			
//			LED_PrintValueF(20,100,USART_RX_BUF[0]-48,24,0)	;
//			LED_PrintValueF(60,100,USART_RX_BUF[1]-48,24,0)	;
//			LED_PrintValueF(100,100,USART_RX_BUF[2]-48,24,0);
//			LED_PrintValueF(140,100,USART_RX_BUF[3]-48,24,0);
//			LED_PrintValueF(180,100,USART_RX_BUF[4]-48,24,0);
//			LED_PrintValueF(220,100,USART_RX_BUF[5]-48,24,0);
			
			//USART_SendData(USART1, USART_RX_BUF[3]);
			
//			LED_PrintValueF(20,100,USART_RX_BUF[0]-48,24,0)	;//ʮ
//			LED_PrintValueF(60,100,USART_RX_BUF[1]-48,24,0)	;//��
//			LED_PrintValueF(100,100,USART_RX_BUF[2]-48,24,0);//С�����һλ
//			LED_PrintValueF(140,100,USART_RX_BUF[3]-48,24,0);//С�������λ
//			LED_PrintValueF(180,100,USART_RX_BUF[4]-48,24,0);//С�������λ
//			LED_PrintValueF(220,100,USART_RX_BUF[5]-48,24,0);//
			
			//USART_RX_BUF[0]=48;
			LED_PrintValueF(150,720,USART_RX_BUF[0]-48,24,0);
			LED_PrintValueF(180,720,USART_RX_BUF[1]-48,24,0);
			LED_PrintValueF(210,720,USART_RX_BUF[2]-48,24,0);
			LED_PrintValueF(240,720,USART_RX_BUF[3]-48,24,0);
			LED_PrintValueF(270,720,USART_RX_BUF[4]-48,24,0);
			LED_PrintValueF(300,720,USART_RX_BUF[5]-48,24,0);
			
			//juli=(USART_RX_BUF[0]-48)*1000+(USART_RX_BUF[1]-48)*100+(USART_RX_BUF[2]-48)*10+(USART_RX_BUF[3]-48);
			//juli=6;
			LED_PrintValueF(150,760,juli,24,0);
			//LED_PrintValueF(100,200,juli,24,0);	
			
			//LED_PrintValueF(260,100,USART_RX_BUF[6]-48,24,0);
			USART_RX_STA=0;
		}
}
/*************************************************************************************
 * @brief   ���� 2 �жϺ����� �����궨λͨ��Э��
 * @version 1.0
 * @date    2020.5.15
 * @author  xb
 * @param		none
 * @retval  none
 *************************************************************************************/
void USART2_IRQHandler(void)                	//����4�жϷ������
{
		static uint8_t ch;
		static union
		{
				uint8_t data[24];
				float ActVal[6];
		}posture;
		
		static uint8_t count=0;
		static uint8_t i=0;
		
		if(USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET) 
		{ 
				USART_ClearFlag(USART2, USART_FLAG_ORE); //�������ж� 
		}
		if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
		{
				USART_ClearITPendingBit(USART2,USART_IT_RXNE);
				ch=USART_ReceiveData(USART2);
				switch(count)
				{
					case 0:
							if(ch==0x0d)
									count++;
							else
									count=0;
							break;
					
					case 1:
							if(ch==0x0a)
							{
									i=0;
									count++;
							}
							else if(ch==0x0d);
							else
									count=0;
							break;
					
					case 2:
							posture.data[i]=ch;
							i++;
							if(i>=24)
							{
									i=0;
									count++;
							}
							break;
							
					case 3:
							if(ch==0x0a)
									count++;
							else
									count=0;
							break;
							
					case 4:
							if(ch==0x0d)
							{				
								Z_Angle =  posture.ActVal[0];
								X_Angle =  posture.ActVal[1];
								Y_Angle =  posture.ActVal[2];
								X_Pos 	=  posture.ActVal[3];
								Y_Pos		=  posture.ActVal[4];	
//								X_Pos 	=  posture.ActVal[3];ԭ����
//								Y_Pos		=  posture.ActVal[4];	
								Z_W 		=  posture.ActVal[5];
								
								X_ActPos = X_Pos;
								Y_ActPos = Y_Pos;
							}
							count=0;
							break;
					
					default:
							count=0;
							break;
				}
		}
}





#endif	



