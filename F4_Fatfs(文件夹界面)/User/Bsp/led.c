#include "led.h"
 
 
void LED_Pin_Init(void)
{
  GPIO_InitTypeDef LED_InitStructure;
	__HAL_RCC_GPIOF_CLK_ENABLE();
	
	LED_InitStructure.Pin=GPIO_PIN_6;
	LED_InitStructure.Mode=GPIO_MODE_OUTPUT_PP;
	LED_InitStructure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;
	LED_InitStructure.Pull=GPIO_NOPULL;
	HAL_GPIO_Init(GPIOF,&LED_InitStructure);
	
	LED_STAET=0;

}