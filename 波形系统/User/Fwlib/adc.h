#ifndef __ADC_H
#define __ADC_H	
#include "stm32f10x.h"


#define DATA_LEN  120

extern uint16_t Adc_Arr[DATA_LEN];
extern uint16_t Adc_Value;

void Adc_Init_Config(void);
void Control_Adc_Dma(FunctionalState NewState);
void Adc_Continu_State(FunctionalState NewState);
void Control_Adc(FunctionalState NewState);

#endif
