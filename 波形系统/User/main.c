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
#include "sys.h"
#include "tim3_timer.h"
#include "adc.h"
#include "tim2.h"
#include "led.h"
#include "key.h"
#include "systick.h"
#include "tim4.h"
#include "stw_tft.h"
#include "mune.h"
#include "oled_funt.h"

uint16_t Index_Value=0;//菜单列表索引号
u8 Key_Value;//全局键值

void Delay(uint16_t Cnt);

void main (void)
{
  uint16_t i;
	uint16_t Cnt=0;
	Led_Pin_Init_Config();
	Key_Pin_Config();
	Systick_Init_Config();
	Usart1_Init_Config();
	printf("dfdf");
	TFT_Init();
	
  Mune_List[Index_Value].Option_Offest=1;
	Mune_List->Fun_Ptr();
	
	while(1)
	{	

		 Key_Value=Key_Chack();        
		if(Mune_List[Index_Value].Key_Lock!=1)
		{
				switch (Key_Value)
				{
				case 1:
								Mune_List[Index_Value].Option_Offest--;
								if(Mune_List[Index_Value].Option_Offest<=0)
								{
									Mune_List[Index_Value].Option_Offest=Mune_List[Index_Value].Sub_Num;
								}
								Key_Value=0;

						break;
				case 2:
								Mune_List[Index_Value].Option_Offest++;
								if(Mune_List[Index_Value].Option_Offest>Mune_List[Index_Value].Sub_Num)
								{
									Mune_List[Index_Value].Option_Offest=1;
								}
								Key_Value=0;
						break;
				case 3:
					      Index_Value+=(Mune_List[Index_Value].Option_Offest+Mune_List[Index_Value].Next_Offest);//在当前菜单位置上偏移
								Mune_List[Index_Value].Option_Offest=1;
				        Mune_List[Index_Value].Inter_Flag=0;
								Key_Value=0;
						break;
		
				case 4:
								if(Mune_List[Index_Value].Prev_Offest>=1)
								{
								 Index_Value-=Mune_List[Index_Value].Prev_Offest;
								 Mune_List[Index_Value].Option_Offest=1;
				         Mune_List[Index_Value].Back_Flag=0;
								}
						break;         
				default:
				
						break;
				}
		}
		else
		{
			if(Key_Value==4)
			{
				if(Mune_List[Index_Value].Prev_Offest>=1)
				{
				 Index_Value-=Mune_List[Index_Value].Prev_Offest;
				 Mune_List[Index_Value].Option_Offest=1;
				 Mune_List[Index_Value].Back_Flag=0;
				}  
			}
		}

        Mune_List[Index_Value].Fun_Ptr();
		
	}
	
}



void Delay(uint16_t Cnt)
{
 while(Cnt--);
}