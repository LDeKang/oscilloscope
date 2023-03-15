/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

#include "common.h"

#define OPTION_COUNT		3

static prev_menu_s prevMenuData;

static void mSelect(void);
static void itemLoader(byte);
static void updatePressure();
static void updateAltitude();
static void updateHigh();


double Pressure;
double H;

void mPressuresOpen()
{
   
	setMenuInfo(OPTION_COUNT, MENU_TYPE_STR, PSTR(STR_PRESSURESX));
	setMenuFuncs(MENUFUNC_NEXT, MENUFUNC_PREV, mSelect, itemLoader);

	setPrevMenuOpen(&prevMenuData, mDiagOpen);
	
	beginAnimation2(NULL);
}

static void mSelect()
{
	setPrevMenuExit(&prevMenuData);
	doAction(exitSelected());
}

static void itemLoader(byte num)
{
	switch(num)
	{
		case 0:
			updatePressure();
			return;
		case 1:
			updateAltitude();
			return;
		case 2:
			updateHigh();
			return;
	}
 
	addBackOption();
}

static void updatePressure()
{
	char buff[24];
	Pressure=BMP280_Get_Pressure();
	H=(1013.25 -Pressure/100)*9;
	sprintf_P(buff, PSTR(STR_PRESSURE), (uint32_t)Pressure/100);

	setMenuOption(0, buff, NULL, NULL);
}

static void updateAltitude()
{

	char buff[24];
	sprintf_P(buff, PSTR(STR_ALTITUDE), (uint16_t)H);

	setMenuOption(1, buff, NULL, NULL);
}

static void updateHigh()
{	

	char buff[20];
	
 	
	sprintf_P(buff, PSTR(STR_HIGH), (uint16_t)(H-335));
	setMenuOption(2, buff, NULL, NULL);
}
 