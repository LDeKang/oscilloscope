#ifndef __TIM2_H
#define __TIM2_H	
#include "stm32f10x.h"

void T_Bmp_Init(void);
void Start_Anm(void);
void Tim2_Nvic_init(void);
void Tim2_Base_Init_Config(uint16_t Period,uint16_t Prescaler);


#endif
