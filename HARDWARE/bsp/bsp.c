/***********************************************************************************************************
 *  @file      bsp.c
 *  @version   1.0
 *  @date      2020.5.4
 *  @author    xb
 *  @brief     ���еĳ�ʼ������
 *  @copyright 2019 SDUST Robocon. All rights reserved.
 ***********************************************************************************************************/
#include "bsp.h"
/*************************************************************************************
 * @brief   �����ʼ������
 * @version 1.0
 * @date    2020.5.15
 * @author  xb
 * @param		none
 * @retval  none
 *************************************************************************************/
void Bsp_Init(void)
{
		delay_init(168);		  //��ʼ����ʱ����
	
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);	 //����NVIC�жϷ���3
	
	
		//Close_Loop(0,0,0,1);
	
	
		Beep_Init ();		//��������ʼ��
		Core_Led_Init();//����LED��ʼ��
		LCD_Init();			//LCD��Ļ��ʼ��
		#ifdef ReceiveDataFromNRF
		NRF_Init();			//NRF��ʼ��
		#endif
		IIC_Init();			//IICͨ�ų�ʼ��
		Color_Init ();	//��Ļ��ɫ��ʼ��
		relay_init();  	//D12 D13 F1 F2 F3 F4 G2 G3
		
		PID_Struct_All_Init();//����PID�ṹ���ʼ��
		Motor_CAN_Init();			//���̵�� RE40 ��ʼ��
		PF_Init();
	
//		PA7_Init();

		Driver_Init();
		//Zero_Init();//�ź��治����
		//One_Init();
		TIM8_OPM_RCR_Init(999, 168-1);
		
		
		
		
		
		
		USART1_Init(115200);
		USART3_Init(115200);	//����1������λ�����Ժ��Ӿ��ӿ�
		#ifndef ReceiveDataFromNRF
		JUMPER_Init();			//sbus����������س�ʼ��
		#endif
		USART2_Init(115200);	//����2���ڰ����궨λ�����ȼ�0
		
		TIM7_Int_Init(100-1,8400-1);//��ʱ���ж� 10ms  ��ռ���ȼ�   4
		
		
		
}

/*************************************************************************************
 * @brief   �����������λ��ʼ��---PG3
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
 * @brief   �������ÿ���ϵ��һ������
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
 * @brief   NRFģ���ʼ��������ң��ͨ��
 * @version 1.0
 * @date    2020.5.15
 * @author  xb
 * @param		none
 * @retval  none
 *************************************************************************************/
static void NRF_Init(void)
{
		//���Ա��ã�����ң��
		NRF24L01_Init();    		//��ʼ��NRF24L01 
		while(NRF24L01_Check());
		NRF24L01_RX_Mode();
		delay_ms(500);
}

/*************************************************************************************
 * @brief   LED��ɫ��ʼ��
 * @version 1.0
 * @date    2020.5.15
 * @author  xb
 * @param		none
 * @retval  none
 *************************************************************************************/
static void Core_Led_Init(void)
{
		GPIO_InitTypeDef  GPIO_InitStructure;

		//���İ�ָʾ��
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;//LED0��LED1��ӦIO��
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
		GPIO_Init(GPIOG, &GPIO_InitStructure);//��ʼ��GPIO

		GPIO_ResetBits(GPIOG,GPIO_Pin_15);
}

/*************************************************************************************
 * @brief   ��������ʼ��
 * @version 1.0
 * @date    2020.5.15
 * @author  xb
 * @param		none
 * @retval  none
 *************************************************************************************/
static void Beep_Init (void)
{
		GPIO_InitTypeDef  GPIO_InitStructure;
		
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//ʹ��GPIOFʱ��
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//;//����
		GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��GPIO
			
		GPIO_ResetBits(GPIOF,GPIO_Pin_8);
}

/*************************************************************************************
 * @brief   LCD��Ļ��ʼ��
 * @version 1.0
 * @date    2020.5.15
 * @author  xb
 * @param		none
 * @retval  none
 *************************************************************************************/
static void Color_Init (void)
{
		GPIO_InitTypeDef  GPIO_InitStructure;

		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOF, ENABLE);//ʹ��GPIOFʱ��
	 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //��Ӧ����
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//����
		GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1; //��Ӧ����
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//����
		GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��	
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;//��ɫLED�ͺ�ɫLED��ӦIO��
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
		GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO

		GPIO_ResetBits(GPIOB,GPIO_Pin_8|GPIO_Pin_9);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//LED0��LED1��ӦIO��
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
		GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��GPIO

		GPIO_ResetBits(GPIOF,GPIO_Pin_10);
		
		delay_ms(10);	
}

/* PA7��ʼ�� */
void PA7_Init()
{
		GPIO_InitTypeDef  GPIO_InitStructure;
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//��ɫLED�ͺ�ɫLED��ӦIO��
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
		GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO
	
}
	


/*************************************************************************************
 * @brief   ���̵�� RE40 CANͨ�ų�ʼ��
 * @version 1.0
 * @date    2020.5.4
 * @author  xb
 * @param		none
 * @retval  none
 *************************************************************************************/
static void Motor_CAN_Init(void)
{
		CAN1_Configuration();                               //CAN��ʼ��   500KBS  1   2
		delay_ms(500);                                      //�㹻����ʱ����֤�������Ѿ���ʼ�����
		
		CAN_RoboModule_DRV_Reset(0,0);                      //��0���������������� ��λ
		delay_ms(500);                                      //��λ֮��ı�Ҫ��ʱ
		
		CAN_RoboModule_DRV_Config(0,1,100,0);               //1������������ 100ms ����һ������
		delay_us(200);                                      //�˴���ʱΪ�˲��ô�������ʱ��4����һ��
		CAN_RoboModule_DRV_Config(0,2,100,0);               //2������������ 100ms ����һ������
		delay_us(200);                                      //�˴���ʱΪ�˲��ô�������ʱ��4����һ��
		CAN_RoboModule_DRV_Config(0,3,100,0);               //3������������ 100ms ����һ������
		delay_us(200);                                      //�˴���ʱΪ�˲��ô�������ʱ��4����һ��
		CAN_RoboModule_DRV_Config(0,4,100,0);               //4������������ 100ms ����һ������
		delay_us(200);                                      //�˴���ʱΪ�˲��ô�������ʱ��4����һ��
		CAN_RoboModule_DRV_Config(1,1,100,0);               //4������������ 100ms ����һ������
		delay_us(200);                                      //�˴���ʱΪ�˲��ô�������ʱ��4����һ��

		//CAN_RoboModule_DRV_Mode_Choice(0,0,Velocity_Mode);  //0������������� �������ٶ�ģʽ                                    
		CAN_RoboModule_DRV_Mode_Choice(0,0,Current_Velocity_Mode);
		
		CAN_RoboModule_DRV_Mode_Choice(1,0,OpenLoop_Mode);	//1������������� �����뿪��ģʽ
		delay_ms(500);  																		//����ģʽѡ��ָ���Ҫ�ȴ�����������ģʽ������������ʱҲ������ȥ����
}

static void PID_Struct_All_Init(void)
{   
		posx_PID.p=0.82;  
		posx_PID.i=0;  
		posx_PID.d=17;
	//�޸�KPΪP
		angle_PID.p = 15;   //10
		angle_PID.i = 0;	  
		angle_PID.d = 1;	  //1
	
		//λ�û�PID
		PID_struct_init(	//x��pid
		&posx_PID,
		POSITION_PID,
		600,  //������ֵ
		0,    //���ֱ���ֵ
		posx_PID.p,   //P   0.82
		posx_PID.i,	  //I		0
		posx_PID.d);	//D		 17
	
		PID_struct_init(	//y��pid
		&posy_PID,
		POSITION_PID,
		600,  //������ֵ
		0,    //���ֱ���ֵ
		2,   //P   0.8
		0,	  //I	
		0.1);	//D   0.1
		
		
		
		
		//�ǶȻ�PID    angle_PID,kp,ki,kd
		PID_struct_init(
		&angle_PID,
		POSITION_PID,
		400,  //������ֵ
		0,    //���ֱ���ֵ
		angle_PID.p,   //P
		angle_PID.i,	  //I	
		angle_PID.d);	//D
		
		//3508�ǶȻ����ٶȻ�
		/*********************************PID1******************************/
		//�ǶȻ�����
		PID_struct_init(&angle_PID_3508_1, 
		                POSITION_PID, 
		                15000, 
		                15000,
		                0.62f,	 //����P  0.08f             //0.58
		                0.00f,	 //����I  0.0f
		                1.7f    //΢��D  0.03f 0.01  0.14   //1.34
                        );

        //�ٶȻ�����
		PID_struct_init(&speed_PID_3508_1, 
                        POSITION_PID, 
                        18000, 
												18000,
												0.45f,	 //����P  1.5f   ��2006 0.4f     // 1.45  2.3
                        0.00f,	 //����I  0.1f 0.50.17		`			// 0.18  0.24
                        0.00f	 //΢��D  0.01f               //0.001  0.4
                        );
												
												
		/*********************************PID2******************************/												
        //�ǶȻ�����
		PID_struct_init(&angle_PID_3508_2, 
		                POSITION_PID, 
		                15000, 
		                15000,
		                0.60f,	 //����P  0.08f             //0.58
		                0.00f,	 //����I  0.0f
		                1.9f    //΢��D  0.03f 0.01  0.14   //1.34
                        );

        //�ٶȻ�����
		PID_struct_init(&speed_PID_3508_2, 
                        POSITION_PID, 
                        18000, 
												18000,
												0.48f,	 //����P  1.5f   ��2006 0.4f     // 1.45  2.3
                        0.08f,	 //����I  0.1f 0.50.17		`			// 0.18  0.24
                        0.00f	 //΢��D  0.01f               //0.001  0.4
                        );			




												
												
/*********************************PID3******************************/												
        //�ǶȻ�����
		PID_struct_init(&angle_PID_3508_3, 
		                POSITION_PID, 
		                15000, 
		                15000,
		                0.62f,	 //����P  0.08f             //0.58
		                0.00f,	 //����I  0.0f
		                1.7f    //΢��D  0.03f 0.01  0.14   //1.34
                        );

        //�ٶȻ�����
		PID_struct_init(&speed_PID_3508_3, 
                        POSITION_PID, 
                        18000, 
												18000,
												0.48f,	 //����P  1.5f   ��2006 0.4f     // 1.45  2.3
                        0.05f,	 //����I  0.1f 0.50.17		`			// 0.18  0.24
                        0.00f	 //΢��D  0.01f               //0.001  0.4
                        );												
												
												
												
												
												
/*********************************PID4******************************/												
        //�ǶȻ�����
		PID_struct_init(&angle_PID_3508_4, 
		                POSITION_PID, 
		                15000, 
		                15000,
		                0.58f,	 //����P  0.08f             //0.58
		                0.00f,	 //����I  0.0f
		                1.9f    //΢��D  0.03f 0.01  0.14   //1.34
                        );

        //�ٶȻ�����
		PID_struct_init(&speed_PID_3508_4, 
                        POSITION_PID, 
                        18000, 
												18000,
												0.60f,	 //����P  1.5f   ��2006 0.4f     // 1.45  2.3
                        0.00f,	 //����I  0.1f 0.50.17		`			// 0.18  0.24
                        0.00f	 //΢��D  0.01f               //0.001  0.4
                        );														
												
	



		//2006 �ǶȻ����ٶȻ�
		
		/*********************************PID1******************************/
		//		//�ǶȻ�����
		PID_struct_init(&angle_PID_2006_1, 
		                POSITION_PID, 
		                20000, 
		                20000,
		                0.15f,	 //����P  0.07f 
		                0.00f,	 //����I  0.0f
		                0.0f     //΢��D  0.03f 0.01
                        );

   	 //�ٶȻ�����
		PID_struct_init(&speed_PID_2006_1, 
                        POSITION_PID, 
                        20000, 
						20000,
					     1.5f,	 //����P  1.4f
                        0.0f,	 //����I  0.1f 0.5
                        0.0f	 //΢��D  0.0f
                        );
		
		/*********************************PID2******************************/
				PID_struct_init(&angle_PID_2006_2, 
		                POSITION_PID, 
		                10000, 
		                10000,
		                0.15f,	 //����P  0.07f 
		                0.00f,	 //����I  0.0f
		                0.0f     //΢��D  0.03f 0.01
                        );

        //�ٶȻ�����
		PID_struct_init(&speed_PID_2006_2, 
                        POSITION_PID, 
                        20000, 
												20000,
					            	1.5f,	 //����P  1.4f
                        0.0f,	 //����I  0.1f 0.5
                        0.0f	 //΢��D  0.0f
                        );
		/*********************************PID3******************************/
				PID_struct_init(&angle_PID_2006_3, 
		                POSITION_PID, 
		                4000, 
		                4000,
		                0.15f,	 //����P  0.07f 
		                0.00f,	 //����I  0.0f
		                0.0f     //΢��D  0.03f 0.01
                        );

        //�ٶȻ�����
		PID_struct_init(&speed_PID_2006_3, 
                        POSITION_PID, 
                        20000, 
						20000,
					    1.5f,	 //����P  1.4f
                        0.0f,	 //����I  0.1f 0.5
                        0.0f	 //΢��D  0.0f
                        );
}



/* PF��ʼ��  ����PF0 1,2,3,4,5 */
void PF_Init()
{
		GPIO_InitTypeDef  GPIO_InitStructure;
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
		GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��GPIO
		
		//GPIO_ResetBits(GPIOF,GPIO_Pin_0);
		GPIO_ResetBits(GPIOF,GPIO_Pin_0);//
		GPIO_ResetBits(GPIOF,GPIO_Pin_1);
		GPIO_ResetBits(GPIOF,GPIO_Pin_2);
		GPIO_ResetBits(GPIOF,GPIO_Pin_3);
		GPIO_ResetBits(GPIOF,GPIO_Pin_4);
		
}
	




