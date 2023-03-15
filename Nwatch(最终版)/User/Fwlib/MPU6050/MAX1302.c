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

//дһ���ֽ�
u8 MAX_Write_Byte(u8 Reg_Addr,u8 Data)
{
	//��ʼ
	IIC_Start();
	//д��ַ
	IIC_WriteByte((max30102_WR_address|0));
	//Ӧ��
	if(IIC_Wait_Ack()==0) 
	{
		printf("ddd");
	return 1;
	}
	//д�Ĵ�����ַ
	IIC_WriteByte(Reg_Addr);
	if(IIC_Wait_Ack()!=1)
	{
	return 1;
	}
	//д����
	IIC_WriteByte(Data);
	//Ӧ��
	if(IIC_Wait_Ack()!=1)
	{
	return 1;
	}
	//ֹͣ
  IIC_Stop();
	return 0;
}
//��һ���ֽ�
u8 MAX_Read_Byte(u8 Reg_Addr,u8 Ack)
{
	u8 Data=0x55;
	//��ʼ
	IIC_Start();
	//д������ַ����ʾ��
	IIC_WriteByte((max30102_WR_address|0));
	//Ӧ��
	if(IIC_Wait_Ack()!=1)
	{
 
	return 1;
	}
	//дҪ���ļĴ�����ַ
	IIC_WriteByte(Reg_Addr);
	if(IIC_Wait_Ack()!=1)
	{
	return 1;
	}
	//Ӧ��
	IIC_Start();
	//д������ַ����ʾ��
	IIC_WriteByte((max30102_WR_address|1));
	//Ӧ��
	if(IIC_Wait_Ack()!=1)
	{
	return 1;
	}
	//��ȡ����
	Data=IIC_Read_Byte(Ack);
  IIC_Stop();
	
	return Data;
}


 
u8 MAX_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{
	u8 i; 
    IIC_Start(); 
	IIC_WriteByte((addr)|0);//����������ַ+д����	
	if(IIC_Wait_Ack()!=1)	//�ȴ�Ӧ��
	{
		IIC_Stop();		 
		return 1;		
	}
    IIC_WriteByte(reg);	//д�Ĵ�����ַ
    IIC_Wait_Ack();		//�ȴ�Ӧ��
	for(i=0;i<len;i++)
	{
		IIC_WriteByte(buf[i]);	//��������
		if(IIC_Wait_Ack()!=1)		//�ȴ�ACK
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
	
		IIC_WriteByte((addr));//����������ַ+д����	
	
	if(IIC_Wait_Ack()!=1)	//�ȴ�Ӧ��
	{
		IIC_Stop();		 
		return 1;		
	}
		IIC_WriteByte(reg);	//д�Ĵ�����ַ
		IIC_Wait_Ack();		//�ȴ�Ӧ��
	
		IIC_Start();
		IIC_WriteByte((addr)|1);//����������ַ+������	
		IIC_Wait_Ack();		//�ȴ�Ӧ�� 
	
	while(len)
	{
		if(len==1)*buf=IIC_Read_Byte(0);//������,����nACK 
		else *buf=IIC_Read_Byte(1);		//������,����ACK  
		len--;
		buf++; 
	}    
    IIC_Stop();	//����һ��ֹͣ���� 
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
	MAX_Write_Byte(REG_MODE_CONFIG,0x03);  	///0x02Ϊ��ɫ��0x03ΪSpO2ģʽ0x07��ģLED
	MAX_Write_Byte(REG_SPO2_CONFIG,0x27);  	// SPO2_ADC��Χ= 4096nA, SPO2������(100 Hz)�� LED������(400uS). ?
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