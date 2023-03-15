#include "BMP280.h"
#include "DEV_IIC.h"
#include "systick.h"
#include "usart.h" 
#include "stdio.h"

/*calibration parameters */
#define BMP280_DIG_T1_LSB_REG                0x88
#define BMP280_DIG_T1_MSB_REG                0x89
#define BMP280_DIG_T2_LSB_REG                0x8A
#define BMP280_DIG_T2_MSB_REG                0x8B
#define BMP280_DIG_T3_LSB_REG                0x8C
#define BMP280_DIG_T3_MSB_REG                0x8D
#define BMP280_DIG_P1_LSB_REG                0x8E
#define BMP280_DIG_P1_MSB_REG                0x8F
#define BMP280_DIG_P2_LSB_REG                0x90
#define BMP280_DIG_P2_MSB_REG                0x91
#define BMP280_DIG_P3_LSB_REG                0x92
#define BMP280_DIG_P3_MSB_REG                0x93
#define BMP280_DIG_P4_LSB_REG                0x94
#define BMP280_DIG_P4_MSB_REG                0x95
#define BMP280_DIG_P5_LSB_REG                0x96
#define BMP280_DIG_P5_MSB_REG                0x97
#define BMP280_DIG_P6_LSB_REG                0x98
#define BMP280_DIG_P6_MSB_REG                0x99
#define BMP280_DIG_P7_LSB_REG                0x9A
#define BMP280_DIG_P7_MSB_REG                0x9B
#define BMP280_DIG_P8_LSB_REG                0x9C
#define BMP280_DIG_P8_MSB_REG                0x9D
#define BMP280_DIG_P9_LSB_REG                0x9E
#define BMP280_DIG_P9_MSB_REG                0x9F
void Check_BMP(void);
 double bmp280_compensate_P_double(BMP280_S32_t adc_P);
 double bmp280_compensate_T_double(BMP280_S32_t adc_T);
 BMP280_S32_t bmp280_compensate_T_int32(BMP280_S32_t adc_T);
 BMP280_U32_t bmp280_compensate_P_int64(BMP280_S32_t adc_P);
//дһ���ֽ�
u8 BMP_Write_Byte(u8 Reg_Addr,u8 Data)
{
	//��ʼ
	IIC_Start();
	//д��ַ
	IIC_WriteByte((BMP280_WR_address<<1|0));
	//Ӧ��
	if(IIC_Wait_Ack()==0) 
	{
		printf("sdsfsf");
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
u8 BMP_Read_Byte(u8 Reg_Addr,u8 Ack)
{
	u8 Data=0x55;
	//��ʼ
	IIC_Start();
	//д������ַ����ʾ��
	IIC_WriteByte((BMP280_WR_address<<1|0));
	//Ӧ��
	if(IIC_Wait_Ack()!=1)
	{
   printf("sdsd");
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
	IIC_WriteByte((BMP280_WR_address<<1|1));
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


 
u8 BMP_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf)
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


 
u8 BMP_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf)
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

void BMP_Write_Reg(u8 regaddr,u8 Data)
{
   BMP_Write_Byte(regaddr,Data);	 
}

void BMP_Read_Reg(u8 regaddr,u8 *Data)
{
   BMP_Read_Len(BMP280_WR_address<<1,regaddr,1,Data);	 
}


void BMP_Rest(void)
{
//MAX_Write_Byte(REG_MODE_CONFIG,0x40);	 
}

 
BMP280 bmp280;		//���ȫ�ֽṹ����������������оƬ��ROM��������
//����BMP���������� MODE 
//BMP280_SLEEP_MODE||BMP280_FORCED_MODE||BMP280_NORMAL_MODE

void BMP280_Set_TemOversamp(BMP_OVERSAMPLE_MODE * Oversample_Mode)
{
	u8 Regtmp;
	Regtmp = ((Oversample_Mode->T_Osample)<<5)|
			 ((Oversample_Mode->P_Osample)<<2)|
			 ((Oversample_Mode)->WORKMODE);
	
	BMP_Write_Byte(BMP280_CTRLMEAS_REG,Regtmp);
}


//���ñ���ʱ����˲�����Ƶ����
void BMP280_Set_Standby_FILTER(BMP_CONFIG * BMP_Config)
{
	u8 Regtmp;
	Regtmp = ((BMP_Config->T_SB)<<5)|
			 ((BMP_Config->FILTER_COEFFICIENT)<<2)|
			 ((BMP_Config->SPI_EN));
	
	BMP_Write_Byte(BMP280_CONFIG_REG,Regtmp);
}



void BMP_Init(void)
{
	u8 Lsb,Msb;
	IIC_GPIO_Init();
 	
	/********************������������������*********************/
	//�¶ȴ������Ľ���ֵ
	Lsb = BMP_Read_Byte(BMP280_DIG_T1_LSB_REG,0);
	Msb = BMP_Read_Byte(BMP280_DIG_T1_MSB_REG,0);
	bmp280.T1 = (((u16)Msb)<<8) + Lsb;			//��λ�ӵ�λ

	Lsb = BMP_Read_Byte(BMP280_DIG_T2_LSB_REG,0);
	Msb = BMP_Read_Byte(BMP280_DIG_T2_MSB_REG,0);
	bmp280.T2 = (((u16)Msb)<<8) + Lsb;		

	Lsb = BMP_Read_Byte(BMP280_DIG_T3_LSB_REG,0);
	Msb = BMP_Read_Byte(BMP280_DIG_T3_MSB_REG,0);
	bmp280.T3 = (((u16)Msb)<<8) + Lsb;		

	
	//����ѹ�������Ľ���ֵ
	Lsb = BMP_Read_Byte(BMP280_DIG_P1_LSB_REG,0);
	Msb = BMP_Read_Byte(BMP280_DIG_P1_MSB_REG,0);
   bmp280.P1 = (((u16)Msb)<<8) + Lsb;		
	printf("%d\n",bmp280.P1);
	
	Lsb = BMP_Read_Byte(BMP280_DIG_P2_LSB_REG,0);
	Msb = BMP_Read_Byte(BMP280_DIG_P2_MSB_REG,0);
	bmp280.P2 = (((u16)Msb)<<8) + Lsb;	
	printf("%d\n",bmp280.P2);
	
	Lsb = BMP_Read_Byte(BMP280_DIG_P3_LSB_REG,0);
	Msb = BMP_Read_Byte(BMP280_DIG_P3_MSB_REG,0);
	bmp280.P3 = (((u16)Msb)<<8) + Lsb;	
	printf("%d\n",bmp280.P3);
	
	Lsb = BMP_Read_Byte(BMP280_DIG_P4_LSB_REG,0);
	Msb = BMP_Read_Byte(BMP280_DIG_P4_MSB_REG,0);
	bmp280.P4 = (((u16)Msb)<<8) + Lsb;	
	printf("%d\n",bmp280.P4);
	
	Lsb = BMP_Read_Byte(BMP280_DIG_P5_LSB_REG,0);
	Msb = BMP_Read_Byte(BMP280_DIG_P5_MSB_REG,0);
	bmp280.P5 = (((u16)Msb)<<8) + Lsb;	
	printf("%d\n",bmp280.P5);
	
	Lsb = BMP_Read_Byte(BMP280_DIG_P6_LSB_REG,0);
	Msb = BMP_Read_Byte(BMP280_DIG_P6_MSB_REG,0);
	bmp280.P6 = (((u16)Msb)<<8) + Lsb;	
	printf("%d\n",bmp280.P6);
	
	Lsb = BMP_Read_Byte(BMP280_DIG_P7_LSB_REG,0);
	Msb = BMP_Read_Byte(BMP280_DIG_P7_MSB_REG,0);
	bmp280.P7 = (((u16)Msb)<<8) + Lsb;	
	printf("%d\n",bmp280.P7);
	
	Lsb = BMP_Read_Byte(BMP280_DIG_P8_LSB_REG,0);
	Msb = BMP_Read_Byte(BMP280_DIG_P8_MSB_REG,0);
	bmp280.P8 = (((u16)Msb)<<8) + Lsb;	
	printf("%d\n",bmp280.P8);
	
	Lsb = BMP_Read_Byte(BMP280_DIG_P9_LSB_REG,0);
	Msb = BMP_Read_Byte(BMP280_DIG_P9_MSB_REG,0);
	bmp280.P9 = (((u16)Msb)<<8) + Lsb;	
	printf("%d\n",bmp280.P9);
	
	/******************************************************/
	BMP_Write_Byte(BMP280_RESET_REG,BMP280_RESET_VALUE);	//����λ�Ĵ���д�����ֵ
//	Check_BMP();
	
	BMP_OVERSAMPLE_MODE			BMP_OVERSAMPLE_MODEStructure;
	BMP_OVERSAMPLE_MODEStructure.P_Osample = BMP280_P_MODE_3;
	BMP_OVERSAMPLE_MODEStructure.T_Osample = BMP280_T_MODE_1;
	BMP_OVERSAMPLE_MODEStructure.WORKMODE  = BMP280_NORMAL_MODE;
	BMP280_Set_TemOversamp(&BMP_OVERSAMPLE_MODEStructure);
	
	BMP_CONFIG					BMP_CONFIGStructure;
	BMP_CONFIGStructure.T_SB = BMP280_T_SB1;
	BMP_CONFIGStructure.FILTER_COEFFICIENT = BMP280_FILTER_MODE_4;
	BMP_CONFIGStructure.SPI_EN = DISABLE;
	
	BMP280_Set_Standby_FILTER(&BMP_CONFIGStructure);
}

//��ȡBMP��ǰ״̬
//status_flag = BMP280_MEASURING ||
//			 	BMP280_IM_UPDATE
u8  BMP280_GetStatus(u8 status_flag)
{
	u8 flag;
	flag = BMP_Read_Byte(BMP280_STATUS_REG,0);
	if(flag&status_flag)	return SET;
	else return RESET;
}

//����ѹֵ-Pa
double BMP280_Get_Pressure(void)
{
	uint8_t XLsb,Lsb, Msb;
	
	long  Bit32;
	double Pressure;
	//double pressure;
	while(BMP280_GetStatus(0x08)!=RESET);
	while(BMP280_GetStatus(0x01)!=RESET);
	XLsb = BMP_Read_Byte(BMP280_PRESSURE_XLSB_REG,0);
//	printf("%d\n",XLsb>>4);
	Lsb	 = BMP_Read_Byte(BMP280_PRESSURE_LSB_REG,0);
//	printf("%d\n",Lsb);
	Msb	 = BMP_Read_Byte(BMP280_PRESSURE_MSB_REG,0);
//	printf("%d\n",Msb);
	Bit32 = ((long)(Msb << 12))|((long)(Lsb << 4))|((long)XLsb>>4);	//�Ĵ�����ֵ�������
	Pressure = bmp280_compensate_P_double(Bit32);
	return Pressure ;
}

double BMP280_Get_Temp(void)
{
	uint8_t XLsb,Lsb, Msb;
	
	long  Bit32;
	double Temp;
	//double pressure;
	while(BMP280_GetStatus(0x08)!=RESET);
	while(BMP280_GetStatus(0x01)!=RESET);
	
	XLsb = BMP_Read_Byte(BMP280_TEMPERATURE_XLSB_REG,0);
//	printf("%d\n",XLsb);
	Lsb	 = BMP_Read_Byte(BMP280_TEMPERATURE_LSB_REG,0);
//	printf("%d\n",Lsb);
	Msb	 = BMP_Read_Byte(BMP280_TEMPERATURE_MSB_REG,0);
//	printf("%d\n",Msb);
	Bit32 = ((long)(Msb << 12))|((long)(Lsb << 4))|((long)XLsb>>4);	//�Ĵ�����ֵ�������
	Temp = bmp280_compensate_T_double(Bit32);
	return Temp ;
}
 
void Check_BMP(void)
{
 u8 Data;
 BMP_Read_Reg(BMP280_CHIPID_REG,&Data);
 printf("%x",Data);
}

 
/**************************������ֵת����ֵ*************************************/
BMP280_S32_t t_fine;			//���ڼ��㲹��
//���ø��㲹��
#ifdef USE_FIXED_POINT_COMPENSATE
// Returns temperature in DegC, resolution is 0.01 DegC. Output value of ��5123�� equals 51.23 DegC. 
// t_fine carries fine temperature as global value
BMP280_S32_t bmp280_compensate_T_int32(BMP280_S32_t adc_T)
{
	BMP280_S32_t var1, var2, T;
	var1 = ((((adc_T>>3) - ((BMP280_S32_t)dig_T1<<1))) * ((BMP280_S32_t)dig_T2)) >> 11;
	var2 = (((((adc_T>>4) - ((BMP280_S32_t)dig_T1)) * ((adc_T>>4) - ((BMP280_S32_t)dig_T1))) >> 12) * 
	((BMP280_S32_t)dig_T3)) >> 14;
	t_fine = var1 + var2;
	T = (t_fine * 5 + 128) >> 8;
	return T;
}

// Returns pressure in Pa as unsigned 32 bit integer in Q24.8 format (24 integer bits and 8 fractional bits).
// Output value of ��24674867�� represents 24674867/256 = 96386.2 Pa = 963.862 hPa
BMP280_U32_t bmp280_compensate_P_int64(BMP280_S32_t adc_P)
{
	BMP280_S64_t var1, var2, p;
	var1 = ((BMP280_S64_t)t_fine) - 128000;
	var2 = var1 * var1 * (BMP280_S64_t)dig_P6;
	var2 = var2 + ((var1*(BMP280_S64_t)dig_P5)<<17);
	var2 = var2 + (((BMP280_S64_t)dig_P4)<<35);
	var1 = ((var1 * var1 * (BMP280_S64_t)dig_P3)>>8) + ((var1 * (BMP280_S64_t)dig_P2)<<12);
	var1 = (((((BMP280_S64_t)1)<<47)+var1))*((BMP280_S64_t)dig_P1)>>33;
	if (var1 == 0)
	{
	return 0; // avoid exception caused by division by zero
	}
	p = 1048576-adc_P;
	p = (((p<<31)-var2)*3125)/var1;
	var1 = (((BMP280_S64_t)dig_P9) * (p>>13) * (p>>13)) >> 25;
	var2 = (((BMP280_S64_t)dig_P8) * p) >> 19;
	p = ((p + var1 + var2) >> 8) + (((BMP280_S64_t)dig_P7)<<4);
	return (BMP280_U32_t)p;
}


/***********************************CUT*************************************/
#else
/**************************������ֵת����ֵ*************************************/
// Returns temperature in DegC, double precision. Output value of ��51.23�� equals 51.23 DegC.
// t_fine carries fine temperature as global value
double bmp280_compensate_T_double(BMP280_S32_t adc_T)
{
	double var1, var2, T;
	var1 = (((double)adc_T)/16384.0 - ((double)dig_T1)/1024.0) * ((double)dig_T2);
	var2 = ((((double)adc_T)/131072.0 - ((double)dig_T1)/8192.0) *
	(((double)adc_T)/131072.0 - ((double) dig_T1)/8192.0)) * ((double)dig_T3);
	t_fine = (BMP280_S32_t)(var1 + var2);
	T = (var1 + var2) / 5120.0;
	return T;
}

// Returns pressure in Pa as double. Output value of ��96386.2�� equals 96386.2 Pa = 963.862 hPa
double bmp280_compensate_P_double(BMP280_S32_t adc_P)
{
	double var1, var2, p;
	var1 = ((double)t_fine/2.0) - 64000.0;
	var2 = var1 * var1 * ((double)dig_P6) / 32768.0;
	var2 = var2 + var1 * ((double)dig_P5) * 2.0;
	var2 = (var2/4.0)+(((double)dig_P4) * 65536.0);
	var1 = (((double)dig_P3) * var1 * var1 / 524288.0 + ((double)dig_P2) * var1) / 524288.0;
	var1 = (1.0 + var1 / 32768.0)*((double)dig_P1);
	if (var1 == 0.0)
	{
	return 0; // avoid exception caused by division by zero
	}
	p = 1048576.0 - (double)adc_P;
	p = (p - (var2 / 4096.0)) * 6250.0 / var1;
	var1 = ((double)dig_P9) * p * p / 2147483648.0;
	var2 = p * ((double)dig_P8) / 32768.0;
	p = p + (var1 + var2 + ((double)dig_P7)) / 16.0;
	return p;
}
#endif
