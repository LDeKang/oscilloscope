#ifndef __LED_H
#define __LED_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "sys.h"
#include "stm32f4xx.h"



#define  LED_STAET PFout(6)
 
void LED_Pin_Init(void);

#endif  

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
