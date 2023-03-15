#ifndef __WQ_FLASH_H
#define __WQ_FLASH_H	
#include "stm32f10x.h"
#include "sys.h"


void Spi_Gpio_Init(void);
//void W25q128_Read_ID(void);
//void W25q128_Read_JEDG(void);
//void W25q128_Sector_Erase(uint32_t Erase_Addr,uint16_t Erase_Num);
//void W25q128_Write_LotData(uint32_t Write_Addr,uint8_t *TxBuff,uint32_t Write_Num);
void W25q128_Read_LotData(uint32_t Read_Addr,uint8_t *RxBuff,uint16_t Read_Num);
//void W25q128_Sector_Block(uint32_t Erase_Addr);
#endif
