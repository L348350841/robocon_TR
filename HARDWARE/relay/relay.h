#ifndef _RELAY_H
#define _RELAY_H

#include "sys.h"

#define relay_1  PFout(2)
#define relay_2  PFout(1)
#define relay_3  PFout(3)
#define relay_4  PDout(13)
#define relay_5  PDout(12) //PGout(3)
#define relay_6  PGout(2)
#define relay_7  PFout(4)
//#define relay_8  PFout(8)

#define platform_up   relay_1=0
#define platform_down relay_1=1

#define unlock_platform relay_7=0
#define lock_platform   relay_7=1

#define platform_out  relay_2=0
#define platform_back relay_2=1

#define grasp_out     relay_3=0 
#define grasp_back    relay_3=1

#define grasp_up      relay_4=0
#define grasp_down    relay_4=1

#define grasp_hold    relay_5=0
#define grasp_release relay_5=1

#define launch_out    relay_6=0
#define launch_back   relay_6=1

#define RED_X_REACHED 		!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12) 
#define BLUE_X_REACHED 		!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)	
#define beep_on        GPIO_SetBits(GPIOF,GPIO_Pin_8)
#define beep_off       GPIO_ResetBits(GPIOF,GPIO_Pin_8)
void relay_init(void);
void optoelectronic_init (void);



#endif
