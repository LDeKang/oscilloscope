#ifndef __TIM4_H
#define __TIM4_H	
#include "stm32f10x.h"

void Tim4_Oc_Config(uint16_t Period,uint16_t Prescaler,uint16_t Pulse,FunctionalState NewState);
#endif
