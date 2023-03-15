#ifndef __USART_H
#define __USART_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "sys.h"
#include "stm32f4xx.h"
#include <stdio.h>

extern UART_HandleTypeDef UART_Handle;
 
void USART1_Init_Config(void);
#endif  

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
