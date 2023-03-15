#ifndef __LIL980_H
#define __LIL980_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "sys.h"
#include "stm32f4xx.h"

#define      WHITE		 		                 0xFFFF	   //白色
#define      BLACK                         0x0000	   //黑色 
#define      GREY                          0xF7DE	   //灰色 
#define      BLUE                          0x001F	   //蓝色 
#define      BLUE2                         0x051F	   //浅蓝色 
#define      RED                           0xF800	   //红色 
#define      MAGENTA                       0xF81F	   //红紫色，洋红色 
#define      GREEN                         0x07E0	   //绿色 
#define      CYAN                          0x7FFF	   //蓝绿色，青色 
#define      YELLOW                        0xFFE0	   //黄色 
#define      BRED                          0xF81F
#define      GRED                          0xFFE0
#define      GBLUE                         0x07FF

void NT35510_Init ( void );
void NT35510_Draw_Win(uint16_t x,uint16_t y,uint16_t Width,uint16_t Hight,uint16_t Colour);
void NT35510_Write_Pixel(uint32_t Number,uint16_t Colour);
void NT35510_Clear(uint16_t x,uint16_t y,uint16_t Width,uint16_t Hight);
void TFT_Dis_Str(uint16_t usX,uint16_t usY,uint8_t*Str,uint16_t Str_Color,uint16_t Back_Color);//输出英文
void TFT_Dis_Image(uint16_t usX,uint16_t usY,uint16_t Width,uint16_t High,const uint8_t *Str);
void TFT_Dis_Chinaese(uint16_t usX,uint16_t usY,uint8_t *Str, uint16_t Str_Color,uint16_t Back_Color);
void TFT_Dis_FsImage(uint16_t usX,uint16_t usY,uint16_t Width,uint16_t High,uint32_t Num);

#endif  

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
