#ifndef __W25Q128_H
#define __W25Q128_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "sys.h"
#include "stm32f4xx.h"
 

extern SPI_HandleTypeDef  SPI_Handle;
 
void SPI1_Init_Config(void);
void W25q128_Read_ID(void);
void W25q128_Read_JEDG(void);
void W25q128_Sector_Erase(uint32_t Erase_Addr,uint16_t Erase_Num);
void W25q128_Write_LotData(uint32_t Write_Addr,const uint8_t *TxBuff,uint32_t Write_Num);
void W25q128_Read_LotData(uint32_t Read_Addr,uint8_t *RxBuff,uint16_t Read_Num);
void W25q128_Sector_Block(uint32_t Erase_Addr);
#endif  

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
