#include "usart.h"
#include "w25q128.h"
#include "led.h"

SPI_HandleTypeDef  SPI1_Handle;

uint8_t W25q_Id[8];
#define W25Q128_WRITE_MAX  256

#define CS_STATE PGout(6)

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

uint16_t SPITimeout=0;
static  uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);





void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
  GPIO_InitTypeDef SPI_Pin_InitStructure;
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
	
	SPI_Pin_InitStructure.Pin=GPIO_PIN_6;
	SPI_Pin_InitStructure.Mode=GPIO_MODE_OUTPUT_PP;
	SPI_Pin_InitStructure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;
	SPI_Pin_InitStructure.Pull=GPIO_NOPULL;
	
	HAL_GPIO_Init(GPIOG,&SPI_Pin_InitStructure);
	
	SPI_Pin_InitStructure.Pin=GPIO_PIN_3|GPIO_PIN_5;
	SPI_Pin_InitStructure.Mode=GPIO_MODE_AF_PP;
	SPI_Pin_InitStructure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;
	SPI_Pin_InitStructure.Pull=GPIO_PULLUP;
	SPI_Pin_InitStructure.Alternate=GPIO_AF5_SPI1;
	
	HAL_GPIO_Init(GPIOB,&SPI_Pin_InitStructure);
	
	SPI_Pin_InitStructure.Pin=GPIO_PIN_4;
	SPI_Pin_InitStructure.Alternate=GPIO_AF5_SPI1;
	
	HAL_GPIO_Init(GPIOB,&SPI_Pin_InitStructure);
	CS_STATE=1;
  LED_STAET=1;

}

void SPI1_Init_Config(void)
{
	__HAL_RCC_SPI1_CLK_ENABLE();		
	
	SPI1_Handle.Instance=SPI1;
 
	SPI1_Handle.Init.Mode=SPI_MODE_MASTER;//主模式
	SPI1_Handle.Init.Direction=SPI_DIRECTION_2LINES;
	SPI1_Handle.Init.DataSize=SPI_DATASIZE_8BIT;//8位传输
	SPI1_Handle.Init.CLKPolarity=SPI_POLARITY_LOW;//SCK低为空闲
	SPI1_Handle.Init.CLKPhase=SPI_PHASE_1EDGE;
	SPI1_Handle.Init.NSS=SPI_NSS_SOFT;//软件触发CS
	SPI1_Handle.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_2;//f/2分频
	SPI1_Handle.Init.FirstBit=SPI_FIRSTBIT_MSB;//高位在前
	SPI1_Handle.Init.TIMode=SPI_TIMODE_DISABLE;
 
	HAL_SPI_Init(&SPI1_Handle);
 
	__HAL_SPI_ENABLE(&SPI1_Handle);
}

uint8_t W25q128_Reg_Read(uint8_t byte)
{
  SPITimeout = SPIT_FLAG_TIMEOUT;

  /* 等待发送缓冲区为空，TXE事件 */
  while (__HAL_SPI_GET_FLAG( &SPI1_Handle, SPI_FLAG_TXE ) == RESET)
   {
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(0);
   }

  /* 写入数据寄存器，把要写入的数据写入发送缓冲区 */
  WRITE_REG(SPI1_Handle.Instance->DR, byte);
	 
  SPITimeout = SPIT_FLAG_TIMEOUT;

  /* 等待接收缓冲区非空，RXNE事件 */
  while (__HAL_SPI_GET_FLAG( &SPI1_Handle, SPI_FLAG_RXNE ) == RESET)
   {
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(1);
   }

  /* 读取数据寄存器，获取接收缓冲区数据 */
  return READ_REG(SPI1_Handle.Instance->DR);
}


void SPI_FLASH_WaitForWriteEnd(void)
{
  uint8_t FLASH_Status = 0,Cmd;

  /* 选择 FLASH: CS 低 */
	CS_STATE=0;

  /* 发送 读状态寄存器 命令 */
	Cmd=0x05;
	HAL_SPI_Transmit(&SPI1_Handle,&Cmd,1,5000);
	
	Cmd=0xff;
  SPITimeout = SPIT_FLAG_TIMEOUT;
  /* 若FLASH忙碌，则等待 */
  do
  {
    /* 读取FLASH芯片的状态寄存器 */
//	    HAL_SPI_Transmit(&SPI1_Handle,&Cmd,1,500);//产生虚拟时钟
      FLASH_Status = W25q128_Reg_Read(0xff);//读取寄存器时间比较慢，不使用HAL_SPI_Receive
//	  HAL_SPI_Receive(&SPI1_Handle,&FLASH_Status,1,500);
 
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
	HAL_SPI_Transmit(&SPI1_Handle,&Cmd,1,500);

  /*通讯结束：CS高 */
	CS_STATE=1;
}

void W25q128_Read_ID(void)
{
	uint8_t Cmd,i;
	
	Cmd=W25Q128_IDCMD;
	
	CS_STATE=0;
	HAL_SPI_Transmit(&SPI1_Handle,&Cmd,1,500);
	
	Cmd=W25Q128_DUMMY;
	
	HAL_SPI_Transmit(&SPI1_Handle,&Cmd,1,500);
	
	HAL_SPI_Transmit(&SPI1_Handle,&Cmd,1,500);
	
	HAL_SPI_Transmit(&SPI1_Handle,&Cmd,1,500);
	
  HAL_SPI_Receive(&SPI1_Handle,W25q_Id,8,500);
	
	CS_STATE=1;
	
	for(i=0;i<8;i++)
		{
			printf("%x ",W25q_Id[i]);
		}
	
}



void W25q128_Read_JEDG(void)
{
	uint8_t Cmd,i;
	
	Cmd=W25Q128_JEDEC;
	
	CS_STATE=0;
	HAL_SPI_Transmit(&SPI1_Handle,&Cmd,1,500);
  HAL_SPI_Receive(&SPI1_Handle,W25q_Id,3,500);
	
	CS_STATE=1;
	
	for(i=0;i<3;i++)
		{
			printf("%x ",W25q_Id[i]);
		}
	
}

//void W25q128_Wait_End(void)
//{
//	uint8_t Cmd,Reg_Value=0;
//	
//	CS_STATE=0;	
//	Cmd=W25Q128_REG1;	
//	HAL_SPI_Transmit(&SPI1_Handle,&Cmd,1,500);
//	
//	while(Reg_Value)
//	{
//		Reg_Value=SPI_FLASH_SendByte(0xff);
//		Reg_Value=Reg_Value&0x01;	
//	}
//	
//	CS_STATE=1;
//}


//只要注意发送擦除地址时高位在前即可。调用扇区擦除指令时注意输入的地址要对齐到 4KB。
void W25q128_Sector_Erase(uint32_t Erase_Addr,uint16_t Erase_Num)
{
	  uint8_t Cmd,Cmd_Value=0,i;
	
		W25q128_Write_Enable();
  	SPI_FLASH_WaitForWriteEnd();
	
		Cmd=W25Q128_SECTOR_ERASE;	
		CS_STATE=0;	
	
	for(i=0;i<Erase_Num;i++)
		{
			HAL_SPI_Transmit(&SPI1_Handle,&Cmd,1,500);
			
			Cmd_Value=((Erase_Addr>>16)&0xff);
			HAL_SPI_Transmit(&SPI1_Handle,&Cmd_Value,1,500);
			
			Cmd_Value=((Erase_Addr>>8)&0xff);
			HAL_SPI_Transmit(&SPI1_Handle,&Cmd_Value,1,500);
			
			Cmd_Value=(Erase_Addr&0xff);
			HAL_SPI_Transmit(&SPI1_Handle,&Cmd_Value,1,500);	
			Erase_Addr+=0x1000;

		}

	 CS_STATE=1;
	 SPI_FLASH_WaitForWriteEnd();	
}
//只要注意发送擦除地址时高位在前即可。调用擦除指令时注意输入的地址要对齐到 64KB。
void W25q128_Sector_Block(uint32_t Erase_Addr)
{
	  uint8_t Cmd,Cmd_Value=0;
	
		W25q128_Write_Enable();
		SPI_FLASH_WaitForWriteEnd();

		Cmd=W25Q128_BLOCK_ERASE;	
		CS_STATE=0;	

		HAL_SPI_Transmit(&SPI1_Handle,&Cmd,1,500);

		Cmd_Value=((Erase_Addr>>16)&0xff);
		HAL_SPI_Transmit(&SPI1_Handle,&Cmd_Value,1,500);

		Cmd_Value=((Erase_Addr>>8)&0xff);
		HAL_SPI_Transmit(&SPI1_Handle,&Cmd_Value,1,500);

		Cmd_Value=(Erase_Addr&0xff);
		HAL_SPI_Transmit(&SPI1_Handle,&Cmd_Value,1,500);	

		CS_STATE=1;
		SPI_FLASH_WaitForWriteEnd();	
}

//写一页 
void W25q128_Write_Page(uint32_t Write_Addr,const uint8_t *TxBuff,uint32_t Write_Num)
{
	uint8_t Cmd,Cmd_Value=0;
	
  W25q128_Write_Enable();
  SPI_FLASH_WaitForWriteEnd();//等待写结束
	
	CS_STATE=0;	
	
	Cmd=W25Q128_WRITE_BYTE;	
	HAL_SPI_Transmit(&SPI1_Handle,&Cmd,1,500);
	
	Cmd_Value=((Write_Addr>>16)&0xff);
	HAL_SPI_Transmit(&SPI1_Handle,&Cmd_Value,1,500);
	
	Cmd_Value=((Write_Addr>>8)&0xff);
	HAL_SPI_Transmit(&SPI1_Handle,&Cmd_Value,1,500);
	
	Cmd_Value=(Write_Addr&0xff);
	HAL_SPI_Transmit(&SPI1_Handle,&Cmd_Value,1,500);	
 
  /* 写入数据*/
  while (Write_Num--)
  {
    /* 发送当前要写入的字节数据 */
    W25q128_Reg_Read(*TxBuff);
    /* 指向下一字节数据 */
    TxBuff++;
  }	

	CS_STATE=1;
	
	SPI_FLASH_WaitForWriteEnd();//等待写结束
}

//写不定量的数据
void W25q128_Write_LotData(uint32_t Write_Addr,const uint8_t *TxBuff,uint32_t Write_Num)
{
	uint32_t Cmd=0,i;
  uint32_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
	
	Addr=Write_Addr%W25Q128_WRITE_MAX;//初始地址是否与页面地址对其
	
	count = W25Q128_WRITE_MAX - Addr;//里下一页还有多少字节
	
	NumOfPage=Write_Num/W25Q128_WRITE_MAX;//写的页数
	NumOfSingle=Write_Num%W25Q128_WRITE_MAX;//写完页后剩余的字节
	
//初始地址与页面对其	
	if(Addr==0)
	{	
		if(NumOfPage==0)//写不满一页
		{
		   W25q128_Write_Page(Write_Addr,TxBuff,Write_Num);
		}
		else//超出一页
		{

			for(i=0;i<NumOfPage;i++)//先把页写完
			{
				W25q128_Write_Page(Write_Addr,&TxBuff[i*W25Q128_WRITE_MAX],W25Q128_WRITE_MAX);
				Write_Addr+=W25Q128_WRITE_MAX;
			}
			 //写剩下的字节
			 W25q128_Write_Page(Write_Addr,&TxBuff[i*W25Q128_WRITE_MAX],NumOfSingle);		
		}

	}
	else//初始地址与页面不对其
	{
		if(NumOfPage==0)//写不满一页
		{
			if(count>=Write_Num)//不超出当前页
			{
				W25q128_Write_Page(Write_Addr,TxBuff,Write_Num);
			}
			else//超出当前页
			{
				W25q128_Write_Page(Write_Addr,TxBuff,count);//把当前页写完
				Write_Addr+=count;
				
				temp=count-Write_Num;//剩余字节
				W25q128_Write_Page(Write_Addr,&TxBuff[count],temp);		
			}		
		
		}
		else 
		{
			Write_Num-=count;//将数据处理成第一种情况
			
			//重新计算
			NumOfPage=Write_Num/W25Q128_WRITE_MAX;
			NumOfSingle=Write_Num%W25Q128_WRITE_MAX;
			
			//写完当前页
			W25q128_Write_Page(Write_Addr,TxBuff,count);
			Write_Addr+=count;//地址偏移到下一页
			
			for(i=0;i<NumOfPage;i++)
			{
				W25q128_Write_Page(Write_Addr,&TxBuff[count+i*W25Q128_WRITE_MAX],W25Q128_WRITE_MAX);
				Write_Addr+=W25Q128_WRITE_MAX;
			}
				W25q128_Write_Page(Write_Addr,&TxBuff[count+i*W25Q128_WRITE_MAX],NumOfSingle);
		}

	}
	

}
//发送了指令编码及要读的起始地址后，FLASH 芯片就会按地址递增的方式返回存储矩阵的内容， 读取的数据量没有限制，只要没有停止通讯，FLASH 芯片就会一直返回数据

void W25q128_Read_LotData(uint32_t Read_Addr,uint8_t *RxBuff,uint16_t Read_Num)
{
	uint8_t Cmd,Cmd_Value=0;
  SPI_FLASH_WaitForWriteEnd();
	
	CS_STATE=0;	
	Cmd=W25Q128_READ_DATA;	
	HAL_SPI_Transmit(&SPI1_Handle,&Cmd,1,500);
	
	Cmd_Value=((Read_Addr>>16)&0xff);
	HAL_SPI_Transmit(&SPI1_Handle,&Cmd_Value,1,500);
	
	Cmd_Value=((Read_Addr>>8)&0xff);
	HAL_SPI_Transmit(&SPI1_Handle,&Cmd_Value,1,500);
	
	Cmd_Value=(Read_Addr&0xff);
	HAL_SPI_Transmit(&SPI1_Handle,&Cmd_Value,1,500);	
 
	HAL_SPI_Receive(&SPI1_Handle,RxBuff,Read_Num,500);	

	CS_STATE=1;

}

static  uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* 等待超时后的处理,输出错误信息 */
  printf("SPI 等待超时!errorCode = %d",errorCode);
  return 0;
}

void W25q128_Write_Chinaese(void)
{
 
//	W25q128_Sector_Block(0x00); 
//	W25q128_Sector_Block(0x10000); 
//	W25q128_Sector_Block(0x20000); 
//	W25q128_Sector_Block(0x30000); 
//	W25q128_Sector_Block(0x40000); 
//	W25q128_Sector_Block(0x50000); 
//	W25q128_Sector_Block(0x60000); 
//	W25q128_Sector_Block(0x70000); 
//	W25q128_Sector_Block(0x80000); 
//	W25q128_Sector_Block(0x90000); 
//	W25q128_Write_LotData(0xA0000,gImage_jj,655360); //先烧录640KB
	
//	W25q128_Sector_Block(0xA0000); 
//	W25q128_Sector_Block(0xB0000); 
//	W25q128_Sector_Block(0xD0000); 
//	W25q128_Sector_Block(0xE0000); 
//	W25q128_Sector_Block(0xF0000); 
//	
//	W25q128_Write_LotData(0xA0000,gImage_cc,400000); //在烧录400KB
 
	
 
// W25q128_Read_LotData(0xFF880,Wq_Rx_Buff,128);
//	
//	for(i=0;i<128;i++)
//		{
//			printf("%x ",Wq_Rx_Buff[i]);
//			if((i%32==0)&&(i!=0))
//			{
//			 printf("\n");
//			}
//		}
}