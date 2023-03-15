/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

// I2C driver
// Polling based, interrupts aren't really needed for this project since only a few bytes are transfered every-so-often

#include "common.h"

#define SDA_SET   PBout(9)=1
#define SDA_CLR   PBout(9)=0

#define SDA_START   PBin(9)


#define SCK_SET   PBout(8)=1
#define SCK_CLR   PBout(8)=0

#define SCL_H         GPIOB->BSRR = GPIO_Pin_8  //SCL=PB(8)
#define SCL_L         GPIOB->BRR  = GPIO_Pin_8  //SDA=PB(9)
        
#define SDA_H         GPIOB->BSRR = GPIO_Pin_9  
#define SDA_L         GPIOB->BRR  = GPIO_Pin_9  
      
#define SCL_read      GPIOB->IDR  & GPIO_Pin_8  
#define SDA_read      GPIOB->IDR  & GPIO_Pin_9  

static void i2c_Delay(void)
{
	uint8_t i;

	/*��
	 	�����ʱ����ͨ���߼������ǲ��Եõ��ġ�
    ����������CPU��Ƶ72MHz ��MDK���뻷����1���Ż�
  
		ѭ������Ϊ10ʱ��SCLƵ�� = 205KHz 
		ѭ������Ϊ7ʱ��SCLƵ�� = 347KHz�� SCL�ߵ�ƽʱ��1.5us��SCL�͵�ƽʱ��2.87us 
	 	ѭ������Ϊ5ʱ��SCLƵ�� = 421KHz�� SCL�ߵ�ƽʱ��1.25us��SCL�͵�ƽʱ��2.375us 
	*/
	for (i = 0; i < 10; i++);
}


void I2c_Gpio_Init(void)
{
	
   GPIO_InitTypeDef I2c_Pin_Structure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	I2c_Pin_Structure.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_9;
	I2c_Pin_Structure.GPIO_Mode=GPIO_Mode_Out_OD;
	I2c_Pin_Structure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&I2c_Pin_Structure);
	
}

static void SDA_OUT(void)
{
   GPIO_InitTypeDef I2c_Pin_Structure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	I2c_Pin_Structure.GPIO_Pin=GPIO_Pin_9;
	I2c_Pin_Structure.GPIO_Mode=GPIO_Mode_Out_OD;
	I2c_Pin_Structure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&I2c_Pin_Structure);
	
}


static void SDA_IN(void)
{
   GPIO_InitTypeDef I2c_Pin_Structure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	I2c_Pin_Structure.GPIO_Pin=GPIO_Pin_9;
	I2c_Pin_Structure.GPIO_Mode=GPIO_Mode_IPU;
 
	GPIO_Init(GPIOB,&I2c_Pin_Structure);
}


void I2c_Start(void)
{
   SDA_OUT();
	
	SDA_SET;
	SCK_SET;
	i2c_Delay();
	SDA_CLR;
	i2c_Delay();
	SCK_CLR;
	i2c_Delay();
	
}

void I2c_Stop(void)
{
   SDA_OUT();
	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C����ֹͣ�ź� */
	SDA_CLR;
	SCK_SET;
	i2c_Delay();
	SDA_SET;
	
}


uint8_t I2c_Wait_Ack(void)
{
	uint8_t re,ucErrTime;
   SDA_OUT();
	
	SCK_CLR;
	i2c_Delay();
	SDA_SET;	/* CPU�ͷ�SDA���� */
	i2c_Delay();
	SCK_SET;	/* CPU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */
//   SDA_IN();
    while(SDA_START)  
    {  
        ucErrTime++;
        if(ucErrTime>250)
        {
            I2c_Stop();
            return 1;
        }
    } 
	SCK_CLR;
	i2c_Delay();
	return 0;
	
}

void I2c_No_Ack(void)
{
   SDA_OUT();
	
	SDA_SET;	/* CPU����SDA = 1 */
	i2c_Delay();
	SCK_SET;	/* CPU����1��ʱ�� */
	i2c_Delay();
	SCK_CLR;
	i2c_Delay();	
 
}

void I2c_Ack(void)
{
   SDA_OUT();
	
	SDA_CLR;	/* CPU����SDA = 1 */
	i2c_Delay();
	SCK_SET;	/* CPU����1��ʱ�� */
	i2c_Delay();
	SCK_CLR;
	i2c_Delay();	
	SDA_SET;	 
}

void I2c_Write_Data(uint8_t Data)
{
	uint8_t i;
   SDA_OUT();
	/* �ȷ����ֽڵĸ�λbit7 */
	for (i = 0; i < 8; i++)
	{		
		if (Data & 0x80)
		{
			SDA_SET;
		}
		else
		{
			SDA_CLR;
		}
		i2c_Delay();
		SCK_SET;
		i2c_Delay();	
		SCK_CLR;
		if (i == 7)
		{
			 SDA_SET; // �ͷ�����
		}
		Data <<= 1;	/* ����һ��bit */
		i2c_Delay();
	}
}

uint8_t I2c_Read_Data(uint16_t Ack)
{
	uint8_t i;
	uint8_t value,Data;
   SDA_IN();
	/* ������1��bitΪ���ݵ�bit7 */
	for (i = 0; i < 8; i++)
	{
		Data<<=1;
		SCK_SET;
		i2c_Delay();
		if (SDA_START==1)
		{
			Data++;
		}
		SCK_CLR;
		i2c_Delay();
	}
 
	if(Ack)
	{
	  I2c_Ack();
	}
	else
	{
	  I2c_No_Ack();
	}
	return Data;
}

void I2C_SendByte(u8 SendByte)  
{  
    u8 i=8;  
    while(i--)  
    {  
        SCL_L;  
        i2c_Delay();
        
        if(SendByte & 0x80)  
            SDA_H;    
        else  
            SDA_L;    
        SendByte<<=1;  
        i2c_Delay();
        
        SCL_H;  
        i2c_Delay();  
    }  
    SCL_L;  
}
/**************************************************************************
* ������: u8 I2C_ReceiveByte(void)  
* ����  : ��ȡһ���ֽ�
* ����  : ��  
* ���  : �ֽ�����
* ˵��  : ReceiveByte : ���ݴӸ�λ����λ
***************************************************************************/
u8 I2C_ReceiveByte(void)    
{  
    u8 i=8;  
    u8 ReceiveByte=0;  
          
    SDA_H;                
    while(i--)  
    {  
        ReceiveByte<<=1;        
        SCL_L;  
        i2c_Delay();  
        SCL_H;
        i2c_Delay();      
        if(SDA_read)  
        {  
            ReceiveByte|=0x01;  
        }  
    }  
    SCL_L;  
	
    return ReceiveByte;  
}

uint8_t DS3231_Write_Data(uint8_t Address,uint8_t Data)
{
 I2c_Start();
 I2C_SendByte(0xD0);
	
 if(I2c_Wait_Ack())
 {
  return 0;
 }
 I2C_SendByte(Address);
 
 if(I2c_Wait_Ack())
 {
  return 0;
 }
 
 I2C_SendByte(Data);
 
 if(I2c_Wait_Ack())
 {  
  return 0;
 }
 
 I2c_Stop();
 
 return 1;
 
}

uint8_t DS3231_Read_Data(uint8_t Address, uint8_t* Data,uint8_t Count)
{
  uint16_t i;
	
 I2c_Start();
 I2C_SendByte(0xD0);
	
 if(I2c_Wait_Ack())
 {
  return 0;
 }
 I2C_SendByte(Address);
 
 if(I2c_Wait_Ack())
 {
  return 0;
 }
 
 I2c_Start();
 I2C_SendByte(0xD1);
 
 if(I2c_Wait_Ack())
 {
  return 0;
 }
 
 Count--;
 for(i=0;i<Count;i++)
 {
   Data[i]=I2c_Read_Data(I2C_ACK);
 }
   Data[i]=I2c_Read_Data(I2C_NACK);
 
 
 I2c_Stop();
 
 return 1;
 
}
 
 

uint8_t i2c_CheckDevice(uint8_t _Address)
{
	uint8_t ucAck;

	I2c_Gpio_Init();		/* ����GPIO */

	
	I2c_Start();		/* ���������ź� */

	/* �����豸��ַ+��д����bit��0 = w�� 1 = r) bit7 �ȴ� */
	I2c_Write_Data(_Address);
	ucAck = I2c_Wait_Ack();	/* ����豸��ACKӦ�� */

	I2c_Stop();			/* ����ֹͣ�ź� */

	return ucAck;
}
