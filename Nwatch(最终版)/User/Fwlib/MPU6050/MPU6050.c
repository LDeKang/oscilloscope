#include "MPU6050.h"
#include "DEV_IIC.h"
#include "systick.h"
#include "usart.h" 
 

uint16_t MPU_ACC[6];
uint16_t MPU_GYRO[6];
uint16_t MPU_TEMP[2];

//дһ���ֽ�
u8 MPU_Write_Byte(u8 Reg_Addr,u8 Data)
{
	//��ʼ
	IIC_Start();
	//д��ַ
	IIC_WriteByte((I2C_MPU_ADDR<<1|0));
	//Ӧ��
	if(IIC_Wait_Ack()==0) 
	{
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
u8 MPU_Read_Byte(u8 Reg_Addr,u8 Ack)
{
	u8 Data=0x55;
	//��ʼ
	IIC_Start();
	//д������ַ����ʾ��
	IIC_WriteByte((I2C_MPU_ADDR<<1|0));
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
	IIC_WriteByte((I2C_MPU_ADDR<<1|1));
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


//IIC����д
//addr:������ַ 
//reg:�Ĵ�����ַ
//len:д�볤��
//buf:������
//����ֵ:0,����
//    ����,�������
u8 MPU_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{
	u8 i; 
    IIC_Start(); 
	IIC_WriteByte((addr<<1)|0);//����������ַ+д����	
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


//IIC������
//addr:������ַ
//reg:Ҫ��ȡ�ļĴ�����ַ
//len:Ҫ��ȡ�ĳ���
//buf:��ȡ�������ݴ洢��
//����ֵ:0,����
//    ����,�������
u8 MPU_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{ 
		IIC_Start(); 
		IIC_WriteByte((addr<<1)|0);//����������ַ+д����	
	if(IIC_Wait_Ack()!=1)	//�ȴ�Ӧ��
	{
		IIC_Stop();		 
		return 1;		
	}
		IIC_WriteByte(reg);	//д�Ĵ�����ַ
		IIC_Wait_Ack();		//�ȴ�Ӧ��
		IIC_Start();
		IIC_WriteByte((addr<<1)|1);//����������ַ+������	
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

//����MPU6050�����ֵ�ͨ�˲���
//lpf:���ֵ�ͨ�˲�Ƶ��(Hz)
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
u8 MPU_Set_LPF(u16 lpf)
{
	u8 data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	return MPU_Write_Byte(MPU_CFG_REG,data);//�������ֵ�ͨ�˲���  
}

//����MPU6050�Ĳ�����(�ٶ�Fs=1KHz)
//rate:4~1000(Hz)
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
u8 MPU_Set_Rate(u16 rate)
{
	u8 data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=MPU_Write_Byte(MPU_SAMPLE_RATE_REG,data);	//�������ֵ�ͨ�˲���
 	return MPU_Set_LPF(rate/2);	//�Զ�����LPFΪ�����ʵ�һ��
}

u8 MUP_Init(void)
{
	u8 res;
  int i=0,j=0;
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);	 //ʹ��B�˿�ʱ��
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE); 
  //�ڳ�ʼ��֮ǰҪ��ʱһ��ʱ�䣬��û����ʱ����ϵ�����ϵ����ݿ��ܻ����
  for (i=0; i<1000; i++) {
  for (j=0; j<1000; j++) {
  ;
  }
  }
  //��
IIC_GPIO_Init();

MPU_Write_Byte(MPU_PWR_MGMT1_REG,0x80);//�Ĵ�����λ
Sys_Del_Ms(100);	
MPU_Write_Byte(MPU_PWR_MGMT1_REG,0x00);//����
	
MPU_Write_Byte(MPU_GYRO_CFG_REG,0x18);//���������� 2000reg/s
MPU_Write_Byte(MPU_ACCEL_CFG_REG,0x00);//���ٶ����� 2g/s
MPU_Set_Rate(200);//����Ƶ������ 50HZ

MPU_Write_Byte(MPU_INT_EN_REG,0X00);	//�ر������ж�
MPU_Write_Byte(MPU_USER_CTRL_REG,0X00);	//I2C��ģʽ�ر�
MPU_Write_Byte(MPU_FIFO_EN_REG,0X00);	//�ر�FIFO
MPU_Write_Byte(MPU_INTBP_CFG_REG,0X80);	//INT���ŵ͵�ƽ��Ч

res=MPU_Read_Byte(MPU_DEVICE_ID_REG,NO_ACK); 
if(res==I2C_MPU_ADDR)//����ID��ȷ
{
	MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X01);	//����CLKSEL,PLL X��Ϊ�ο�
	MPU_Write_Byte(MPU_PWR_MGMT2_REG,0X00);	//���ٶ��������Ƕ�����(�Ǵ���)
  MPU_Write_Byte(MPU_SAMPLE_RATE_REG,1);  //����Ƶ������ 50HZ
}else return 1;

return 0;
}

//��ȡԭʼ���ٶ�
void MPU_Read_Gyro(void)
{
	u8 Buf[6];
	
  MPU_Read_Len(I2C_MPU_ADDR,MPU_GYRO_XOUTH_REG,6,Buf);
	
	MPU_GYRO[0]=((Buf[0]<<8)|Buf[1]);//X
	MPU_GYRO[1]=((Buf[2]<<8)|Buf[3]);//Y
	MPU_GYRO[2]=((Buf[4]<<8)|Buf[5]);//Z
}


//��ȡԭʼ���ٶ�
void MPU_Read_Acc(void)
{
	u8 Buf[6];
	
  MPU_Read_Len(I2C_MPU_ADDR,MPU_ACCEL_XOUTH_REG,6,Buf);
	
	MPU_ACC[0]=((Buf[0]<<8)|Buf[1]);//X
	MPU_ACC[1]=((Buf[2]<<8)|Buf[3]);//Y
	MPU_ACC[2]=((Buf[4]<<8)|Buf[5]);//Z
}

//�õ��¶�ֵ
//����ֵ:�¶�ֵ(������100��)
short MPU_Get_Temperature(void)
{
    u8 buf[2]; 
    short raw;
	float temp;
	MPU_Read_Len(I2C_MPU_ADDR,MPU_TEMP_OUTH_REG,2,buf); 
    raw=((u16)buf[0]<<8)|buf[1];  
    temp=36.53+((double)raw)/340;  
    return temp*100;;
}