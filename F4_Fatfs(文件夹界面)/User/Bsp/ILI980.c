#include "ILI980.h"
#include "oled_funt.h"
#include "w25q128.h"
#include "ff.h"			/* Declarations of FatFs API */
#include "usart.h"

#define BL_STATE PFout(9)
#define REST_STATE PFout(11)

#define LIL_CMD_ADDR  (*(volatile uint16_t *)(0x68000000))
#define LIL_DATA_ADDR  (*(volatile uint16_t *)(0x68000002))
	
#define      CMD_SetCoordinateX		 		    0x2A00	     //设置X坐标
#define      CMD_SetCoordinateY		 		    0x2B00	     //设置Y坐标
#define      CMD_SetPixel		 		          0x2C00	     //填充像素
SRAM_HandleTypeDef  SRAM_Handler;
FMC_NORSRAM_TimingTypeDef Timing;

extern FIL File1;

uint8_t China_Buff[128];
uint8_t Photo_Buff[10240];

void NT35510_Pin_Init(void)
{
  GPIO_InitTypeDef GPIO_FSMC_InitStructure;
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
	
	GPIO_FSMC_InitStructure.Pin=GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
	GPIO_FSMC_InitStructure.Mode=GPIO_MODE_AF_PP;
	GPIO_FSMC_InitStructure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_FSMC_InitStructure.Pull=GPIO_PULLUP;
	GPIO_FSMC_InitStructure.Alternate=GPIO_AF12_FSMC;
	
	HAL_GPIO_Init(GPIOE,&GPIO_FSMC_InitStructure);

	GPIO_FSMC_InitStructure.Pin=GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10;
	HAL_GPIO_Init(GPIOD,&GPIO_FSMC_InitStructure);
	
	GPIO_FSMC_InitStructure.Pin=GPIO_PIN_4|GPIO_PIN_5;
	HAL_GPIO_Init(GPIOD,&GPIO_FSMC_InitStructure);

	GPIO_FSMC_InitStructure.Pin=GPIO_PIN_10;
	HAL_GPIO_Init(GPIOG,&GPIO_FSMC_InitStructure);

	GPIO_FSMC_InitStructure.Pin=GPIO_PIN_0;
	HAL_GPIO_Init(GPIOF,&GPIO_FSMC_InitStructure);
	
	GPIO_FSMC_InitStructure.Pin=GPIO_PIN_9|GPIO_PIN_11;
	GPIO_FSMC_InitStructure.Mode=GPIO_MODE_OUTPUT_PP;
	GPIO_FSMC_InitStructure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_FSMC_InitStructure.Pull=GPIO_PULLUP;

	HAL_GPIO_Init(GPIOF,&GPIO_FSMC_InitStructure);
	

}

void NT35510_Init_Confige(void)
{
   SRAM_Handler.Instance = FMC_NORSRAM_DEVICE;
   SRAM_Handler.Extended = FMC_NORSRAM_EXTENDED_DEVICE;
   __HAL_RCC_FSMC_CLK_ENABLE();	
  /* SRAM device configuration */  
  Timing.AddressSetupTime      = 0x04;
  Timing.AddressHoldTime       = 0x05;
  Timing.DataSetupTime         = 0x04;
  Timing.BusTurnAroundDuration = 0x00;
  Timing.CLKDivision           = 0x00;
  Timing.DataLatency           = 0x00;
  Timing.AccessMode            = FSMC_ACCESS_MODE_B;
 //使用NE3
  SRAM_Handler.Init.NSBank=FSMC_NORSRAM_BANK3;     					        
	//地址/数据线不复用
	SRAM_Handler.Init.DataAddressMux=FSMC_DATA_ADDRESS_MUX_DISABLE; 	
	//NOR
	SRAM_Handler.Init.MemoryType= FSMC_MEMORY_TYPE_NOR;   				    
	//16位数据宽度
	SRAM_Handler.Init.MemoryDataWidth=FSMC_NORSRAM_MEM_BUS_WIDTH_16; 	
	//是否使能突发访问,仅对同步突发存储器有效,此处未用到
	SRAM_Handler.Init.BurstAccessMode=FSMC_BURST_ACCESS_MODE_DISABLE; 
	//等待信号的极性,仅在突发模式访问下有用
	SRAM_Handler.Init.WaitSignalPolarity=FSMC_WAIT_SIGNAL_POLARITY_LOW;
	//存储器是在等待周期之前的一个时钟周期还是等待周期期间使能NWAIT
	SRAM_Handler.Init.WaitSignalActive=FSMC_WAIT_TIMING_BEFORE_WS;   	
	//存储器写使能
	SRAM_Handler.Init.WriteOperation=FSMC_WRITE_OPERATION_ENABLE;    	
	//等待使能位,此处未用到
	SRAM_Handler.Init.WaitSignal=FSMC_WAIT_SIGNAL_DISABLE;           	
	//读写使用相同的时序
	SRAM_Handler.Init.ExtendedMode=FSMC_EXTENDED_MODE_DISABLE;        
	//是否使能同步传输模式下的等待信号,此处未用到
	SRAM_Handler.Init.AsynchronousWait=FSMC_ASYNCHRONOUS_WAIT_DISABLE;
	//禁止突发写
	SRAM_Handler.Init.WriteBurst=FSMC_WRITE_BURST_DISABLE;           	
  SRAM_Handler.Init.ContinuousClock=FSMC_CONTINUOUS_CLOCK_SYNC_ASYNC;
	HAL_SRAM_Init(&SRAM_Handler,&Timing,&Timing);
	 
}
 
void NT35510_Write_Cmd(volatile uint16_t Cmd)
{
  LIL_CMD_ADDR=Cmd;
}

void NT35510_Write_Data(volatile uint16_t Data)
{
  LIL_DATA_ADDR=Data;
}

static void Delay ( __IO uint32_t nCount )
{
  for ( ; nCount != 0; nCount -- );
	
}
static void NT35510_REG_Config ( void )
{	
	NT35510_Init_Confige();
///NT35510-HSD43
  //PAGE1
  NT35510_Write_Cmd(0xF000);    NT35510_Write_Data(0x0055);
  NT35510_Write_Cmd(0xF001);    NT35510_Write_Data(0x00AA);
  NT35510_Write_Cmd(0xF002);    NT35510_Write_Data(0x0052);
  NT35510_Write_Cmd(0xF003);    NT35510_Write_Data(0x0008);
  NT35510_Write_Cmd(0xF004);    NT35510_Write_Data(0x0001);

  //Set AVDD 5.2V
  NT35510_Write_Cmd(0xB000);    NT35510_Write_Data(0x000D);
  NT35510_Write_Cmd(0xB001);    NT35510_Write_Data(0x000D);
  NT35510_Write_Cmd(0xB002);    NT35510_Write_Data(0x000D);

  //Set AVEE 5.2V
  NT35510_Write_Cmd(0xB100);    NT35510_Write_Data(0x000D);
  NT35510_Write_Cmd(0xB101);    NT35510_Write_Data(0x000D);
  NT35510_Write_Cmd(0xB102);    NT35510_Write_Data(0x000D);

  //Set VCL -2.5V
  NT35510_Write_Cmd(0xB200);    NT35510_Write_Data(0x0000);
  NT35510_Write_Cmd(0xB201);    NT35510_Write_Data(0x0000);
  NT35510_Write_Cmd(0xB202);    NT35510_Write_Data(0x0000);				

  //Set AVDD Ratio
  NT35510_Write_Cmd(0xB600);    NT35510_Write_Data(0x0044);
  NT35510_Write_Cmd(0xB601);    NT35510_Write_Data(0x0044);
  NT35510_Write_Cmd(0xB602);    NT35510_Write_Data(0x0044);

  //Set AVEE Ratio
  NT35510_Write_Cmd(0xB700);    NT35510_Write_Data(0x0034);
  NT35510_Write_Cmd(0xB701);    NT35510_Write_Data(0x0034);
  NT35510_Write_Cmd(0xB702);    NT35510_Write_Data(0x0034);

  //Set VCL -2.5V
  NT35510_Write_Cmd(0xB800);    NT35510_Write_Data(0x0034);
  NT35510_Write_Cmd(0xB801);    NT35510_Write_Data(0x0034);
  NT35510_Write_Cmd(0xB802);    NT35510_Write_Data(0x0034);
        
  //Control VGH booster voltage rang
  NT35510_Write_Cmd(0xBF00);    NT35510_Write_Data(0x0001); //VGH:7~18V	

  //VGH=15V(1V/step)	Free pump
  NT35510_Write_Cmd(0xB300);    NT35510_Write_Data(0x000f);		//08
  NT35510_Write_Cmd(0xB301);    NT35510_Write_Data(0x000f);		//08
  NT35510_Write_Cmd(0xB302);    NT35510_Write_Data(0x000f);		//08

  //VGH Ratio
  NT35510_Write_Cmd(0xB900);    NT35510_Write_Data(0x0034);
  NT35510_Write_Cmd(0xB901);    NT35510_Write_Data(0x0034);
  NT35510_Write_Cmd(0xB902);    NT35510_Write_Data(0x0034);

  //VGL_REG=-10(1V/step)
  NT35510_Write_Cmd(0xB500);    NT35510_Write_Data(0x0008);
  NT35510_Write_Cmd(0xB501);    NT35510_Write_Data(0x0008);
  NT35510_Write_Cmd(0xB502);    NT35510_Write_Data(0x0008);

  NT35510_Write_Cmd(0xC200);    NT35510_Write_Data(0x0003);

  //VGLX Ratio
  NT35510_Write_Cmd(0xBA00);    NT35510_Write_Data(0x0034);
  NT35510_Write_Cmd(0xBA01);    NT35510_Write_Data(0x0034);
  NT35510_Write_Cmd(0xBA02);    NT35510_Write_Data(0x0034);

    //VGMP/VGSP=4.5V/0V
  NT35510_Write_Cmd(0xBC00);    NT35510_Write_Data(0x0000);		//00
  NT35510_Write_Cmd(0xBC01);    NT35510_Write_Data(0x0078);		//C8 =5.5V/90=4.8V
  NT35510_Write_Cmd(0xBC02);    NT35510_Write_Data(0x0000);		//01

  //VGMN/VGSN=-4.5V/0V
  NT35510_Write_Cmd(0xBD00);    NT35510_Write_Data(0x0000); //00
  NT35510_Write_Cmd(0xBD01);    NT35510_Write_Data(0x0078); //90
  NT35510_Write_Cmd(0xBD02);    NT35510_Write_Data(0x0000);

  //Vcom=-1.4V(12.5mV/step)
  NT35510_Write_Cmd(0xBE00);    NT35510_Write_Data(0x0000);
  NT35510_Write_Cmd(0xBE01);    NT35510_Write_Data(0x0064); //HSD:64;Novatek:50=-1.0V, 80  5f


  //PAGE0
  NT35510_Write_Cmd(0xF000);    NT35510_Write_Data(0x0055);
  NT35510_Write_Cmd(0xF001);    NT35510_Write_Data(0x00AA);
  NT35510_Write_Cmd(0xF002);    NT35510_Write_Data(0x0052);
  NT35510_Write_Cmd(0xF003);    NT35510_Write_Data(0x0008);	
  NT35510_Write_Cmd(0xF004);    NT35510_Write_Data(0x0000); 

  //480x800
  NT35510_Write_Cmd(0xB500);    NT35510_Write_Data(0x0050);

  //NT35510_Write_Cmd(0x2C00);    NT35510_Write_Data(0x0006); //8BIT 6-6-6?

  //Dispay control
  NT35510_Write_Cmd(0xB100);    NT35510_Write_Data(0x00CC);	
  NT35510_Write_Cmd(0xB101);    NT35510_Write_Data(0x0000); // S1->S1440:00;S1440->S1:02

  //Source hold time (Nova non-used)
  NT35510_Write_Cmd(0xB600);    NT35510_Write_Data(0x0005);

  //Gate EQ control	 (Nova non-used)
  NT35510_Write_Cmd(0xB700);    NT35510_Write_Data(0x0077);  //HSD:70;Nova:77	 
  NT35510_Write_Cmd(0xB701);    NT35510_Write_Data(0x0077);	//HSD:70;Nova:77

  //Source EQ control (Nova non-used)
  NT35510_Write_Cmd(0xB800);    NT35510_Write_Data(0x0001);  
  NT35510_Write_Cmd(0xB801);    NT35510_Write_Data(0x0003);	//HSD:05;Nova:07
  NT35510_Write_Cmd(0xB802);    NT35510_Write_Data(0x0003);	//HSD:05;Nova:07
  NT35510_Write_Cmd(0xB803);    NT35510_Write_Data(0x0003);	//HSD:05;Nova:07

  //Inversion mode: column
  NT35510_Write_Cmd(0xBC00);    NT35510_Write_Data(0x0002);	//00: column
  NT35510_Write_Cmd(0xBC01);    NT35510_Write_Data(0x0000);	//01:1dot
  NT35510_Write_Cmd(0xBC02);    NT35510_Write_Data(0x0000); 

  //Frame rate	(Nova non-used)
  NT35510_Write_Cmd(0xBD00);    NT35510_Write_Data(0x0001);
  NT35510_Write_Cmd(0xBD01);    NT35510_Write_Data(0x0084);
  NT35510_Write_Cmd(0xBD02);    NT35510_Write_Data(0x001c); //HSD:06;Nova:1C
  NT35510_Write_Cmd(0xBD03);    NT35510_Write_Data(0x001c); //HSD:04;Nova:1C
  NT35510_Write_Cmd(0xBD04);    NT35510_Write_Data(0x0000);

  //LGD timing control(4H/4-delay_ms)
  NT35510_Write_Cmd(0xC900);    NT35510_Write_Data(0x00D0);	//3H:0x50;4H:0xD0	 //D
  NT35510_Write_Cmd(0xC901);    NT35510_Write_Data(0x0002);  //HSD:05;Nova:02
  NT35510_Write_Cmd(0xC902);    NT35510_Write_Data(0x0050);	//HSD:05;Nova:50
  NT35510_Write_Cmd(0xC903);    NT35510_Write_Data(0x0050);	//HSD:05;Nova:50	;STV delay_ms time
  NT35510_Write_Cmd(0xC904);    NT35510_Write_Data(0x0050);	//HSD:05;Nova:50	;CLK delay_ms time

  NT35510_Write_Cmd(0x3600);    NT35510_Write_Data(0x0040);
  NT35510_Write_Cmd(0x3500);    NT35510_Write_Data(0x0000);

  NT35510_Write_Cmd(0xFF00);    NT35510_Write_Data(0x00AA);
  NT35510_Write_Cmd(0xFF01);    NT35510_Write_Data(0x0055);
  NT35510_Write_Cmd(0xFF02);    NT35510_Write_Data(0x0025);
  NT35510_Write_Cmd(0xFF03);    NT35510_Write_Data(0x0001);

  NT35510_Write_Cmd(0xFC00);    NT35510_Write_Data(0x0016);
  NT35510_Write_Cmd(0xFC01);    NT35510_Write_Data(0x00A2);
  NT35510_Write_Cmd(0xFC02);    NT35510_Write_Data(0x0026);
  NT35510_Write_Cmd(0x3A00);    NT35510_Write_Data(0x0006);

  NT35510_Write_Cmd(0x3A00);    NT35510_Write_Data(0x0055);
  //Sleep out
  NT35510_Write_Cmd(0x1100);	   //?
  Delay(0xFFFFFF);

  //Display on
  NT35510_Write_Cmd(0x2900);
 
}
void NT35510_Rst(void)
{
	REST_STATE=0;
	Delay ( 0xAFF ); 					   

	REST_STATE=1;	 
	Delay ( 0xAFF ); 	
}

void NT35510_Init ( void )
{
	NT35510_Pin_Init();
 	BL_STATE=1;	
//  //引脚复位
	NT35510_Rst ();
	NT35510_REG_Config ();
  
	NT35510_Clear(0,0,800,480);
	
}

/**
 * @brief  在NT35510显示器上开辟一个窗口
 * @param  usX ：在特定扫描方向下窗口的起点X坐标
 * @param  usY ：在特定扫描方向下窗口的起点Y坐标
 * @param  usWidth ：窗口的宽度
 * @param  usHeight ：窗口的高度
 * @retval 无
 */
void NT35510_OpenWindow ( uint16_t usX, uint16_t usY, uint16_t usWidth, uint16_t usHeight )
{	
	NT35510_Write_Cmd ( CMD_SetCoordinateX ); 				 /* 设置X坐标 */
	
	NT35510_Write_Data ( usY >> 8  );
  NT35510_Write_Cmd ( CMD_SetCoordinateX + 1);
	NT35510_Write_Data ( usY & 0xff  );
  NT35510_Write_Cmd ( CMD_SetCoordinateX + 2);
	NT35510_Write_Data ( ( usY + usHeight - 1 ) >> 8 );
  NT35510_Write_Cmd ( CMD_SetCoordinateX + 3);
	NT35510_Write_Data ( ( usY + usHeight - 1) & 0xff );

	NT35510_Write_Cmd ( CMD_SetCoordinateY ); 			     /* 设置Y坐标*/

	NT35510_Write_Data ( usX >> 8  );	 /* 先高8位，然后低8位 */
  NT35510_Write_Cmd ( CMD_SetCoordinateY + 1 ); 
	NT35510_Write_Data ( usX & 0xff  );	 /* 设置起始点和结束点*/
  NT35510_Write_Cmd ( CMD_SetCoordinateY + 2 );
	NT35510_Write_Data ( ( usX + usWidth - 1 ) >> 8  );
  NT35510_Write_Cmd ( CMD_SetCoordinateY + 3 );
	NT35510_Write_Data ( ( usX + usWidth - 1 ) & 0xff  );	
}

void NT35510_Write_Pixel(uint32_t Number,uint16_t Colour)
{
	uint32_t i;
 NT35510_Write_Cmd(CMD_SetPixel);
	
	for(i=0;i<Number;i++)
	{
	    NT35510_Write_Data(Colour);
	}

}
 
/* 清屏，显示全黑 */
void NT35510_Clear(uint16_t x,uint16_t y,uint16_t Width,uint16_t Hight)	
{

 NT35510_OpenWindow(x,y,Width,Hight);
 NT35510_Write_Pixel(Width*Hight,0x5555);
}

 
/* 清屏，显示全黑 */
void NT35510_Draw_Win(uint16_t x,uint16_t y,uint16_t Width,uint16_t Hight,uint16_t Colour)	
{

 NT35510_OpenWindow(x,y,Width,Hight);
 NT35510_Write_Pixel(Width*Hight,Colour);
}


/* 涂色 */
void NT35510_Coloring(uint16_t x,uint16_t y,uint16_t Width,uint16_t Hight,uint16_t Colour)	
{

 NT35510_OpenWindow(x,y,Width,Hight);

 NT35510_Write_Pixel(Width*Hight,Colour);
}


 void TFT_Dis_Char(uint16_t usX,uint16_t usY,uint8_t Ch,uint16_t Str_Color,uint16_t Back_Color)
{
	uint16_t count_bety,i;
	uint8_t Ch_len=16;//字节数	
	
  NT35510_OpenWindow(usX,usY,8,16);
  NT35510_Write_Cmd(CMD_SetPixel);
	
	for(count_bety=0;count_bety<Ch_len;count_bety++)
	{
		for(i=0;i<8;i++)
		{
			if(Asiic_Arr[Ch][count_bety]&(0x01<<i))
			{
				  NT35510_Write_Data(Str_Color);
			}
			else
			{
			  NT35510_Write_Data(Back_Color);
			}
		}
	 
	}	
}

void TFT_Dis_Str(uint16_t usX,uint16_t usY,uint8_t*Str,uint16_t Str_Color,uint16_t Back_Color)//输出英文
{
    uint8_t Ch_len=8;
	  uint16_t Buffx=usX;
	while(*Str!='\0')
	{
		TFT_Dis_Char(usX,usY,*Str,Str_Color,Back_Color);
		usX+=Ch_len;
		if(usX>800)
		{
		 usY+=25;
		 usX=Buffx;
		}
		Str++;
	}

}

void TFT_Dis_Image(uint16_t usX,uint16_t usY,uint16_t Width,uint16_t High,const uint8_t *Str)
{
   uint32_t ROW;
	 uint16_t column;
	
   NT35510_OpenWindow(usX,usY,Width,High);
   NT35510_Write_Cmd(CMD_SetPixel);
	
	for(ROW=0;ROW<(Width*High*2);ROW++)             //ROW loop
		{ 
			    column=Str[ROW+1];
			    column=(column<<8)|Str[ROW];
					NT35510_Write_Data(column);
			    ROW+=1;
		}
}

void TFT_Dis_FsImage(uint16_t usX,uint16_t usY,uint16_t Width,uint16_t High,uint32_t Num)
{
   uint32_t ROW,i,Page_Num,Count;
	 uint16_t column;
	 uint32_t Br;	
	
	 Page_Num=Num/10240;//可以都几页缓冲区
	 Count=Num%10240;//剩余字节

   NT35510_OpenWindow(usX,usY,Width,High);
   NT35510_Write_Cmd(CMD_SetPixel);
	
	for(ROW=0;ROW<Page_Num;ROW++)             //ROW loop
		{ 
    	f_read(&File1,&Photo_Buff,10240,&Br);//读取10240个数据并显示
			for(i=0;i<10240;i++)             //ROW loop
				{ 	
						column=Photo_Buff[i+1];
						column=(column<<8)|Photo_Buff[i];
						NT35510_Write_Data(column);
						i+=1;
				}
		}
     f_read(&File1,&Photo_Buff,Count,&Br);
		
		for(i=0;i<Count;i++)             //ROW loop
			{ 	
					column=Photo_Buff[i+1];
					column=(column<<8)|Photo_Buff[i];
					NT35510_Write_Data(column);
					i+=1;
			}
}

 void TFT_Dis_Chinaese(uint16_t usX,uint16_t usY,uint8_t *Str, uint16_t Str_Color,uint16_t Back_Color)
{
	uint16_t count_bety,i,Num=2,j,Buffx=usX;
	uint8_t Ch_len=128;//字节数	
	uint32_t Addr;
	
  NT35510_OpenWindow(usX,usY,32,32);
  NT35510_Write_Cmd(CMD_SetPixel);
	
 while(*Str!='\0')
 {
	  Addr=(((Str[0]-0xA0-1)*94) +(Str[1]-0xA0-1))*Ch_len;//Str[0]代表区，Str[1]代表位
    W25q128_Read_LotData(Addr,China_Buff,128);
	 
		for(count_bety=0;count_bety<Ch_len;count_bety++)
		{
			for(i=0;i<8;i++)
			{
				if(China_Buff[count_bety]&(0x01<<i))
				{
						NT35510_Write_Data(Str_Color);
				}
				else
				{
					NT35510_Write_Data(Back_Color);
				}
			}
		
		}	
			usX+=32;
		if(usX>800)
		{
		 usY+=40;
		 usX=Buffx;
		}
		
			NT35510_OpenWindow(usX,usY,32,32);
			NT35510_Write_Cmd(CMD_SetPixel);  
		  Str+=2;
 }
	
}
