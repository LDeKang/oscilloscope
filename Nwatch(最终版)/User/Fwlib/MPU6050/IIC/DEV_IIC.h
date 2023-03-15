#ifndef _DEV_IIC__H
#define _DEV_IIC__H
#include "stm32f10x.h"

#include "sys.h"		

//IO方向设置
//#define SDA_IN()  {GPIOA->CRL&=0XF0FFFFFF;GPIOA->CRL|=8<<6;}
//#define SDA_OUT() {GPIOA->CRL&=0XF0FFFFFF;GPIOA->CRL|=3<<6;}

//#define SDA_IN()  {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=0X00008000;}
//#define SDA_OUT() {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=0X00003000;}


//IO操作函数	 
#define IIC_SCL    PBout(5) //SCL
#define IIC_SDA    PBout(4) //SDA	 
#define READ_SDA   PBin(4)  //输入SDA 

#define NO_ACK    0   
#define ACK       1   

void IIC_Start(void);
void IIC_Stop(void);
void IIC_WriteByte(u8 txe);
u8 IIC_Read_Byte(u8 ack);
void IIC_GPIO_Init(void);
u8 IIC_Wait_Ack(void);

#endif
