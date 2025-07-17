#include "musart.h"
#include "sys.h"
#include "ALLPara.h"

#ifndef ReceiveDataFromNRF
static u32 recvStat = COM_STOP_BIT;     // ����״̬��
static u16 recvData = 0;                 // ����һ���������ݱ���

//u8 USART_buf[25]={0};

/*!
 * @brief ģ�⴮��1 RX IO������
 * @param	none
 * @return	none
 * @note	Rx(PA9)
 * @author ��˧
 */
void MUSART1_RX_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
 
    RCC_APB2PeriphClockCmd(MCLOCK_SYS, ENABLE);//!<�ⲿ�ж�ʱ��
    RCC_AHB1PeriphClockCmd(MCLOCK_IO,ENABLE); //ʹ��GPIOAʱ��

    GPIO_InitStructure.GPIO_Pin = MRXD1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(MGPIO, &GPIO_InitStructure);

    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOG,EXTI_PinSource5);

    EXTI_InitStructure.EXTI_Line=EXTI_Line5;
    EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;//�½����ж�
    EXTI_InitStructure.EXTI_LineCmd=ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel=EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x2; //��ռ���ȼ� 2��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x1;
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*!
 * @brief	��ʱ��14��ʼ��
 * @param   ��ʱ��14��λ��ʱ��=168M/2=84M
 * @return	NONE
 * @note	8us��ʱ��,���ڴ������ݲ���
 * @author ��˧
 */
void Time14Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimerBaseStruct;
    NVIC_InitTypeDef NVIC_InitStructure;
 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);//!<ʱ��ʹ��
    TIM_DeInit(TIM14);	//TIM14��ʼ��ΪĬ��ֵ
    TIM_TimerBaseStruct.TIM_Period=arr;//!<�������ؼĴ�����ֵ (����Ϊ7��������ʱ8us)
    TIM_TimerBaseStruct.TIM_Prescaler=psc; //!<ʹ��42Mʱ�ӣ�T=(83+1)*(7+1) /84= 8us
    TIM_TimerBaseStruct.TIM_ClockDivision=0;	//��Ƶϵ��  0������Ƶ��1����Ƶϵ��Ϊ1
    TIM_TimerBaseStruct.TIM_CounterMode=TIM_CounterMode_Up; //!<���ü��������ϼ���ģʽ
    TIM_TimeBaseInit(TIM14,&TIM_TimerBaseStruct);
    
    
    NVIC_InitStructure.NVIC_IRQChannel=TIM8_TRG_COM_TIM14_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;//��ʱ�����ȼ�һ��Ҫ���
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    
    TIM_ClearITPendingBit(TIM14, TIM_FLAG_Update);
    TIM_ITConfig(TIM14,TIM_IT_Update,ENABLE);       //!<ʹ��TIM1�ж�Դ
    TIM_Cmd(TIM14,DISABLE);                     	//!<����TIM1��ʱ��
}

/*!
 * @brief	���ڽ���IO�жϴ�����
 * @param	none
 * @return	NONE
 * @note	none
 * @author ��˧
 */
void EXTI9_5_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line5) != RESET) 
    {
        if(RX_READ() == 0)
        {
            if(recvStat == COM_STOP_BIT)
            {
                recvStat = COM_START_BIT;
                TIM_Cmd(TIM14, ENABLE);
            }
        }
        EXTI_ClearITPendingBit(EXTI_Line5);
    }
}


/*!
 * @brief	��ʱ��14�жϴ�����
 * @param
 * @return	NONE
 * @note
 * @author ��˧
 */
u8 count=0;
u8 len = 0;

void TIM8_TRG_COM_TIM14_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM14, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM14, TIM_FLAG_Update);	
        recvStat++;
        if(recvStat == COM_STOP_BIT)
        {
            TIM_Cmd(TIM14, DISABLE);
			if(recvData>=256)
			{
				recvData =  recvData-256;
			}
			USART_buf[len++] = recvData;
			if(len>24)
			{
				len=0;
				READ_();
				
			}
			
			
			if(USART_buf[0] !=15)
			{
			  len = 0;
			}
						
						
         }
        if(RX_READ())		
        {		  
            recvData |= (1 << (recvStat - 1));
        }
        else
        {
            recvData &= ~(1 << (recvStat - 1));
        }	
    }		
}

//SBUSЭ��
/*!
 * @brief	sbus���ݴ���
 * @param
 * @return	NONE
 * @note
 * @author ��˧
 */
//uint16_t sbus_ch[16];

void Sbus_Data_Count(uint8_t *buf)
{
	      sbus_ch[0] = ((int16_t)buf[ 1] >> 0 | ((int16_t)buf[ 2] << 8 )) & 0x07FF;
        sbus_ch[1] = ((int16_t)buf[ 2] >> 3 | ((int16_t)buf[ 3] << 5 )) & 0x07FF;
        sbus_ch[2] = ((int16_t)buf[ 3] >> 6 | ((int16_t)buf[ 4] << 2 ) | (int16_t)buf[ 5] << 10 ) & 0x07FF;
        sbus_ch[3] = ((int16_t)buf[ 5] >> 1 | ((int16_t)buf[ 6] << 7 )) & 0x07FF;
        sbus_ch[4] = ((int16_t)buf[ 6] >> 4 | ((int16_t)buf[ 7] << 4 )) & 0x07FF;
        sbus_ch[5] = ((int16_t)buf[ 7] >> 7 | ((int16_t)buf[ 8] << 1 ) | (int16_t)buf[9] << 9 ) & 0x07FF;
        sbus_ch[6]= ((int16_t)buf[ 9] >> 2 | ((int16_t)buf[10] << 6 )) & 0x07FF;
        sbus_ch[7] = ((int16_t)buf[10] >> 5 | ((int16_t)buf[11] << 3 )) & 0x07FF;
        sbus_ch[8]= ((int16_t)buf[12] << 0 | ((int16_t)buf[13] << 8 )) & 0x07FF;
        sbus_ch[9] = ((int16_t)buf[13] >> 3 | ((int16_t)buf[14] << 5 )) & 0x07FF;
        sbus_ch[10] = ((int16_t)buf[14] >> 6 | ((int16_t)buf[15] << 2 ) | (int16_t)buf[16] << 10 ) & 0x07FF;
        sbus_ch[11] = ((int16_t)buf[16] >> 1 | ((int16_t)buf[17] << 7 )) & 0x07FF;
        sbus_ch[12] = ((int16_t)buf[17] >> 4 | ((int16_t)buf[18] << 4 )) & 0x07FF;
        sbus_ch[13] = ((int16_t)buf[18] >> 7 | ((int16_t)buf[19] << 1 ) | (int16_t)buf[20] << 9 ) & 0x07FF;
        sbus_ch[14] = ((int16_t)buf[20] >> 2 | ((int16_t)buf[21] << 6 )) & 0x07FF;
        sbus_ch[15] = ((int16_t)buf[21] >> 5 | ((int16_t)buf[22] << 3 )) & 0x07FF;

}
	
		
//���ݶ�ȡ
/*!
 * @brief	��ȡsbus����
 * @param
 * @return	NONE
 * @note
 * @author ��˧
 */
int i = 0;
u16 SBUS_CH[16];

void READ_()
{
	if (USART_buf[0] == 0x0F &&  USART_buf [23]==0 && USART_buf [24] == 0 )
	{ 
		Sbus_Data_Count(USART_buf);
		if(sbus_ch[10]<2000)
		{
			for(i =0;i<16;i++)
			{
			  SBUS_CH[i] = sbus_ch[i];
			}
		}
		if(SE == 1 || SE == 2)
        {
		    for(i =0;i<16;i++)
			{
			  SBUS_CH[i] = 0;
			}
		
		}
	}
 
}
 
 
 //����ת��
 u32 change(u16 num)
{
   if(num==172)
		 num = 1;
	 else if(num==992)
		 num = 2;
	 else if(num==1810)
		 num = 3;
   else
		 num = num;

   return num;


}

//ң������ʼ�� �������� ��ʱ��
 
void JUMPER_Init()
{
	MUSART1_RX_init();
	
	Time14Init(11-1,84-1);
}

#endif

