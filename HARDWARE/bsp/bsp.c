/***********************************************************************************************************
 *  @file      bsp.c
 *  @version   1.0
 *  @date      2020.5.4
 *  @author    xb
 *  @brief     所有的初始化函数
 *  @copyright 2019 SDUST Robocon. All rights reserved.
 ***********************************************************************************************************/
#include "bsp.h"
/*************************************************************************************
 * @brief   总体初始化函数
 * @version 1.0
 * @date    2020.5.15
 * @author  xb
 * @param		none
 * @retval  none
 *************************************************************************************/
void Bsp_Init(void)
{
		delay_init(168);		  //初始化延时函数
	
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);	 //设置NVIC中断分组3
	
	
		//Close_Loop(0,0,0,1);
	
	
		Beep_Init ();		//蜂鸣器初始化
		Core_Led_Init();//主板LED初始化
		LCD_Init();			//LCD屏幕初始化
		#ifdef ReceiveDataFromNRF
		NRF_Init();			//NRF初始化
		#endif
		IIC_Init();			//IIC通信初始化
		Color_Init ();	//屏幕颜色初始化
		relay_init();  	//D12 D13 F1 F2 F3 F4 G2 G3
		
		PID_Struct_All_Init();//所有PID结构体初始化
		Motor_CAN_Init();			//底盘电机 RE40 初始化
		PF_Init();
	
//		PA7_Init();

		Driver_Init();
		//Zero_Init();//放后面不能用
		//One_Init();
		TIM8_OPM_RCR_Init(999, 168-1);
		
		
		
		
		
		
		USART1_Init(115200);
		USART3_Init(115200);	//串口1用于上位机调试和视觉接口
		#ifndef ReceiveDataFromNRF
		JUMPER_Init();			//sbus接收数据相关初始化
		#endif
		USART2_Init(115200);	//串口2用于艾克申定位，优先级0
		
		TIM7_Int_Init(100-1,8400-1);//定时器中断 10ms  抢占优先级   4
		
		
		
}

/*************************************************************************************
 * @brief   发射机构零点归位初始化---PG3
 * @version 1.0
 * @date    2020.7.24
 * @author  lg
 * @param		none
 * @retval  none
 *************************************************************************************/
static void Zero_Init(void)
{
	Locate_Abs(-200, 200);//void Locate_Abs(long num,u32 frequency)
	while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_3)==1);//GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
	Locate_Abs(100, 0);
}

/*************************************************************************************
 * @brief   发射机构每次上电给一个脉冲
 * @version 1.0
 * @date    2020.7.24
 * @author  lg
 * @param		none
 * @retval  none
 *************************************************************************************/
static void One_Init(void)
{
	Locate_Rle(1,200,CCW);
}


/*************************************************************************************
 * @brief   NRF模块初始化，用于遥控通信
 * @version 1.0
 * @date    2020.5.15
 * @author  xb
 * @param		none
 * @retval  none
 *************************************************************************************/
static void NRF_Init(void)
{
		//调试备用，无线遥控
		NRF24L01_Init();    		//初始化NRF24L01 
		while(NRF24L01_Check());
		NRF24L01_RX_Mode();
		delay_ms(500);
}

/*************************************************************************************
 * @brief   LED颜色初始化
 * @version 1.0
 * @date    2020.5.15
 * @author  xb
 * @param		none
 * @retval  none
 *************************************************************************************/
static void Core_Led_Init(void)
{
		GPIO_InitTypeDef  GPIO_InitStructure;

		//核心板指示灯
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;//LED0和LED1对应IO口
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
		GPIO_Init(GPIOG, &GPIO_InitStructure);//初始化GPIO

		GPIO_ResetBits(GPIOG,GPIO_Pin_15);
}

/*************************************************************************************
 * @brief   蜂鸣器初始化
 * @version 1.0
 * @date    2020.5.15
 * @author  xb
 * @param		none
 * @retval  none
 *************************************************************************************/
static void Beep_Init (void)
{
		GPIO_InitTypeDef  GPIO_InitStructure;
		
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIOF时钟
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//;//上拉
		GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化GPIO
			
		GPIO_ResetBits(GPIOF,GPIO_Pin_8);
}

/*************************************************************************************
 * @brief   LCD屏幕初始化
 * @version 1.0
 * @date    2020.5.15
 * @author  xb
 * @param		none
 * @retval  none
 *************************************************************************************/
static void Color_Init (void)
{
		GPIO_InitTypeDef  GPIO_InitStructure;

		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIOF时钟
	 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //对应引脚
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//浮空
		GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1; //对应引脚
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//下拉
		GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化	
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;//蓝色LED和红色LED对应IO口
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
		GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO

		GPIO_ResetBits(GPIOB,GPIO_Pin_8|GPIO_Pin_9);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//LED0和LED1对应IO口
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
		GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化GPIO

		GPIO_ResetBits(GPIOF,GPIO_Pin_10);
		
		delay_ms(10);	
}

/* PA7初始化 */
void PA7_Init()
{
		GPIO_InitTypeDef  GPIO_InitStructure;
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//蓝色LED和红色LED对应IO口
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
		GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO
	
}
	


/*************************************************************************************
 * @brief   底盘电机 RE40 CAN通信初始化
 * @version 1.0
 * @date    2020.5.4
 * @author  xb
 * @param		none
 * @retval  none
 *************************************************************************************/
static void Motor_CAN_Init(void)
{
		CAN1_Configuration();                               //CAN初始化   500KBS  1   2
		delay_ms(500);                                      //足够的延时，保证驱动器已经初始化完成
		
		CAN_RoboModule_DRV_Reset(0,0);                      //对0组所有驱动器进行 复位
		delay_ms(500);                                      //复位之后的必要延时
		
		CAN_RoboModule_DRV_Config(0,1,100,0);               //1号驱动器配置 100ms 传回一次数据
		delay_us(200);                                      //此处延时为了不让传回数据时候4个不一起传
		CAN_RoboModule_DRV_Config(0,2,100,0);               //2号驱动器配置 100ms 传回一次数据
		delay_us(200);                                      //此处延时为了不让传回数据时候4个不一起传
		CAN_RoboModule_DRV_Config(0,3,100,0);               //3号驱动器配置 100ms 传回一次数据
		delay_us(200);                                      //此处延时为了不让传回数据时候4个不一起传
		CAN_RoboModule_DRV_Config(0,4,100,0);               //4号驱动器配置 100ms 传回一次数据
		delay_us(200);                                      //此处延时为了不让传回数据时候4个不一起传
		CAN_RoboModule_DRV_Config(1,1,100,0);               //4号驱动器配置 100ms 传回一次数据
		delay_us(200);                                      //此处延时为了不让传回数据时候4个不一起传

		//CAN_RoboModule_DRV_Mode_Choice(0,0,Velocity_Mode);  //0组的所有驱动器 都进入速度模式                                    
		CAN_RoboModule_DRV_Mode_Choice(0,0,Current_Velocity_Mode);
		
		CAN_RoboModule_DRV_Mode_Choice(1,0,OpenLoop_Mode);	//1阻的所有驱动器 都进入开环模式
		delay_ms(500);  																		//发送模式选择指令后，要等待驱动器进入模式就绪。所以延时也不可以去掉。
}

static void PID_Struct_All_Init(void)
{   
		posx_PID.p=0.82;  
		posx_PID.i=0;  
		posx_PID.d=17;
	//修改KP为P
		angle_PID.p = 15;   //10
		angle_PID.i = 0;	  
		angle_PID.d = 1;	  //1
	
		//位置环PID
		PID_struct_init(	//x轴pid
		&posx_PID,
		POSITION_PID,
		600,  //输出最大值
		0,    //积分饱和值
		posx_PID.p,   //P   0.82
		posx_PID.i,	  //I		0
		posx_PID.d);	//D		 17
	
		PID_struct_init(	//y轴pid
		&posy_PID,
		POSITION_PID,
		600,  //输出最大值
		0,    //积分饱和值
		2,   //P   0.8
		0,	  //I	
		0.1);	//D   0.1
		
		
		
		
		//角度环PID    angle_PID,kp,ki,kd
		PID_struct_init(
		&angle_PID,
		POSITION_PID,
		400,  //输出最大值
		0,    //积分饱和值
		angle_PID.p,   //P
		angle_PID.i,	  //I	
		angle_PID.d);	//D
		
		//3508角度环和速度环
		/*********************************PID1******************************/
		//角度环参数
		PID_struct_init(&angle_PID_3508_1, 
		                POSITION_PID, 
		                15000, 
		                15000,
		                0.62f,	 //比例P  0.08f             //0.58
		                0.00f,	 //积分I  0.0f
		                1.7f    //微分D  0.03f 0.01  0.14   //1.34
                        );

        //速度环参数
		PID_struct_init(&speed_PID_3508_1, 
                        POSITION_PID, 
                        18000, 
												18000,
												0.45f,	 //比例P  1.5f   改2006 0.4f     // 1.45  2.3
                        0.00f,	 //积分I  0.1f 0.50.17		`			// 0.18  0.24
                        0.00f	 //微分D  0.01f               //0.001  0.4
                        );
												
												
		/*********************************PID2******************************/												
        //角度环参数
		PID_struct_init(&angle_PID_3508_2, 
		                POSITION_PID, 
		                15000, 
		                15000,
		                0.60f,	 //比例P  0.08f             //0.58
		                0.00f,	 //积分I  0.0f
		                1.9f    //微分D  0.03f 0.01  0.14   //1.34
                        );

        //速度环参数
		PID_struct_init(&speed_PID_3508_2, 
                        POSITION_PID, 
                        18000, 
												18000,
												0.48f,	 //比例P  1.5f   改2006 0.4f     // 1.45  2.3
                        0.08f,	 //积分I  0.1f 0.50.17		`			// 0.18  0.24
                        0.00f	 //微分D  0.01f               //0.001  0.4
                        );			




												
												
/*********************************PID3******************************/												
        //角度环参数
		PID_struct_init(&angle_PID_3508_3, 
		                POSITION_PID, 
		                15000, 
		                15000,
		                0.62f,	 //比例P  0.08f             //0.58
		                0.00f,	 //积分I  0.0f
		                1.7f    //微分D  0.03f 0.01  0.14   //1.34
                        );

        //速度环参数
		PID_struct_init(&speed_PID_3508_3, 
                        POSITION_PID, 
                        18000, 
												18000,
												0.48f,	 //比例P  1.5f   改2006 0.4f     // 1.45  2.3
                        0.05f,	 //积分I  0.1f 0.50.17		`			// 0.18  0.24
                        0.00f	 //微分D  0.01f               //0.001  0.4
                        );												
												
												
												
												
												
/*********************************PID4******************************/												
        //角度环参数
		PID_struct_init(&angle_PID_3508_4, 
		                POSITION_PID, 
		                15000, 
		                15000,
		                0.58f,	 //比例P  0.08f             //0.58
		                0.00f,	 //积分I  0.0f
		                1.9f    //微分D  0.03f 0.01  0.14   //1.34
                        );

        //速度环参数
		PID_struct_init(&speed_PID_3508_4, 
                        POSITION_PID, 
                        18000, 
												18000,
												0.60f,	 //比例P  1.5f   改2006 0.4f     // 1.45  2.3
                        0.00f,	 //积分I  0.1f 0.50.17		`			// 0.18  0.24
                        0.00f	 //微分D  0.01f               //0.001  0.4
                        );														
												
	



		//2006 角度环和速度环
		
		/*********************************PID1******************************/
		//		//角度环参数
		PID_struct_init(&angle_PID_2006_1, 
		                POSITION_PID, 
		                20000, 
		                20000,
		                0.15f,	 //比例P  0.07f 
		                0.00f,	 //积分I  0.0f
		                0.0f     //微分D  0.03f 0.01
                        );

   	 //速度环参数
		PID_struct_init(&speed_PID_2006_1, 
                        POSITION_PID, 
                        20000, 
						20000,
					     1.5f,	 //比例P  1.4f
                        0.0f,	 //积分I  0.1f 0.5
                        0.0f	 //微分D  0.0f
                        );
		
		/*********************************PID2******************************/
				PID_struct_init(&angle_PID_2006_2, 
		                POSITION_PID, 
		                10000, 
		                10000,
		                0.15f,	 //比例P  0.07f 
		                0.00f,	 //积分I  0.0f
		                0.0f     //微分D  0.03f 0.01
                        );

        //速度环参数
		PID_struct_init(&speed_PID_2006_2, 
                        POSITION_PID, 
                        20000, 
												20000,
					            	1.5f,	 //比例P  1.4f
                        0.0f,	 //积分I  0.1f 0.5
                        0.0f	 //微分D  0.0f
                        );
		/*********************************PID3******************************/
				PID_struct_init(&angle_PID_2006_3, 
		                POSITION_PID, 
		                4000, 
		                4000,
		                0.15f,	 //比例P  0.07f 
		                0.00f,	 //积分I  0.0f
		                0.0f     //微分D  0.03f 0.01
                        );

        //速度环参数
		PID_struct_init(&speed_PID_2006_3, 
                        POSITION_PID, 
                        20000, 
						20000,
					    1.5f,	 //比例P  1.4f
                        0.0f,	 //积分I  0.1f 0.5
                        0.0f	 //微分D  0.0f
                        );
}



/* PF初始化  主板PF0 1,2,3,4,5 */
void PF_Init()
{
		GPIO_InitTypeDef  GPIO_InitStructure;
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
		GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化GPIO
		
		//GPIO_ResetBits(GPIOF,GPIO_Pin_0);
		GPIO_ResetBits(GPIOF,GPIO_Pin_0);//
		GPIO_ResetBits(GPIOF,GPIO_Pin_1);
		GPIO_ResetBits(GPIOF,GPIO_Pin_2);
		GPIO_ResetBits(GPIOF,GPIO_Pin_3);
		GPIO_ResetBits(GPIOF,GPIO_Pin_4);
		
}
	




