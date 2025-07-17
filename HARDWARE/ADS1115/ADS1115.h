#ifndef __ADS1115_H
#define __ADS1115_H
#include "myiic.h"   

//ADS1115模块
/****************************************************************************************                               
*说明：当端输入时候正输入为输入信号，负输入为地但是输入信号不能为负电压(不能比地电位低)
*	   双端输入时候正输入为输入信号，负输入为负输入输入信号的差值可以为负电压
****************************************************************************************/
#define	 SDA_A1     PAout(13)=1   //SDA输出
#define	 SDA_A0     PAout(13)=0
#define	 SCL_A1	    PAout(14)=1    //SCL
#define	 SCL_A0	    PAout(14)=0
#define	 SDA_AI	    PAin(13)   //SDA读入


//I2C地址以及读写设置
#define  WR_REG 0x90       //写寄存器
#define  RE_REG 0x91       //读寄存器

/***********************************寄存器控制字**********************************************/
#define  DATA_REG  0x00		//转换数据寄存器
#define  CONF_REG  0x01   //控制字设置寄存器
#define  LOTH_REG  0x02		//最低阀值寄存器
#define  HITH_REG  0x03		//最高阀值寄存器

#define  ch0  0x42       //通道0
#define  ch1  0x52       //通道1
#define  ch2  0x62       //通道2
#define  ch3  0x72       //通道3

/***********************控制字申明*************************************************************
*|  OS | MUX2 | MUX1 | MUX0 | PGA2 | PGA1 | PGA0 | MODE  |------HCMD   高8位
*|  DR2| DR1  | DR0  | COMP_MODE | COMP_POL | COMP_LAT |  COMP_QUE1 | COMP_QUE0 |-----LCMD  低8位
***********************************************************************************************/
//#define  HCMD1    0x40   //AIN0单端输入 +-6.144量程  连续模式  0100|0000b
//#define  LCMD1	  0x83	 //128sps 无比较器模式 1000|0011b
#define  HCMD1    0x41   //AIN0单端输入 +-6.144量程  单次模式  0100|0001b
#define  LCMD1	  0x83	 //128sps 无比较器模式 1000|0011b

/************************函数申明****************************/
void delay_nms(u16 ms);
void delay_nus(u16 us);
u16 ReadAD_A(void);														 //读adc数据

//void SetThresHold_A(u16 L_TH,u16 H_TH);        //高低阀门设置

//可直接调用
u16 getadc(u16 channel);//配置该次模式及读取adc数据

#endif
















