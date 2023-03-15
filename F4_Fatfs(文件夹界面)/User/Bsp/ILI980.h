#ifndef __LIL980_H
#define __LIL980_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "sys.h"
#include "stm32f4xx.h"

#define      WHITE		 		                 0xFFFF	   //��ɫ
#define      BLACK                         0x0000	   //��ɫ 
#define      GREY                          0xF7DE	   //��ɫ 
#define      BLUE                          0x001F	   //��ɫ 
#define      BLUE2                         0x051F	   //ǳ��ɫ 
#define      RED                           0xF800	   //��ɫ 
#define      MAGENTA                       0xF81F	   //����ɫ�����ɫ 
#define      GREEN                         0x07E0	   //��ɫ 
#define      CYAN                          0x7FFF	   //����ɫ����ɫ 
#define      YELLOW                        0xFFE0	   //��ɫ 
#define      BRED                          0xF81F
#define      GRED                          0xFFE0
#define      GBLUE                         0x07FF

void NT35510_Init ( void );
void NT35510_Draw_Win(uint16_t x,uint16_t y,uint16_t Width,uint16_t Hight,uint16_t Colour);
void NT35510_Write_Pixel(uint32_t Number,uint16_t Colour);
void NT35510_Clear(uint16_t x,uint16_t y,uint16_t Width,uint16_t Hight);
void TFT_Dis_Str(uint16_t usX,uint16_t usY,uint8_t*Str,uint16_t Str_Color,uint16_t Back_Color);//���Ӣ��
void TFT_Dis_Image(uint16_t usX,uint16_t usY,uint16_t Width,uint16_t High,const uint8_t *Str);
void TFT_Dis_Chinaese(uint16_t usX,uint16_t usY,uint8_t *Str, uint16_t Str_Color,uint16_t Back_Color);
void TFT_Dis_FsImage(uint16_t usX,uint16_t usY,uint16_t Width,uint16_t High,uint32_t Num);

#endif  

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
