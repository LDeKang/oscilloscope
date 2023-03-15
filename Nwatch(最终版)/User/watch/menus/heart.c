/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

#include "common.h"
#include "arm_math.h"
#include "arm_const_structs.h"
#define OPTION_COUNT	0
 
#define BLOCK_SIZE           1     /* ����һ��arm_fir_f32����Ĳ�������� */
#define NUM_TAPS             19     /* �˲���ϵ������ */

extern u8 MAX_INT_FLAG;
#define PEAK_NUM 20
#define MAX_LEN 500

uint32_t blockSize = BLOCK_SIZE;
uint32_t numBlocks = BLOCK_SIZE;            /* ��Ҫ����arm_fir_f32�Ĵ��� */

static float firStateF32[BLOCK_SIZE + NUM_TAPS - 1];        /* ״̬���棬��СnumTaps + blockSize - 1*/
u8 Heart_Arr[]="H:XX";

/* ��ͨ�˲���ϵ�� ͨ��fadtool��ȡ*/
const float firCoeffs32LP[NUM_TAPS] = {
  0.001050907278223,0.003900401872444,0.009931421520278, 0.020186987747788,0.034955126760822,
  0.053297734925034, 0.072939024167560, 0.090640193301301, 0.103001497903959,  0.107442624028692,
   0.103001497903959,   0.090640193301301,   0.072939024167560,   0.053297734925034,  0.034955126760822,
     0.020186987747788,   0.009931421520278,   0.003900401872444,  0.001050907278223,
};

arm_fir_instance_f32 S;

HEART_ST Heart_Rate;

static prev_menu_s prevMenuData;

//static void mSelect(void);
//static void itemLoader(byte);
//static void updateHeartRate();
//static void updateBlood();
//static void updateOxygen();
void Deaw_Heart(void);
static display_t draw();
	
static void Oled_Str(u8 x,u8 y,u8* Str)
{
 while(*Str!='\0')
 {
  GUI_ShowChar(x,y,*Str,16,1);
	 *Str++;
	 x+=8;
 }
}

static bool btnExit()
{
	appConfig.sleepTimeout = 1;
	Heart_Rate.Dis_Start=0;
	oled_setInvert(1);
	OFF_MAX();
	display_load();
	return true;
}


void mHeartsOpen()
{
	menu_close();
	display_setDrawFunc(draw);
	buttons_setFuncs(NULL, btnExit, NULL);
 
	ON_MAX();
   oled_setInvert(0);
	appConfig.sleepTimeout = 10000;
	Heart_Rate.Dis_Start=1;
 
	OLED_DrawLine(0,48,120,48);
	OLED_DrawLine(0,0,0,48);
	OLED_DrawLine(0,58,30,58);
   GUI_ShowString(40,49,Heart_Arr,16,1);
	OLED_DrawLine(83,58,120,58);
	OLED_DrawLine(20,49,20,67);
	OLED_DrawLine(95,49,95,67);
	
}


float Reat;
static display_t draw()
{
		Reat=Heart_Calculate();

		if(Reat>0)
		{
			Heart_Arr[2]=((uint16_t)Reat)/10+0x30;
			Heart_Arr[3]=((uint16_t)Reat)%10+0x30;
			Deaw_Heart();
		}
	
	return 0;
}
void Heart_Init(void)
{
   MAX_Init();
	Tim2_Base_Init_Config(100,7199);
   arm_fir_init_f32(&S, NUM_TAPS,(float32_t *)&firCoeffs32LP[0], &firStateF32[0], blockSize);
   Heart_Rate.Cnt=0;
	Heart_Rate.Ir_Data=0;
	Heart_Rate.Peak=0;
	Heart_Rate.Start=0;
	Heart_Rate.Red_Data=0;
	Heart_Rate.Dis_Start=0;
}
 
//�м����

//�м����
uint16_t Low=0,Mid=1,Hi=2;
float Ave=0;//ƽ��ֵ
uint16_t Sun=0;//���
u8 k=0;
uint16_t Num[PEAK_NUM];//�洢��ֵ֮��ľ���
float Heat=0;//ʵ������
float input,output;
float Max,Min,XiShu;
uint16_t x=1,Numx;


float Heart_Calculate(void)
{
	uint16_t i,j;
	
	 if(MAX_INT_FLAG==1)
	 {
		 MAX_INT_FLAG=0;
		 MAX_Read_Data(&Heart_Rate.Ir_Data,&Heart_Rate.Red_Data);
		 input=Heart_Rate.Red_Data;
		 arm_fir_f32(&S,&input,&output,  blockSize);//�˲�
		 
		 if(output>150000)//������ֵ��ʾ��ָ����
		 {
			if(Heart_Rate.Start==1)
			{
			 Heart_Rate.Data_Buf[Heart_Rate.Cnt]=output;
			}
 
			Heart_Rate.Cnt++;
			if((Heart_Rate.Cnt==300)&&(Heart_Rate.Start==0))//���˵�ǰ���������
			{
				Heart_Rate.Cnt=0;
				Heart_Rate.Start=1;//��ʼ�洢��־
			}
			if((Heart_Rate.Cnt==MAX_LEN)&&(Heart_Rate.Start==1))
			{
				Heart_Rate.Cnt=0;
				Heart_Rate.Start=1;
				LED1_STATE=0;
				//Ѱ�Ҳ��岨��
				for(Hi=2;Hi<MAX_LEN;Hi++)
				{
					//���������ϲ��岨������������
				  while((Mid<MAX_LEN)&&(Heart_Rate.Data_Buf[Mid]==Heart_Rate.Data_Buf[Low]))
				  {
				    Mid++;
				  }
				  Hi=Mid+1;
			     while((Hi<MAX_LEN)&&(Heart_Rate.Data_Buf[Mid]==Heart_Rate.Data_Buf[Hi]))
				  {
				    Hi++;
				  }
				  
				  if(Hi>MAX_LEN)
				  {
				   break;
				  }
				  //��ǲ��岨��
				  if((Heart_Rate.Data_Buf[Mid]>Heart_Rate.Data_Buf[Low])&&(Heart_Rate.Data_Buf[Mid]>Heart_Rate.Data_Buf[Hi]))
				  {
				    Heart_Rate.Data_Pix[Mid]=1;//��¼λ��
				  }
				  else if((Heart_Rate.Data_Buf[Mid]<Heart_Rate.Data_Buf[Low])&&(Heart_Rate.Data_Buf[Mid]<Heart_Rate.Data_Buf[Hi]))
				  {
				    Heart_Rate.Data_Pix[Mid]=-1;
				  }
				  
				  Low=Mid;
				  Mid=Hi;
				  
				}
	         Min=210000.0;
			   Max=0;				
			   //����ƽ��ֵ.�����Сֵ
				for(i=0;i<MAX_LEN;i++)
				{
					if(Min>Heart_Rate.Data_Buf[i])
					Min=Heart_Rate.Data_Buf[i];
					
					if(Max<Heart_Rate.Data_Buf[i])
					Max=Heart_Rate.Data_Buf[i];
					
				   Ave+=Heart_Rate.Data_Buf[i];
				}
				 Ave=Ave/500.0;
				//�˳�����ƽ��ֵ�Ĳ��Ⱥ�С��ƽ��ֵ�Ĳ���
				for(i=0;i<MAX_LEN;i++)
				{
				  if(((Heart_Rate.Data_Pix[i]>0)&&(Heart_Rate.Data_Buf[i]<Ave))||((Heart_Rate.Data_Pix[i]<0)&&(Heart_Rate.Data_Buf[i]>Ave)))
				  {
				    Heart_Rate.Data_Pix[i]=0;
				  }
				}
				
				//ͳ�Ʋ��壬��ѭ���� ���� ���� ���� �Ĺ���
				for(i=0;i<MAX_LEN;i++)
				{
		        while((i<MAX_LEN)&&(Heart_Rate.Data_Pix[i]<=0))
				  {
				   i++;
				  }
				  
				  if(i>MAX_LEN)
				  {
				   break;
				  }
				  
				  Num[Heart_Rate.Peak]=i-1;//�洢�������������λ��				  
				  Heart_Rate.Peak++;
		        while((i<MAX_LEN)&&(Heart_Rate.Data_Pix[i]>=0))
				  {
				   i++;
				  }
				  
				}
				//ȥ�����С����ֵ�Ĳ��壬�����
				for(i=0;i<Heart_Rate.Peak;i++)
				{
				 if(Num[i]-Num[i-1]>65)
				 {
				   Sun+=(Num[i]-Num[i-1]);
				 }
				 else
				 {
				  k++;
				 }
				} 		
				Ave=(float)((float)Sun/(float)(Heart_Rate.Peak-k));//�����ھ�ֵ
				Heat=(1000.0/(Ave*8.5))*60;
		      OLED_Qclear();
				LED1_STATE=1;
				Heart_Rate.Peak=0;
				Low=0;
				Mid=1;
				Hi=2;
				Sun=0;
				k=0;
				return Heat;
				
			}
			
		 }
		 else
		 {
			Heart_Rate.Start=0;
		   output=100;
			Heart_Rate.Cnt=0;
		   return -1;
		 }		 
	 }
	 return -1;
	
}

void Deaw_Heart(void)
{
	uint16_t i,j;
	
	XiShu=(40.0/(Max-Min));				

	OLED_DrawLine(0,48,120,48);
	OLED_DrawLine(0,0,0,48);
	OLED_DrawLine(0,58,30,58);
   GUI_ShowString(40,49,Heart_Arr,16,1);
	OLED_DrawLine(83,58,120,58);
	OLED_DrawLine(20,49,20,67);
	OLED_DrawLine(95,49,95,67);
	
	for(i=1;i<5;i++)
	{
	
		if(i>=2)
		{
			OLED_Set_Pixel(30+i*30,(45-(u8)((Heart_Rate.Data_Buf[Num[i]]-Min)*XiShu)),1);
			Numx=(uint16_t)((Num[i]-Num[i-1])/30);
			for(j=Num[i-1];j<Num[i];j++)
			{
			 if(j%Numx==0)
			 {
//				OLED_Set_Pixel(x,(45-(u8)((Heart_Rate.Data_Buf[j]-Min)*XiShu)),1); 
				 OLED_DrawLine(x-1,(45-(u8)((Heart_Rate.Data_Buf[j-1]-Min)*XiShu)),x,(45-(u8)((Heart_Rate.Data_Buf[j]-Min)*XiShu)));
				x++;
			 }
			 
			}	
		}
		else
		{
		 OLED_Set_Pixel(30,(45-(u8)((Heart_Rate.Data_Buf[Num[i]]-Min)*XiShu)),1);
		  Numx=(uint16_t)(Num[i]/30);
			for(j=1;j<Num[i];j++)
			{
			 if(j%Numx==0)
			 {
//				OLED_Set_Pixel(x,(45-(u8)((Heart_Rate.Data_Buf[j]-Min)*XiShu)),1); 
            OLED_DrawLine(x-1,(45-(u8)((Heart_Rate.Data_Buf[j-1]-Min)*XiShu)),x,(45-(u8)((Heart_Rate.Data_Buf[j]-Min)*XiShu)));
				x++;
			 }
			 
			}	
		}
	}
		for(i=0;i<Heart_Rate.Peak;i++)
		{
		  Num[i]=0;
		}
			x=1;

}