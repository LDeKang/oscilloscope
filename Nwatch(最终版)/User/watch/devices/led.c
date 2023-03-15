/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

// LED control

#include "common.h"
#include "sys.h"
// Registers and stuff

typedef struct {
	byte flashLen;			// How long to light up for
	millis8_t startTime;	// 
}led_s;


static void flash(led_s*, byte, byte, volatile byte*, byte, byte);
static BOOL update(led_s*, byte, byte);



void Led_Pin_Init_Config(void)
{
  GPIO_InitTypeDef Led_Pin_Init_Structure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	Led_Pin_Init_Structure.GPIO_Pin=GPIO_Pin_2|GPIO_Pin_11;
	Led_Pin_Init_Structure.GPIO_Mode=GPIO_Mode_Out_PP;
	Led_Pin_Init_Structure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&Led_Pin_Init_Structure);
  LED1_STATE=OFF;
	LED2_STATE=OFF;
	
 

}

void led_init()
{
  Led_Pin_Init_Config();
}

static void flash(led_s* led, byte len, byte brightness, volatile byte* ocr, byte com, byte pin)
{

}

void led_flash(led_t led, byte len, byte brightness)
{


}

// Is an LED on?
BOOL led_flashing()
{
	return 0;
}

static BOOL update(led_s* led, byte com, byte pin)
{

	return 0;
}

void led_update()
{

}

// Turn off LEDs
void led_stop()
{

}
