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

	/*　
	 	下面的时间是通过逻辑分析仪测试得到的。
    工作条件：CPU主频72MHz ，MDK编译环境，1级优化
  
		循环次数为10时，SCL频率 = 205KHz 
		循环次数为7时，SCL频率 = 347KHz， SCL高电平时间1.5us，SCL低电平时间2.87us 
	 	循环次数为5时，SCL频率 = 421KHz， SCL高电平时间1.25us，SCL低电平时间2.375us 
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
	/* 当SCL高电平时，SDA出现一个上跳沿表示I2C总线停止信号 */
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
	SDA_SET;	/* CPU释放SDA总线 */
	i2c_Delay();
	SCK_SET;	/* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
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
	
	SDA_SET;	/* CPU驱动SDA = 1 */
	i2c_Delay();
	SCK_SET;	/* CPU产生1个时钟 */
	i2c_Delay();
	SCK_CLR;
	i2c_Delay();	
 
}

void I2c_Ack(void)
{
   SDA_OUT();
	
	SDA_CLR;	/* CPU驱动SDA = 1 */
	i2c_Delay();
	SCK_SET;	/* CPU产生1个时钟 */
	i2c_Delay();
	SCK_CLR;
	i2c_Delay();	
	SDA_SET;	 
}

void I2c_Write_Data(uint8_t Data)
{
	uint8_t i;
   SDA_OUT();
	/* 先发送字节的高位bit7 */
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
			 SDA_SET; // 释放总线
		}
		Data <<= 1;	/* 左移一个bit */
		i2c_Delay();
	}
}

uint8_t I2c_Read_Data(uint16_t Ack)
{
	uint8_t i;
	uint8_t value,Data;
   SDA_IN();
	/* 读到第1个bit为数据的bit7 */
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
* 函数名: u8 I2C_ReceiveByte(void)  
* 描述  : 读取一个字节
* 输入  : 无  
* 输出  : 字节数据
* 说明  : ReceiveByte : 数据从高位到低位
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

	I2c_Gpio_Init();		/* 配置GPIO */

	
	I2c_Start();		/* 发送启动信号 */

	/* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
	I2c_Write_Data(_Address);
	ucAck = I2c_Wait_Ack();	/* 检测设备的ACK应答 */

	I2c_Stop();			/* 发送停止信号 */

	return ucAck;
}
