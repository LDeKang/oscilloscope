#include "tim2.h"
#include "common.h"

u8 MAX_INT_FLAG;
const byte *T_BMP[10];
uint TIME_OUT=3000; 
void Tim2_Nvic_init(void)
{
	NVIC_InitTypeDef Tim2_Nvic_Structure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	Tim2_Nvic_Structure.NVIC_IRQChannel=TIM2_IRQn;
	Tim2_Nvic_Structure.NVIC_IRQChannelPreemptionPriority=1;
	Tim2_Nvic_Structure.NVIC_IRQChannelSubPriority=0;
	Tim2_Nvic_Structure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&Tim2_Nvic_Structure);
	
}
 
void Tim2_Base_Init_Config(uint16_t Period,uint16_t Prescaler)
{
	
	TIM_TimeBaseInitTypeDef Tim2_Base_Structure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	Tim2_Base_Structure.TIM_Period=Period;
	Tim2_Base_Structure.TIM_Prescaler=Prescaler;	
	Tim2_Base_Structure.TIM_ClockDivision=TIM_CKD_DIV1;
	Tim2_Base_Structure.TIM_CounterMode=TIM_CounterMode_Up;
   Tim2_Base_Structure.TIM_RepetitionCounter=0;
	
	TIM_TimeBaseInit(TIM2,&Tim2_Base_Structure);
   TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	Tim2_Nvic_init();
   TIM_Cmd(TIM2,DISABLE);

	
}
void TIM2_IRQHandler(void)
{
  if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET)
  {
	  MAX_INT_FLAG=1;

	  
	  TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
  }
}

void T_Bmp_Init(void)
{
	T_BMP[0]=T_BMP1;
	T_BMP[1]=T_BMP2;
	T_BMP[2]=T_BMP3;
	T_BMP[3]=T_BMP4;
	T_BMP[4]=T_BMP5;
	T_BMP[5]=T_BMP6;
	T_BMP[6]=T_BMP7;
	T_BMP[7]=T_BMP8;
	T_BMP[8]=T_BMP9;
	T_BMP[9]=T_BMP10;
	Start_Anm();
}

 



void Start_Anm(void)
{
	uint i,Time=millis();
   while(1)
	{
		for(i=0;i<10;i++)
		{
			draw_bitmap(38, 0, T_BMP[i], 64, 64, NOINVERT, 0);
			oled_flush();  
			OLED_Qclear();
			Sys_Del_Ms(50);	
		}
	   if(millis()-Time>TIME_OUT)
		{
		  break;
		}
	}

  OLED_Qclear();
  oled_flush();
}

 
 

