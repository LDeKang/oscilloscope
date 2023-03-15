#ifndef __SYSTICK_H
#define __SYSTICK_H	
#include "stm32f10x.h"


extern uint16_t Systick_Cnt;

void Systick_Init_Config(void);
void Systick_Count_Fun(void);
void Sys_Del_Ms(uint16_t Ms);

#endif
