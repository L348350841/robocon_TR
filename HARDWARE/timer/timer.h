#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"

#define reset_zhuan 		GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)//PE3 
#define floor_limit 		GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)	//PE3 
#define up_limit 		    GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_3)	//PD3 

#define pul PAout(5)	// DS1	
#define dirct PDout(13)	// DS1


void TIM7_Int_Init(u16 arr,u16 psc);
void TIM7_IRQHandler(void);
void TIM3_Int_Init(u16 arr,u16 psc);



#endif

