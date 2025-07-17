#ifndef __MCP4725_H
#define __mcp4725_H
#include "myiic.h"   
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

#define VREF_5V      4095


void MCP4725_Init(void);
void MCP4725_WriteData_Digital(u16 data);
void MCP4725_WriteData_Voltage(u16 Vout);
void MCP4725_WriteData_Eeprom(u16 data) ;  //12位数字量
#endif
















