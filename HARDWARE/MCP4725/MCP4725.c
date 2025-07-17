#include "MCP4725.h" 
#include "delay.h"
#include "usart.h"	
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

//��ʼ��IIC�ӿ�
void MCP4725_Init(void)
{
		IIC_Init();
}

//�����������ûʲô��
#if 0
//ʹ�ÿ���ģʽд����дDAC�Ĵ���
void MCP4725_WriteData_Voltage(u16 Vout)   //��ѹ��λmV
{
		u8 temp;
		u16 Dn;
		Dn = ( 4096 * Vout) / VREF_5V;
		temp = (0x0F00 & Dn) >> 8;  //12λ����

		IIC_Start();
		IIC_Send_Byte(0Xc0);      //����Ѱַ����������1100�� ��ַλA2��A1��A0Ϊ 0 �� 0 �� 1��-> 1100 0010
		IIC_Wait_Ack();	 
		IIC_Send_Byte(temp); 	
		IIC_Wait_Ack();	 
		IIC_Send_Byte(Dn);        //����8λ�͵�DAC�Ĵ���
		IIC_Wait_Ack();	
		IIC_Stop();//����һ��ֹͣ����  	
		delay_ms(10);	
}
 
 #endif
 
void MCP4725_WriteData_Digital(u16 data)   //����дdac �� 12λ������ , ���4095
{
		u8 data_H=0,data_L=0;
		data_H = ( 0x0F00 & data) >> 8;
		data_L = 0X00FF & data ;
		IIC_Start();
		IIC_Send_Byte(0XC0);      //����Ѱַ����������1100�� ��ַλA2��A1��A0Ϊ 0 �� 0 �� 0��-> 1100 0000
		IIC_Wait_Ack();	 
		IIC_Send_Byte(data_H); 	
		IIC_Wait_Ack();	 
		IIC_Send_Byte(data_L);
		IIC_Wait_Ack();	

		IIC_Send_Byte(data_H); 	
		IIC_Wait_Ack();	 
		IIC_Send_Byte(data_L);
		IIC_Wait_Ack();	

		IIC_Stop();//����һ��ֹͣ����  	
		delay_ms(10);	
}

void MCP4725_WriteData_Eeprom(u16 data)   //д��dac�Ĵ���������������д��eeprom���������ѹ��д��eeprom�ĵ�ѹ�����´��ϵ����������
{
		u8 data_H=0,data_L=0;
		data_H = ( 0x0Ff0 & data) >> 4;
		data_L = (0X00f & data)<<4 ;
		IIC_Start();
		IIC_Send_Byte(0XC0);      //����Ѱַ����������1100�� ��ַλA2��A1��A0Ϊ 0 �� 0 �� 0��-> 1100 0000
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
		IIC_Stop();//����һ��ֹͣ����  	
		delay_ms(10);	
}

