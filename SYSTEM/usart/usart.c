#include "sys.h"
#include "AllPara.h"
#include "usart.h"	
#include "arm_math.h"
#include "math.h"
#include "lcd.h"
float tmpPos_x = 0;
float tmpPos_y = 0;

////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif

//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif
 
#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记





void USART1_Init(u32 bound)
{
		//GPIO端口设置
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART2时钟

		//串口2对应引脚复用映射
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); 
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); 

		//USART2端口配置
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9| GPIO_Pin_10; 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
		GPIO_Init(GPIOA,&GPIO_InitStructure); 

		//USART2 初始化设置
		USART_InitStructure.USART_BaudRate = bound;//波特率设置
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
		USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
		USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
		USART_InitStructure.USART_Mode =USART_Mode_Tx;	//收发模式
		USART_Init(USART1, &USART_InitStructure); //初始化串口1

		USART_Cmd(USART1, ENABLE);  //使能串口1

		USART_ClearFlag(USART1, USART_FLAG_TC);
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断

		//Usart1 NVIC 配置
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级3
		NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级3
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
		NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
}







/*************************************************************************************
 * @brief   串口 3 初始化： 用于上位机调试和后期视觉接口
 * @version 1.0
 * @date    2020.9.10
 * @author  xb
* @param		bound:波特率
 * @retval  none
 *************************************************************************************/
void USART3_Init(u32 bound)
{
		//GPIO端口设置
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); 
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//使能USART2时钟

		//串口2对应引脚复用映射
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); 
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); 

		//USART2端口配置
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10| GPIO_Pin_11; 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
		GPIO_Init(GPIOB,&GPIO_InitStructure); 

		//USART2 初始化设置
		USART_InitStructure.USART_BaudRate = bound;//波特率设置
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
		USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
		USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
		USART_InitStructure.USART_Mode =USART_Mode_Rx | USART_Mode_Tx;	//收发模式
		USART_Init(USART3, &USART_InitStructure); //初始化串口1

		USART_Cmd(USART3, ENABLE);  //使能串口1

		USART_ClearFlag(USART3, USART_FLAG_TC);
		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启相关中断

		//Usart1 NVIC 配置
		NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//串口1中断通道
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级3
		NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级3
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
		NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
}

/*************************************************************************************
 * @brief   串口 2 初始化： 用于艾克申定位通信
 * @version 1.0
 * @date    2020.5.15
 * @author  xb
 * @param		bound:波特率
 * @retval  none
 *************************************************************************************/

void USART2_Init(u32 bound)
{
		//GPIO端口设置
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); 
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能USART2时钟

		//串口2对应引脚复用映射
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); 
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); 

		//USART2端口配置
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
		GPIO_Init(GPIOA,&GPIO_InitStructure); 

		//USART2 初始化设置
		USART_InitStructure.USART_BaudRate = bound;//波特率设置
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
		USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
		USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
		USART_Init(USART2, &USART_InitStructure); //初始化串口2

		USART_Cmd(USART2, ENABLE);  //使能串口2
		
		USART_ClearFlag(USART2, USART_FLAG_TC);
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启相关中断

		//Usart2 NVIC 配置
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;				//串口2中断通道
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;	//抢占优先级0
		NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;				//子优先级0
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQ通道使能
		NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
}



/*************************************************************************************
 * @brief   串口 1 中断函数： 视觉雷达
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
void USART3_IRQHandler(void)                	//串口1中断服务程序
{
	
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res =USART_ReceiveData(USART3);//(USART1->DR);	//读取接收到的数据
		
		if((USART_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART_RX_STA&0x4000)//接收到了0x0d
			{
				if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else USART_RX_STA|=0x8000;	//接收完成了 
			}
			else //还没收到0X0D
			{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}   		 
  } 
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
} 




void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
		
		if((USART_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART_RX_STA&0x4000)//接收到了0x0d
			{
				if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else USART_RX_STA|=0x8000;	//接收完成了 
			}
			else //还没收到0X0D
			{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}   		 
  } 
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
} 
/*************************************************************************************
 * @brief   串口 1 处理函数--雷达正对桶的距离
 * @version 1.0
 * @date    2020.5.15
 * @author  xb
 * @param		none
 * @retval  none
 *************************************************************************************/
void RpiLidar_Task()
{
	
	if(USART_RX_STA&0x8000)              //一次接收完成  USART_RX_STA&0x8000
		{		
				//LED0=!LED0;
//			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
			//printf("\r\n您发送的消息为:\r\n");
//			for(t=0;t<len;t++)
//			{
//				USART_SendData(USART3, USART_RX_BUF[t]);         //向串口1发送数据
//				while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//等待发送结束
//			}
			
			
			//USART_SendData(USART1, USART_RX_BUF[3]);
//			150,600,X_LeftDis,24,0);//激光数据
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
			
//			LED_PrintValueF(20,100,USART_RX_BUF[0]-48,24,0)	;//十
//			LED_PrintValueF(60,100,USART_RX_BUF[1]-48,24,0)	;//个
//			LED_PrintValueF(100,100,USART_RX_BUF[2]-48,24,0);//小数点后一位
//			LED_PrintValueF(140,100,USART_RX_BUF[3]-48,24,0);//小数点后两位
//			LED_PrintValueF(180,100,USART_RX_BUF[4]-48,24,0);//小数点后三位
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
 * @brief   串口 2 中断函数： 艾克申定位通信协议
 * @version 1.0
 * @date    2020.5.15
 * @author  xb
 * @param		none
 * @retval  none
 *************************************************************************************/
void USART2_IRQHandler(void)                	//串口4中断服务程序
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
				USART_ClearFlag(USART2, USART_FLAG_ORE); //清除溢出中断 
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
//								X_Pos 	=  posture.ActVal[3];原来的
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



