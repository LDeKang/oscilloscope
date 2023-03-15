/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

// Deals with sleeping and waking up

#include "common.h"

#define BATTERY_CUTOFF	2800


typedef enum
{
	SYS_AWAKE,
	SYS_CRTANIM,
	SYS_SLEEP
} sys_t;

typedef enum
{
	USER_ACTIVE,
	USER_INACTIVE
} user_t;

static pwr_state_t active[PWR_ACTIVE_COUNT];
static sys_t systemState;
static user_t userState;

static void batteryCutoff(void);
static void userWake(void);
static void userSleep(void);
 
void pwrmgr_init()
{
	systemState = SYS_AWAKE;
	userState = USER_ACTIVE;
}

static void batteryCutoff()
{
   static uint Value=1650;
	uint voltage = battery_voltage();
	if(voltage <Value)
	{

		   oled_power(OLED_PWR_OFF);//�ر�OLED
		
			TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Disable);//�رշ�����
			TIM_Cmd(TIM3,DISABLE);	
			Beep_Gpio_Init();
			Key_Exti_Init_Config();//���ð�������
			SysTick->CTRL&=(~(SysTick_CTRL_ENABLE_Msk|SysTick_CTRL_TICKINT_Msk));//�ر�ϵͳ��ʱ��
			__WFI();      
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,DISABLE);
			battery_updateNow();
		// Wake up
		time_wake();
		oled_power(OLED_PWR_ON);
 	
		Value=Value-80;
	}
}

void pwrmgr_update()
{

	static u8 Flag=0;
//	
   batteryCutoff();		
   Shut_Down();
	bool buttonsActive1 = buttons_isActive();//��ȡ����״̬
	bool buttonsActive2 = Check_Acttive();//��ȡMPU��״̬
	
	if(buttonsActive1||buttonsActive2)
	{
	  if(Flag==1)
	  {
	     userWake();//����
		  Flag=0;
		 
		  
	  }
	  
	}
	else
	{
	  Flag=1;
	  userSleep();//Ϣ��
	}
}

void pwrmgr_setState(pwr_active_t thing, pwr_state_t state)
{
 
}

bool pwrmgr_userActive()
{
	return userState == USER_ACTIVE;
}

static void userWake()
{
	userState = USER_ACTIVE;
	oled_power(OLED_PWR_ON);
}

static void userSleep()
{
	userState = USER_INACTIVE;
	oled_power(OLED_PWR_OFF);
}



