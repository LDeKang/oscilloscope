#include "key.h"
#include "led.h"


KEY_ST Key_Even_St;
 
 
static void Key_Delay(uint16_t Cnt)
{
 u8 i;
	while(Cnt--)
	{
		for(i=0;i<16;i++);
	}

}
 
void Key_Pin_Config(void)
{
  GPIO_InitTypeDef Key_Pin_Init_Structure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);
	
	Key_Pin_Init_Structure.GPIO_Pin=GPIO_Pin_1|GPIO_Pin_7|GPIO_Pin_8;
	Key_Pin_Init_Structure.GPIO_Mode=GPIO_Mode_IPU;
	
	GPIO_Init(GPIOA,&Key_Pin_Init_Structure);
	
}

static void Exti_Nvic_Config(void)
{
  NVIC_InitTypeDef Exti_Nvic_Structure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	Exti_Nvic_Structure.NVIC_IRQChannel=EXTI0_IRQn;
	Exti_Nvic_Structure.NVIC_IRQChannelPreemptionPriority=1;
	Exti_Nvic_Structure.NVIC_IRQChannelSubPriority=1;
	Exti_Nvic_Structure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&Exti_Nvic_Structure);
}



void Key_Exti_Init_Config(void)
{
  EXTI_InitTypeDef Key_Exti_Structure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource0);
	Key_Pin_Config();
	Exti_Nvic_Config();
	
	Key_Exti_Structure.EXTI_Line=EXTI_Line0;
	Key_Exti_Structure.EXTI_Mode=EXTI_Mode_Interrupt;
	Key_Exti_Structure.EXTI_Trigger=EXTI_Trigger_Falling;
	Key_Exti_Structure.EXTI_LineCmd=ENABLE;
	
	EXTI_Init(&Key_Exti_Structure);

}


 
u8 Key1_Chack(void)
{
  if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)==0)
	{
	  Key_Delay(100);
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)==0);
		return 1;
	}
	else
	{
	  return 0;
	}
}

u8 Key2_Chack(void)
{
  if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)==0)
	{
	  Key_Delay(100);
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)==0);
		return 1;
	}
	else
	{
	  return 0;
	}
}

u8 Key3_Chack(void)
{
  if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0)
	{
	  Key_Delay(100);
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0);
		return 1;
	}
	else
	{
	  return 0;
	}
}

//���������жϼ�����
u8 Key4_Chack(void)
{
  if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)==0)
	{
	  Key_Delay(100);
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)==0);
		return 1;
	}
	else
	{
	  return 0;
	}
}

u8 Key_Chack(void)
{
  static uint16_t Count=0;
	u8 Kset_Even1=0,Key_Value=0;
	
  if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==0)Kset_Even1=1;
	else if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)==0)Kset_Even1=1;
	else if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)==0)Kset_Even1=1;
	
	if(Kset_Even1!=0)//��һ���а�������
	{	
	  Count++;
	}
	else
	{
    Key_Even_St.No_Down=1;//û�а������£���Ϊ��Ӧ��־
	}
	
	if(Count>10)//ȷ���а�������
	{
		Count=0;
    if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)==0)Key_Value=2;
		else if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==0)Key_Value=3;
		else if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)==0)Key_Value=4;

		Key_Even_St.No_Down=0;//�������£������Ӧ��־
		Key_Even_St.Down=1;//�������£���Ϊ��Ӧ��־
		Key_Even_St.Value=Key_Value;//���水��ֵ������ʱ����	

	}
	else
	{
 	   Key_Value=0;//�����ּ���������׶ζ�Ϊ0

	}
	
	if((Key_Even_St.Down==1)&&(Key_Even_St.No_Down==1))//���������ж�
	{
	  Key_Even_St.Down=0;
		Key_Value=Key_Even_St.Value;
		
		return Key_Value;
	}
	else
	{
	 		Key_Value=0;//�����ּ���������׶ζ�Ϊ0
	}
	
		return Key_Value;
}




























