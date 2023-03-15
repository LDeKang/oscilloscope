#ifndef __STW_TFT_H
#define __STW_TFT_H	
#include "stm32f10x.h"
 
 
#define BACK_COLOR 0xffff
#define LINE_COLOR 0xf52c
#define SPOCE_COLOR 0x6b4d
 
void TFT_Init(void);
void TFT_Full(unsigned int color);
void TFT_Dis_Image(uint16_t usX,uint16_t usY,uint16_t Width,uint16_t High,const u8 *Str);
void TFT_Clear(void);
void TFT_Draw_Point(uint16_t usX,uint16_t usY,uint16_t Color);
void TFT_Draw_Line(float usX1,float usY1,float usX2,float usY2,uint16_t Color);
void TFT_Set_Win(uint16_t usX,uint16_t usY,uint16_t Width,uint16_t High,uint16_t Color);
void TFT_Dis_Str(uint16_t usX,uint16_t usY,u8*Str,uint16_t Str_Color,uint16_t Back_Color);//Êä³öÓ¢ÎÄ

#endif
