#ifndef  _MUSART_H_
#define  _MUSART_H_

#include "sys.h"
#include "delay.h"
#include "stdio.h"

#ifndef ReceiveDataFromNRF
//引脚及功能宏定义，修改引脚仅需修改此处即可，方便移植
//#define MCLOCK_SYS        RCC_APB2Periph_SYSCFG       //系统时钟
//#define MCLOCK_IO         RCC_AHB1Periph_GPIOC        //IO时钟
//#define MGPIO             GPIOC                       //收发IO端口
//#define MTXD1             GPIO_Pin_10                 //IO-TXD
//#define MRXD1             GPIO_Pin_5                  //IO-RXD

#define MCLOCK_SYS        RCC_APB2Periph_SYSCFG       //系统时钟
#define MCLOCK_IO         RCC_AHB1Periph_GPIOC        //IO时钟
#define MGPIO             GPIOG                       //收发IO端口
#define MTXD1             GPIO_Pin_10                 //IO-TXD
#define MRXD1             GPIO_Pin_5                  //IO-RXD


//遥控器按键

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

//收电平宏定义

#define RX_READ()   (MGPIO->IDR & MRXD1)

extern u8 USART_buf[25];
extern u8 len;
extern uint16_t sbus_ch[16]; 
extern int i ;
extern u16 SBUS_CH[16];
//模拟串口的数据位状态机
enum{
COM_START_BIT,  //起始位
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


//函数声明
void JUMPER_Init(void);  //整体遥控器初始化
void READ_(void); // 通道值读取  放入主循环中
u32 change(u16 num);
#endif
#endif

