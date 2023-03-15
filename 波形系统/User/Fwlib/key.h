#ifndef __KEY_H
#define __KEY_H	
#include "stm32f10x.h"


typedef struct Key_St
{ 
  u8 Down;
  u8 No_Down;
	u8 Value;
	
}KEY_ST;

extern KEY_ST Key_Even_St;

void Key_Pin_Config(void);
void Key_Exti_Init_Config(void);
u8 Key1_Chack(void);
u8 Key2_Chack(void);
u8 Key3_Chack(void);
u8 Key4_Chack(void);
u8 Key_Chack(void);

#endif
