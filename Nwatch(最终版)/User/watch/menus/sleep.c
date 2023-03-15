/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

#include "common.h"

#define OPTION_COUNT	2

static prev_menu_s prevMenuData;

static void mSelect(void);
static void itemLoader(byte);
static display_t mDraw(void);
static void setTimeout(void);
static void setSleep(void);

void mSleepOpen()
{
	setMenuInfo(OPTION_COUNT, MENU_TYPE_ICON, PSTR(STR_SLEEPMENU));
	setMenuFuncs(MENUFUNC_NEXT, mSelect, MENUFUNC_PREV, itemLoader);
	menuData.func.draw = mDraw;

	setPrevMenuOpen(&prevMenuData, mSleepOpen);

	beginAnimation2(NULL);
}

static void mSelect()
{
	bool isExiting = exitSelected();
	if(isExiting)
		appconfig_save();
	setPrevMenuExit(&prevMenuData);
	doAction(isExiting);
}

static void itemLoader(byte num)
{
	UNUSED(num);
	setMenuOption_P(0, PSTR(STR_TIMEOUT), menu_sleeptimeout, setTimeout);
	setMenuOption_P(1, PSTR(STR_SLEEP), menu_sleeptimeout, setSleep);
//	setMenuOption_P(2, PSTR(STR_CLOCKMODE), NULL, NULL);
	addBackOption();
}

static display_t mDraw()
{
	if(menuData.selected == 0)
	{
		char buff[4];
		if(appConfig.sleepTimeout!=10000)
		{
		 sprintf_P(buff, PSTR("%hhuS"), appConfig.sleepTimeout * 5);
		}
		else
		{
		 sprintf_P(buff, PSTR("%xxS"), 0);
		}
		
		draw_string(buff, NOINVERT, 56, 40);
	}
	else if(menuData.selected == 1)
	{
		char buff[5]={'s','l','e','e','p'};
		
		draw_string(buff, NOINVERT, 48, 40);
	}
	return DISPLAY_DONE;
}

static void setTimeout()
{
	uint timeout = appConfig.sleepTimeout;
	timeout++;
	if(timeout > 12)
	{
	 timeout = 0;
	 appConfig.sleepTimeout = timeout;
	}
	else if(timeout==12)
	{
	 appConfig.sleepTimeout = 10000;
	}
	else 
	{
	 appConfig.sleepTimeout = timeout;
	}
		
	
}

static void setSleep(void)
{
	oled_power(OLED_PWR_OFF);

	TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Disable);
	TIM_Cmd(TIM3,DISABLE);	
	Beep_Gpio_Init();
	Key_Exti_Init_Config();
	SysTick->CTRL&=(~(SysTick_CTRL_ENABLE_Msk|SysTick_CTRL_TICKINT_Msk));
	__WFI();      
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,DISABLE);
	battery_updateNow();
	// Wake up
	time_wake();
	oled_power(OLED_PWR_ON);
}

