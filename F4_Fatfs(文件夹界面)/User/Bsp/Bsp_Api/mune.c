#include "mune.h"
#include "bsp_api.h"

void Main_Mune(void);   

void Sub_Mune1(void);
void Sub_Mune2(void);
void Sub_Mune3(void);
void Sub_Mune4(void);
void Sub_Mune5(void);
void Sub_Mune6(void);

void Sub_Mune5_1(void);
void Sub_Mune5_2(void);
void Sub_Mune5_3(void);
void Sub_Mune5_4(void);
void Sub_Mune5_5(void);
void Sub_Mune5_6(void);

extern uint8_t Key_Value;//全局键值
extern uint16_t Index_Value;//菜单列表索引号

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
    {0,0,0,6,0,0,1,Main_Mune},

    {0,1,0,0,1,0,1,Sub_Mune1},
    {0,2,0,0,1,0,1,Sub_Mune2},
    {0,3,0,0,1,0,1,Sub_Mune3},//含有子目录 6个成员
    {0,4,0,0,1,0,1,Sub_Mune4},
    {1,5,0,6,0,0,1,Sub_Mune5},
    {0,6,0,0,1,0,1,Sub_Mune6},
		
    {0,2,0,0,1,0,1,Sub_Mune5_1},
    {0,3,0,0,1,0,1,Sub_Mune5_2},
    {0,4,0,0,1,0,1,Sub_Mune5_3},
    {0,5,0,0,1,0,1,Sub_Mune5_4},
    {0,6,0,0,1,0,1,Sub_Mune5_5},
    {0,7,0,0,1,0,1,Sub_Mune5_6},
		
};


void Main_Mune(void)
{
	if(Mune_List[Index_Value].Inter_Flag==0)
	{
		Dir_Frist_Gui();//根目录界面
		TFT_Dis_Str(250,50+(40*(Mune_List[Index_Value].Option_Offest-1)),"<<--",0x8010,0x2c4a);
		Mune_List[Index_Value].Inter_Flag=1;
	}
	
	if(Mune_List[Index_Value].Back_Flag==0)//从下一级推出
	{
		Dir_Frist_Gui();//根目录界面
		TFT_Dis_Str(250,50+(40*(Mune_List[Index_Value].Option_Offest-1)),"<<--",0x8010,0x2c4a);
		Mune_List[Index_Value].Back_Flag=1;	
	}
	
	if(Key_Value==40)
	{
	  TFT_Dis_Str(250,50+(40*(Mune_List[Index_Value].Option_Offest-2)),"    ",0x8010,0x2c4a);
	}
	 if(Mune_List[Index_Value].Option_Offest==1)
	{
	  TFT_Dis_Str(250,50+(40*(Mune_List[Index_Value].Sub_Num-1)),"    ",0x8010,0x2c4a);
	}
	  TFT_Dis_Str(250,50+(40*(Mune_List[Index_Value].Option_Offest-1)),"<<--",0x8010,0x2c4a);
	
}

void Sub_Mune1(void)
{
	if(Mune_List[Index_Value].Inter_Flag==0)////从上一级进入
	{
		Mune_List[Index_Value].Inter_Flag=1;	
		
		TFT_Slate_Files(Dir_Path,0);
    if(Dir_Flag==1)
		{
		  Dir_Sub1_Gui();//某子目录界面 需要动态编写 Sub1,Sub2....
		}
		else//需要根据实际情况动态编写
		{
			f_open(&File1,File_Name, FA_READ);
			f_read(&File1,&Wq_Rx_Buff,1024,&Br);
			f_close(&File1);		
			TFT_Dis_Str(301,100,Wq_Rx_Buff,0x0000,0x5555);
		}
	}		
	
	if(Mune_List[Index_Value].Back_Flag==0)//从下一级推出
	{
		Mune_List[Index_Value].Back_Flag=1;	
	}

}	

void Sub_Mune2(void)
{
	if(Mune_List[Index_Value].Inter_Flag==0)////从上一级进入
	{
		Mune_List[Index_Value].Inter_Flag=1;
		
		TFT_Slate_Files(Dir_Path,1);//某子目录界面 需要动态编写 Sub1,Sub2...
    if(Dir_Flag==1)
		{
		  Dir_Sub1_Gui();//某子目录界面 需要动态编写 Sub1,Sub2....
		}
		else//需要根据实际情况动态编写
		{
			f_open(&File1,File_Name, FA_READ);
			f_read(&File1,&Wq_Rx_Buff,1024,&Br);
			f_close(&File1);		
			TFT_Dis_Chinaese(301,0,Wq_Rx_Buff,0x0000,0x5555);
		}

	}		
	if(Mune_List[Index_Value].Back_Flag==0)//从下一级推出
	{
		Mune_List[Index_Value].Back_Flag=1;	
	}

}	

void Sub_Mune3(void)
{
	if(Mune_List[Index_Value].Inter_Flag==0)////从上一级进入
	{
		Mune_List[Index_Value].Inter_Flag=1;	
		
		TFT_Slate_Files(Dir_Path,2);
    if(Dir_Flag==1)
		{
		  Dir_Sub1_Gui();
		}
		else
		{
		  f_open(&File1,File_Name, FA_READ);
			TFT_Dis_FsImage(300,0,499,480,479048);
			f_close(&File1);
		}
		
	}		
	
	if(Mune_List[Index_Value].Back_Flag==0)//从下一级推出
	{
		Mune_List[Index_Value].Back_Flag=1;	
	}
	

}	

void Sub_Mune4(void)
{
	if(Mune_List[Index_Value].Inter_Flag==0)////从上一级进入
	{
		Mune_List[Index_Value].Inter_Flag=1;	
		
		TFT_Slate_Files(Dir_Path,3);
    if(Dir_Flag==1)
		{
		 Dir_Sub1_Gui();
		}
		else
		{
		  f_open(&File1,File_Name, FA_READ);
			TFT_Dis_FsImage(400,100,283,100,56608);
			f_close(&File1);			
		}
	}		
	if(Mune_List[Index_Value].Back_Flag==0)//从下一级推出
	{
		Mune_List[Index_Value].Back_Flag=1;	
	}

}	

void Sub_Mune5(void)
{
	if(Mune_List[Index_Value].Inter_Flag==0)////从上一级进入
	{
		Mune_List[Index_Value].Inter_Flag=1;
		
		TFT_Slate_Files(Dir_Path,4);
    if(Dir_Flag==1)
		{
		 Dir_Sub1_Gui();
		}
		else
		{
		
		}
	}		
	if(Mune_List[Index_Value].Back_Flag==0)//从下一级推出
	{
		Mune_List[Index_Value].Back_Flag=1;	
		Dir_Sub1_Gui();
	}
	
	if(Key_Value==40)
	{
	  TFT_Dis_Str(250,50+(40*(Mune_List[Index_Value].Option_Offest-2)),"    ",0x8010,0x2c4a);
	}
	 if(Mune_List[Index_Value].Option_Offest==1)
	{
	  TFT_Dis_Str(250,50+(40*(Mune_List[Index_Value].Sub_Num-1)),"    ",0x8010,0x2c4a);
	}
	  TFT_Dis_Str(250,50+(40*(Mune_List[Index_Value].Option_Offest-1)),"<<--",0x8010,0x2c4a);
}	

void Sub_Mune6(void)
{
	if(Mune_List[Index_Value].Inter_Flag==0)////从上一级进入
	{
		Mune_List[Index_Value].Inter_Flag=1;	
		
		TFT_Slate_Files(Dir_Path,5);
    if(Dir_Flag==1)
		{
		 Dir_Sub1_Gui();
		}
		else
		{
		
		}
	}		
	if(Mune_List[Index_Value].Back_Flag==0)//从下一级推出
	{
		Mune_List[Index_Value].Back_Flag=1;	
	}

}	


void Sub_Mune5_1(void)
{
	
	if(Mune_List[Index_Value].Inter_Flag==0)//从上一级进入
	{
		
		TFT_Slate_Files("0:/Sub1",0);
    if(Dir_Flag==1)
		{
		 Dir_Sub1_Gui();
		}
		else
		{
			f_open(&File1,File_Name, FA_READ);
			f_read(&File1,&Wq_Rx_Buff,1024,&Br);
			f_close(&File1);		
			TFT_Dis_Str(301,0,Wq_Rx_Buff,0xf800,0x5555);		
		}

		
		Mune_List[Index_Value].Inter_Flag=1;	 
	}
	if(Mune_List[Index_Value].Back_Flag==0)//从下一级推出
	{
		Mune_List[Index_Value].Back_Flag=1;		
	}
	
}	

void Sub_Mune5_2(void)
{
	
	if(Mune_List[Index_Value].Inter_Flag==0)//从上一级进入
	{
		Mune_List[Index_Value].Inter_Flag=1;	 
		
		TFT_Slate_Files("0:/Sub1",1);
    if(Dir_Flag==1)
		{
		 Dir_Sub1_Gui();
		}
		else
		{	
	
		}
		
	}
	if(Mune_List[Index_Value].Back_Flag==0)//从下一级推出
	{
		Mune_List[Index_Value].Back_Flag=1;		
	}
	
}	
void Sub_Mune5_3(void)
{
	
	if(Mune_List[Index_Value].Inter_Flag==0)//从上一级进入
	{
		Mune_List[Index_Value].Inter_Flag=1;	 

		TFT_Slate_Files("0:/Sub1",2);
    if(Dir_Flag==1)
		{
		 Dir_Sub1_Gui();
		}
		else
		{	
		}
	}
	if(Mune_List[Index_Value].Back_Flag==0)//从下一级推出
	{
		Mune_List[Index_Value].Back_Flag=1;		
	}
	
}	
void Sub_Mune5_4(void)
{
	
	if(Mune_List[Index_Value].Inter_Flag==0)//从上一级进入
	{
		Mune_List[Index_Value].Inter_Flag=1;	

		TFT_Slate_Files("0:/Sub1",3);
    if(Dir_Flag==1)
		{
		 Dir_Sub1_Gui();
		}
		else
		{	
		}
	}
	if(Mune_List[Index_Value].Back_Flag==0)//从下一级推出
	{
		Mune_List[Index_Value].Back_Flag=1;		
	}
	
}	
void Sub_Mune5_5(void)
{
	
	if(Mune_List[Index_Value].Inter_Flag==0)//从上一级进入
	{
		Mune_List[Index_Value].Inter_Flag=1;	 
		
		TFT_Slate_Files("0:/Sub1",4);
    if(Dir_Flag==1)
		{
		 Dir_Sub1_Gui();
		}
		else
		{	
		}
	}
	if(Mune_List[Index_Value].Back_Flag==0)//从下一级推出
	{
		Mune_List[Index_Value].Back_Flag=1;		
	}
	
}	
void Sub_Mune5_6(void)
{
	
	if(Mune_List[Index_Value].Inter_Flag==0)//从上一级进入
	{
		Mune_List[Index_Value].Inter_Flag=1;	 
		
		TFT_Slate_Files("0:/Sub1",5);
    if(Dir_Flag==1)
		{
		 Dir_Sub1_Gui();
		}
		else
		{	
		}
		
	}
	if(Mune_List[Index_Value].Back_Flag==0)//从下一级推出
	{
		Mune_List[Index_Value].Back_Flag=1;		
	}
	
}	



