#include "MAX1302.h"
#include "DEV_IIC.h"
#include "systick.h"
 
#include "usart.h" 
#define MAXINT_START   PBin(9)

#define MAX_INT_PIN GPIO_Pin_9

void MAX_GPIO_Init(void)
{
	
	GPIO_InitTypeDef Init_GPIO_Structure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	Init_GPIO_Structure.GPIO_Pin=MAX_INT_PIN;
	Init_GPIO_Structure.GPIO_Mode=GPIO_Mode_IPU;
	Init_GPIO_Structure.GPIO_Speed=GPIO_Speed_50MHz;

	GPIO_Init(GPIOB,&Init_GPIO_Structure);
 
}

//写一个字节
u8 MAX_Write_Byte(u8 Reg_Addr,u8 Data)
{
	//起始
	IIC_Start();
	//写地址
	IIC_WriteByte((max30102_WR_address|0));
	//应答
	if(IIC_Wait_Ack()==0) 
	{
		printf("ddd");
	return 1;
	}
	//写寄存器地址
	IIC_WriteByte(Reg_Addr);
	if(IIC_Wait_Ack()!=1)
	{
	return 1;
	}
	//写数据
	IIC_WriteByte(Data);
	//应答
	if(IIC_Wait_Ack()!=1)
	{
	return 1;
	}
	//停止
  IIC_Stop();
	return 0;
}
//读一个字节
u8 MAX_Read_Byte(u8 Reg_Addr,u8 Ack)
{
	u8 Data=0x55;
	//起始
	IIC_Start();
	//写器件地址，表示读
	IIC_WriteByte((max30102_WR_address|0));
	//应答
	if(IIC_Wait_Ack()!=1)
	{
 
	return 1;
	}
	//写要读的寄存器地址
	IIC_WriteByte(Reg_Addr);
	if(IIC_Wait_Ack()!=1)
	{
	return 1;
	}
	//应答
	IIC_Start();
	//写器件地址，表示读
	IIC_WriteByte((max30102_WR_address|1));
	//应答
	if(IIC_Wait_Ack()!=1)
	{
	return 1;
	}
	//读取数据
	Data=IIC_Read_Byte(Ack);
  IIC_Stop();
	
	return Data;
}


 
u8 MAX_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{
	u8 i; 
    IIC_Start(); 
	IIC_WriteByte((addr)|0);//发送器件地址+写命令	
	if(IIC_Wait_Ack()!=1)	//等待应答
	{
		IIC_Stop();		 
		return 1;		
	}
    IIC_WriteByte(reg);	//写寄存器地址
    IIC_Wait_Ack();		//等待应答
	for(i=0;i<len;i++)
	{
		IIC_WriteByte(buf[i]);	//发送数据
		if(IIC_Wait_Ack()!=1)		//等待ACK
		{
			IIC_Stop();	 
			return 1;		 
		}		
	}    
    IIC_Stop();	 
	return 0;	
} 


 
u8 MAX_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{ 
		IIC_Start(); 
	
		IIC_WriteByte((addr));//发送器件地址+写命令	
	
	if(IIC_Wait_Ack()!=1)	//等待应答
	{
		IIC_Stop();		 
		return 1;		
	}
		IIC_WriteByte(reg);	//写寄存器地址
		IIC_Wait_Ack();		//等待应答
	
		IIC_Start();
		IIC_WriteByte((addr)|1);//发送器件地址+读命令	
		IIC_Wait_Ack();		//等待应答 
	
	while(len)
	{
		if(len==1)*buf=IIC_Read_Byte(0);//读数据,发送nACK 
		else *buf=IIC_Read_Byte(1);		//读数据,发送ACK  
		len--;
		buf++; 
	}    
    IIC_Stop();	//产生一个停止条件 
	return 0;	
}

void MAX_Write_Reg(u8 regaddr,u8 Data)
{
   MAX_Write_Byte(regaddr,Data);	 
}

void MAX_Read_Reg(u8 regaddr,u8 *Data)
{
   MAX_Read_Len(max30102_WR_address,regaddr,1,Data);	 
}


void MAX_Rest(void)
{
   MAX_Write_Byte(REG_MODE_CONFIG,0x40);	 
}
void MAX_Init(void)
{
	IIC_GPIO_Init();
	MAX_GPIO_Init();
	
	MAX_Rest();
	Sys_Del_Ms(20);
	
   MAX_Write_Byte(REG_INTR_ENABLE_1,0xc0);	// INTR setting
	MAX_Write_Byte(REG_INTR_ENABLE_2,0x00);
	
	MAX_Write_Byte(REG_FIFO_WR_PTR,0x00);  	//FIFO_WR_PTR[4:0]
	MAX_Write_Byte(REG_OVF_COUNTER,0x00);  	//OVF_COUNTER[4:0]
	MAX_Write_Byte(REG_FIFO_RD_PTR,0x00);  	//FIFO_RD_PTR[4:0]
	
	MAX_Write_Byte(REG_FIFO_CONFIG,0x0f);  	//sample avg = 1, fifo rollover=false, fifo almost full = 17
	MAX_Write_Byte(REG_MODE_CONFIG,0x03);  	///0x02为红色，0x03为SpO2模式0x07多模LED
	MAX_Write_Byte(REG_SPO2_CONFIG,0x27);  	// SPO2_ADC范围= 4096nA, SPO2采样率(100 Hz)， LED脉冲宽度(400uS). ?
   OFF_MAX();
	
	
 
}

void OFF_MAX(void)
{
	MAX_Write_Byte(REG_LED1_PA,0x00);   	//Choose value for ~ 7mA for LED1
	MAX_Write_Byte(REG_LED2_PA,0x00);   	// Choose value for ~ 7mA for LED2
	MAX_Write_Byte(REG_PILOT_PA,0x00);   	// Choose value for ~ 25mA for Pilot LE
  TIM_Cmd(TIM2,DISABLE);
}

void ON_MAX(void)
{
	MAX_Write_Byte(REG_LED1_PA,0x30);   	//Choose value for ~ 7mA for LED1
	MAX_Write_Byte(REG_LED2_PA,0x30);   	// Choose value for ~ 7mA for LED2
	MAX_Write_Byte(REG_PILOT_PA,0x7f);   	// Choose value for ~ 25mA for Pilot LE
	TIM_Cmd(TIM2,ENABLE);
}

void MAX_Read_Data(uint32_t *Ir_Data,uint32_t * Red_Data)
{ 
 uint32_t Um_Temp;
 u8 Data[6];
 u8 Temp;
    uint8_t receive_data[6],temp_data=0;	

	MAX_Read_Reg(REG_INTR_STATUS_1,&temp_data);
//   *Ir_Data=0;
//	* Red_Data=0;
//	while(MAXINT_START);
	
	while((temp_data&0x40)!=0x40)
	{
		MAX_Read_Reg(REG_INTR_STATUS_1,&temp_data);
	}
		
  MAX_Read_Len(max30102_WR_address,REG_FIFO_DATA,6,Data);
	
//  Um_Temp=(unsigned char) Data[0];
//  Um_Temp<<=16;
//  *Red_Data+=Um_Temp;
//	
//  Um_Temp=(unsigned char) Data[1];
//  Um_Temp<<=8;
//  *Red_Data+=Um_Temp;
//	
//  Um_Temp=(unsigned char) Data[2];
//  *Red_Data+=Um_Temp;
//  
//  Um_Temp=(unsigned char) Data[3];
//  Um_Temp<<=16;
//  *Ir_Data+=Um_Temp;
//  
//  Um_Temp=(unsigned char) Data[4];
//  Um_Temp<<=8;
//  *Ir_Data+=Um_Temp;
//  
//  Um_Temp=(unsigned char) Data[5];
//  *Ir_Data+=Um_Temp;
//  
//  *Red_Data&=0x03FFFF;  //Mask MSB [23:18]
//  *Ir_Data&=0x03FFFF;  //Mask MSB [23:18]
  
 	    *Red_Data =  (long)((long)((long)Data[0]&0x03)<<16) | (long)Data[1]<<8 | (long)Data[2];    // Combine values to get the actual number
 	    *Ir_Data = (long)((long)((long)Data[3] & 0x03)<<16) |(long)Data[4]<<8 | (long)Data[5];   // Combine values to get the actual number
}

void Check_Max(void)
{
 u8 Data;
 MAX_Read_Len(max30102_WR_address,REG_REV_ID,1,&Data);
// printf("%x",Data);
}