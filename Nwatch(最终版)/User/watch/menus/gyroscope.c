/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

#include "common.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#define OPTION_COUNT		0

void Dis_Attitude_angle(void);

u8 Pitch_Arr[]="Pitch=+00.0";
u8 Roll_Arr[]="roll=+00.0";
u8 Yaw_Arr[]="yaw=+00.0";
u8 Gyr_State;
uint32_t pressedTime;
float pitch=-11.4,roll=12.8,yaw=44.4;


static prev_menu_s prevMenuData;
static display_t draw();
 
static bool btnExit()
{
   appConfig.sleepTimeout = 1;
	display_load();
	return true;
}

void mGyoscopeOpen()
{
	menu_close();
	appConfig.sleepTimeout = 10000;
	display_setDrawFunc(draw);
	buttons_setFuncs(NULL, btnExit, NULL);
}



 
 static display_t draw()
{
	mpu_dmp_get_data(&pitch,&roll,&yaw);
   Dis_Attitude_angle();
	return 0;
}

u8 Check_Acttive(void)
{
   mpu_dmp_get_data(&pitch,&roll,&yaw);
	 
	if(!appConfig.sleepTimeout)
	   return true;

	uint32_t timeout = (appConfig.sleepTimeout * 10) * 1000;

	if(millis() - pressedTime < timeout)
	   return true;
 
	if((pitch<-70)&&(pitch>-90)&&(Gyr_State==0))
	{
		Gyr_State=1;
	}
	if((pitch<30)&&(pitch>0)&&(Gyr_State==1))
	{
		pressedTime=millis();
		Gyr_State=0;
	}

	return false;
}
void Dis_Attitude_angle(void)
{
  float  pitch_x,roll_x,yaw_x;
	  if(pitch>=0)//x
			{
				
			  pitch_x=pitch*10.0;
				Pitch_Arr[6]='+';
				Pitch_Arr[7]=((uint16_t)pitch_x)/100+0x30;
				Pitch_Arr[8]=((uint16_t)pitch_x)/10%10+0x30;
				Pitch_Arr[10]=((uint16_t)pitch_x)%10+0x30;
				draw_string(Pitch_Arr,0,10,10);

			}
			else
			{
			 pitch_x=pitch*-10;
				Pitch_Arr[6]='-';
				Pitch_Arr[7]=((uint16_t)pitch_x)/100+0x30;
				Pitch_Arr[8]=(((uint16_t)pitch_x)/10)%10+0x30;
				Pitch_Arr[10]=((uint16_t)pitch_x)%10+0x30;
				draw_string(Pitch_Arr,0,10,10);
				
			}	 
			
		  if(roll>=0)//y
			{
				
			  roll_x=roll*10;
				Roll_Arr[5]='+';
				Roll_Arr[6]=((uint16_t)roll_x)/100+0x30;
				Roll_Arr[7]=((uint16_t)roll_x)/10%10+0x30;
				Roll_Arr[9]=((uint16_t)roll_x)%10+0x30;
				draw_string(Roll_Arr,0,10,20);

			}
			else
			{
			 roll_x=roll*-10;
				Roll_Arr[5]='-';
				Roll_Arr[6]=((uint16_t)roll_x)/100+0x30;
				Roll_Arr[7]=(((uint16_t)roll_x)/10)%10+0x30;
				Roll_Arr[9]=((uint16_t)roll_x)%10+0x30;
				draw_string(Roll_Arr,0,10,20);
				
			}	
		  if(yaw>=0)//z
			{
				
			  yaw_x=yaw*10.0;
				Yaw_Arr[4]='+';
				Yaw_Arr[5]=((uint16_t)yaw_x)/100+0x30;
				Yaw_Arr[6]=((uint16_t)yaw_x)/10%10+0x30;
				Yaw_Arr[8]=((uint16_t)yaw_x)%10+0x30;
				draw_string(Yaw_Arr,0,10,30);

			}
			else
			{
			 yaw_x=yaw;
				Yaw_Arr[4]='-';
				Yaw_Arr[5]=((uint16_t)yaw_x)/100+0x30;
				Yaw_Arr[6]=(((uint16_t)yaw_x)/10)%10+0x30;
				Yaw_Arr[8]=((uint16_t)yaw_x)%10+0x30;
				draw_string(Yaw_Arr,0,10,30);
				
			}	
}
