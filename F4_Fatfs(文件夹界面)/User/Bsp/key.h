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
	
	uint16_t Key_Tick_Flag;//��ʼ������־
	uint16_t Key_Tick;//������
	uint16_t Key_Tick_Max;//�������ֵ

}KEY_ST;

extern KEY_ST Key_Start;


void Key_Init(void);
void Key_Exti_Init(void);
uint8_t Key1_Chack(void);
uint8_t Key2_Chack(void);
uint8_t Key_Scan(void);

#endif  

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
