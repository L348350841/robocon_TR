#include "driver.h"
#include "delay.h"
#include "usart.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//�������вο���̳�������̵�һ����(http://www.openedv.com/thread-41832-1-1.html)
//ALIENTEK STM32F407������
//������������� ���Դ���			   
//lycreturn@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2016/05/12
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved	
//********************************************************************************
//�޸�����:2016/05/12
//////////////////////////////////////////////////////////////////////////////////
/********** ������ �˿ڶ��� **************
//DRIVER_DIR   PE5 (DCMI_D6)
//DRIVER_OE    PE6 (DCMI_D7)
//STEP_PULSE   PC7 (TIM8_CH2  DCMI_D1)
******************************************/




u8 rcr_remainder;   //�ظ�������������
u8 is_rcr_finish=1; //�ظ��������Ƿ��������
long rcr_integer;	//�ظ�������������
long target_pos=0;  //�з��ŷ���
long current_pos=0; //�з��ŷ���
DIR_Type motor_dir=CW;//˳ʱ��



u8 rcr2_remainder;   //�ظ�������������
u8 is_rcr2_finish=1; //�ظ��������Ƿ��������
long rcr2_integer;	//�ظ�������������
long target2_pos=0;  //�з��ŷ���
long current2_pos=0; //�з��ŷ���
DIR_Type motor2_dir=CW;//˳ʱ��

/************** �����������ź��߳�ʼ�� ****************/
void Driver2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��GPIOEʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8; //DRIVER_DIR DRIVER_OE��Ӧ����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIOE5,6
	
	GPIO_SetBits(GPIOE,GPIO_Pin_7);//PE5����� ˳ʱ�뷽��  DRIVER_DIR
	GPIO_ResetBits(GPIOE,GPIO_Pin_8);//PE6����� ʹ�����  DRIVER_OE
}
/***********************************************
//TIM8_CH2(PC7) ���������+�ظ��������ܳ�ʼ��
//TIM8 ʱ��Ƶ�� 84*2=168MHz
//arr:�Զ���װֵ
//psc:ʱ��Ԥ��Ƶ��
************************************************/
void TIM1_OPM_RCR_Init(u16 arr,u16 psc)
{		 					 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  	//TIM8ʱ��ʹ��    
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 	//ʹ��PORTCʱ��	                                                                     	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM1); //GPIOC7����Ϊ��ʱ��8
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;           //GPIOC7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;      //����
	GPIO_Init(GPIOA,&GPIO_InitStructure);               //��ʼ��PF9
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ   
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);

	TIM_UpdateRequestConfig(TIM1,TIM_UpdateSource_Regular); /********* ����ֻ�м��������Ϊ�����ж� ********/
	TIM_SelectOnePulseMode(TIM1,TIM_OPMode_Single);/******* ������ģʽ **********/
 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; //�Ƚ����2ʹ��
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable; /****** �Ƚ����2Nʧ�� *******/
	TIM_OCInitStructure.TIM_Pulse = arr>>1; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //CH1Ԥװ��ʹ��	 
	TIM_ARRPreloadConfig(TIM1, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	
	TIM_ITConfig(TIM1, TIM_IT_Update ,ENABLE);  //TIM8   ʹ�ܻ���ʧ��ָ����TIM�ж�
 
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;  //TIM8�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
	
	TIM_ClearITPendingBit(TIM1, TIM_IT_Update);  //���TIMx���жϴ�����λ:TIM �ж�Դ
	TIM_Cmd(TIM1, ENABLE);  //ʹ��TIM8											  
}
/******* TIM8�����жϷ������ *********/
void TIM1_UP_TIM10_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM1,TIM_FLAG_Update)!=RESET)//�����ж�
	{
		TIM_ClearITPendingBit(TIM1,TIM_FLAG_Update);//��������жϱ�־λ		
		if(is_rcr2_finish==0)//�ظ�������δ�������
		{
			if(rcr2_integer!=0) //�����������廹δ�������
			{
				TIM1->RCR=RCR_VAL;//�����ظ�����ֵ
				rcr2_integer--;//����RCR_VAL+1������				
			}else if(rcr2_remainder!=0)//������������ ��λ0
			{
				TIM1->RCR=rcr2_remainder-1;//������������
				rcr2_remainder=0;//����
				is_rcr2_finish=1;//�ظ��������������				
			}else goto out;   //rcr_remainder=0��ֱ���˳�			 
			TIM_GenerateEvent(TIM1,TIM_EventSource_Update);//����һ�������¼� ���³�ʼ��������
			TIM_CtrlPWMOutputs(TIM1,ENABLE);	//MOE �����ʹ��	
			TIM_Cmd(TIM1, ENABLE);  //ʹ��TIM8			
			if(motor_dir==CW) //�������Ϊ˳ʱ��   
				current_pos+=(TIM1->RCR+1);//�����ظ�����ֵ
			else          //������Ϊ��ʱ��
				current_pos-=(TIM1->RCR+1);//��ȥ�ظ�����ֵ			
		}else
		{
out:		is_rcr2_finish=1;//�ظ��������������
			TIM_CtrlPWMOutputs(TIM1,DISABLE);	//MOE ������ر�
			TIM_Cmd(TIM1, DISABLE);  //�ر�TIM8				
			printf("��ǰλ��=%ld\r\n",current_pos);//��ӡ���
		}	
	}
}
/***************** ����TIM8 *****************/
void TIM1_Startup(u32 frequency)   //������ʱ��8
{
	u16 temp_arr=1000000/frequency-1; 
	TIM_SetAutoreload(TIM1,temp_arr);//�趨�Զ���װֵ	
	TIM_SetCompare1(TIM1,temp_arr>>1); //ƥ��ֵ2������װֵһ�룬����ռ�ձ�Ϊ50%	
	TIM_SetCounter(TIM1,0);//����������
	TIM_Cmd(TIM1, ENABLE);  //ʹ��TIM8
}
/********************************************
//��Զ�λ���� 
//num 0��2147483647
//frequency: 20Hz~100KHz
//dir: CW(˳ʱ�뷽��)  CCW(��ʱ�뷽��)
*********************************************/
void Locate2_Rle(long num,u32 frequency,DIR_Type dir) //��Զ�λ����
{
	if(num<=0) //��ֵС����0 ��ֱ�ӷ���
	{
		printf("\r\nThe num should be greater than zero!!\r\n");
		return;
	}
	if(TIM1->CR1&0x01)//��һ�����廹δ�������  ֱ�ӷ���
	{
		printf("\r\nThe last time pulses is not send finished,wait please!\r\n");
		return;
	}
	if((frequency<20)||(frequency>100000))//����Ƶ�ʲ��ڷ�Χ�� ֱ�ӷ���
	{
		printf("\r\nThe frequency is out of range! please reset it!!(range:20Hz~100KHz)\r\n");
		return;
	}
	motor2_dir=dir;//�õ�����	
	DRIVER2_DIR=motor2_dir;//���÷���
	
	if(motor2_dir==CW)//˳ʱ��
		target2_pos=current2_pos+num;//Ŀ��λ��
	else if(motor2_dir==CCW)//��ʱ��
		target2_pos=current2_pos-num;//Ŀ��λ��
	
	rcr2_integer=num/(RCR_VAL+1);//�ظ�������������
	rcr2_remainder=num%(RCR_VAL+1);//�ظ�������������
	is_rcr2_finish=0;//�ظ�������δ�������
	TIM1_Startup(frequency);//����TIM8
}
/********************************************
//���Զ�λ���� 
//num   -2147483648��2147483647
//frequency: 20Hz~100KHz
*********************************************/
void Locate2_Abs(long num,u32 frequency)//���Զ�λ����
{
	if(TIM1->CR1&0x01)//��һ�����廹δ������� ֱ�ӷ���
	{
		printf("\r\nThe last time pulses is not send finished,wait please!\r\n");
		return;
	}
	if((frequency<20)||(frequency>100000))//����Ƶ�ʲ��ڷ�Χ�� ֱ�ӷ���
	{
		printf("\r\nThe frequency is out of range! please reset it!!(range:20Hz~100KHz)\r\n");
		return;
	}
	target2_pos=num;//����Ŀ��λ��
	if(target2_pos!=current2_pos)//Ŀ��͵�ǰλ�ò�ͬ
	{
		if(target2_pos>current2_pos)
			motor2_dir=CW;//˳ʱ��
		else
			motor2_dir=CCW;//��ʱ��
		DRIVER_DIR=motor_dir;//���÷���
		
		rcr2_integer=abs(target2_pos-current2_pos)/(RCR_VAL+1);//�ظ�������������
		rcr2_remainder=abs(target2_pos-current2_pos)%(RCR_VAL+1);//�ظ�������������
		is_rcr2_finish=0;//�ظ�������δ�������
		TIM1_Startup(frequency);//����TIM8
	}
}



void Driver_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);//ʹ��GPIOEʱ��
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; //DRIVER_DIR DRIVER_OE��Ӧ����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; //DRIVER_DIR DRIVER_OE��Ӧ����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//����
	
	
	
	GPIO_Init(GPIOG, &GPIO_InitStructure);//��ʼ��GPIOG5,6
	
	GPIO_SetBits(GPIOG,GPIO_Pin_4);//PE5����� ˳ʱ�뷽��  DRIVER_DIR
	//GPIO_ResetBits(GPIOG,GPIO_Pin_3);//PE6����� ʹ�����  DRIVER_OE
}
/***********************************************
//TIM8_CH2(PC7) ���������+�ظ��������ܳ�ʼ��
//TIM8 ʱ��Ƶ�� 84*2=168MHz
//arr:�Զ���װֵ
//psc:ʱ��Ԥ��Ƶ��
************************************************/
void TIM8_OPM_RCR_Init(u16 arr,u16 psc)
{		 					 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);  	//TIM8ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//ʹ��PORTCʱ��	  
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_TIM8); //GPIOC7����Ϊ��ʱ��8
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;           //GPIOC7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;      //����
	GPIO_Init(GPIOB,&GPIO_InitStructure);               //��ʼ��PF9
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ   
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	TIM_ClearITPendingBit(TIM8,TIM_IT_Update);

	TIM_UpdateRequestConfig(TIM8,TIM_UpdateSource_Regular); /********* ����ֻ�м��������Ϊ�����ж� ********/
	TIM_SelectOnePulseMode(TIM8,TIM_OPMode_Single);/******* ������ģʽ **********/
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; //�Ƚ����2ʹ��
	//TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable; /****** �Ƚ����2Nʧ�� *******/
	TIM_OCInitStructure.TIM_Pulse = arr>>1; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OCInitStructure.TIM_OCNIdleState=TIM_OCNIdleState_Reset;
	TIM_OC2Init(TIM8, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);  //CH2Ԥװ��ʹ��	 
	
	TIM_ARRPreloadConfig(TIM8, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	TIM_ITConfig(TIM8, TIM_IT_Update ,ENABLE);  //TIM8   ʹ�ܻ���ʧ��ָ����TIM�ж�
    
	NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_TIM13_IRQn;  //TIM8�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
	TIM_CtrlPWMOutputs(TIM8,ENABLE);
	TIM_ClearITPendingBit(TIM8, TIM_IT_Update);  //���TIMx���жϴ�����λ:TIM �ж�Դ
	TIM_Cmd(TIM8, ENABLE);  //ʹ��TIM8											  
}

/******* TIM8�����жϷ������ *********/
void TIM8_UP_TIM13_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM8,TIM_FLAG_Update)!=RESET)//�����ж�
	{
		TIM_ClearITPendingBit(TIM8,TIM_FLAG_Update);//��������жϱ�־λ		
		if(is_rcr_finish==0)//�ظ�������δ�������
		{
			if(rcr_integer!=0) //�����������廹δ�������
			{
				TIM8->RCR=RCR_VAL;//�����ظ�����ֵ
				rcr_integer--;//����RCR_VAL+1������				
			}else if(rcr_remainder!=0)//������������ ��λ0
			{
				TIM8->RCR=rcr_remainder-1;//������������
				rcr_remainder=0;//����
				is_rcr_finish=1;//�ظ��������������				
			}else goto out;   //rcr_remainder=0��ֱ���˳�			 
			TIM_GenerateEvent(TIM8,TIM_EventSource_Update);//����һ�������¼� ���³�ʼ��������
			TIM_CtrlPWMOutputs(TIM8,ENABLE);	//MOE �����ʹ��	
			TIM_Cmd(TIM8, ENABLE);  //ʹ��TIM8			
			if(motor_dir==CW) //�������Ϊ˳ʱ��   
				current_pos+=(TIM8->RCR+1);//�����ظ�����ֵ
			else          //������Ϊ��ʱ��
				current_pos-=(TIM8->RCR+1);//��ȥ�ظ�����ֵ			
		}else
		{
out:		is_rcr_finish=1;//�ظ��������������
			TIM_CtrlPWMOutputs(TIM8,DISABLE);	//MOE ������ر�
			TIM_Cmd(TIM8, DISABLE);  //�ر�TIM8				
			printf("��ǰλ��=%ld\r\n",current_pos);//��ӡ���
		}	
	}
}
/***************** ����TIM8 *****************/
void TIM8_Startup(u32 frequency)   //������ʱ��8
{
	u16 temp_arr=1000000/frequency-1; 
	TIM_SetAutoreload(TIM8,temp_arr);//�趨�Զ���װֵ	
	TIM_SetCompare2(TIM8,temp_arr>>1); //ƥ��ֵ2������װֵһ�룬����ռ�ձ�Ϊ50%	
	TIM_SetCounter(TIM8,0);//����������
	TIM_Cmd(TIM8, ENABLE);  //ʹ��TIM8
}
/********************************************
//��Զ�λ���� 
//num 0��2147483647
//frequency: 20Hz~100KHz
//dir: CW(˳ʱ�뷽��)  CCW(��ʱ�뷽��)
*********************************************/
void Locate_Rle(long num,u32 frequency,DIR_Type dir) //��Զ�λ����
{
	if(num<=0) //��ֵС����0 ��ֱ�ӷ���
	{
		printf("\r\nThe num should be greater than zero!!\r\n");
		return;
	}
	if(TIM8->CR1&0x01)//��һ�����廹δ�������  ֱ�ӷ���
	{
		printf("\r\nThe last time pulses is not send finished,wait please!\r\n");
		return;
	}
	if((frequency<20)||(frequency>250000))//����Ƶ�ʲ��ڷ�Χ�� ֱ�ӷ���
	{
		printf("\r\nThe frequency is out of range! please reset it!!(range:20Hz~100KHz)\r\n");
		return;
	}
	motor_dir=dir;//�õ�����	
	DRIVER_DIR=motor_dir;//���÷���
	
	if(motor_dir==CW)//˳ʱ��
		target_pos=current_pos+num;//Ŀ��λ��
	else if(motor_dir==CCW)//��ʱ��
		target_pos=current_pos-num;//Ŀ��λ��
	
	rcr_integer=num/(RCR_VAL+1);//�ظ�������������
	rcr_remainder=num%(RCR_VAL+1);//�ظ�������������
	is_rcr_finish=0;//�ظ�������δ�������
	TIM8_Startup(frequency);//����TIM8
}
/********************************************
//���Զ�λ���� 
//num   -2147483648��2147483647
//frequency: 20Hz~100KHz
*********************************************/
void Locate_Abs(long num,u32 frequency)//���Զ�λ����
{
	if(TIM8->CR1&0x01)//��һ�����廹δ������� ֱ�ӷ���
	{
		printf("\r\nThe last time pulses is not send finished,wait please!\r\n");
		return;
	}
	if((frequency<20)||(frequency>100000))//����Ƶ�ʲ��ڷ�Χ�� ֱ�ӷ���
	{
		printf("\r\nThe frequency is out of range! please reset it!!(range:20Hz~100KHz)\r\n");
		return;
	}
	target_pos=num;//����Ŀ��λ��
	if(target_pos!=current_pos)//Ŀ��͵�ǰλ�ò�ͬ
	{
		if(target_pos>current_pos)
			motor_dir=CW;//˳ʱ��
		else
			motor_dir=CCW;//��ʱ��
		DRIVER_DIR=motor_dir;//���÷���
		
		rcr_integer=abs(target_pos-current_pos)/(RCR_VAL+1);//�ظ�������������
		rcr_remainder=abs(target_pos-current_pos)%(RCR_VAL+1);//�ظ�������������
		is_rcr_finish=0;//�ظ�������δ�������
		TIM8_Startup(frequency);//����TIM8
	}
}



