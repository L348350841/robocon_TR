#ifndef  _MUSART_H_
#define  _MUSART_H_

#include "sys.h"
#include "delay.h"
#include "stdio.h"

#ifndef ReceiveDataFromNRF
//���ż����ܺ궨�壬�޸����Ž����޸Ĵ˴����ɣ�������ֲ
//#define MCLOCK_SYS        RCC_APB2Periph_SYSCFG       //ϵͳʱ��
//#define MCLOCK_IO         RCC_AHB1Periph_GPIOC        //IOʱ��
//#define MGPIO             GPIOC                       //�շ�IO�˿�
//#define MTXD1             GPIO_Pin_10                 //IO-TXD
//#define MRXD1             GPIO_Pin_5                  //IO-RXD

#define MCLOCK_SYS        RCC_APB2Periph_SYSCFG       //ϵͳʱ��
#define MCLOCK_IO         RCC_AHB1Periph_GPIOC        //IOʱ��
#define MGPIO             GPIOG                       //�շ�IO�˿�
#define MTXD1             GPIO_Pin_10                 //IO-TXD
#define MRXD1             GPIO_Pin_5                  //IO-RXD


//ң��������

#define LX SBUS_CH[1]    //172-1810
#define RY SBUS_CH[2]
#define RX SBUS_CH[3]
#define SA change(SBUS_CH[4])
#define SB change(SBUS_CH[5])
#define SC change(SBUS_CH[6])
#define SD change(SBUS_CH[7])
#define SE change(SBUS_CH[8])
#define SF change(SBUS_CH[9])
#define SG change(SBUS_CH[10])
#define SH change(SBUS_CH[11])

//�յ�ƽ�궨��

#define RX_READ()   (MGPIO->IDR & MRXD1)

extern u8 USART_buf[25];
extern u8 len;
extern uint16_t sbus_ch[16]; 
extern int i ;
extern u16 SBUS_CH[16];
//ģ�⴮�ڵ�����λ״̬��
enum{
COM_START_BIT,  //��ʼλ
COM_D0_BIT,     //bit0
COM_D1_BIT,     //bit1
COM_D2_BIT,     //bit2
COM_D3_BIT,     //bit3
COM_D4_BIT,     //bit4
COM_D5_BIT,     //bit5
COM_D6_BIT,     //bit6
COM_D7_BIT,     //bit7
COM_D8_BIT,     //
COM_STOP_BIT,   //bit8
};


//��������
void JUMPER_Init(void);  //����ң������ʼ��
void READ_(void); // ͨ��ֵ��ȡ  ������ѭ����
u32 change(u16 num);
#endif
#endif

