#ifndef __MCP4725_H
#define __mcp4725_H
#include "myiic.h"   
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//24CXX���� ����(�ʺ�24C01~24C16,24C32~256δ��������!�д���֤!)		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/9
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

#define VREF_5V      4095


void MCP4725_Init(void);
void MCP4725_WriteData_Digital(u16 data);
void MCP4725_WriteData_Voltage(u16 Vout);
void MCP4725_WriteData_Eeprom(u16 data) ;  //12λ������
#endif
















