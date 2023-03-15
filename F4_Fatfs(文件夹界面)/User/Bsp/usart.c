#include "usart.h"
#include "led.h"
uint8_t Rx_Buff[1];


UART_HandleTypeDef UART_Handle;


void HAL_UART_MspInit(UART_HandleTypeDef *husart)
{
  GPIO_InitTypeDef USART_Pin_InitStructure;
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	USART_Pin_InitStructure.Pin=GPIO_PIN_9;
	USART_Pin_InitStructure.Mode=GPIO_MODE_AF_PP;
	USART_Pin_InitStructure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;
	USART_Pin_InitStructure.Pull=GPIO_PULLUP;
	USART_Pin_InitStructure.Alternate=GPIO_AF7_USART1;
	
	HAL_GPIO_Init(GPIOA,&USART_Pin_InitStructure);
	
	USART_Pin_InitStructure.Pin=GPIO_PIN_10;
	USART_Pin_InitStructure.Alternate=GPIO_AF7_USART1;
	HAL_GPIO_Init(GPIOA,&USART_Pin_InitStructure);
	
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_1);
	HAL_NVIC_SetPriority(USART1_IRQn,1,0);
	HAL_NVIC_EnableIRQ(USART1_IRQn);
	
}

void USART1_Init_Config(void)
{
	__HAL_RCC_USART1_CLK_ENABLE();		
	
	UART_Handle.Instance=USART1;
 	UART_Handle.pRxBuffPtr=Rx_Buff;
	UART_Handle.RxXferSize=5;//注意 串口上位机发送数据超过RxXferSize，会出问题
	
	UART_Handle.Init.BaudRate=115200;
	UART_Handle.Init.Mode=UART_MODE_TX_RX;
	UART_Handle.Init.StopBits=UART_STOPBITS_1;
	UART_Handle.Init.WordLength=UART_WORDLENGTH_8B;
	UART_Handle.Init.Parity=UART_PARITY_NONE;
	UART_Handle.Init.HwFlowCtl=UART_HWCONTROL_NONE;
	UART_Handle.Init.OverSampling=UART_OVERSAMPLING_16;
	
	HAL_UART_Init(&UART_Handle);
	
  __HAL_UART_ENABLE_IT(&UART_Handle,UART_IT_RXNE);  
	HAL_UART_Receive_IT(&UART_Handle,UART_Handle.pRxBuffPtr,UART_Handle.RxXferSize);
	
}

void Send_Str(uint8_t *c)
{
	while(*c)
	{
	  HAL_UART_Transmit(&UART_Handle,c,1,1000);
		c++;
		
	}

 
}
uint16_t Cnt=0;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	uint16_t i;	
  UNUSED(huart);

  HAL_UART_Transmit(&UART_Handle, Rx_Buff, UART_Handle.RxXferSize, 100);    // 把收到的字节原样发送出去
// 	HAL_UART_Receive_IT(&UART_Handle,(uint8_t*)UART_Handle.pRxBuffPtr,UART_Handle.RxXferSize);//在多次之后会出问题
  	HAL_UART_Receive_IT(&UART_Handle,Rx_Buff,UART_Handle.RxXferSize);
	
}


///重定向c库函数printf到串口DEBUG_USART，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
	/* 发送一个字节数据到串口DEBUG_USART,也是直接操作寄存器，不使用HAL库结构体*/
	HAL_UART_Transmit(&UART_Handle, (uint8_t *)&ch, 1, 1000);
	
	return (ch);
}

///重定向c库函数scanf到串口DEBUG_USART，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
		
	int ch;
	HAL_UART_Receive(&UART_Handle, (uint8_t *)&ch, 1, 1000);	//直接操作寄存器，不使用HAL库结构体
	return (ch);
}