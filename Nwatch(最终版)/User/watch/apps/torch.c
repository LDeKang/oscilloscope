/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

#include "common.h"

static uint strobe;

static bool btnExit(void);
static bool btnFlashRate(void);
static display_t draw(void);

void torch_open()
{
	menu_close();
	LED1_STATE=0;
	LED2_STATE=0;	
	strobe = 0;
	display_setDrawFunc(draw);
	buttons_setFuncs(btnExit, btnFlashRate, btnExit);
}

static bool btnExit()
{
	LED1_STATE=1;
	LED2_STATE=1;	
	oled_setInvert(appConfig.invert);
	display_load();

	return true;
}

static bool btnFlashRate()
{
	if(strobe < 500)
		strobe += 50 * ((strobe / 50) + 1);
	else
		strobe = 0;
	return true;
}

static display_t draw()
{
	static bool invert;
	static millis_t lastStrobe;

	if(strobe)
	{
		millis_t now = millis();
		if(now - lastStrobe >= strobe)
		{
			lastStrobe = now;
			invert = !invert;
			oled_setInvert(invert);
			LED1_STATE=~LED1_STATE;
			LED2_STATE=~LED1_STATE;
		}
		return DISPLAY_BUSY;
	}

	oled_setInvert(true);


	return DISPLAY_DONE;
}
