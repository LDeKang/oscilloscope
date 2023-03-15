/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "usart.h"
#include "adc.h"
#include "led.h"
#include "tim2.h"
#include "scope_api.h"

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	Systick_Count_Fun();
}

void TIM3_IRQHandler(void)
{
  
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET)
	{
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	}
	
}

void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)
	{
		if(Usart1_Buf_St.Udata_Len<BUF_MAX_LEN)
		{
				Usart1_Buf_St.Udata=USART_ReceiveData(USART1);
				Usart1_Buf_St.USART_BUF[Usart1_Buf_St.Udata_Len]=Usart1_Buf_St.Udata;
				Usart1_Buf_St.Udata_Len++;
		}
		else
		{
		    Usart1_Buf_St.Udata_Len=0;
		}
	  
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
	
}
uint16_t Ad_Cnt=0;

void ADC1_2_IRQHandler(void)
{
	if(ADC_GetITStatus(ADC1,ADC_IT_EOC)!=RESET)
	{
		 Adc_Arr[Ad_Cnt]=ADC_GetConversionValue(ADC1);
		if(Ad_Cnt>=1)
		{
			if(Single_Mode.Fall_Mode==1)
			{
				if((Adc_Arr[Ad_Cnt-1]>500)&&(Adc_Arr[Ad_Cnt]<50))//检测到下降延
				{
					
					Single_Mode.Fall_Finish=1;
					Single_Mode.Count=Ad_Cnt;
				}			
			}
             else if(Single_Mode.Reise_Mode==1)
			{
			    if((Adc_Arr[Ad_Cnt-1]<50)&&(Adc_Arr[Ad_Cnt]>500))//检测到上升延
				{ 
				  Single_Mode.Reise_Finish=1;
				  Single_Mode.Count=Ad_Cnt;
				}
			}		
		}
		  Ad_Cnt++;
		if(Ad_Cnt>=DATA_LEN)
		{
			Wave_Image.Finish_Flag=1;
			Scope_Single_Control(DISABLE);
			Ad_Cnt=0;
		}
		  ADC_ClearITPendingBit(ADC1,ADC_IT_EOC);	
    }
	
}

void DMA1_Channel1_IRQHandler(void)
{
 u8 i;
	
	if(DMA_GetITStatus(DMA1_IT_TC1)!=RESET)
	{
		Wave_Image.Finish_Flag=1;
		DMA_ClearITPendingBit(DMA1_IT_TC1);
	}

}

u8 Cap_Cnt=0;
void TIM2_IRQHandler(void)
{
if(TIM_GetITStatus(TIM2,TIM_IT_CC1)!=RESET)
{

	Wave_Mag.Cycle=TIM2->CCR1;
	Wave_Mag.Pulse_W=TIM2->CCR2;
	Cap_Cnt++;
	Wave_Mag.Frequency=(float)(72000000.0/72.0/TIM2->CCR1);
//		printf("%d\n",Wave_Mag.Pulse_W);
  if(Wave_Mag.Frequency>=1000)
	{	
		if((Wave_Mag.Frequency/1000>20))//高频信号
		{
			TIM3->ARR=5;
			TIM3->PSC=Con_Shape.Hight_Psc; 
			
		}
		else if((Wave_Mag.Frequency/1000>=15)&&(Wave_Mag.Frequency/1000<=20))
		{
			TIM3->ARR=20;
			TIM3->PSC=Con_Shape.Hight_Psc; 
		}//高频信号
		else//中频信号
		{
			TIM3->ARR=(uint32_t)(Con_Shape.Wave_Expand/(Wave_Mag.Frequency/1000.0));
			TIM3->PSC=71; 
		}
	 
	}
	else//低频信号
	{
		 TIM3->ARR=(uint32_t)(Con_Shape.Wave_Expand*(1000.0/Wave_Mag.Frequency));
		 TIM3->PSC=71; 
	}
	
	if(Cap_Cnt>5)//多次捕获频率为了获取到稳定的数据
	{
		 Cap_Cnt=0;
		 Cap_Flag=1;
		 Scope_Shut_Cap(0);//关闭捕获开启ADC采样
	}
		
}
	 TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
}
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
