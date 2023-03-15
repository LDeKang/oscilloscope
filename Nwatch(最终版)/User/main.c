/**
  ******************************************************************************
  * @file    stm32\野火\指南者\比较完善的工程\USART\User\mian.c 
  * @author  Lin De Kang
  * @version V3.5.0
  * @date    2022/4/8
  * @brief   Main program
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "usart.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "common.h"
 
uint tunex[10]={TONE_2KHZ,TONE_2_5KHZ,TONE_3KHZ,TONE_4KHZ,TONE_REPEAT};


void Delay(uint16_t Cnt);
static void Dev_init();
 void Start_Anm(void);

int main (void)
{
	uint16_t Cnt=1;
	
	Systick_Init_Config();
	Power_Pin_Init();
   Dev_init();

	display_set(watchface_normal);
	display_load();
   tune_play(tunex, VOL_UI, PRIO_UI);
	while(1)
	{
		if(Heart_Rate.Dis_Start==1)
		{
		 buttons_update();
		 Heart_Updata();
		 draw_end();
		}
		else
		{
			  time_update();
	//		if(pwrmgr_userActive())
	//		{
				buttons_update();
				buzzer_update();

	//		}
			  stopwatch_update();		
			  

			if(pwrmgr_userActive())//系统状态
			{
//            alarm_update();	
				battery_update();
				display_update();	
			}
			  pwrmgr_update();		
		}

	}
		

}
 
static void Dev_init()
{

	appconfig_init();
	Usart1_Init_Config();
	buttons_init();
	I2c_Gpio_Init();
	Spi_Gpio_Init();
	Led_Pin_Init_Config();
   rtc_init();	
	time_init();	
   buzzer_init();
	battery_init();
	Usb_Pin_Init();
   Heart_Init();
   BMP_Init();
	MUP_Init();
	mpu_dmp_init();	//=====初始化MPU6050的DMP模式	
	oled_init();						 	
	T_Bmp_Init();
 

}
void Delay(uint16_t Cnt)
{
 while(Cnt--);
}










