#include "key.h"
 
KEY_ST Key_Start;

void Key_Pin_Init(void)
{
  GPIO_InitTypeDef KEY_InitStructure;
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	
	KEY_InitStructure.Pin=GPIO_PIN_0;
	KEY_InitStructure.Mode=GPIO_MODE_INPUT;
	KEY_InitStructure.Pull=GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOA,&KEY_InitStructure);
	
	KEY_InitStructure.Pin=GPIO_PIN_13;
	HAL_GPIO_Init(GPIOC,&KEY_InitStructure);
	
 
}
void Key_St_Init(void)
{
  Key_Start.Key_Down=0;
	Key_Start.Key_No_Down=0;
	Key_Start.Key_Value=0;
	Key_Start.Key_Tick=0;
	Key_Start.Key_Tick_Flag=0;
	Key_Start.Key_Tick_Max=100;
}

void Key_Exti_Init(void)
{
  GPIO_InitTypeDef KEY_EXTI_InitStructure;
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	KEY_EXTI_InitStructure.Pin=GPIO_PIN_0;
	KEY_EXTI_InitStructure.Mode=GPIO_MODE_IT_RISING;
	KEY_EXTI_InitStructure.Pull=GPIO_PULLDOWN;
	
	HAL_GPIO_Init(GPIOA,&KEY_EXTI_InitStructure);
	
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_1);
	HAL_NVIC_SetPriority(EXTI0_IRQn, 1, 0);
	/* Ê¹ÄÜÖÐ¶Ï */
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	
	
}

void Key_Init(void)
{
 Key_Pin_Init();
 Key_St_Init();
}
static void Key_Delay(uint16_t Del)
{
	uint8_t i;
	for(i=0;i<15;i++)
	{
    while(Del--);		 
	}
	
}

uint8_t Key1_Chack(void)
{
 
	if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==1)
	{
	  Key_Delay(200);
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==1)
		{ 
			while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==1);
      return 1;
		}
	}
	
	return 0;

}

uint8_t Key2_Chack(void)
{
 
	if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13)==1)
	{
	  Key_Delay(200);
		if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13)==1)
		{ 
			while(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13)==1);
      return 1;
		}
	}
	
	return 0;

}

uint8_t Key_Scan(void)
{
  uint16_t Key_Board=20,Even_Start=12;
	
	if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==1)Even_Start=1;
	else if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13)==1)Even_Start=1;
	
	if(Even_Start!=12)
	{
		 Key_Start.Key_Tick_Flag=1;
	}
	else
	{
		 Key_Start.Key_Tick_Flag=0;
		 Key_Start.Key_Tick=0;
		 Key_Start.Key_No_Down=1;
	}

	if(Key_Start.Key_Tick>Key_Start.Key_Tick_Max)
	{
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==1)Key_Start.Key_Value=1;
		else if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13)==1)Key_Start.Key_Value=2;
		
		Key_Start.Key_No_Down=0;
		Key_Start.Key_Down=1;
	}
	
	if((Key_Start.Key_Down==1)&&(Key_Start.Key_No_Down==1))
	{
	  Key_Start.Key_Down=0;
		Key_Board=Key_Start.Key_Value;
	}
	
	return Key_Board;
}














