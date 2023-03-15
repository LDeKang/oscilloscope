#include "DEV_IIC.h" 
#include "systick.h"
//#include "led.h"

#define SDA_SET   PBout(5)=1
#define SDA_CLR   PBout(5)=0

#define SCK_SET   PBout(6)=1
#define SCK_CLR   PBout(6)=0

#define SDA_START   PBin(5)



#define IIC_GPIO_CLK  RCC_APB2Periph_GPIOB

#define IIC_SDA_PIN   GPIO_Pin_5
#define IIC_SCL_PIN   GPIO_Pin_6


#define IIC_GPIOx    GPIOB

#define SDA_read      GPIOB->IDR  & GPIO_Pin_5

static void i2c_Delay()
{
	uint8_t i;

	/*��
	 	�����ʱ����ͨ���߼������ǲ��Եõ��ġ�
    ����������CPU��Ƶ72MHz ��MDK���뻷����1���Ż�
  
		ѭ������Ϊ10ʱ��SCLƵ�� = 205KHz 
		ѭ������Ϊ7ʱ��SCLƵ�� = 347KHz�� SCL�ߵ�ƽʱ��1.5us��SCL�͵�ƽʱ��2.87us 
	 	ѭ������Ϊ5ʱ��SCLƵ�� = 421KHz�� SCL�ߵ�ƽʱ��1.25us��SCL�͵�ƽʱ��2.375us 
	*/
	for (i = 0; i < 15; i++);
}

static void SDA_OUT()
{
	GPIO_InitTypeDef Init_GPIO_Structure;
	Init_GPIO_Structure.GPIO_Pin=IIC_SDA_PIN;
	Init_GPIO_Structure.GPIO_Mode=GPIO_Mode_Out_OD;//�������
	Init_GPIO_Structure.GPIO_Speed=GPIO_Speed_50MHz;

	GPIO_Init(IIC_GPIOx,&Init_GPIO_Structure);
}
static void SDA_IN()
{
	GPIO_InitTypeDef Init_GPIO_Structure;
	Init_GPIO_Structure.GPIO_Pin=IIC_SDA_PIN;
	Init_GPIO_Structure.GPIO_Mode=GPIO_Mode_IPU;//��������
	Init_GPIO_Structure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(IIC_GPIOx,&Init_GPIO_Structure);
}

void IIC_GPIO_Init(void)
{
	
	GPIO_InitTypeDef Init_GPIO_Structure;
	RCC_APB2PeriphClockCmd(IIC_GPIO_CLK,ENABLE);

	Init_GPIO_Structure.GPIO_Pin=IIC_SDA_PIN|IIC_SCL_PIN;
	Init_GPIO_Structure.GPIO_Mode=GPIO_Mode_Out_OD;
	Init_GPIO_Structure.GPIO_Speed=GPIO_Speed_50MHz;

	GPIO_Init(IIC_GPIOx,&Init_GPIO_Structure);

	IIC_SCL=1;
	IIC_SDA=1;
	IIC_Stop();
}


void IIC_Start(void)
{
	SDA_SET;
	SCK_SET;
	i2c_Delay();
	SDA_CLR;
	i2c_Delay();
	SCK_CLR;
	i2c_Delay();
}

void IIC_Stop(void)
{
   SDA_OUT();
	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C����ֹͣ�ź� */
	SDA_CLR;
	SCK_SET;
	i2c_Delay();
	SDA_SET;
}

u8 IIC_Wait_Ack(void)
{
	uint8_t re,ucErrTime=0;
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
            IIC_Stop();
            return 0;
        }
		  i2c_Delay();
    } 
	SCK_CLR;
	i2c_Delay();
	return 1;
}

void IIC_Ack(void)//��������������֮ǰ������SDA��ʾӦ��
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
void IIC_No_Ack(void)//��������������֮ǰ������SDA��ʾ��Ӧ��
{

   SDA_OUT();
	
	SDA_SET;	/* CPU����SDA = 1 */
	i2c_Delay();
	SCK_SET;	/* CPU����1��ʱ�� */
	i2c_Delay();
	SCK_CLR;
	i2c_Delay();

}
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��
void IIC_WriteByte(u8 txe)
{
	uint8_t i;
   SDA_OUT();
	/* �ȷ����ֽڵĸ�λbit7 */
	for (i = 0; i < 8; i++)
	{		
		if (txe & 0x80)
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
		txe <<= 1;	/* ����һ��bit */
		i2c_Delay();
	}

}
u8 IIC_Read_Byte(u8 ack)
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
 
	if(ack)
	{
	  IIC_Ack();
	}
	else
	{
	  IIC_No_Ack();
	}
	return Data;
}


