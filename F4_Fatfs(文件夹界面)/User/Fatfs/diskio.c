/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
#include "w25q128.h"
#include "usart.h"

/* Definitions of physical drive number for each drive */
#define DEV_RAM		0	/* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */

extern uint8_t Wq_Rx_Buff[20];
/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;

	switch (pdrv) {
	 case DEV_RAM :

	  stat=RES_OK;
	 
		return stat;

	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;

	switch (pdrv) {
	case DEV_RAM :
		
		SPI1_Init_Config();
	  W25q128_Read_JEDG();
    stat=RES_OK;
	
		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* 要读取的扇区数 */
)
{
	DRESULT res;
	int result;
  uint32_t Read_Addr;
	
	switch (pdrv) {
	case DEV_RAM :
		
	    sector+=1536;
	    Read_Addr=(sector<<12);
      W25q128_Read_LotData(Read_Addr,buff,count<<12);
	    res=RES_OK;

		return res;
	}

	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res;
	int result;
  uint32_t Write_Addr;
	switch (pdrv) {
	case DEV_RAM :
		
			sector+=1536;
	    Write_Addr=(sector<<12);
	    W25q128_Sector_Erase(Write_Addr,1);
			W25q128_Write_LotData(Write_Addr,buff,count<<12);
//	    printf("Write_sector=%d\n",sector-1536);
			res=RES_OK;
	
		return res;
	}

	return RES_PARERR;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
		case DEV_RAM :
			
				switch (cmd) 
				{
					case GET_SECTOR_COUNT :
						*(LBA_t *)buff=2560;
					
						break;
					
					case GET_SECTOR_SIZE :
						*(WORD *)buff=4096;
					
						break;
					
					case GET_BLOCK_SIZE :
						*(DWORD *)buff=1;
					
						break;
			 
				}
				 res=RES_OK;
				
				return res;
	}

	return RES_PARERR;
}

