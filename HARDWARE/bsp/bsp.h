#ifndef _BSP_H
#define _BSP_H

#include "sys.h"
#include "public.h"


void Bsp_Init(void);
static void NRF_Init(void);
static void Color_Init (void);
static void Beep_Init (void);
static void Core_Led_Init(void);

void PA7_Init(void);

static void Motor_CAN_Init(void);
static void PID_Struct_All_Init(void);

void PF_Init(void);
static void Zero_Init(void);
static void One_Init(void);
#endif

