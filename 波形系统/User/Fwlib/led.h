#ifndef __LED_H
#define __LED_H	
#include "stm32f10x.h"
#include "sys.h"


#define LED1_STATE  PAout(2)
#define LED2_STATE  PAout(3)


//#define LED1_TTL  PAout(1)
//#define LED2_TTL  PAout(2)
//#define LED3_TTL  PAout(3)

#define ON    0
#define OFF   1


void Led_Pin_Init_Config(void);

#endif
