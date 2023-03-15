#include "wq_flash.h"
#include "sys.h"
#include "systick.h"
 
uint32_t SPITimeout;

#define SPIT_FLAG_TIMEOUT 0x5

#define CS_STATE PAout(12)
#define CLK_STATE PAout(11)

#define SI_STATE PAin(8)
#define SO_STATE PBout(15)

uint8_t W25q_Id[8];
#define W25Q128_WRITE_MAX  256

#define W25Q128_WRITE_ENABLE  0x06

#define W25Q128_IDCMD  0x4B
#define W25Q128_DUMMY  0xFF
#define W25Q128_JEDEC  0x9F
#define W25Q128_SECTOR_ERASE  0x20
#define W25Q128_WRITE_BYTE  0x02
#define W25Q128_REG1  0x05
#define W25Q128_READ_DATA  0x03
#define W25Q128_BLOCK_ERASE  0xD8
#define SPIT_FLAG_TIMEOUT 0x1000;

static void spi_Delay(void)
{
	uint8_t i;

	/*　
	 	下面的时间是通过逻辑分析仪测试得到的。
    工作条件：CPU主频72MHz ，MDK编译环境，1级优化
  
		循环次数为10时，SCL频率 = 205KHz 
		循环次数为7时，SCL频率 = 347KHz， SCL高电平时间1.5us，SCL低电平时间2.87us 
	 	循环次数为5时，SCL频率 = 421KHz， SCL高电平时间1.25us，SCL低电平时间2.375us 
	*/
	for (i = 0; i < 10; i++);
}
void Spi_Gpio_Init(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;
		
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;

	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;

	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;

	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
   CS_STATE=1;
}

 
 static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* 等待超时后的处理,输出错误信息 */
//  printf("SPI 等待超时!errorCode = %d",errorCode);
  return 0;
}

void Spi_Write_Byte(uint8_t Byte)
{
  u8 i;
 
	for(i=0;i<8;i++)
	{
	   CLK_STATE=0;
		if(Byte&0x80)
		{
			SO_STATE=1;
		}
		else
		{
		  SO_STATE=0;
		}
		 CLK_STATE=1;
 
       Byte<<=1;
	}
}

uint8_t Spi_Read_Byte(void)
{
  u8 i,Data=0;
	
	for(i=0;i<8;i++)
	{
	   CLK_STATE=0;
		Data<<=1;
		if(SI_STATE)
		{
			Data+=1;
		}
		else
		{
		   Data+=0;
		}
		 CLK_STATE=1;
	}
	
	return Data;
}

uint8_t W25q64_Oprate_Wreg(uint8_t Byte)
{
	Spi_Write_Byte(Byte);
}

uint8_t W25q64_Oprate_Rreg(uint8_t Byte)
{
	uint16_t Data;
	Data=Spi_Read_Byte();
	
	return Data;
}

void W25q64_Spi_Transmit(uint8_t*Subff,uint16_t Size)
{
  while(Size)
  {
		W25q64_Oprate_Wreg(*Subff);
		Subff++;
		Size--;
  }
}

void W25q64_Spi_Receive(uint8_t*Subff,uint16_t Size)
{
	while(Size) //读0-256字节
	{
		*Subff=W25q64_Oprate_Rreg(W25Q128_DUMMY);
		Size--;
		Subff++;
	}
}


void SPI_FLASH_WaitForWriteEnd(void)
{
  uint8_t FLASH_Status = 0,Cmd;

  /* 选择 FLASH: CS 低 */
	CS_STATE=0;

  /* 发送 读状态寄存器 命令 */
	Cmd=0x05;
	W25q64_Spi_Transmit(&Cmd,1);
	
	Cmd=0xff;
  SPITimeout = SPIT_FLAG_TIMEOUT;
  /* 若FLASH忙碌，则等待 */
  do
  {
    /* 读取FLASH芯片的状态寄存器 */
//	    W25q64_Spi_Transmit(&SPI1_Handle,&Cmd,1,500);//产生虚拟时钟
      FLASH_Status = Spi_Read_Byte();//读取寄存器时间比较慢，不使用HAL_SPI_Receive
//	  HAL_SPI_Receive(&SPI1_Handle,&FLASH_Status,1,500);
      Sys_Del_Ms(1);
    {
      if((SPITimeout--) == 0) 
      {
        SPI_TIMEOUT_UserCallback(4);
        return;
      }
    } 
  }
  while ((FLASH_Status & 0x01) == SET); /* 正在写入标志 */

  /* 停止信号  FLASH: CS 高 */
	CS_STATE=1;
}

void W25q128_Write_Enable(void)
{
	uint8_t Cmd;
	
	Cmd=W25Q128_WRITE_ENABLE;
  /* 通讯开始：CS低 */
	CS_STATE=0;

  /* 发送写使能命令*/
	W25q64_Spi_Transmit(&Cmd,1);

  /*通讯结束：CS高 */
	CS_STATE=1;
}

 
//发送了指令编码及要读的起始地址后，FLASH 芯片就会按地址递增的方式返回存储矩阵的内容， 读取的数据量没有限制，只要没有停止通讯，FLASH 芯片就会一直返回数据

void W25q128_Read_LotData(uint32_t Read_Addr,uint8_t *RxBuff,uint16_t Read_Num)
{
	uint8_t Cmd,Cmd_Value=0;
	
	CS_STATE=0;	
	Cmd=W25Q128_READ_DATA;	
	W25q64_Spi_Transmit(&Cmd,1);
	
	Cmd_Value=((Read_Addr>>16)&0xff);
	W25q64_Spi_Transmit(&Cmd_Value,1);
	
	Cmd_Value=((Read_Addr>>8)&0xff);
	W25q64_Spi_Transmit(&Cmd_Value,1);
	
	Cmd_Value=(Read_Addr&0xff);
	W25q64_Spi_Transmit(&Cmd_Value,1);	
 
	W25q64_Spi_Receive(RxBuff,Read_Num);	

	CS_STATE=1;

}