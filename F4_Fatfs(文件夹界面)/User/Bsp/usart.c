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
	UART_Handle.RxXferSize=5;//ע�� ������λ���������ݳ���RxXferSize���������
	
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

  HAL_UART_Transmit(&UART_Handle, Rx_Buff, UART_Handle.RxXferSize, 100);    // ���յ����ֽ�ԭ�����ͳ�ȥ
// 	HAL_UART_Receive_IT(&UART_Handle,(uint8_t*)UART_Handle.pRxBuffPtr,UART_Handle.RxXferSize);//�ڶ��֮��������
  	HAL_UART_Receive_IT(&UART_Handle,Rx_Buff,UART_Handle.RxXferSize);
	
}


///�ض���c�⺯��printf������DEBUG_USART���ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
	/* ����һ���ֽ����ݵ�����DEBUG_USART,Ҳ��ֱ�Ӳ����Ĵ�������ʹ��HAL��ṹ��*/
	HAL_UART_Transmit(&UART_Handle, (uint8_t *)&ch, 1, 1000);
	
	return (ch);
}

///�ض���c�⺯��scanf������DEBUG_USART����д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
		
	int ch;
	HAL_UART_Receive(&UART_Handle, (uint8_t *)&ch, 1, 1000);	//ֱ�Ӳ����Ĵ�������ʹ��HAL��ṹ��
	return (ch);
}