#ifndef __TIM2_H
#define __TIM2_H	
#include "stm32f10x.h"



extern u8 Cap_Flag;

void Wave_Struct_Init(void);
void Tim2_Ic_Config(FunctionalState NewState);
void Control_Ic_Pin(FunctionalState NewState);

#endif
