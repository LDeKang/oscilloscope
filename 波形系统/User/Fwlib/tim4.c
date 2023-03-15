#include "tim4.h"

#define TIMx  TIM4

static void Tim4_Gpio_Init(void)
{

  GPIO_InitTypeDef Usart_Pin_Structure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	Usart_Pin_Structure.GPIO_Pin=GPIO_Pin_7;
	Usart_Pin_Structure.GPIO_Mode=GPIO_Mode_AF_PP;
	Usart_Pin_Structure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&Usart_Pin_Structure);

}

void Tim4_Base_Init_Config(uint16_t Period,uint16_t Prescaler)
{
	
	TIM_TimeBaseInitTypeDef Tim4_Base_Structure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	
	Tim4_Base_Structure.TIM_Period=Period;
	Tim4_Base_Structure.TIM_Prescaler=Prescaler;	
	Tim4_Base_Structure.TIM_ClockDivision=TIM_CKD_DIV1;
	Tim4_Base_Structure.TIM_CounterMode=TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIMx,&Tim4_Base_Structure);


	
}

void Tim4_Oc_Config(uint16_t Period,uint16_t Prescaler,uint16_t Pulse,FunctionalState NewState)
{
	
	TIM_OCInitTypeDef Tim4_Oc_Init_Structure;
	
	Tim4_Gpio_Init();
  Tim4_Base_Init_Config(Period,Prescaler);
	
	Tim4_Oc_Init_Structure.TIM_OCMode=TIM_OCMode_PWM2;
	Tim4_Oc_Init_Structure.TIM_OCPolarity=TIM_OCPolarity_High;//有效是输出高电平
	Tim4_Oc_Init_Structure.TIM_OutputState=TIM_OutputState_Enable;
	Tim4_Oc_Init_Structure.TIM_Pulse=Pulse;

	TIM_OC2Init(TIMx,&Tim4_Oc_Init_Structure);
	
	TIM_ARRPreloadConfig(TIMx,ENABLE);
	
	TIM_OC2PreloadConfig(TIMx,TIM_OCPreload_Enable);
	
	TIM_Cmd(TIMx,NewState);
	
}












