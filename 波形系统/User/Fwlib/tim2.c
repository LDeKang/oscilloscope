#include "tim2.h"

u8 Cap_Flag=0;

#define TIMx  TIM2


//ENABLE 开启捕获 DISABLE 开启ADC
void Control_Ic_Pin(FunctionalState NewState)
{
  if(NewState)
	{
		GPIO_InitTypeDef Gpio_Pin_Structure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
		
		Gpio_Pin_Structure.GPIO_Pin=GPIO_Pin_0;
		Gpio_Pin_Structure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
		
		GPIO_Init(GPIOA,&Gpio_Pin_Structure);
		
	}
	else
	{
		GPIO_InitTypeDef Gpio_Pin_Structure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
		
		Gpio_Pin_Structure.GPIO_Pin=GPIO_Pin_0;
		Gpio_Pin_Structure.GPIO_Mode=GPIO_Mode_AIN;
		
		GPIO_Init(GPIOA,&Gpio_Pin_Structure);
	}
}

static void Tim2_Gpio_Init(void)
{
	
  GPIO_InitTypeDef Gpio_Pin_Structure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	Gpio_Pin_Structure.GPIO_Pin=GPIO_Pin_0;
	Gpio_Pin_Structure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	
	GPIO_Init(GPIOA,&Gpio_Pin_Structure);
	

}
void Tim2_Nvic_Config(void)
{
 NVIC_InitTypeDef Tim2_Nvic_Init_Structure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	Tim2_Nvic_Init_Structure.NVIC_IRQChannel=TIM2_IRQn;
	Tim2_Nvic_Init_Structure.NVIC_IRQChannelPreemptionPriority=1;
	Tim2_Nvic_Init_Structure.NVIC_IRQChannelSubPriority=1;
	Tim2_Nvic_Init_Structure.NVIC_IRQChannelCmd=ENABLE;
	
	NVIC_Init(&Tim2_Nvic_Init_Structure);
}

void Tim2_Base_Init_Config(void)
{
	
	TIM_TimeBaseInitTypeDef Tim2_Base_Structure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	Tim2_Base_Structure.TIM_Period=65535;
	Tim2_Base_Structure.TIM_Prescaler=71;	
	Tim2_Base_Structure.TIM_ClockDivision=TIM_CKD_DIV1;
	Tim2_Base_Structure.TIM_CounterMode=TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIMx,&Tim2_Base_Structure);


	
}

void Tim2_Ic_Config(FunctionalState NewState)
{
	
	TIM_ICInitTypeDef Tim2_Ic_Init_Structure;
	
	Tim2_Gpio_Init();
  Tim2_Base_Init_Config();
	
	Tim2_Ic_Init_Structure.TIM_Channel=TIM_Channel_1;//选择CH1
	Tim2_Ic_Init_Structure.TIM_ICPolarity=TIM_ICPolarity_Rising;//捕获极性
	Tim2_Ic_Init_Structure.TIM_ICPrescaler=TIM_ICPSC_DIV1;//一个事件捕获一次
	Tim2_Ic_Init_Structure.TIM_ICSelection=TIM_ICSelection_DirectTI;//选择配置为主输入口
	Tim2_Ic_Init_Structure.TIM_ICFilter=0x00;
	
	TIM_ICInit(TIMx,&Tim2_Ic_Init_Structure);
	
	Tim2_Nvic_Config();
	
	TIM_ITConfig(TIMx,TIM_IT_CC1,NewState);
	
	TIM_SelectInputTrigger(TIMx,TIM_TS_TI1FP1);
	
  TIM_SelectSlaveMode(TIMx, TIM_SlaveMode_Reset);
  TIM_SelectMasterSlaveMode(TIMx,TIM_MasterSlaveMode_Enable); 
	
	TIM_Cmd(TIMx,NewState);
	
}






