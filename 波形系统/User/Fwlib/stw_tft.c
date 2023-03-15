#include "stw_tft.h"
#include "sys.h"
#include "led.h"
#include "usart.h"
#include "oled_funt.h"

#define SCL_SET   PBout(10)=1
#define SCL_CLR   PBout(10)=0

#define SDA_SET   PBout(11)=1
#define SDA_CLR   PBout(11)=0

#define RST_SET   PBout(12)=1
#define RST_CLR   PBout(12)=0

#define DC_SET   PBout(13)=1
#define DC_CLR   PBout(13)=0

#define CS_SET   PBout(14)=1
#define CS_CLR   PBout(14)=0

#define BL_SET   PBout(15)=1
#define BL_CLR   PBout(15)=0

#define TFT_COLUMN_NUMBER 320
#define TFT_LINE_NUMBER 240
void TFT_Dis_Chinaese(uint16_t usX,uint16_t usY,u8 const (*Str)[16],u8 Start,u8 Num,uint16_t Str_Color,uint16_t Back_Color);

static void Spi_Delay(uint16_t Cnt)
{
  while(Cnt--);
}
void Tft_Gpio_Init(void)
{
	
  GPIO_InitTypeDef Oled_Pin_Structure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	Oled_Pin_Structure.GPIO_Pin=GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	Oled_Pin_Structure.GPIO_Mode=GPIO_Mode_Out_PP;
	Oled_Pin_Structure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&Oled_Pin_Structure);
	

}

void St77_Write_Data(u8 Data)
{
	u8 i;
	
  CS_SET;
	CS_CLR;
	DC_SET;
	SCL_CLR;
	
	for(i=0;i<8;i++)
	{
	   SCL_CLR;
	  if((Data&0x80)==0x80)
		{
		  SDA_SET;
		}
		else
		{
		  SDA_CLR;
		}
		Spi_Delay(1);
		 SCL_SET;
	   Data<<=1;
	}
	
  CS_SET;
	
}

void St77_Write_Cmd(u8 Cmd)
{
	  u8 i;
	
	CS_SET;
	CS_CLR;

	DC_CLR;
	SCL_CLR;
	for(i=0;i<8;i++)
	{
		 SCL_CLR;
	  if((Cmd&0x80)==0x80)
		{
		  SDA_SET;
		}
		else
		{
		  SDA_CLR;
		}
		 Spi_Delay(1);
		 SCL_SET;
	   Cmd<<=1;
	}
  CS_SET;
	
}


void TFT_Init(void)				////ST7789V2
 {
	Tft_Gpio_Init();
	SCL_SET;			//特别注意！！
	RST_CLR;

	Spi_Delay(20000);//1000
	RST_SET;
	Spi_Delay(10000);//1000

	St77_Write_Cmd(0x11); 			//Sleep Out
	Spi_Delay(50000);               //DELAY120ms 
	BL_SET;
 //-----------------------ST7789V Frame rate setting-----------------//
//************************************************
	St77_Write_Cmd(0x3A);        //65k mode
	St77_Write_Data(0x05);
	St77_Write_Cmd(0xC5); 		//VCOM
	St77_Write_Data(0x1A);
	St77_Write_Cmd(0x36);                 // 屏幕显示方向设置
	St77_Write_Data(0x60);
	//-------------ST7789V Frame rate setting-----------//
	St77_Write_Cmd(0xb2);		//Porch Setting
	St77_Write_Data(0x05);
	St77_Write_Data(0x05);
	St77_Write_Data(0x00);
	St77_Write_Data(0x33);
	St77_Write_Data(0x33);

	St77_Write_Cmd(0xb7);			//Gate Control
	St77_Write_Data(0x05);			//12.2v   -10.43v
	//--------------ST7789V Power setting---------------//
	St77_Write_Cmd(0xBB);//VCOM
	St77_Write_Data(0x3F);

	St77_Write_Cmd(0xC0); //Power control
	St77_Write_Data(0x2c);

	St77_Write_Cmd(0xC2);		//VDV and VRH Command Enable
	St77_Write_Data(0x01);

	St77_Write_Cmd(0xC3);			//VRH Set
	St77_Write_Data(0x0F);		//4.3+( vcom+vcom offset+vdv)

	St77_Write_Cmd(0xC4);			//VDV Set
	St77_Write_Data(0x20);				//0v

	St77_Write_Cmd(0xC6);				//Frame Rate Control in Normal Mode
	St77_Write_Data(0X01);			//111Hz

	St77_Write_Cmd(0xd0);				//Power Control 1
	St77_Write_Data(0xa4);
	St77_Write_Data(0xa1);

	St77_Write_Cmd(0xE8);				//Power Control 1
	St77_Write_Data(0x03);

	St77_Write_Cmd(0xE9);				//Equalize time control
	St77_Write_Data(0x09);
	St77_Write_Data(0x09);
	St77_Write_Data(0x08);
	//---------------ST7789V gamma setting-------------//
	St77_Write_Cmd(0xE0); //Set Gamma
	St77_Write_Data(0xD0);
	St77_Write_Data(0x05);
	St77_Write_Data(0x09);
	St77_Write_Data(0x09);
	St77_Write_Data(0x08);
	St77_Write_Data(0x14);
	St77_Write_Data(0x28);
	St77_Write_Data(0x33);
	St77_Write_Data(0x3F);
	St77_Write_Data(0x07);
	St77_Write_Data(0x13);
	St77_Write_Data(0x14);
	St77_Write_Data(0x28);
	St77_Write_Data(0x30);
	 
	St77_Write_Cmd(0XE1); //Set Gamma
	St77_Write_Data(0xD0);
	St77_Write_Data(0x05);
	St77_Write_Data(0x09);
	St77_Write_Data(0x09);
	St77_Write_Data(0x08);
	St77_Write_Data(0x03);
	St77_Write_Data(0x24);
	St77_Write_Data(0x32);
	St77_Write_Data(0x32);
	St77_Write_Data(0x3B);
	St77_Write_Data(0x14);
	St77_Write_Data(0x13);
	St77_Write_Data(0x28);
	St77_Write_Data(0x2F);

	St77_Write_Cmd(0x20); 		//反显
   
	St77_Write_Cmd(0x29);         //开启显示 

     TFT_Clear();
	 TFT_Full(BACK_COLOR);
 }




void TFT_Full(unsigned int color)
{
  unsigned int ROW,column;
  St77_Write_Cmd(0x2a);     //Column address set
  St77_Write_Data(0x00);    //start row
  St77_Write_Data(0x00); 
  St77_Write_Data(0x01);    //end row
  St77_Write_Data(0x40);
		
  St77_Write_Cmd(0x2b);     //Row address set
  St77_Write_Data(0x00);    //start column
  St77_Write_Data(0x00); 
  St77_Write_Data(0x00);    //end column
  St77_Write_Data(0xF0);
		
  St77_Write_Cmd(0x2C);     //Memory write
		
	for(ROW=0;ROW<TFT_LINE_NUMBER;ROW++)             //ROW loop
		{ 

	for(column=0;column<TFT_COLUMN_NUMBER ;column++) //column loop
				{

		  St77_Write_Data(color>>8);
			St77_Write_Data(color);
				}
		}
}
	
void TFT_Clear(void)
{
    unsigned int ROW,column;
  St77_Write_Cmd(0x2a);     //Column address set
  St77_Write_Data(0x00);    //start row
  St77_Write_Data(0x00); 
  St77_Write_Data(0x01);    //end row
  St77_Write_Data(0x40);
		
  St77_Write_Cmd(0x2b);     //Row address set
  St77_Write_Data(0x00);    //start column
  St77_Write_Data(0x00); 
  St77_Write_Data(0x00);    //end column
  St77_Write_Data(0xF0);
		
  St77_Write_Cmd(0x2C);     //Memory write
		
	for(ROW=0;ROW<TFT_LINE_NUMBER;ROW++)             //ROW loop
		{ 

	for(column=0;column<TFT_COLUMN_NUMBER ;column++) //column loop
				{

		  St77_Write_Data(0xff);
			St77_Write_Data(0xff);
				}
		}
}

void TFT_Dis_Image(uint16_t usX,uint16_t usY,uint16_t Width,uint16_t High,const u8 *Str)
{
   unsigned int ROW,column;
	
  St77_Write_Cmd(0x2a);     //Column address set
	
  St77_Write_Data(((usX&0xff00)>>8)); 	
  St77_Write_Data((usX&0xff));    //start row
	
  St77_Write_Data((((Width+usX-1)&0xff00)>>8));	
  St77_Write_Data((Width+usX-1)&0xff);    //end row
	
  St77_Write_Cmd(0x2b);     //Column address set
	
  St77_Write_Data(((usY&0xff00)>>8)); 	
  St77_Write_Data((usY&0xff));    //start row
	
  St77_Write_Data((((High+usY-1)&0xff00)>>8));	
  St77_Write_Data((High+usY-1)&0xff);    //end row

	
  St77_Write_Cmd(0x2C);     //Memory write
	for(ROW=0;ROW<Width*High*2;ROW++)             //ROW loop
		{ 
					St77_Write_Data(Str[ROW]);
		}
		
}

void TFT_Draw_Point(uint16_t usX,uint16_t usY,uint16_t Color)
{
   unsigned int ROW,column;
	
  St77_Write_Cmd(0x2a);     //Column address set
	
  St77_Write_Data(((usX&0xff00)>>8)); 	
  St77_Write_Data((usX&0xff));    //start row
	
  St77_Write_Data((((usX)&0xff00)>>8));	
  St77_Write_Data((usX)&0xff);    //end row
	
  St77_Write_Cmd(0x2b);     //Column address set
	
  St77_Write_Data(((usY&0xff00)>>8)); 	
  St77_Write_Data((usY&0xff));    //start row
	
  St77_Write_Data((((usY)&0xff00)>>8));	
  St77_Write_Data((usY)&0xff);    //end row

	
  St77_Write_Cmd(0x2C);     //Memory write
	for(ROW=0;ROW<1;ROW++)             //ROW loop
		{ 
		    St77_Write_Data(Color>>8);
			St77_Write_Data(Color);
		}
}
 
void TFT_Draw_Line(float usX1,float usY1,float usX2,float usY2,uint16_t Color)
{
    float A,B;
	uint16_t Offest;
	uint16_t usX,usY;
	uint16_t i;
	
	if(usX2==usX1)
	{
		if((usY2-usY1)<0)
		{
		  Offest=usY1-usY2;
		  usY=usY2;
		}
		else
		{
		  Offest=usY2-usY1;
		  usY=usY1;
		}

		for(i=0;i<Offest;i++)
		{
		 usY++;
		 TFT_Draw_Point(usX1,usY,Color);
		}

		
	}
	else
	{
		A=(float)((usY2-usY1)/(usX2-usX1));
		B=usY1-A*usX1;
		
		if((usX2-usX1)<0)
		{
		  Offest=usX1-usX2;
		  usX=usX2;
		}
		else
		{
		  Offest=usX2-usX1;
		  usX=usX1;
		}

		for(i=0;i<Offest;i++)
		{
		 usX++;
		 usY=(uint16_t)(A*usX+B);
		 TFT_Draw_Point(usX,usY,Color);
		}
	}


	
}


void TFT_Set_Win(uint16_t usX,uint16_t usY,uint16_t Width,uint16_t High,uint16_t Color)
{
   unsigned int ROW,column;
	
  St77_Write_Cmd(0x2a);     //Column address set
	
  St77_Write_Data(((usX&0xff00)>>8)); 	
  St77_Write_Data((usX&0xff));    //start row
	
  St77_Write_Data((((Width+usX-1)&0xff00)>>8));	
  St77_Write_Data((Width+usX-1)&0xff);    //end row
	
  St77_Write_Cmd(0x2b);     //Column address set
	
  St77_Write_Data(((usY&0xff00)>>8)); 	
  St77_Write_Data((usY&0xff));    //start row
	
  St77_Write_Data((((High+usY-1)&0xff00)>>8));	
  St77_Write_Data((High+usY-1)&0xff);    //end row
	
  St77_Write_Cmd(0x2C);     //Memory write
	for(ROW=0;ROW<Width*High;ROW++)             //ROW loop
	{ 
		St77_Write_Data(Color>>8);
		St77_Write_Data(Color);
	}
}



void TFT_Dis_Char(uint16_t usX,uint16_t usY,u8 Ch,uint16_t Str_Color,uint16_t Back_Color)
{
	uint16_t count_bety,i;
	uint8_t Ch_len=16;//字节数	
	
  St77_Write_Cmd(0x2a);     //Column address set
	
  St77_Write_Data(((usX&0xff00)>>8)); 	
  St77_Write_Data((usX&0xff));    //start row
	
  St77_Write_Data((((7+usX)&0xff00)>>8));	
  St77_Write_Data((7+usX)&0xff);    //end row
	
  St77_Write_Cmd(0x2b);     //Column address set
	
  St77_Write_Data(((usY&0xff00)>>8)); 	
  St77_Write_Data((usY&0xff));    //start row
	
  St77_Write_Data((((15+usY)&0xff00)>>8));	
  St77_Write_Data((15+usY)&0xff);    //end row
	
  St77_Write_Cmd(0x2C);     //Memory write
  
	for(count_bety=0;count_bety<Ch_len;count_bety++)
	{
		for(i=0;i<8;i++)
		{
			if(Asiic_Arr[Ch][count_bety]&(0x01<<i))
			{
				St77_Write_Data(Str_Color>>8);
				St77_Write_Data(Str_Color);
			}
			else
			{
				St77_Write_Data(Back_Color>>8);
				St77_Write_Data(Back_Color);
			}
		}
	 
	}	
}

void TFT_Dis_Str(uint16_t usX,uint16_t usY,u8*Str,uint16_t Str_Color,uint16_t Back_Color)//输出英文
{
    uint8_t Ch_len=8;
	while(*Str!='\0')
	{
		TFT_Dis_Char(usX,usY,*Str,Str_Color,Back_Color);
		usX+=Ch_len;
		Str++;
	}

}


void TFT_Dis_Chinaese(uint16_t usX,uint16_t usY,u8 const (*Str)[16],u8 Start,u8 Num,uint16_t Str_Color,uint16_t Back_Color)
{
	uint16_t count_bety,i,j;
	uint8_t Ch_Num=32;//字节数	
		
 for(j=0;j<Num;j++)
  {
	  St77_Write_Cmd(0x2a);     //Column address set
		
	  St77_Write_Data(((usX&0xff00)>>8)); 	
	  St77_Write_Data((usX&0xff));    //start row
		
	  St77_Write_Data((((15+usX)&0xff00)>>8));	
	  St77_Write_Data((15+usX)&0xff);    //end row
		
	  St77_Write_Cmd(0x2b);     //Column address set
		
	  St77_Write_Data(((usY&0xff00)>>8)); 	
	  St77_Write_Data((usY&0xff));    //start row
		
	  St77_Write_Data((((15+usY)&0xff00)>>8));	
	  St77_Write_Data((15+usY)&0xff);    //end row
		
	  St77_Write_Cmd(0x2C);     //Memory write
  

		for(count_bety=0;count_bety<Ch_Num;count_bety++)
		{
			for(i=0;i<8;i++)
			{
				if(Str[Start*2+j*2][count_bety]&(0x01<<i))
				{
					St77_Write_Data(Str_Color>>8);
					St77_Write_Data(Str_Color);
				}
				else
				{
					St77_Write_Data(Back_Color>>8);
					St77_Write_Data(Back_Color);
				}
			}
		}
		usX+=16;
   }
	
}






