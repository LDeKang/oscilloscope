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
 
	SPI1_Handle.Init.Mode=SPI_MODE_MASTER;//��ģʽ
	SPI1_Handle.Init.Direction=SPI_DIRECTION_2LINES;
	SPI1_Handle.Init.DataSize=SPI_DATASIZE_8BIT;//8λ����
	SPI1_Handle.Init.CLKPolarity=SPI_POLARITY_LOW;//SCK��Ϊ����
	SPI1_Handle.Init.CLKPhase=SPI_PHASE_1EDGE;
	SPI1_Handle.Init.NSS=SPI_NSS_SOFT;//�������CS
	SPI1_Handle.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_2;//f/2��Ƶ
	SPI1_Handle.Init.FirstBit=SPI_FIRSTBIT_MSB;//��λ��ǰ
	SPI1_Handle.Init.TIMode=SPI_TIMODE_DISABLE;
 
	HAL_SPI_Init(&SPI1_Handle);
 
	__HAL_SPI_ENABLE(&SPI1_Handle);
}

uint8_t W25q128_Reg_Read(uint8_t byte)
{
  SPITimeout = SPIT_FLAG_TIMEOUT;

  /* �ȴ����ͻ�����Ϊ�գ�TXE�¼� */
  while (__HAL_SPI_GET_FLAG( &SPI1_Handle, SPI_FLAG_TXE ) == RESET)
   {
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(0);
   }

  /* д�����ݼĴ�������Ҫд�������д�뷢�ͻ����� */
  WRITE_REG(SPI1_Handle.Instance->DR, byte);
	 
  SPITimeout = SPIT_FLAG_TIMEOUT;

  /* �ȴ����ջ������ǿգ�RXNE�¼� */
  while (__HAL_SPI_GET_FLAG( &SPI1_Handle, SPI_FLAG_RXNE ) == RESET)
   {
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(1);
   }

  /* ��ȡ���ݼĴ�������ȡ���ջ��������� */
  return READ_REG(SPI1_Handle.Instance->DR);
}


void SPI_FLASH_WaitForWriteEnd(void)
{
  uint8_t FLASH_Status = 0,Cmd;

  /* ѡ�� FLASH: CS �� */
	CS_STATE=0;

  /* ���� ��״̬�Ĵ��� ���� */
	Cmd=0x05;
	HAL_SPI_Transmit(&SPI1_Handle,&Cmd,1,5000);
	
	Cmd=0xff;
  SPITimeout = SPIT_FLAG_TIMEOUT;
  /* ��FLASHæµ����ȴ� */
  do
  {
    /* ��ȡFLASHоƬ��״̬�Ĵ��� */
//	    HAL_SPI_Transmit(&SPI1_Handle,&Cmd,1,500);//��������ʱ��
      FLASH_Status = W25q128_Reg_Read(0xff);//��ȡ�Ĵ���ʱ��Ƚ�������ʹ��HAL_SPI_Receive
//	  HAL_SPI_Receive(&SPI1_Handle,&FLASH_Status,1,500);
 
    {
      if((SPITimeout--) == 0) 
      {
        SPI_TIMEOUT_UserCallback(4);
        return;
      }
    } 
  }
  while ((FLASH_Status & 0x01) == SET); /* ����д���־ */

  /* ֹͣ�ź�  FLASH: CS �� */
	CS_STATE=1;
}

void W25q128_Write_Enable(void)
{
	uint8_t Cmd;
	
	Cmd=W25Q128_WRITE_ENABLE;
  /* ͨѶ��ʼ��CS�� */
	CS_STATE=0;

  /* ����дʹ������*/
	HAL_SPI_Transmit(&SPI1_Handle,&Cmd,1,500);

  /*ͨѶ������CS�� */
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


//ֻҪע�ⷢ�Ͳ�����ַʱ��λ��ǰ���ɡ�������������ָ��ʱע������ĵ�ַҪ���뵽 4KB��
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
//ֻҪע�ⷢ�Ͳ�����ַʱ��λ��ǰ���ɡ����ò���ָ��ʱע������ĵ�ַҪ���뵽 64KB��
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

//дһҳ 
void W25q128_Write_Page(uint32_t Write_Addr,const uint8_t *TxBuff,uint32_t Write_Num)
{
	uint8_t Cmd,Cmd_Value=0;
	
  W25q128_Write_Enable();
  SPI_FLASH_WaitForWriteEnd();//�ȴ�д����
	
	CS_STATE=0;	
	
	Cmd=W25Q128_WRITE_BYTE;	
	HAL_SPI_Transmit(&SPI1_Handle,&Cmd,1,500);
	
	Cmd_Value=((Write_Addr>>16)&0xff);
	HAL_SPI_Transmit(&SPI1_Handle,&Cmd_Value,1,500);
	
	Cmd_Value=((Write_Addr>>8)&0xff);
	HAL_SPI_Transmit(&SPI1_Handle,&Cmd_Value,1,500);
	
	Cmd_Value=(Write_Addr&0xff);
	HAL_SPI_Transmit(&SPI1_Handle,&Cmd_Value,1,500);	
 
  /* д������*/
  while (Write_Num--)
  {
    /* ���͵�ǰҪд����ֽ����� */
    W25q128_Reg_Read(*TxBuff);
    /* ָ����һ�ֽ����� */
    TxBuff++;
  }	

	CS_STATE=1;
	
	SPI_FLASH_WaitForWriteEnd();//�ȴ�д����
}

//д������������
void W25q128_Write_LotData(uint32_t Write_Addr,const uint8_t *TxBuff,uint32_t Write_Num)
{
	uint32_t Cmd=0,i;
  uint32_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
	
	Addr=Write_Addr%W25Q128_WRITE_MAX;//��ʼ��ַ�Ƿ���ҳ���ַ����
	
	count = W25Q128_WRITE_MAX - Addr;//����һҳ���ж����ֽ�
	
	NumOfPage=Write_Num/W25Q128_WRITE_MAX;//д��ҳ��
	NumOfSingle=Write_Num%W25Q128_WRITE_MAX;//д��ҳ��ʣ����ֽ�
	
//��ʼ��ַ��ҳ�����	
	if(Addr==0)
	{	
		if(NumOfPage==0)//д����һҳ
		{
		   W25q128_Write_Page(Write_Addr,TxBuff,Write_Num);
		}
		else//����һҳ
		{

			for(i=0;i<NumOfPage;i++)//�Ȱ�ҳд��
			{
				W25q128_Write_Page(Write_Addr,&TxBuff[i*W25Q128_WRITE_MAX],W25Q128_WRITE_MAX);
				Write_Addr+=W25Q128_WRITE_MAX;
			}
			 //дʣ�µ��ֽ�
			 W25q128_Write_Page(Write_Addr,&TxBuff[i*W25Q128_WRITE_MAX],NumOfSingle);		
		}

	}
	else//��ʼ��ַ��ҳ�治����
	{
		if(NumOfPage==0)//д����һҳ
		{
			if(count>=Write_Num)//��������ǰҳ
			{
				W25q128_Write_Page(Write_Addr,TxBuff,Write_Num);
			}
			else//������ǰҳ
			{
				W25q128_Write_Page(Write_Addr,TxBuff,count);//�ѵ�ǰҳд��
				Write_Addr+=count;
				
				temp=count-Write_Num;//ʣ���ֽ�
				W25q128_Write_Page(Write_Addr,&TxBuff[count],temp);		
			}		
		
		}
		else 
		{
			Write_Num-=count;//�����ݴ���ɵ�һ�����
			
			//���¼���
			NumOfPage=Write_Num/W25Q128_WRITE_MAX;
			NumOfSingle=Write_Num%W25Q128_WRITE_MAX;
			
			//д�굱ǰҳ
			W25q128_Write_Page(Write_Addr,TxBuff,count);
			Write_Addr+=count;//��ַƫ�Ƶ���һҳ
			
			for(i=0;i<NumOfPage;i++)
			{
				W25q128_Write_Page(Write_Addr,&TxBuff[count+i*W25Q128_WRITE_MAX],W25Q128_WRITE_MAX);
				Write_Addr+=W25Q128_WRITE_MAX;
			}
				W25q128_Write_Page(Write_Addr,&TxBuff[count+i*W25Q128_WRITE_MAX],NumOfSingle);
		}

	}
	

}
//������ָ����뼰Ҫ������ʼ��ַ��FLASH оƬ�ͻᰴ��ַ�����ķ�ʽ���ش洢��������ݣ� ��ȡ��������û�����ƣ�ֻҪû��ֹͣͨѶ��FLASH оƬ�ͻ�һֱ��������

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
  /* �ȴ���ʱ��Ĵ���,���������Ϣ */
  printf("SPI �ȴ���ʱ!errorCode = %d",errorCode);
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
//	W25q128_Write_LotData(0xA0000,gImage_jj,655360); //����¼640KB
	
//	W25q128_Sector_Block(0xA0000); 
//	W25q128_Sector_Block(0xB0000); 
//	W25q128_Sector_Block(0xD0000); 
//	W25q128_Sector_Block(0xE0000); 
//	W25q128_Sector_Block(0xF0000); 
//	
//	W25q128_Write_LotData(0xA0000,gImage_cc,400000); //����¼400KB
 
	
 
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