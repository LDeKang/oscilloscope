/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

#include "common.h"
 
 
static prev_menu_s prevMenuData;
static display_t draw();
u8 Arr1[]="Good morning,and in";
u8 Arr2[]="case Idon't see you";
u8 Arr3[]="good afternoon,";
u8 Arr4[]="good evening,";
u8 Arr5[]="and good night!";
u8 Arr6[]="Dear friends:";
u8 Arr7[]="--<Truman's world>";
static bool btnExit()
{
   appConfig.sleepTimeout = 5;
	display_load();
	return true;
}

void mLetterOpen()
{
	menu_close();
	appConfig.sleepTimeout = 10000;
	display_setDrawFunc(draw);
	buttons_setFuncs(NULL, btnExit, NULL);
}
 
 static display_t draw()
{
   draw_string(Arr6,0,0,0);
   draw_string(Arr1,0,0,9);
   draw_string(Arr2,0,0,19);	
   draw_string(Arr3,0,0,29);	
   draw_string(Arr4,0,0,39);	
   draw_string(Arr5,0,0,49);
   draw_string(Arr7,0,0,58);
	
	return 0;
}


 