#include "scope_api.h"
#include "tim2.h"
#include "tim4.h"
#include "tim3_timer.h"
#include "oled_funt.h"
#include "stw_tft.h"
#include "usart.h"
#include "adc.h"
#include "led.h"

SCOPE_MODE Scope_Mode;

WAVEFROM Wave_Mag;
CONTRL_WAVE Con_Shape;
WAVE_IMAGE Wave_Image;
SINGLE_MODE Single_Mode;


u8 Freq_Arr[]="000.0KHZ";
u8 Cycle_Arr[]="000.0us";
u8 Maxdc_Arr[]="0.00V";
u8 Mindc_Arr[]="0.00V";
uint16_t F_Arr[8]={50,1000,4676,5000,500};
uint16_t Y_Arr[8]={30,200,1000,2500,250};


void Scope_Interface(void);
void Wave_Struct_Init(void);
void Wave_Cycle_Display(void);
void Wave_Dc_Display(void);

//ʾ������ʼ��
void Scope_Init(void)
{
	Wave_Struct_Init();
	Tim3_Timer_Config(23,71,ENABLE);
	Adc_Init_Config();
	Tim2_Ic_Config(ENABLE);	
	Tim4_Oc_Config(1000,71,500,ENABLE);	//80K		
	Scope_Interface();
}

//ʾ�������ݴ���
void Scope_Data_Process(void)
{
	 uint16_t i=0;
	if(Wave_Image.Finish_Flag==1)//������ʾģ�飬�ɲ���Ƶ��10Hz-100KHZ��
	{
		Wave_Image.Finish_Flag=0;
		Scope_Open_Process(DISABLE);//�ر�ADC/DMA

		Wave_Image.usX=20;
		Wave_Image.Max_Dc=Adc_Arr[Wave_Image.usX];
		Wave_Image.Min_Dc=Adc_Arr[Wave_Image.usX];
			
		for(i=0;i<DATA_LEN;i++)//��ʾ����
		{
			if((i>=Con_Shape.Low_Range)&&(i<=Con_Shape.Hight_Range))//��ȡ���е�һ����ʾ
			{
				 Wave_Image.Prve_usY=(33+(80-Adc_Arr[i]/50));
				 Wave_Image.Prve_usX=Wave_Image.usX;
				
				 Wave_Image.usX+=Con_Shape.To_Expand;
				 Wave_Image.usY=(33+(80-Adc_Arr[i+1]/50));
				 
				if((Adc_Arr[i+1]>4000)&&(Adc_Arr[i]<100))//ת�ۻ�����
				{
					TFT_Draw_Line(Wave_Image.usX,Wave_Image.usY,Wave_Image.usX,112,0x7fe0);
				}
				else if((Adc_Arr[i+1]<100)&&(Adc_Arr[i]>4000))
				{
					TFT_Draw_Line(Wave_Image.usX,Wave_Image.usY,Wave_Image.usX,33,0x7fe0);
				}
				else
				{
					if(Wave_Image.Max_Dc<=Adc_Arr[i])//ѡ����Сֵ�����ֵ
					{
					  Wave_Image.Max_Dc=Adc_Arr[i];
					}
					if(Wave_Image.Min_Dc>Adc_Arr[i])
					{
					  Wave_Image.Min_Dc=Adc_Arr[i];
					}
					
		           TFT_Draw_Line(Wave_Image.Prve_usX,Wave_Image.Prve_usY,Wave_Image.usX,Wave_Image.usY,0x7fe0);
				}							   
			}
		}
			Scope_Data_Dispaly(DISABLE);//��ѹ��ֵ��ʾ
	}
	else if(Cap_Flag==1)//������ɣ��رղ���ͨ��������ADCת��
	{
		Cap_Flag=0;
		Scope_Data_Dispaly(ENABLE);//����Ƶ����ʾ
		Scope_Open_Process(ENABLE);//����ADC
		Scope_Shut_Cap(DISABLE);		
	}
}

//�ر�ʾ����
void Scope_Shut(void)
{
	Scope_Open_Process(DISABLE);//�ر�ADC/DMA
	Tim3_Timer_Config(23,71,DISABLE);//�ر�ADC������ʱ��
	Tim4_Oc_Config(1000,71,500,DISABLE);	//�ر�auto�������
	Tim2_Ic_Config(DISABLE);//�رղ���
}

//ʾ��������
void Scope_Interface(void)
{
	u8 i;
	TFT_Full(SPOCE_COLOR);
	//������ʾ����
	TFT_Set_Win(20,30,255,170,0x0);
	//�߿�
	TFT_Draw_Line(17,28,276,28,0xfea0);
	TFT_Draw_Line(17,29,276,29,0xfea0);

	TFT_Draw_Line(17,200,276,200,0xfea0);
	TFT_Draw_Line(17,201,276,201,0xfea0);

	TFT_Draw_Line(19,29,19,200,0xfea0);
	TFT_Draw_Line(18,29,18,200,0xfea0);

	TFT_Draw_Line(275,29,275,200,0xfea0);
	TFT_Draw_Line(276,29,276,200,0xfea0);
	
	TFT_Dis_Str(0,108,"0V",0xffff,SPOCE_COLOR);
	TFT_Dis_Str(0,12,"3.3V",0xffff,SPOCE_COLOR);

  for(i=1;i<=4;i++)
	{
	  TFT_Draw_Line((i*50+20),30,(i*50+20),200,LINE_COLOR);
	}
	
  for(i=1;i<=3;i++)
	{
	  	TFT_Draw_Line(20,(30+i*42),273,(30+i*42),LINE_COLOR);
	}
	if(Scope_Mode.Pwm_Mode_Flag==1)
	{
		TFT_Dis_Str(80,8,"Range:1Hz-300KHZ",0xffff,0xd8a7);
		TFT_Dis_Chinaese(0,205,China_Arr,0,2,0x7e0,0x9999);
		TFT_Dis_Chinaese(70,205,China_Arr,2,2,0x7e0,0x9999);	
		TFT_Dis_Chinaese(145,205,China_Arr,4,3,0x7e0,0x9999);
		TFT_Dis_Chinaese(230,205,China_Arr,7,3,0x7e0,0x9999);
		
	}
	
	if(Scope_Mode.Single_Mode_Flag==1)
	{
		TFT_Dis_Str(86,8,"Single Trigger",0xffff,0xd8a7);
		TFT_Dis_Str(278,40,"MODE",0xffe0,0x451);
		TFT_Dis_Str(278,60,">Fall",0xffff,0x3666);
		TFT_Dis_Str(278,80,">Rise",0xffff,0xd8a7);
		TFT_Dis_Str(18,205,"|___|=120ms",0xffff,0x7b5d);
	}
	
}
//ʾ����������Ա��ʼ��
void Wave_Struct_Init(void)
{
	Con_Shape.Wave_Expand=25;//���ο�ȱ�������
	Con_Shape.Hight_Psc=5;//��Ƶ��Ƶ����

	Con_Shape.Hight_Range=102;//�����ʾ������Χ
	Con_Shape.Low_Range=20;//��С��ʾ������Χ
	Con_Shape.To_Expand=3;//������ʾ��ȿ���
	Scope_Mode.Pwm_Mode_Flag=1;//���β���ģʽ
	Scope_Mode.Single_Mode_Flag=0;//���δ���ģʽ
}

//ENABLE �������� DISABLE ����ADCģ������
void Scope_Shut_Cap(FunctionalState NewState)
{
 Control_Ic_Pin(NewState);
}
//ENABLE ����ADC/DMA DISABLE �ر�ADC/DMA
void Scope_Open_Process(FunctionalState NewState)
{
  Control_Adc_Dma(NewState);
}


//ˢ�²���ͼ��
void Scope_Wave_Refresh(void)
{
	uint16_t i;
	Wave_Image.usX=20;
	
	if(Scope_Mode.Pwm_Mode_Flag==1)//���β�����
	{
		for(i=0;i<DATA_LEN;i++)
		{
			if((i>=Con_Shape.Low_Range)&&(i<=Con_Shape.Hight_Range))
			{
				 Wave_Image.Prve_usY=(33+(80-Adc_Arr[i]/50));
				 Wave_Image.Prve_usX=Wave_Image.usX;
				
				 Wave_Image.usX+=Con_Shape.To_Expand;
				 Wave_Image.usY=(33+(80-Adc_Arr[i+1]/50));
				
				if((Adc_Arr[i+1]>4000)&&(Adc_Arr[i]<100))//ת�ۻ�����
				{
					TFT_Draw_Line(Wave_Image.usX,Wave_Image.usY,Wave_Image.usX,112,0x0);
				}
				else if((Adc_Arr[i+1]<100)&&(Adc_Arr[i]>4000))
				{
					TFT_Draw_Line(Wave_Image.usX,Wave_Image.usY,Wave_Image.usX,33,0x0);
				}
				else
				{
				    TFT_Draw_Line(Wave_Image.Prve_usX,Wave_Image.Prve_usY,Wave_Image.usX,Wave_Image.usY,0x0);
				}				
				 
			}
					 Adc_Arr[i]=0;	
		}

	}
	else if(Scope_Mode.Single_Mode_Flag==1)//���δ�����
	{
		Wave_Image.usX=30;
		for(i=0;i<DATA_LEN;i++)
		{
			 Wave_Image.Prve_usY=(33+(80-Adc_Arr[i]/50));
			 Wave_Image.Prve_usX=Wave_Image.usX;
			
			 Wave_Image.usX+=2;
			 Wave_Image.usY=(33+(80-Adc_Arr[i+1]/50));
			
			if((Adc_Arr[i+1]>4000)&&(Adc_Arr[i]<100))//ת�ۻ�����
			{
				TFT_Draw_Line(Wave_Image.usX,Wave_Image.usY,Wave_Image.usX,112,0x0);
			}
			else if((Adc_Arr[i+1]<100)&&(Adc_Arr[i]>4000))
			{
				TFT_Draw_Line(Wave_Image.usX,Wave_Image.usY,Wave_Image.usX,33,0x0);
			}
			else
			{
				TFT_Draw_Line(Wave_Image.Prve_usX,Wave_Image.usY,Wave_Image.usX,Wave_Image.Prve_usY,0x0);
			}
			Adc_Arr[i]=0;
		}	

	}
//ˢ������
	for(i=1;i<=4;i++)
	{
		  TFT_Draw_Line((i*50+20),30,(i*50+20),200,LINE_COLOR);
	}
	
	for(i=1;i<=3;i++)
	{
			TFT_Draw_Line(20,(30+i*42),273,(30+i*42),LINE_COLOR);
	}
}

//ˢ�²��δ���
void Scope_Win_Refresh(void)
{
	u8 i;
		TFT_Set_Win(20,30,255,170,0x0);
		for(i=1;i<=4;i++)
		{
			TFT_Draw_Line((i*50+20),30,(i*50+20),200,LINE_COLOR);
		}
		
		for(i=1;i<=3;i++)
		{
				TFT_Draw_Line(20,(30+i*42),273,(30+i*42),LINE_COLOR);
		}

}

void Scope_Single_Init(void)
{
	Tim3_Timer_Config(5000,71,ENABLE);
	
	Scope_Mode.Pwm_Mode_Flag=0;
	Scope_Mode.Single_Mode_Flag=1;
	Single_Mode.Fall_Finish=0;	
	Single_Mode.Reise_Mode=0;	
	Single_Mode.Fall_Mode=1;
	Single_Mode.Reise_Mode=0;	
	Scope_Shut_Cap(ENABLE);	
	Adc_Init_Config();	
	Scope_Interface();

}

void Scope_Single_Control(FunctionalState NewState)
{
  if(NewState)
	{
	  Control_Adc(ENABLE);			
	}
	else
	{
	  Control_Adc(DISABLE);			
	}
}


void Scope_Single_Operate(void)
{
	uint16_t i=0;
	if(Wave_Image.Finish_Flag==1)//������ʾģ�飬�ɲ���Ƶ��10Hz-100KHZ��
	{
		Wave_Image.Finish_Flag=0;
		Wave_Image.usX=30;
		if((Single_Mode.Fall_Finish==1)||(Single_Mode.Reise_Finish==1))
		{
			TFT_Dis_Str(18,205,"|___|=20ms",0xffff,0x7b5d);
			if(Single_Mode.Fall_Mode==1)
			{
				Single_Mode.Fall_Finish=2;
			}
			else if(Single_Mode.Reise_Mode==1)
			{
				Single_Mode.Reise_Finish=2;
			}

			if(Single_Mode.Count-10>240)
			{
				Single_Mode.Count+=10;
			}
			else if(Single_Mode.Count+10>DATA_LEN)
			{
				Single_Mode.Count-=10;
			}
			for(i=0;i<DATA_LEN;i++)
			{
				if((i>=(Single_Mode.Count-10))&&(i<=(Single_Mode.Count+10)))//��ȡ���е�һ����ʾ
				{
						Wave_Image.Prve_usY=(33+(80-Adc_Arr[i]/50));
						Wave_Image.Prve_usX=Wave_Image.usX;

						Wave_Image.usX+=10;
						Wave_Image.usY=(33+(80-Adc_Arr[i+1]/50));
					if((Adc_Arr[i+1]>4000)&&(Adc_Arr[i]<100))//ת�ۻ�����
					{
						TFT_Draw_Line(Wave_Image.usX,Wave_Image.usY,Wave_Image.usX,112,0x7fe0);
					}
					else if((Adc_Arr[i+1]<100)&&(Adc_Arr[i]>4000))
					{
						TFT_Draw_Line(Wave_Image.usX,Wave_Image.usY,Wave_Image.usX,33,0x7fe0);
					}
					else
					{
						 TFT_Draw_Line(Wave_Image.Prve_usX,Wave_Image.Prve_usY,Wave_Image.usX,Wave_Image.usY,0x7fe0);
					}							   
				}
			}
		}
		else if((Single_Mode.Fall_Finish==0)&&(Single_Mode.Reise_Finish==0))
		{
			TFT_Dis_Str(18,205,"|___|=120ms",0xffff,0x7b5d);
			for(i=0;i<DATA_LEN;i++)
			{
				 Wave_Image.Prve_usY=(33+(80-Adc_Arr[i]/50));
				 Wave_Image.Prve_usX=Wave_Image.usX;
				
				 Wave_Image.usX+=2;
				 Wave_Image.usY=(33+(80-Adc_Arr[i+1]/50));
				
				if((Adc_Arr[i+1]>4000)&&(Adc_Arr[i]<100))//ת�ۻ�����
				{
					TFT_Draw_Line(Wave_Image.usX,Wave_Image.usY,Wave_Image.usX,112,0x7fe0);
				}
				else if((Adc_Arr[i+1]<100)&&(Adc_Arr[i]>4000))
				{
					TFT_Draw_Line(Wave_Image.usX,Wave_Image.usY,Wave_Image.usX,33,0x7fe0);
				}
				else
				{
					TFT_Draw_Line(Wave_Image.Prve_usX,Wave_Image.usY,Wave_Image.usX,Wave_Image.Prve_usY,0x7fe0);
				}
			}	
		}	
	}	
}
//ENABLE ������ʾ DISABLE ��ѹ��ʾ
void Scope_Data_Dispaly(FunctionalState NewState)
{
 if(NewState)
 {
   Wave_Cycle_Display();
 }
 else
 {
   Wave_Dc_Display();
 }
}


void Wave_Cycle_Display(void)
{
	if(Wave_Mag.Frequency>=1000)
	{
		 Freq_Arr[0]=((uint32_t)(Wave_Mag.Frequency)/100)/1000+0x30;
		 Freq_Arr[1]=((uint32_t)(Wave_Mag.Frequency)/100)/100%10+0x30;
		 Freq_Arr[2]=((uint32_t)(Wave_Mag.Frequency)/100)/10%10+0x30;
		 Freq_Arr[4]=((uint32_t)(Wave_Mag.Frequency)/100)%10+0x30;	
		 Freq_Arr[5]='K';
	}
	else
	{
		Freq_Arr[0]=(uint16_t)(Wave_Mag.Frequency*100)/10000+0x30;
		Freq_Arr[1]=(uint16_t)(Wave_Mag.Frequency*100)/1000%10+0x30;
		Freq_Arr[2]=(uint16_t)(Wave_Mag.Frequency*100)/100%10+0x30;
		Freq_Arr[4]=(uint16_t)(Wave_Mag.Frequency*100)/10%10+0x30;	
		Freq_Arr[5]=(uint16_t)(Wave_Mag.Frequency*100)%10+0x30;	
	}
	if(Wave_Mag.Cycle>=1000)
	{
		Cycle_Arr[0]=((uint16_t)(Wave_Mag.Cycle)/100)/1000+0x30;
		Cycle_Arr[1]=((uint16_t)(Wave_Mag.Cycle)/100)/100%10+0x30;
		Cycle_Arr[2]=((uint16_t)(Wave_Mag.Cycle)/100)/10%10+0x30;
		Cycle_Arr[4]=((uint16_t)(Wave_Mag.Cycle)/100)%10+0x30;	
		Cycle_Arr[5]='m';
		Cycle_Arr[6]='s';
	}
	else
	{
		Cycle_Arr[0]=(uint16_t)(Wave_Mag.Cycle)/1000+0x30;
		Cycle_Arr[1]=(uint16_t)(Wave_Mag.Cycle)/100%10+0x30;
		Cycle_Arr[2]=(uint16_t)(Wave_Mag.Cycle)/10%10+0x30;
		Cycle_Arr[4]=(uint16_t)(Wave_Mag.Cycle)%10+0x30;	
		Cycle_Arr[5]='u';
		Cycle_Arr[6]='s';
	}
	
		TFT_Dis_Str(0,225,Freq_Arr,0xfea0,SPOCE_COLOR);
		TFT_Dis_Str(70,225,Cycle_Arr,0xfea0,SPOCE_COLOR);

}

void Wave_Dc_Display(void)
{
	Wave_Mag.Max_Dcvalue=(float)(Wave_Image.Max_Dc/4095.0)*3.3;
	Wave_Mag.Min_Dcvalue=(float)(Wave_Image.Min_Dc/4095.0)*3.3;

	Maxdc_Arr[0]=(uint16_t)(Wave_Mag.Max_Dcvalue*100)/100+0x30;
	Maxdc_Arr[2]=(uint16_t)(Wave_Mag.Max_Dcvalue*100)/10%10+0x30;
	Maxdc_Arr[3]=(uint16_t)(Wave_Mag.Max_Dcvalue*100)%10+0x30;

	Mindc_Arr[0]=(uint16_t)(Wave_Mag.Min_Dcvalue*100)/100+0x30;
	Mindc_Arr[2]=(uint16_t)(Wave_Mag.Min_Dcvalue*100)/10%10+0x30;
	Mindc_Arr[3]=(uint16_t)(Wave_Mag.Min_Dcvalue*100)%10+0x30;

	TFT_Dis_Str(145,225,Maxdc_Arr,0xfea0,SPOCE_COLOR);
	TFT_Dis_Str(230,225,Mindc_Arr,0xfea0,SPOCE_COLOR);

}




