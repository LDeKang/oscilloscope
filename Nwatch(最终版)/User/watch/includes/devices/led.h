/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

#ifndef LED_H_
#define LED_H_
#include "stm32f10x.h"
#include "sys.h"

typedef enum
{
	LED_RED,
	LED_GREEN
} led_t;

#define LED1_STATE  PBout(2)
#define LED2_STATE  PBout(11)


//#define LED1_TTL  PAout(1)
//#define LED2_TTL  PAout(2)
//#define LED3_TTL  PAout(3)

#define ON    0
#define OFF   1

//#define LED_FLASH_INFINITE	0
//#define LED_ALL				255
#define LED_FLASH_FAST	50
#define LED_BRIGHTNESS_MAX	255

void led_init(void);
//void led_flash(led_t, byte, byte);
//BOOL led_flashing(void);
void led_update(void);
void led_stop(void);

#endif /* LED_H_ */
