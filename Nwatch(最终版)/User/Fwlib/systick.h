#ifndef __SYSTICK_H
#define __SYSTICK_H	
#include "stm32f10x.h"

#define millis() millis_get()

#define MILLIS_DATATYPE	unsigned int

#define CLOCKSEL (_BV(CS21)|_BV(CS20))
#define PRESCALER 32

#define INCREMENT_COUNT	1

#define REG_TCCRA		 
#define REG_TCCRB		 
#define REG_TIMSK		 
#define REG_OCR			 
#define BIT_WGM			 
#define BIT_OCIE		 


#define SET_TCCRA()	(EMPTY_FUNC)
#define SET_TCCRB()	(REG_TCCRB = CLOCKSEL)

#define OCR_VAL ((F_CPU / PRESCALER) / 1000)

extern uint32_t Systick_Cnt;

typedef unsigned char millis8_t;
typedef uint32_t millis_t;

void Systick_Init_Config(void);
void Systick_Count_Fun(void);
void Sys_Del_Ms(uint16_t Ms);
uint32_t millis_get(void);

#endif
