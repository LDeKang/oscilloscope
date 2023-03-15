/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

#ifndef BUTTONS_H_
#define BUTTONS_H_
#include "stm32f10x.h"
#include "stdio.h"

typedef enum
{
	BTN_1 = 0,
	BTN_2 = 1,
	BTN_3 = 2,
	BTN_COUNT = 3, // must be last
} btn_t; 

//typedef uint16_t (*button_f)(void);


//#define false 0
//#define true  1
//#define BOOL u8
//#define millis_t uint32_t
//#define byte u8

void Key_Pin_Config(void);


void buttons_init(void);
void buttons_update(void);
void buttons_startup(void);
void buttons_shutdown(void);
button_f buttons_setFunc(btn_t, button_f);
void buttons_setFuncs(button_f, button_f, button_f);
//millis_t buttons_pressTime(btn_t);
 bool buttons_isActive();
void buttons_wake(void);
u8 Key1_Chack(void);
u8 Key2_Chack(void);
u8 Key3_Chack(void);
void Shut_Down(void);
#endif /* BUTTONS_H_ */
