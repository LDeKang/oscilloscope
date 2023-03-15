#include "systick.h"

uint16_t Systick_Cnt=0;


void Systick_Init_Config(void)
{
	SysTick_Config(72000);
	
}


void Systick_Count_Fun(void)
{
 Systick_Cnt++;
}


void Sys_Del_Ms(uint16_t Ms)
{
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;  
  Systick_Cnt=0;
	while(Systick_Cnt<Ms);
	Systick_Cnt=0;	
	SysTick->CTRL |= (~SysTick_CTRL_ENABLE_Msk); 
}
