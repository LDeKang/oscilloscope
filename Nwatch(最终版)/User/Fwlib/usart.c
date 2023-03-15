#include "usart.h"

USART1_BUF_STRUCT Usart1_Buf_St;

static void Usart_Gpio_Init(void)
{
	
  GPIO_InitTypeDef Usart_Pin_Structure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	Usart_Pin_Structure.GPIO_Pin=GPIO_Pin_9;
	Usart_Pin_Structure.GPIO_Mode=GPIO_Mode_AF_PP;
	Usart_Pin_Structure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&Usart_Pin_Structure);

	
	Usart_Pin_Structure.GPIO_Pin=GPIO_Pin_10;
	Usart_Pin_Structure.GPIO_Mode=GPIO_Mode_IPU;
	
	GPIO_Init(GPIOA,&Usart_Pin_Structure);
}

static void Usart_Nvic_Config(void)
{
  NVIC_InitTypeDef Usart_Nvic_Structure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	Usart_Nvic_Structure.NVIC_IRQChannel=USART1_IRQn;
	Usart_Nvic_Structure.NVIC_IRQChannelPreemptionPriority=1;
	Usart_Nvic_Structure.NVIC_IRQChannelSubPriority=1;
	Usart_Nvic_Structure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&Usart_Nvic_Structure);
}


void Usart1_Init_Config(void)
{
	USART_InitTypeDef Usart_Init_Structure;

	Usart_Gpio_Init();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	Usart_Init_Structure.USART_BaudRate=115200;
	Usart_Init_Structure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	Usart_Init_Structure.USART_StopBits=USART_StopBits_1;
	Usart_Init_Structure.USART_Parity=USART_Parity_No;
	Usart_Init_Structure.USART_WordLength=USART_WordLength_8b;
	Usart_Init_Structure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_Init(USART1,&Usart_Init_Structure);
	Usart_Nvic_Config();
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
	USART_Cmd(USART1,ENABLE);
	
}

void Usart1_Send_Char(u8 c)
{
  USART_SendData(USART1,c);//д��DR���ұ�־λTx��0
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==0);
	
}


void Usart1_Send_Str(u8 *Str)
{
 while(*Str)
 {
   Usart1_Send_Char(*Str);
	 Str++;
 }
	
}

int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ����� */
		USART_SendData(USART1, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}








 