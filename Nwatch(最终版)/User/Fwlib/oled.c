#include "oled.h"
#include "oled_funt.h"
#include "sys.h"
#include "wq_flash.h"

#define NSS_STATE PAout(2)
#define DC_STATE PAout(3)
#define CS_STATE PAout(1)

#define PAGE_SIZE    8
#define WIDTH 	     128
#define HEIGHT 	     64	
u8 Flash_Buff[16];
u8 oledBuffer[FRAME_BUFFER_SIZE];
#ifndef _swap_char
#define _swap_char(a, b) \
    {                    \
        u8 t = a;     \
        a = b;           \
        b = t;           \
    }
#endif
void Oled_Gpio_Init(void)
{
	
  GPIO_InitTypeDef Oled_Pin_Structure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	Oled_Pin_Structure.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_7;
	
	Oled_Pin_Structure.GPIO_Mode=GPIO_Mode_AF_PP;
	Oled_Pin_Structure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&Oled_Pin_Structure);
	
	Oled_Pin_Structure.GPIO_Pin=GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_1;
	Oled_Pin_Structure.GPIO_Mode=GPIO_Mode_Out_PP;
	Oled_Pin_Structure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&Oled_Pin_Structure);
	CS_STATE=1;
}

void Oled_DMA_Init(void)
{
	
  DMA_InitTypeDef Oled_Dma_Structure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	Oled_Dma_Structure.DMA_PeripheralBaseAddr=(uint32_t)&(SPI1->DR);
	Oled_Dma_Structure.DMA_MemoryBaseAddr=(uint32_t)&oledBuffer[0];
   Oled_Dma_Structure.DMA_DIR=DMA_DIR_PeripheralDST;
	Oled_Dma_Structure.DMA_BufferSize=FRAME_BUFFER_SIZE;
	Oled_Dma_Structure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	Oled_Dma_Structure.DMA_MemoryInc=DMA_MemoryInc_Enable;
	Oled_Dma_Structure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
	Oled_Dma_Structure.DMA_Mode=DMA_Mode_Circular;
	Oled_Dma_Structure.DMA_Priority=DMA_Priority_VeryHigh;
	Oled_Dma_Structure.DMA_M2M=DMA_M2M_Disable;
	
	DMA_Init(DMA1_Channel3,&Oled_Dma_Structure);
	DMA_Cmd(DMA1_Channel3,DISABLE);
 
}

void Oled_Spi_Init(void)
{
	
  SPI_InitTypeDef Oled_Spi_Structure;
	
	Oled_Gpio_Init();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	
	Oled_Spi_Structure.SPI_Direction=SPI_Direction_2Lines_FullDuplex;
	Oled_Spi_Structure.SPI_Mode=SPI_Mode_Master;
	Oled_Spi_Structure.SPI_DataSize=SPI_DataSize_8b;
	Oled_Spi_Structure.SPI_CPOL=SPI_CPOL_Low;
	Oled_Spi_Structure.SPI_CPHA=SPI_CPHA_1Edge;
	Oled_Spi_Structure.SPI_NSS=SPI_NSS_Soft;
	Oled_Spi_Structure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_2;
	Oled_Spi_Structure.SPI_FirstBit=SPI_FirstBit_MSB;
  Oled_Spi_Structure.SPI_CRCPolynomial=7;//随意
	
	SPI_Init(SPI1,&Oled_Spi_Structure);
	SPI_Cmd(SPI1,ENABLE);
	Oled_DMA_Init();
	SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Tx,ENABLE);
	
	NSS_STATE=1;
	Sys_Del_Ms(100);
	NSS_STATE=0;
	Sys_Del_Ms(100);
	NSS_STATE=1;
}


void Oled_Write_Data(u8 Data)
{
	DC_STATE=1;
	CS_STATE=0;
	
	SPI_I2S_SendData(SPI1,Data);	
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==0);
	
	CS_STATE=1;
	DC_STATE=1;
}

void Oled_Write_Cmd(u8 Cmd)
{
	DC_STATE=0;
	CS_STATE=0;
	SPI_I2S_SendData(SPI1,Cmd);	
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==0);
	CS_STATE=1;
	DC_STATE=1;
}

 

uint16_t OLED_Qclear(void)
{
    memset(oledBuffer, 0x00, FRAME_BUFFER_SIZE);
	
	return 1;
}
uint16_t OLED_Qfull(void)
{
    memset(oledBuffer, 0xff, FRAME_BUFFER_SIZE);
	  return 1;
}

void oled_flush(void)
{
	uint16_t i,j;
	u8 *Str=oledBuffer;
	
	for(i=0;i<FRAME_BUFFER_SIZE;i++)
	{
	  Oled_Write_Data(*Str++);
	}
//	DC_STATE=1;
//	
//	DMA_SetCurrDataCounter(DMA1_Channel3, FRAME_BUFFER_SIZE); 	 
//	DMA_Cmd(DMA1_Channel3, ENABLE); 
//	
//	while(!DMA_GetFlagStatus(DMA1_FLAG_TC3)); 	 
//	
//	DMA_Cmd(DMA1_Channel3, DISABLE); 			 
//	DMA_ClearFlag(DMA1_FLAG_TC3); 		
//	
//	DC_STATE=1;
}

 
void OLED_Set_Pixel(unsigned char x, unsigned char y,unsigned char color)
{
	if(color)
	{
		oledBuffer[(y/PAGE_SIZE)*WIDTH+x]|= (1<<(y%PAGE_SIZE))&0xff;
	}
	else
	{
		oledBuffer[(y/PAGE_SIZE)*WIDTH+x]&= ~((1<<(y%PAGE_SIZE))&0xff);
	}
}		
 
/*****************************************************************************
 * @name       :void GUI_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size,u8 mode)
 * @date       :2018-08-27 
 * @function   :Display a single English character
 * @parameters :x:the bebinning x coordinate of the Character display position
                y:the bebinning y coordinate of the Character display position
								chr:the ascii code of display character(0~94)
								Char_Size:the size of display character(8,16)
								mode:0-white background and black character
								     1-black background and white character
 * @retvalue   :None
******************************************************************************/ 
void GUI_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size,u8 mode)
{      	
	  u8 c=0,i=0,tmp,j=0;	
 	  uint32_t Read_Addr;
	
//	   c=chr-' ';
	   Read_Addr=(chr-3)*16;
	   W25q128_Read_LotData(Read_Addr,Flash_Buff,16);
		if(x>WIDTH-1){x=0;y=y+2;}
		if(Char_Size ==16)
		{
			for(i=0;i<16;i++)
			{
			  if(mode)
				{
				tmp = Flash_Buff[i];
				}
				else
				{
    			tmp = Flash_Buff[i];
				}
				for(j=0;j<8;j++)
				{
					if(tmp&(0x80>>j))
					{
						OLED_Set_Pixel(x+j, y+i,1);
					}
					else
					{
						OLED_Set_Pixel(x+j, y+i,0);
					}
				}
			}
		}
		else if(Char_Size==8)
		{	
				for(i=0;i<8;i++)
				{
				  if(mode)
					{
//						tmp = Asiic_Arr[c][i];
					}
					else
					{
//						tmp = ~(Asiic_Arr[c][i]);
					}
					for(j=0;j<8;j++)
					{
						if(tmp&(0x80>>j))
						{
							OLED_Set_Pixel(x+j, y+i,1);
						}
						else
						{
							OLED_Set_Pixel(x+j, y+i,0);
						}
					}
			}
	}
	else
	{
		return;
	}
 
}

//void Oled_Str(u8 x,u8 y,u8* Str)
//{
// while(*Str!='\0')
// {
//  GUI_ShowChar(x,y,*Str,16,1);
//	 *Str++;
//	 x+=8;
// }
//}

/*****************************************************************************
 * @name       :void GUI_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size,u8 mode)
 * @date       :2018-08-27 
 * @function   :Display English string
 * @parameters :x:the bebinning x coordinate of the English string
                y:the bebinning y coordinate of the English string
								chr:the start address of the English string
								Char_Size:the size of display character
								mode:0-white background and black character
								     1-black background and white character
 * @retvalue   :None
******************************************************************************/   	  
void GUI_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size,u8 mode)
{
	unsigned char j=0,csize;
	if(Char_Size == 16)
  {
	  csize = Char_Size/2;
	}
  else if(Char_Size == 8)
  {
	  csize = Char_Size/2+2;
	}
	else
	{
		return;
	}
	while (chr[j]!='\0')
	{		
		GUI_ShowChar(x,y,chr[j],Char_Size,mode);
		x+=csize;
		if(x>120)
		{
			x=0;
			y+=Char_Size;
		}
		j++;
	}
}

static u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}
//void GUI_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 Size,u8 mode)
//{         	
//	u8 t,temp;
//	u8 enshow=0,csize;
//  if(Size == 16)
//  {
//	  csize = Size/2;
//	}
//  else if(Size == 8)
//  {
//	  csize = Size/2+2;
//	} 	
//	else
//	{
//		return;
//	}
//	for(t=0;t<len;t++)
//	{
//		temp=(num/mypow(10,len-t-1))%10;
//		if(enshow==0&&t<(len-1))
//		{
//			if(temp==0)
//			{
//				GUI_ShowChar(x+csize*t,y,' ',Size,mode);
//				continue;
//			}else enshow=1; 
//		 	 
//		}
//	 	GUI_ShowChar(x+csize*t,y,temp+'0',Size,mode); 
//	}
//}

/*  绘制两点线段至缓存
    Draw line between two points to buffer  */
void OLED_DrawLine(u8 x0, u8 y0, u8 x1, u8 y1)
{
    char dx, dy, ystep;
    int err;
    u8 swapxy = 0;

    if (x0 > WIDTH - 1)
        x0 = WIDTH - 1;

    if (y0 > HEIGHT - 1)
        y0 = HEIGHT - 1;

    if (x1 > WIDTH - 1)
        x1 = WIDTH - 1;

    if (y1 > HEIGHT - 1)
        y1 = HEIGHT - 1;

    dx = x1 - x0;
    dy = y1 - y0;

    if (dy > dx)
    {
        swapxy = 1;
        _swap_char(dx, dy);
        _swap_char(x0, y0);
        _swap_char(x1, y1);
    }

    if (x0 > x1)
    {
        _swap_char(x0, x1);
        _swap_char(y0, y1);
    }

    err = dx >> 1;

    if (y0 < y1)
    {
        ystep = 1;
    }
    else
    {
        ystep = -1;
    }

    for (; x0 <= x1; x0++)
    {
        if (swapxy == 0)
            OLED_Set_Pixel(x0, y0,1);
        else
            OLED_Set_Pixel(y0, x0,1);

        err -= dy;

        if (err < 0)
        {
            y0 += ystep;
            err += dx;
        }
    }
}

//初始化SSD1306					    
void oled_init(void)
{ 	
  Oled_Spi_Init();
	
   Oled_Write_Cmd(0xAE); //--Turn off oled panel
	
    /*  
        以下三条命令是给页面模式用的，行模式和列模式不要设置，会引起问题
        行模式或列模式，要用0x21和0x22设置范围
        Following three commands are for Page Addressing Mode. Do not set them in Horizontal addressing mode or Vertical addressing mode, will rise problem.
        For Horizontal addressing mode or Vertical addressing mode, should use 0x21 and 0x22 set column and page address
    */
    //	Oled_Write_Cmd(0x00); //-Set Lower Column Start Address for Page Addressing Mode
    //	Oled_Write_Cmd(0x10); //-Set Higher Column Start Address for Page Addressing Mode
    //	Oled_Write_Cmd(0xB0); //-Set the page start address of the target display location by command for Page Addressing Mode

    Oled_Write_Cmd(0x20); //-Set Page Addressing Mode (0x00/0x01/0x02)
    Oled_Write_Cmd(0x00); //--0x00: Horizontal addressing mode, 0x01: Vertical addressing mode, 0x02: Page addressing mode
    
    Oled_Write_Cmd(0x21); //-Set Column Address
    Oled_Write_Cmd(0x00); //--Start address
    Oled_Write_Cmd(0x7f); //--End address

    Oled_Write_Cmd(0x22); //-Set Page Address
    Oled_Write_Cmd(0x00); //---Start address
    Oled_Write_Cmd(0x07); //--End address

    Oled_Write_Cmd(0x40); //-Set Display Start Line (0x40h~0x7F)

    Oled_Write_Cmd(0x81);                 //-Set Contrast Control for BANK0
    Oled_Write_Cmd(1); //    -0x00 to 0xFF, The segment output current increases as the contrast step value increases

    Oled_Write_Cmd(0xA1); //-Set Segment Re-map. 0xA1: Normal, 0xA0: Re-map left and right
    Oled_Write_Cmd(0xC8); //-Set COM Output Scan Direction. 0xC8: Normal, 0xC0: Re-map up and down
    Oled_Write_Cmd(0xA6); //-Set Normal/Inverse Display, 0xA6:Normal, 0xA7: Inverse

    Oled_Write_Cmd(0xA8); //-Set Multiplex Ratio (16~63)
    Oled_Write_Cmd(0x3F); //--63 multiplex mode

    Oled_Write_Cmd(0xD3); //-Set Display Offset (0x00~0x3F)
    Oled_Write_Cmd(0x00); //--No offset

    Oled_Write_Cmd(0xD5); //-Set display clock divide ratio/oscillator frequency
    Oled_Write_Cmd(0x20); //--Set Clock as 60 Frames/Sec

    Oled_Write_Cmd(0xD9); //-Set pre-charge period
    Oled_Write_Cmd(0xF2); //--Set Pre-Charge as 15 Clocks & Discharge as 2 Clock

    Oled_Write_Cmd(0xDA); //-Set com pins hardware configuration
    Oled_Write_Cmd(0x12); //

    Oled_Write_Cmd(0xDB); //-Set VCOM Deselect Level
    Oled_Write_Cmd(0x30); //--0.83xVcc

    Oled_Write_Cmd(0x8D); //-Set Charge Pump enable/disable
    Oled_Write_Cmd(0x14); //--0x14: Enable, 0x10: Disable

    Oled_Write_Cmd(0xA4); //-Entire Display ON, 0xA4: Disable, 0xA5: Enable
    Oled_Write_Cmd(0xAF); //-Turn on oled panel
	  OLED_Qclear();
	  oled_setInvert(1);
//	  OLED_Qfull();
//	  oled_flush();
}  
 
static void sendCmd2(u8 cmd, u8 val)
{
		Oled_Write_Cmd(cmd);
		Oled_Write_Cmd(val);
}
void oled_power(u8 on)
{
	Oled_Write_Cmd(on == OLED_PWR_ON ? OLED_ON : OLED_OFF);
}

void oled_setBrightness(u8 brightness)
{
	sendCmd2(OLED_SETCONTRAST, brightness);
}

void oled_setInvert(u8 invert)
{
	Oled_Write_Cmd(invert ? OLED_INVERT : OLED_NONINVERT);
}



void oled_set180(u8 rotate)
{
	if(rotate)
		sendCmd2(OLED_SEG_REMAP, OLED_SCANDIR_REMAP);
	else
		sendCmd2(OLED_SEG_NML, OLED_SCANDIR_NML);
}





