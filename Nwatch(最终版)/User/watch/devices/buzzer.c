

#include "common.h"

static byte buzzLen;
static millis8_t startTime;
static buzzFinish_f onFinish;
static tonePrio_t prio;

static void stop(void);
#define TIMx TIM3

static void Timx_Gpio_Init(void)
{

   GPIO_InitTypeDef Beep_Pin_Structure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	Beep_Pin_Structure.GPIO_Pin=GPIO_Pin_0;
	Beep_Pin_Structure.GPIO_Mode=GPIO_Mode_AF_PP;
	Beep_Pin_Structure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&Beep_Pin_Structure);

}

  void  Beep_Gpio_Init(void)
{

   GPIO_InitTypeDef Beep_Pin_Structure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	Beep_Pin_Structure.GPIO_Pin=GPIO_Pin_0;
	Beep_Pin_Structure.GPIO_Mode=GPIO_Mode_AIN; 
	Beep_Pin_Structure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&Beep_Pin_Structure);
	

}

void Timx_Base_Init_Config(uint16_t Period,uint16_t Prescaler)
{
	
	TIM_TimeBaseInitTypeDef Timx_Base_Structure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	Timx_Base_Structure.TIM_Period=Period;
	Timx_Base_Structure.TIM_Prescaler=Prescaler;	
	Timx_Base_Structure.TIM_ClockDivision=TIM_CKD_DIV1;
	Timx_Base_Structure.TIM_CounterMode=TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIMx,&Timx_Base_Structure);


	
}
//PWM����
void Timx_Oc_Config(uint16_t Period,uint16_t Prescaler,uint16_t Pulse,FunctionalState NewState)
{
	
	TIM_OCInitTypeDef Timx_Oc_Init_Structure;
	
	Timx_Gpio_Init();
   Timx_Base_Init_Config(Period,Prescaler);
	
	Timx_Oc_Init_Structure.TIM_OCMode=TIM_OCMode_PWM2;
	Timx_Oc_Init_Structure.TIM_OCPolarity=TIM_OCPolarity_High;//��Ч������ߵ�ƽ
	Timx_Oc_Init_Structure.TIM_OutputState=TIM_OutputState_Enable;
	Timx_Oc_Init_Structure.TIM_Pulse=Pulse;

	TIM_OC3Init(TIMx,&Timx_Oc_Init_Structure);
	
	TIM_ARRPreloadConfig(TIMx,ENABLE);
	
	TIM_OC3PreloadConfig(TIMx,TIM_OCPreload_Enable);
	
	TIM_Cmd(TIMx,NewState);
	
}

void Set_Pwm(uint16_t Period,uint16_t Prescaler,uint16_t Pulse)
{
	TIM3->ARR = Period ;
	/* Set the Prescaler value */
	TIM3->PSC = Prescaler;
	TIM3->CCR3=Pulse;
}


void buzzer_init()
{
 Timx_Oc_Config(1000,71,1000,DISABLE);
}

//����������
void buzzer_buzz(byte len, tone_t tone, vol_t volType, tonePrio_t _prio, buzzFinish_f _onFinish)
{
	if(_prio < prio)
		return;
	else if(tone == TONE_STOP)
	{
		stop();
		return;
	}
 
	prio = _prio;
	onFinish = _onFinish;
	buzzLen = len;
	startTime = millis();

	//��Ĭͣ����
	if(tone == TONE_PAUSE)
	{
		stop();
		return;
	}
	uint ocr;//��װ�ص�ֵ
	byte vol;//��Чʱ��

	switch(volType)
	{
		case VOL_UI:
			vol = appConfig.volUI;
			break;
		case VOL_ALARM:
			vol = appConfig.volAlarm;
			break;
		case VOL_HOUR:
			vol = appConfig.volHour;
			break;
		default:
			vol = 2;
			break;
	}

	//��������Ƶ�ʵ����Ӷ���С
	//��ʹ����Ƶ�ʵĹ��ı��ֲ��䣬����������Ƶ�ʵ����߶��½�

	vol--;
	if(vol > 2)
		return;
	uint icr = tone * 10;
	ocr = icr / (64>>vol);//��������Ƶ�ʵ����Ӷ���С
	Timx_Gpio_Init();
	TIM_OC3PreloadConfig(TIMx,TIM_OCPreload_Enable);
	TIM_Cmd(TIMx,ENABLE);
   Set_Pwm(71,icr,ocr);
}

// ��ȡ������������ʱ��
bool buzzer_buzzing()
{
	return buzzLen;
}

//�����ǲ��Ǹ�ֹͣ��������
void buzzer_update()
{
	if(buzzLen && (millis8_t)(millis() - startTime) >= buzzLen)
	{
		stop();

		if(onFinish != NULL)
			onFinish();
	}
}

static void stop(void)
{
	buzzLen = 0;
	TIM_OC3PreloadConfig(TIMx,TIM_OCPreload_Disable);
	TIM_Cmd(TIMx,DISABLE);	
	Beep_Gpio_Init();
	
	prio = PRIO_MIN;
}