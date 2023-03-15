#include "mune.h"
#include "usart.h"
#include "sys.h"
#include "led.h"
#include "key.h"
#include "systick.h"
#include "stw_tft.h"
#include "oled_funt.h"
#include "scope_api.h"
#include "tim2.h"


uint16_t Sub1_Flash;
uint16_t Sub2_Flash;



void Main_Mune(void);   
void Sub_Mune1(void);
void Sub_Mune2(void);
void Sub_Mune1_1(void);
void Sub_Mune1_2(void);

void Sub_Mune2_1(void);
void Sub_Mune2_2(void);
void Sub_Mune2_3(void);
void Sub_Mune2_4(void);


extern u8 Key_Value;//全局键值
extern uint16_t Index_Value;//菜单列表索引号

u8 Cnt=0;

/*typedef struct Mune_St
{
  u8 Next_Offest;//对子菜单的偏移
  u8 Prev_Offest;//对最近主菜单的偏移
  u8 Option_Offest;//菜单中选项偏移
  u8 Sub_Num;//菜单下的子菜单数量
  u8 Key_Lock;//菜单按键锁，执行某个功能时与控制菜单的按键不冲突
  u8 Inter_Flag;// 
  u8 Back_Flag;// 
	
  void (*Fun_Ptr)();//菜单执行函数
}MUNE_ST;
*/

//菜单列表
MUNE_ST Mune_List[20]=
{
    {0,0,0,2,0,0,1,Main_Mune},

    {1,1,0,2,0,0,1,Sub_Mune1},
    {2,2,0,4,0,0,1,Sub_Mune2},

    {0,2,0,0,1,0,1,Sub_Mune1_1},
    {4,3,0,0,1,0,1,Sub_Mune1_2},
		
    {0,3,0,0,1,0,1,Sub_Mune2_1},
    {0,4,0,0,1,0,1,Sub_Mune2_2},
    {0,5,0,0,1,0,1,Sub_Mune2_3},
    {0,6,0,0,1,0,1,Sub_Mune2_4},
		
};


void Main_Mune(void)
{
	if(Mune_List[Index_Value].Back_Flag==0)
	{
		TFT_Clear();
		TFT_Dis_Image(110,60,100,100,gImage_cc);
		TFT_Set_Win(0,18,339,15,0x5ff);
		TFT_Set_Win(0,200,339,15,0x5ff);
		TFT_Dis_Str(0,20,"-------------Oscilloscope---------------",0x0,0x5ff);

		Mune_List[Index_Value].Back_Flag=1;
	}

	if((Mune_List[Index_Value].Option_Offest==1)&&(Sub1_Flash==0))
	{
		Sub1_Flash=1;
		Sub2_Flash=0;
		TFT_Dis_Image(110,60,100,100,gImage_cc);
		TFT_Set_Win(0,18,339,15,0x5ff);
		TFT_Set_Win(0,200,339,15,0x5ff);
		
		TFT_Dis_Str(0,20,"-------------Oscilloscope---------------",0x0,0x5ff);
	}
	else if((Mune_List[Index_Value].Option_Offest==2)&&(Sub2_Flash==0))
	{
		Sub1_Flash=0;
		Sub2_Flash=1;
		TFT_Dis_Image(110,60,100,100,gImage_jj);
		
		TFT_Set_Win(0,18,339,15,0xfe19);
		TFT_Set_Win(0,200,339,15,0xfe19);
		TFT_Dis_Str(0,20,"----------Waveform Generator------------",0x0,0xfe19);
	}

}	


void Sub_Mune1(void)
{
	if(Mune_List[Index_Value].Inter_Flag==0)////从上一级进入
	{
		Mune_List[Index_Value].Inter_Flag=1;		

		TFT_Clear();	  
		TFT_Dis_Str(0,20,"1.PWM",0x7fe0,0xf8b2);
		TFT_Dis_Str(0,40,"2.Single",0x7fe0,0xf8b2);	
	}		

	if(Mune_List[Index_Value].Back_Flag==0)//从下一级推出
	{
		Mune_List[Index_Value].Back_Flag=1;	

		TFT_Clear();
		Scope_Shut();	
		Scope_Single_Control(DISABLE);

		TFT_Dis_Str(0,20,"1.PWM",0x7fe0,0xf8b2);
		TFT_Dis_Str(0,40,"2.Single",0x7fe0,0xf8b2);	
		Cnt=0;
	}

	if(Mune_List[Index_Value].Option_Offest==1)//变化显示
	{
		TFT_Dis_Str(40,20,"<-",0x7fe0,0xf8b2);
		TFT_Dis_Str(64,40,"  ",0x7fe0,0xf8b2);
	}
	else if(Mune_List[Index_Value].Option_Offest==2)
	{
		TFT_Dis_Str(40,20,"  ",0x7fe0,0xf8b2);
		TFT_Dis_Str(64,40,"<-",0x7fe0,0xf8b2);
	}
		

}	

void Sub_Mune2(void)
{
  if(Systick_Cnt>700)
	{
		Systick_Cnt=0;
		if(Mune_List[Index_Value].Option_Offest==1)
		{

		}
		else if(Mune_List[Index_Value].Option_Offest==2)
		{

		}
		else if(Mune_List[Index_Value].Option_Offest==3)
		{

		}
		else if(Mune_List[Index_Value].Option_Offest==4)
		{

		}		
	}
}	
void Sub_Mune1_1(void)
{
	
	if(Mune_List[Index_Value].Inter_Flag==0)//上一级进入 
	{
    Mune_List[Index_Value].Inter_Flag=1;	 		
		Scope_Init();
	}	
	
	if(Mune_List[Index_Value].Back_Flag==0)//从下一级推出
	{
		Mune_List[Index_Value].Back_Flag=1;		
	  TFT_Clear();
	}
	
	if(Key_Value==2)// 
	{
		Con_Shape.Wave_Expand++;
	}
	else if(Key_Value==3)// 
	{
		Con_Shape.Wave_Expand--;
	}
	
	  Scope_Data_Process();

	
	if(Systick_Cnt>1000)//定时处理任务
	{
		Systick_Cnt=0;
		Cnt++;
		if(Cnt==3)
		{ 
			if((Wave_Image.Finish_Flag==0)&&(Cap_Flag==0))//更新刚上电带来的显示干扰
			{
	      Scope_Win_Refresh();//窗口刷新
			}
		}
		else
		{
			if((Wave_Image.Finish_Flag==0)&&(Cap_Flag==0))//空闲时间开启捕获通道
			{
			 Scope_Wave_Refresh();//波形刷新
			 Scope_Shut_Cap(ENABLE);			
			}			
		}
	}
}	

void Sub_Mune1_2(void)
{
   static u8 Mode_Falg=0;
	
	if(Mune_List[Index_Value].Inter_Flag==0)//从上一级进入
	{
		Scope_Single_Init();
		Mode_Falg=0;
		Mune_List[Index_Value].Inter_Flag=1;	 
	}

	if(Key_Value==2)//刷新窗口，再次启动单次触发
	{
			Scope_Win_Refresh();
			if(Single_Mode.Fall_Mode==1)
			{
				Single_Mode.Fall_Finish=0;	
			}
			else if(Single_Mode.Reise_Mode==1)
			{
				Single_Mode.Reise_Finish=0;	
			}
			 Scope_Single_Control(ENABLE);
	}
	else if(Key_Value==3)//模式调节
	{
	   Mode_Falg++;		
		if(Mode_Falg==1)
		{
			TFT_Dis_Str(278,60,">Fall",0xffff,0xd8a7);
			TFT_Dis_Str(278,80,">Rise",0xffff,0x3666);
			Single_Mode.Reise_Mode=1;
			Single_Mode.Fall_Mode=0;
		}
		else if(Mode_Falg==2)
		{
		  Mode_Falg=0;
			TFT_Dis_Str(278,60,">Fall",0xffff,0x3666);
			TFT_Dis_Str(278,80,">Rise",0xffff,0xd8a7);
			Single_Mode.Reise_Mode=0;
			Single_Mode.Fall_Mode=1;
		}
		
	}
    
	  Scope_Single_Operate();
	
		if(Systick_Cnt>1000)
		{
			  Systick_Cnt=0;

			  if((Wave_Image.Finish_Flag==0))
				{
					if((Single_Mode.Fall_Mode==1)&&(Single_Mode.Fall_Finish==0))
					{
						Scope_Wave_Refresh();
            Scope_Single_Control(ENABLE);			
					}
					else if((Single_Mode.Reise_Mode==1)&&(Single_Mode.Reise_Finish==0))
					{
						Scope_Wave_Refresh();
            Scope_Single_Control(ENABLE);				
					}

				}
			  
		}
}	

void Sub_Mune2_1(void)
{

}	
 
void Sub_Mune2_2(void)
{
	
	
}	


void Sub_Mune2_3(void)
{
		

}

void Sub_Mune2_4(void)
{
		
	
}




