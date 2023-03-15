/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

#include "common.h"
//500m -13h vreaure 38mv/h
//max:2100mv,min:1655
//1700 一下还可运行2个小时左右
void drawBattery()
{
	uint battLevel = battery_voltage();
	const byte* battIcon;
	if(battLevel < 1700)
		battIcon = battIconEmpty;
	else if(battLevel < 1850)
		battIcon = battIconLow;
	else if(battLevel < 2000)
		battIcon = battIconHigh;
	else
		battIcon = battIconFull;

	draw_bitmap(0, FRAME_HEIGHT - 8, battIcon, 16, 8, NOINVERT, 0);
}
