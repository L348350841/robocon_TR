#include "driver.h"
#include "delay.h"
#include "usart.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//此例程有参考论坛网友例程的一部分(http://www.openedv.com/thread-41832-1-1.html)
//ALIENTEK STM32F407开发板
//步进电机驱动器 测试代码			   
//lycreturn@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2016/05/12
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved	
//********************************************************************************
//修改日期:2016/05/12
//////////////////////////////////////////////////////////////////////////////////
/********** 驱动器 端口定义 **************
//DRIVER_DIR   PE5 (DCMI_D6)
//DRIVER_OE    PE6 (DCMI_D7)
//STEP_PULSE   PC7 (TIM8_CH2  DCMI_D1)
******************************************/




u8 rcr_remainder;   //重复计数余数部分
u8 is_rcr_finish=1; //重复计数器是否设置完成
long rcr_integer;	//重复计数整数部分
long target_pos=0;  //有符号方向
long current_pos=0; //有符号方向
DIR_Type motor_dir=CW;//顺时针



u8 rcr2_remainder;   //重复计数余数部分
u8 is_rcr2_finish=1; //重复计数器是否设置完成
long rcr2_integer;	//重复计数整数部分
long target2_pos=0;  //有符号方向
long current2_pos=0; //有符号方向
DIR_Type motor2_dir=CW;//顺时针

/************** 驱动器控制信号线初始化 ****************/
void Driver2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//使能GPIOE时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8; //DRIVER_DIR DRIVER_OE对应引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE5,6
	
	GPIO_SetBits(GPIOE,GPIO_Pin_7);//PE5输出高 顺时针方向  DRIVER_DIR
	GPIO_ResetBits(GPIOE,GPIO_Pin_8);//PE6输出低 使能输出  DRIVER_OE
}
/***********************************************
//TIM8_CH2(PC7) 单脉冲输出+重复计数功能初始化
//TIM8 时钟频率 84*2=168MHz
//arr:自动重装值
//psc:时钟预分频数
************************************************/
void TIM1_OPM_RCR_Init(u16 arr,u16 psc)
{		 					 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  	//TIM8时钟使能    
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 	//使能PORTC时钟	                                                                     	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM1); //GPIOC7复用为定时器8
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;           //GPIOC7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;      //下拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);               //初始化PF9
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值   
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);

	TIM_UpdateRequestConfig(TIM1,TIM_UpdateSource_Regular); /********* 设置只有计数溢出作为更新中断 ********/
	TIM_SelectOnePulseMode(TIM1,TIM_OPMode_Single);/******* 单脉冲模式 **********/
 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; //比较输出2使能
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable; /****** 比较输出2N失能 *******/
	TIM_OCInitStructure.TIM_Pulse = arr>>1; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //CH1预装载使能	 
	TIM_ARRPreloadConfig(TIM1, ENABLE); //使能TIMx在ARR上的预装载寄存器
	
	TIM_ITConfig(TIM1, TIM_IT_Update ,ENABLE);  //TIM8   使能或者失能指定的TIM中断
 
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;  //TIM8中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级1级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级1级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	
	TIM_ClearITPendingBit(TIM1, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源
	TIM_Cmd(TIM1, ENABLE);  //使能TIM8											  
}
/******* TIM8更新中断服务程序 *********/
void TIM1_UP_TIM10_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM1,TIM_FLAG_Update)!=RESET)//更新中断
	{
		TIM_ClearITPendingBit(TIM1,TIM_FLAG_Update);//清除更新中断标志位		
		if(is_rcr2_finish==0)//重复计数器未设置完成
		{
			if(rcr2_integer!=0) //整数部分脉冲还未发送完成
			{
				TIM1->RCR=RCR_VAL;//设置重复计数值
				rcr2_integer--;//减少RCR_VAL+1个脉冲				
			}else if(rcr2_remainder!=0)//余数部分脉冲 不位0
			{
				TIM1->RCR=rcr2_remainder-1;//设置余数部分
				rcr2_remainder=0;//清零
				is_rcr2_finish=1;//重复计数器设置完成				
			}else goto out;   //rcr_remainder=0，直接退出			 
			TIM_GenerateEvent(TIM1,TIM_EventSource_Update);//产生一个更新事件 重新初始化计数器
			TIM_CtrlPWMOutputs(TIM1,ENABLE);	//MOE 主输出使能	
			TIM_Cmd(TIM1, ENABLE);  //使能TIM8			
			if(motor_dir==CW) //如果方向为顺时针   
				current_pos+=(TIM1->RCR+1);//加上重复计数值
			else          //否则方向为逆时针
				current_pos-=(TIM1->RCR+1);//减去重复计数值			
		}else
		{
out:		is_rcr2_finish=1;//重复计数器设置完成
			TIM_CtrlPWMOutputs(TIM1,DISABLE);	//MOE 主输出关闭
			TIM_Cmd(TIM1, DISABLE);  //关闭TIM8				
			printf("当前位置=%ld\r\n",current_pos);//打印输出
		}	
	}
}
/***************** 启动TIM8 *****************/
void TIM1_Startup(u32 frequency)   //启动定时器8
{
	u16 temp_arr=1000000/frequency-1; 
	TIM_SetAutoreload(TIM1,temp_arr);//设定自动重装值	
	TIM_SetCompare1(TIM1,temp_arr>>1); //匹配值2等于重装值一半，是以占空比为50%	
	TIM_SetCounter(TIM1,0);//计数器清零
	TIM_Cmd(TIM1, ENABLE);  //使能TIM8
}
/********************************************
//相对定位函数 
//num 0～2147483647
//frequency: 20Hz~100KHz
//dir: CW(顺时针方向)  CCW(逆时针方向)
*********************************************/
void Locate2_Rle(long num,u32 frequency,DIR_Type dir) //相对定位函数
{
	if(num<=0) //数值小等于0 则直接返回
	{
		printf("\r\nThe num should be greater than zero!!\r\n");
		return;
	}
	if(TIM1->CR1&0x01)//上一次脉冲还未发送完成  直接返回
	{
		printf("\r\nThe last time pulses is not send finished,wait please!\r\n");
		return;
	}
	if((frequency<20)||(frequency>100000))//脉冲频率不在范围内 直接返回
	{
		printf("\r\nThe frequency is out of range! please reset it!!(range:20Hz~100KHz)\r\n");
		return;
	}
	motor2_dir=dir;//得到方向	
	DRIVER2_DIR=motor2_dir;//设置方向
	
	if(motor2_dir==CW)//顺时针
		target2_pos=current2_pos+num;//目标位置
	else if(motor2_dir==CCW)//逆时针
		target2_pos=current2_pos-num;//目标位置
	
	rcr2_integer=num/(RCR_VAL+1);//重复计数整数部分
	rcr2_remainder=num%(RCR_VAL+1);//重复计数余数部分
	is_rcr2_finish=0;//重复计数器未设置完成
	TIM1_Startup(frequency);//开启TIM8
}
/********************************************
//绝对定位函数 
//num   -2147483648～2147483647
//frequency: 20Hz~100KHz
*********************************************/
void Locate2_Abs(long num,u32 frequency)//绝对定位函数
{
	if(TIM1->CR1&0x01)//上一次脉冲还未发送完成 直接返回
	{
		printf("\r\nThe last time pulses is not send finished,wait please!\r\n");
		return;
	}
	if((frequency<20)||(frequency>100000))//脉冲频率不在范围内 直接返回
	{
		printf("\r\nThe frequency is out of range! please reset it!!(range:20Hz~100KHz)\r\n");
		return;
	}
	target2_pos=num;//设置目标位置
	if(target2_pos!=current2_pos)//目标和当前位置不同
	{
		if(target2_pos>current2_pos)
			motor2_dir=CW;//顺时针
		else
			motor2_dir=CCW;//逆时针
		DRIVER_DIR=motor_dir;//设置方向
		
		rcr2_integer=abs(target2_pos-current2_pos)/(RCR_VAL+1);//重复计数整数部分
		rcr2_remainder=abs(target2_pos-current2_pos)%(RCR_VAL+1);//重复计数余数部分
		is_rcr2_finish=0;//重复计数器未设置完成
		TIM1_Startup(frequency);//开启TIM8
	}
}



void Driver_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);//使能GPIOE时钟
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; //DRIVER_DIR DRIVER_OE对应引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; //DRIVER_DIR DRIVER_OE对应引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//下拉
	
	
	
	GPIO_Init(GPIOG, &GPIO_InitStructure);//初始化GPIOG5,6
	
	GPIO_SetBits(GPIOG,GPIO_Pin_4);//PE5输出高 顺时针方向  DRIVER_DIR
	//GPIO_ResetBits(GPIOG,GPIO_Pin_3);//PE6输出低 使能输出  DRIVER_OE
}
/***********************************************
//TIM8_CH2(PC7) 单脉冲输出+重复计数功能初始化
//TIM8 时钟频率 84*2=168MHz
//arr:自动重装值
//psc:时钟预分频数
************************************************/
void TIM8_OPM_RCR_Init(u16 arr,u16 psc)
{		 					 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);  	//TIM8时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//使能PORTC时钟	  
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_TIM8); //GPIOC7复用为定时器8
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;           //GPIOC7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;      //下拉
	GPIO_Init(GPIOB,&GPIO_InitStructure);               //初始化PF9
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值   
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	TIM_ClearITPendingBit(TIM8,TIM_IT_Update);

	TIM_UpdateRequestConfig(TIM8,TIM_UpdateSource_Regular); /********* 设置只有计数溢出作为更新中断 ********/
	TIM_SelectOnePulseMode(TIM8,TIM_OPMode_Single);/******* 单脉冲模式 **********/
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; //比较输出2使能
	//TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable; /****** 比较输出2N失能 *******/
	TIM_OCInitStructure.TIM_Pulse = arr>>1; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OCInitStructure.TIM_OCNIdleState=TIM_OCNIdleState_Reset;
	TIM_OC2Init(TIM8, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);  //CH2预装载使能	 
	
	TIM_ARRPreloadConfig(TIM8, ENABLE); //使能TIMx在ARR上的预装载寄存器
	TIM_ITConfig(TIM8, TIM_IT_Update ,ENABLE);  //TIM8   使能或者失能指定的TIM中断
    
	NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_TIM13_IRQn;  //TIM8中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级1级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级1级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	TIM_CtrlPWMOutputs(TIM8,ENABLE);
	TIM_ClearITPendingBit(TIM8, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源
	TIM_Cmd(TIM8, ENABLE);  //使能TIM8											  
}

/******* TIM8更新中断服务程序 *********/
void TIM8_UP_TIM13_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM8,TIM_FLAG_Update)!=RESET)//更新中断
	{
		TIM_ClearITPendingBit(TIM8,TIM_FLAG_Update);//清除更新中断标志位		
		if(is_rcr_finish==0)//重复计数器未设置完成
		{
			if(rcr_integer!=0) //整数部分脉冲还未发送完成
			{
				TIM8->RCR=RCR_VAL;//设置重复计数值
				rcr_integer--;//减少RCR_VAL+1个脉冲				
			}else if(rcr_remainder!=0)//余数部分脉冲 不位0
			{
				TIM8->RCR=rcr_remainder-1;//设置余数部分
				rcr_remainder=0;//清零
				is_rcr_finish=1;//重复计数器设置完成				
			}else goto out;   //rcr_remainder=0，直接退出			 
			TIM_GenerateEvent(TIM8,TIM_EventSource_Update);//产生一个更新事件 重新初始化计数器
			TIM_CtrlPWMOutputs(TIM8,ENABLE);	//MOE 主输出使能	
			TIM_Cmd(TIM8, ENABLE);  //使能TIM8			
			if(motor_dir==CW) //如果方向为顺时针   
				current_pos+=(TIM8->RCR+1);//加上重复计数值
			else          //否则方向为逆时针
				current_pos-=(TIM8->RCR+1);//减去重复计数值			
		}else
		{
out:		is_rcr_finish=1;//重复计数器设置完成
			TIM_CtrlPWMOutputs(TIM8,DISABLE);	//MOE 主输出关闭
			TIM_Cmd(TIM8, DISABLE);  //关闭TIM8				
			printf("当前位置=%ld\r\n",current_pos);//打印输出
		}	
	}
}
/***************** 启动TIM8 *****************/
void TIM8_Startup(u32 frequency)   //启动定时器8
{
	u16 temp_arr=1000000/frequency-1; 
	TIM_SetAutoreload(TIM8,temp_arr);//设定自动重装值	
	TIM_SetCompare2(TIM8,temp_arr>>1); //匹配值2等于重装值一半，是以占空比为50%	
	TIM_SetCounter(TIM8,0);//计数器清零
	TIM_Cmd(TIM8, ENABLE);  //使能TIM8
}
/********************************************
//相对定位函数 
//num 0～2147483647
//frequency: 20Hz~100KHz
//dir: CW(顺时针方向)  CCW(逆时针方向)
*********************************************/
void Locate_Rle(long num,u32 frequency,DIR_Type dir) //相对定位函数
{
	if(num<=0) //数值小等于0 则直接返回
	{
		printf("\r\nThe num should be greater than zero!!\r\n");
		return;
	}
	if(TIM8->CR1&0x01)//上一次脉冲还未发送完成  直接返回
	{
		printf("\r\nThe last time pulses is not send finished,wait please!\r\n");
		return;
	}
	if((frequency<20)||(frequency>250000))//脉冲频率不在范围内 直接返回
	{
		printf("\r\nThe frequency is out of range! please reset it!!(range:20Hz~100KHz)\r\n");
		return;
	}
	motor_dir=dir;//得到方向	
	DRIVER_DIR=motor_dir;//设置方向
	
	if(motor_dir==CW)//顺时针
		target_pos=current_pos+num;//目标位置
	else if(motor_dir==CCW)//逆时针
		target_pos=current_pos-num;//目标位置
	
	rcr_integer=num/(RCR_VAL+1);//重复计数整数部分
	rcr_remainder=num%(RCR_VAL+1);//重复计数余数部分
	is_rcr_finish=0;//重复计数器未设置完成
	TIM8_Startup(frequency);//开启TIM8
}
/********************************************
//绝对定位函数 
//num   -2147483648～2147483647
//frequency: 20Hz~100KHz
*********************************************/
void Locate_Abs(long num,u32 frequency)//绝对定位函数
{
	if(TIM8->CR1&0x01)//上一次脉冲还未发送完成 直接返回
	{
		printf("\r\nThe last time pulses is not send finished,wait please!\r\n");
		return;
	}
	if((frequency<20)||(frequency>100000))//脉冲频率不在范围内 直接返回
	{
		printf("\r\nThe frequency is out of range! please reset it!!(range:20Hz~100KHz)\r\n");
		return;
	}
	target_pos=num;//设置目标位置
	if(target_pos!=current_pos)//目标和当前位置不同
	{
		if(target_pos>current_pos)
			motor_dir=CW;//顺时针
		else
			motor_dir=CCW;//逆时针
		DRIVER_DIR=motor_dir;//设置方向
		
		rcr_integer=abs(target_pos-current_pos)/(RCR_VAL+1);//重复计数整数部分
		rcr_remainder=abs(target_pos-current_pos)%(RCR_VAL+1);//重复计数余数部分
		is_rcr_finish=0;//重复计数器未设置完成
		TIM8_Startup(frequency);//开启TIM8
	}
}



