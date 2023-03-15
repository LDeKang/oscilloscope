#include "MPU6050.h"
#include "DEV_IIC.h"
#include "systick.h"
#include "usart.h" 
 

uint16_t MPU_ACC[6];
uint16_t MPU_GYRO[6];
uint16_t MPU_TEMP[2];

//写一个字节
u8 MPU_Write_Byte(u8 Reg_Addr,u8 Data)
{
	//起始
	IIC_Start();
	//写地址
	IIC_WriteByte((I2C_MPU_ADDR<<1|0));
	//应答
	if(IIC_Wait_Ack()==0) 
	{
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
u8 MPU_Read_Byte(u8 Reg_Addr,u8 Ack)
{
	u8 Data=0x55;
	//起始
	IIC_Start();
	//写器件地址，表示读
	IIC_WriteByte((I2C_MPU_ADDR<<1|0));
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
	IIC_WriteByte((I2C_MPU_ADDR<<1|1));
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


//IIC连续写
//addr:器件地址 
//reg:寄存器地址
//len:写入长度
//buf:数据区
//返回值:0,正常
//    其他,错误代码
u8 MPU_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{
	u8 i; 
    IIC_Start(); 
	IIC_WriteByte((addr<<1)|0);//发送器件地址+写命令	
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


//IIC连续读
//addr:器件地址
//reg:要读取的寄存器地址
//len:要读取的长度
//buf:读取到的数据存储区
//返回值:0,正常
//    其他,错误代码
u8 MPU_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{ 
		IIC_Start(); 
		IIC_WriteByte((addr<<1)|0);//发送器件地址+写命令	
	if(IIC_Wait_Ack()!=1)	//等待应答
	{
		IIC_Stop();		 
		return 1;		
	}
		IIC_WriteByte(reg);	//写寄存器地址
		IIC_Wait_Ack();		//等待应答
		IIC_Start();
		IIC_WriteByte((addr<<1)|1);//发送器件地址+读命令	
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

//设置MPU6050的数字低通滤波器
//lpf:数字低通滤波频率(Hz)
//返回值:0,设置成功
//    其他,设置失败 
u8 MPU_Set_LPF(u16 lpf)
{
	u8 data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	return MPU_Write_Byte(MPU_CFG_REG,data);//设置数字低通滤波器  
}

//设置MPU6050的采样率(假定Fs=1KHz)
//rate:4~1000(Hz)
//返回值:0,设置成功
//    其他,设置失败 
u8 MPU_Set_Rate(u16 rate)
{
	u8 data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=MPU_Write_Byte(MPU_SAMPLE_RATE_REG,data);	//设置数字低通滤波器
 	return MPU_Set_LPF(rate/2);	//自动设置LPF为采样率的一半
}

u8 MUP_Init(void)
{
	u8 res;
  int i=0,j=0;
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);	 //使能B端口时钟
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE); 
  //在初始化之前要延时一段时间，若没有延时，则断电后再上电数据可能会出错
  for (i=0; i<1000; i++) {
  for (j=0; j<1000; j++) {
  ;
  }
  }
  //解
IIC_GPIO_Init();

MPU_Write_Byte(MPU_PWR_MGMT1_REG,0x80);//寄存器复位
Sys_Del_Ms(100);	
MPU_Write_Byte(MPU_PWR_MGMT1_REG,0x00);//启动
	
MPU_Write_Byte(MPU_GYRO_CFG_REG,0x18);//陀螺仪配置 2000reg/s
MPU_Write_Byte(MPU_ACCEL_CFG_REG,0x00);//加速度配置 2g/s
MPU_Set_Rate(200);//采样频率配置 50HZ

MPU_Write_Byte(MPU_INT_EN_REG,0X00);	//关闭所有中断
MPU_Write_Byte(MPU_USER_CTRL_REG,0X00);	//I2C主模式关闭
MPU_Write_Byte(MPU_FIFO_EN_REG,0X00);	//关闭FIFO
MPU_Write_Byte(MPU_INTBP_CFG_REG,0X80);	//INT引脚低电平有效

res=MPU_Read_Byte(MPU_DEVICE_ID_REG,NO_ACK); 
if(res==I2C_MPU_ADDR)//器件ID正确
{
	MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X01);	//设置CLKSEL,PLL X轴为参考
	MPU_Write_Byte(MPU_PWR_MGMT2_REG,0X00);	//加速度与陀螺仪都工作(非待机)
  MPU_Write_Byte(MPU_SAMPLE_RATE_REG,1);  //采样频率配置 50HZ
}else return 1;

return 0;
}

//读取原始角速度
void MPU_Read_Gyro(void)
{
	u8 Buf[6];
	
  MPU_Read_Len(I2C_MPU_ADDR,MPU_GYRO_XOUTH_REG,6,Buf);
	
	MPU_GYRO[0]=((Buf[0]<<8)|Buf[1]);//X
	MPU_GYRO[1]=((Buf[2]<<8)|Buf[3]);//Y
	MPU_GYRO[2]=((Buf[4]<<8)|Buf[5]);//Z
}


//读取原始加速度
void MPU_Read_Acc(void)
{
	u8 Buf[6];
	
  MPU_Read_Len(I2C_MPU_ADDR,MPU_ACCEL_XOUTH_REG,6,Buf);
	
	MPU_ACC[0]=((Buf[0]<<8)|Buf[1]);//X
	MPU_ACC[1]=((Buf[2]<<8)|Buf[3]);//Y
	MPU_ACC[2]=((Buf[4]<<8)|Buf[5]);//Z
}

//得到温度值
//返回值:温度值(扩大了100倍)
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