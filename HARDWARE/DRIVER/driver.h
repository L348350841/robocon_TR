#ifndef __DRIVER_H
#define __DRIVER_H
#include "sys.h"
#include "stdlib.h"	

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//�������вο���̳�������̵�һ����(http://www.openedv.com/thread-41832-1-1.html)
//ALIENTEK STM32F407������
//������������� ���Դ���			   
//lycreturn@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2016/05/12
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved	
//********************************************************************************
//�޸�����:2016/05/12
//////////////////////////////////////////////////////////////////////////////////
/********** ������ �˿ڶ��� **************
//DRIVER_DIR   PE5 (DCMI_D6)
//DRIVER_OE    PE6 (DCMI_D7)
//STEP_PULSE   PC7 (TIM8_CH2  DCMI_D1)
******************************************/
#define DRIVER_DIR   PGout(4) // ��ת����
#define DRIVER_OE    PEout(6) // ʹ�ܽ� �͵�ƽ��Ч 
#define RCR_VAL    255  //ÿ������RCR_VAL+1���Σ��ж�һ�Σ����ֵ��0~255�����ô�һЩ���Խ����ж�Ƶ��


#define DRIVER2_DIR   PEout(7) // ��ת����
#define DRIVER2_OE    PEout(8) // ʹ�ܽ� �͵�ƽ��Ч 
#define RCR2_VAL    255  //ÿ������RCR_VAL+1���Σ��ж�һ�Σ����ֵ��0~255�����ô�
typedef enum
{
	CW = 1,//�ߵ�ƽ˳ʱ��
	CCW = 0,//�͵�ƽ��ʱ��
}DIR_Type;//���з���

extern long target_pos;//�з��ŷ���
extern long current_pos;//�з��ŷ���

extern long target2_pos;//�з��ŷ���
extern long current2_pos;//�з��ŷ���

void Driver_Init(void);//��������ʼ��
void TIM8_OPM_RCR_Init(u16 arr,u16 psc);//TIM8_CH2 ���������+�ظ��������ܳ�ʼ��
void TIM8_Startup(u32 frequency);   //������ʱ��8
void Locate_Rle(long num,u32 frequency,DIR_Type dir); //��Զ�λ����
void Locate_Abs(long num,u32 frequency);//���Զ�λ����


void Driver2_Init(void);//��������ʼ��
void TIM1_OPM_RCR_Init(u16 arr,u16 psc);//TIM8_CH2 ���������+�ظ��������ܳ�ʼ��
void TIM1_Startup(u32 frequency);   //������ʱ��8
void Locate2_Rle(long num,u32 frequency,DIR_Type dir); //��Զ�λ����
void Locate2_Abs(long num,u32 frequency);//���Զ�λ����
#endif


