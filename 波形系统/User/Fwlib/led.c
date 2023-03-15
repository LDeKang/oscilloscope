#include "led.h"
#include "sys.h"


void Led_Pin_Init_Config(void)
{
  GPIO_InitTypeDef Led_Pin_Init_Structure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	Led_Pin_Init_Structure.GPIO_Pin=GPIO_Pin_2|GPIO_Pin_3;
	Led_Pin_Init_Structure.GPIO_Mode=GPIO_Mode_Out_PP;
	Led_Pin_Init_Structure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&Led_Pin_Init_Structure);
  LED1_STATE=OFF;
	LED2_STATE=OFF;
	
 

}
