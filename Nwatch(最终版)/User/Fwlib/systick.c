#include "systick.h"
#include "common.h"

uint32_t Systick_Cnt=0;


void Systick_Init_Config(void)
{
	SysTick_Config(72000);
	NVIC_SetPriority (SysTick_IRQn, 1);
}


void Systick_Count_Fun(void)
{
 Systick_Cnt++;
}


void Sys_Del_Ms(uint16_t Ms)
{
	uint32_t Now;
	Now=Systick_Cnt;
	while(Systick_Cnt<(Now+Ms));
}


// Get current milliseconds
uint32_t millis_get(void)
{
	uint32_t ms;
	
  ms = Systick_Cnt;
	
	return ms;
}
