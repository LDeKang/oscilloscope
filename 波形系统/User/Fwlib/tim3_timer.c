#include "tim3_timer.h"

#define TIMx  TIM3

uint32_t Tim_Count;

void Tim3_Nvic_Config(void)
{
 NVIC_InitTypeDef Tim3_Nvic_Init_Structure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	Tim3_Nvic_Init_Structure.NVIC_IRQChannel=TIM3_IRQn;
	Tim3_Nvic_Init_Structure.NVIC_IRQChannelPreemptionPriority=1;
	Tim3_Nvic_Init_Structure.NVIC_IRQChannelSubPriority=1;
	Tim3_Nvic_Init_Structure.NVIC_IRQChannelCmd=ENABLE;
	
	NVIC_Init(&Tim3_Nvic_Init_Structure);
}


void Tim3_Timer_Config(uint16_t Period,uint16_t Prescaler,FunctionalState NewState)
{
  TIM_TimeBaseInitTypeDef TIM3_Init_Structure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	TIM3_Init_Structure.TIM_Period=Period;
  TIM3_Init_Structure.TIM_Prescaler=Prescaler;
	TIM3_Init_Structure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM3_Init_Structure.TIM_ClockDivision=TIM_CKD_DIV1;

	TIM_TimeBaseInit(TIMx,&TIM3_Init_Structure);
	
//	Tim3_Nvic_Config();
//	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	TIM_SelectOutputTrigger(TIMx,TIM_TRGOSource_Update);
	TIM_SelectMasterSlaveMode(TIMx,TIM_MasterSlaveMode_Enable);
	
	TIM_Cmd(TIMx,NewState);
}




