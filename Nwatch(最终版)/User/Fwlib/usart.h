#ifndef __USART_H
#define __USART_H	
#include "stm32f10x.h"
#include "stdio.h"

#define BUF_MAX_LEN 100

typedef struct Usart1_Buf_Struct
{
	u8 Udata;
  u8 USART_BUF[BUF_MAX_LEN];
	u8 Udata_Len;
	u8 USART_RX_FLAG;

}USART1_BUF_STRUCT;

extern USART1_BUF_STRUCT Usart1_Buf_St;

void Usart1_Init_Config(void);
void Usart1_Send_Char(u8 c);
void Usart1_Send_Str(u8 *Str);

#endif
