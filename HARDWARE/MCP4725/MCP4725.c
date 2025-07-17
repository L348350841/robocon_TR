#include "MCP4725.h" 
#include "delay.h"
#include "usart.h"	
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//24CXX驱动 代码(适合24C01~24C16,24C32~256未经过测试!有待验证!)		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/9
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

//初始化IIC接口
void MCP4725_Init(void)
{
		IIC_Init();
}

//下面这个程序没什么用
#if 0
//使用快速模式写命令写DAC寄存器
void MCP4725_WriteData_Voltage(u16 Vout)   //电压单位mV
{
		u8 temp;
		u16 Dn;
		Dn = ( 4096 * Vout) / VREF_5V;
		temp = (0x0F00 & Dn) >> 8;  //12位数据

		IIC_Start();
		IIC_Send_Byte(0Xc0);      //器件寻址，器件代吗：1100； 地址位A2，A1，A0为 0 ， 0 ， 1；-> 1100 0010
		IIC_Wait_Ack();	 
		IIC_Send_Byte(temp); 	
		IIC_Wait_Ack();	 
		IIC_Send_Byte(Dn);        //将低8位送到DAC寄存器
		IIC_Wait_Ack();	
		IIC_Stop();//产生一个停止条件  	
		delay_ms(10);	
}
 
 #endif
 
void MCP4725_WriteData_Digital(u16 data)   //快速写dac ， 12位数字量 , 最大4095
{
		u8 data_H=0,data_L=0;
		data_H = ( 0x0F00 & data) >> 8;
		data_L = 0X00FF & data ;
		IIC_Start();
		IIC_Send_Byte(0XC0);      //器件寻址，器件代吗：1100； 地址位A2，A1，A0为 0 ， 0 ， 0；-> 1100 0000
		IIC_Wait_Ack();	 
		IIC_Send_Byte(data_H); 	
		IIC_Wait_Ack();	 
		IIC_Send_Byte(data_L);
		IIC_Wait_Ack();	

		IIC_Send_Byte(data_H); 	
		IIC_Wait_Ack();	 
		IIC_Send_Byte(data_L);
		IIC_Wait_Ack();	

		IIC_Stop();//产生一个停止条件  	
		delay_ms(10);	
}

void MCP4725_WriteData_Eeprom(u16 data)   //写入dac寄存机立刻输出，并且写入eeprom储存输出电压（写入eeprom的电压会在下次上电立刻输出）
{
		u8 data_H=0,data_L=0;
		data_H = ( 0x0Ff0 & data) >> 4;
		data_L = (0X00f & data)<<4 ;
		IIC_Start();
		IIC_Send_Byte(0XC0);      //器件寻址，器件代吗：1100； 地址位A2，A1，A0为 0 ， 0 ， 0；-> 1100 0000
		IIC_Wait_Ack();
		IIC_Send_Byte(0X60); 	
		IIC_Wait_Ack();	 
		IIC_Send_Byte(data_H); 	
		IIC_Wait_Ack();	 
		IIC_Send_Byte(data_L);
		IIC_Wait_Ack();	
		IIC_Send_Byte(0X60); 	
		IIC_Wait_Ack();	
		IIC_Send_Byte(data_H); 	
		IIC_Wait_Ack();	 
		IIC_Send_Byte(data_L);
		IIC_Wait_Ack();	
		IIC_Stop();//产生一个停止条件  	
		delay_ms(10);	
}

