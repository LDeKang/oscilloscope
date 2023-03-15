#ifndef __KEY_H
#define __KEY_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "sys.h"
#include "stm32f4xx.h"

typedef struct key_st
{
  uint16_t Key_Value;
	uint8_t  Key_Down;
	uint8_t  Key_No_Down;
	
	uint16_t Key_Tick_Flag;//开始计数标志
	uint16_t Key_Tick;//计数器
	uint16_t Key_Tick_Max;//计数最大值

}KEY_ST;

extern KEY_ST Key_Start;


void Key_Init(void);
void Key_Exti_Init(void);
uint8_t Key1_Chack(void);
uint8_t Key2_Chack(void);
uint8_t Key_Scan(void);

#endif  

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
