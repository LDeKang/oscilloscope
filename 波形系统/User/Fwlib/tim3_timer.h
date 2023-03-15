#ifndef __TIM3_TIMER_H
#define __TIM3_TIMER_H	
#include "stm32f10x.h"
 
void Tim3_Timer_Config(uint16_t Period,uint16_t Prescaler,FunctionalState NewState);

#endif
