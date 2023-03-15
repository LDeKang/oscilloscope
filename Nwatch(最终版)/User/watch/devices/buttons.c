
#include "systick.h"
#include "common.h"
 

#define BTN_IS_PRESSED	4
#define BTN_NOT_PRESSED	4

#define BTN_1_P	0
#define BTN_2_P	1
#define BTN_3_P	2
 
#define TIME_OUT	1000
typedef struct 
{
	millis_t pressedTime;	// Time of press
	uint32_t processed;			//已存储按下时间(不要再次存储，直到下次按下)
	byte counter;			// 消抖动
	bool funcDone;			//函数已运行(在下次按下之前不要再运行)
	button_f onPress;		//按下时运行的函数
	const uint16_t* tune;		// 按键的声音
	
}s_button;

static s_button buttons[BTN_COUNT];

static void processButtons(void);
static void processButton(s_button*, BOOL);
static byte bitCount(byte);

void Key_Pin_Config(void)
{
  GPIO_InitTypeDef Key_Pin_Init_Structure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	Key_Pin_Init_Structure.GPIO_Pin=GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_10;
	Key_Pin_Init_Structure.GPIO_Mode=GPIO_Mode_IPU;
	
	GPIO_Init(GPIOB,&Key_Pin_Init_Structure);
}


static void Exti_Nvic_Config(void)
{
   NVIC_InitTypeDef Exti_Nvic_Structure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	Exti_Nvic_Structure.NVIC_IRQChannel=EXTI15_10_IRQn;
	Exti_Nvic_Structure.NVIC_IRQChannelPreemptionPriority=1;
	Exti_Nvic_Structure.NVIC_IRQChannelSubPriority=1;
	Exti_Nvic_Structure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&Exti_Nvic_Structure);
}



void Key_Exti_Init_Config(void)
{
   EXTI_InitTypeDef Key_Exti_Structure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource10);
	Key_Pin_Config();
	Exti_Nvic_Config();
	
	Key_Exti_Structure.EXTI_Line=EXTI_Line10;
	Key_Exti_Structure.EXTI_Mode=EXTI_Mode_Interrupt;
	Key_Exti_Structure.EXTI_Trigger=EXTI_Trigger_Falling;
	Key_Exti_Structure.EXTI_LineCmd=ENABLE;
	
	EXTI_Init(&Key_Exti_Structure);

}

void buttons_init()
{
	buttons_startup();
}

//10ms更新一次按键 lastUpdate为上一次进入函数的时间
void buttons_update()
{
	static millis_t lastUpdate;

	millis_t now = millis();
	if((millis_t)(now - lastUpdate) >= 10)
	{
		lastUpdate = now;
		processButtons();
	}		
}

void buttons_startup()
{
  Key_Pin_Config();
}
 

static void processButtons()
{
	// 获取按钮按下状态
	BOOL isPressed[BTN_COUNT];
	isPressed[BTN_1] = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14);
	isPressed[BTN_2] = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10);
	isPressed[BTN_3] = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13);

	//检测每个按键的状态
	LOOPR(BTN_COUNT, i)
		processButton(&buttons[i], !isPressed[i]);
}

static void processButton(s_button* button, BOOL isPressed)
{
	button->counter <<= 1;
	if(isPressed)
	{
		//设置反弹计数器位
		button->counter |= 1;
		
		//是否设置了足够的位作为按下计数?
		if(bitCount(button->counter) >= BTN_IS_PRESSED)
		{
			//保存按压时间
			if(!button->processed)
			{
				button->pressedTime = millis();
				button->processed = true;
			}

			// 跑函数
			if(!button->funcDone && button->onPress != NULL && button->onPress())
			{
				button->funcDone = true;//函数已运行(在下次按下之前不要再运行)
//             buzzer_buzz(200, TONE_2KHZ, VOL_UI, PRIO_UI, NULL);   
			}
			else
			{
			
			}
		}
	}
	else //不在按压的情况
	{
		//按钮按的时间不够长吗?
		if(bitCount(button->counter) <= BTN_NOT_PRESSED)
		{
			button->processed = false;
			button->funcDone = false;
		}
	}	
}

//计数值中的设置位
static byte bitCount(byte val)
{
	byte count = 0;
	for(;val;val >>= 1)
		count += val & 1;
	return count;
}

//设置新函数在按下按钮时运行，并返回旧函数
button_f buttons_setFunc(btn_t btn, button_f func)
{
	button_f old = buttons[btn].onPress;
	buttons[btn].onPress = func;
	return old;
}

//设置每个按钮的运行函数
void buttons_setFuncs(button_f btn1, button_f btn2, button_f btn3)
{
	buttons[BTN_1].onPress = btn1;
	buttons[BTN_2].onPress = btn2;
	buttons[BTN_3].onPress = btn3;
}

 

 //查看按键多久没有按下了
bool buttons_isActive()
{
	// If sleep has been disabled then just say that the buttons are always active
	if(!appConfig.sleepTimeout)
		return true;

	// Get timeout val in ms
	uint32_t timeout = (appConfig.sleepTimeout * 10) * 1000;

	// 扫描每个按键
	LOOPR(BTN_COUNT, i)
	{
		if(millis() - buttons[i].pressedTime < timeout)
			return true;
	}

	return false;
}
//关机函数
void Shut_Down(void)
{
 
	if(buttons[0].processed == true)//按键没有松开之前一直都是ture
	{
	  if(millis()-(buttons[0].pressedTime)>TIME_OUT)
	  {
		LED1_STATE=0;	
		T_Bmp_Init();

		GPIO_ResetBits(GPIOB,GPIO_Pin_12); //拉低IO使手表关机
	  }
	}
 
 	 
}