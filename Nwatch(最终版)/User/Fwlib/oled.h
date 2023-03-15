#ifndef __OLED_H
#define __OLED_H	
#include "stm32f10x.h"

// Commands
#define OLED_OFF				0xAE // Power off
#define OLED_ON					0xAF // Power on
#define OLED_MEMMODE			0x20 // Memory addressing mode
#define OLED_NONINVERT			0xA6 // Noninvert
#define OLED_INVERT				0xA7 // Invert
#define OLED_SEG_NML			0xA0 // Seg remap left to right
#define OLED_SEG_REMAP			0xA1 // Seg remap right to left
#define OLED_SETCONTRAST		0x81 // Set contrast
#define OLED_SETMUX				0xA8 // MUX
#define OLED_SCANDIR_NML		0xC0 // Scan direction top to bottom
#define OLED_SCANDIR_REMAP		0xC8 // Scan direction bottom to top
#define OLED_DISPLAY_OFFSET		0xD3
#define OLED_COMCFG				0xDA
#define OLED_DISPLAY_SHOWRAM	0xA4 // Show RAM contents
#define OLED_DISPLAY_HIDERAM	0xA5 // Blank screen, but still on
#define OLED_CHARGEPUMP			0x8D
#define OLED_CLOCK_FREQ			0xD5
#define OLED_DISPLAY_START		0x40
#define OLED_PRECHARGE_PERIOD	0xD9
#define OLED_VCOM_LEVEL			0xDB

// Data
#define OLED_MEM_HORIZ		0x00
#define OLED_MEM_VERT		0x01
#define OLED_MEM_PAGE		0x02
#define OLED_CHARGEPUMP_EN	0x14
#define OLED_CHARGEPUMP_DIS	0x10


typedef enum
{
	OLED_PWR_OFF = 0,
	OLED_PWR_ON = 1
} oled_pwr_t;

#define FRAME_WIDTH			128
#define FRAME_HEIGHT		64
#define FRAME_BUFFER_SIZE	((FRAME_WIDTH * FRAME_HEIGHT) / 8)

extern u8 oledBuffer[FRAME_BUFFER_SIZE];

uint16_t OLED_Qfull(void);
uint16_t OLED_Qclear(void);

void oled_init(void);
void oled_flush(void);
void oled_power(u8 on);
void oled_setBrightness(u8 brightness);
void oled_setInvert(u8 invert);
void oled_set180(u8 rotate);
void GUI_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size,u8 mode);
void GUI_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size,u8 mode);
void GUI_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 Size,u8 mode);
void OLED_DrawLine(u8 x0, u8 y0, u8 x1, u8 y1);
//void Oled_Str(u8 x,u8 y,u8* Str);
#endif
