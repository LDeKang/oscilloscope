#ifndef __BSP_API_H
#define __BSP_API_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "sys.h"
#include "stm32f4xx.h"
#include "led.h"
#include "key.h"
#include "usart.h"
#include "w25q128.h"
#include "oled_funt.h"
#include "ILI980.h"
#include "ff.h"			/* Declarations of FatFs API */
#include "gb2312_fon2.h"
#include "gb2312_fon1.h"
#include "mune.h"

extern FATFS Fs;
extern FIL File1;
extern DIR File_Dir;
extern FILINFO File_Fno;

extern uint32_t Br;
extern uint32_t Bw;
extern uint8_t Wq_Rx_Buff[1024];
extern char Dir_Path[];
extern TCHAR* File_Name;
extern uint8_t Dir_Flag;

void Fatfs_Init(void);
void Dir_Frist_Gui(void);
void Dir_Sub1_Gui(void);
FRESULT TFT_Slate_Files (char* path,uint16_t Num);
#endif  

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
