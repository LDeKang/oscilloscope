#include "adc.h"
#include "scope_api.h"

uint16_t Adc_Arr[DATA_LEN]={0};
uint16_t Adc_Value;

 static void Adc_In_Gpio_Config(void)
{
	
  GPIO_InitTypeDef Adc_Pin_Structure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	Adc_Pin_Structure.GPIO_Pin=GPIO_Pin_0;
	Adc_Pin_Structure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&Adc_Pin_Structure);
}

 static void Adc_Nvic_Config(void)
{
	
  NVIC_InitTypeDef Adc_Nvic_Structure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	Adc_Nvic_Structure.NVIC_IRQChannel=ADC1_2_IRQn;
	Adc_Nvic_Structure.NVIC_IRQChannelPreemptionPriority=1;
	Adc_Nvic_Structure.NVIC_IRQChannelSubPriority=0;
	Adc_Nvic_Structure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&Adc_Nvic_Structure);

}

 static void Dma_Nvic_Config(void)
{
	
  NVIC_InitTypeDef Dma_Nvic_Structure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	Dma_Nvic_Structure.NVIC_IRQChannel=DMA1_Channel1_IRQn;
	Dma_Nvic_Structure.NVIC_IRQChannelPreemptionPriority=1;
	Dma_Nvic_Structure.NVIC_IRQChannelSubPriority=0;
	Dma_Nvic_Structure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&Dma_Nvic_Structure);

}


 void Adc_Init_Config(void)
{
	
  ADC_InitTypeDef Adc_Init_Structure;
  DMA_InitTypeDef Dma_Init_Structure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	Adc_In_Gpio_Config();
	
	Adc_Init_Structure.ADC_Mode=ADC_Mode_Independent;//独立
	Adc_Init_Structure.ADC_ContinuousConvMode=DISABLE;//不使用ADC连续触发，不然第一次触发以后会连续触发！
	Adc_Init_Structure.ADC_DataAlign=ADC_DataAlign_Right;//右
	Adc_Init_Structure.ADC_ScanConvMode=DISABLE;//单通道
	Adc_Init_Structure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_T3_TRGO;//用外部
	Adc_Init_Structure.ADC_NbrOfChannel=ADC_Channel_1;//通道数量
//	
	Dma_Init_Structure.DMA_PeripheralBaseAddr=(uint32_t)(&(ADC1->DR));//外设地址是ADC1_DR寄存器地址;
	Dma_Init_Structure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;//双字节
	Dma_Init_Structure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;//ADC不自增
	Dma_Init_Structure.DMA_MemoryBaseAddr=(uint32_t)(&Adc_Arr[0]);
	Dma_Init_Structure.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;//双字节
	Dma_Init_Structure.DMA_MemoryInc=DMA_MemoryInc_Enable;//目标地址自增
	Dma_Init_Structure.DMA_BufferSize=DATA_LEN;
	Dma_Init_Structure.DMA_Mode=DMA_Mode_Circular;//循环存储
	Dma_Init_Structure.DMA_DIR=DMA_DIR_PeripheralSRC;//方向 从外设到内存
	Dma_Init_Structure.DMA_M2M=DMA_M2M_Disable;
	Dma_Init_Structure.DMA_Priority=DMA_Priority_High;
	
	DMA_Init(DMA1_Channel1,&Dma_Init_Structure);
	DMA_Cmd(DMA1_Channel1,ENABLE);
	Dma_Nvic_Config();
	DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE);//开启DMA中断
	
	ADC_Init(ADC1,&Adc_Init_Structure);
	 Adc_Nvic_Config();//配置ADC中断		
	 
	if(Scope_Mode.Single_Mode_Flag==1)
	{
	  ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);//EOC中断
		ADC_DMACmd(ADC1,DISABLE);
	}
	else if(Scope_Mode.Pwm_Mode_Flag==1)
	{
	  ADC_ITConfig(ADC1,ADC_IT_EOC,DISABLE);//EOC中断
		ADC_DMACmd(ADC1,ENABLE);
	}
	
	ADC_Cmd(ADC1,ENABLE);	

	RCC_ADCCLKConfig(RCC_PCLK2_Div2);//ADC时钟设置
	//最大28us 最小0.5us   
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_1Cycles5);//选择通道，通道优先级，采样时间  12MHZ,26个周期,26/12=2.1us采样一次
	
	ADC_ExternalTrigConvCmd(ADC1,ENABLE);//使用外部触发转换
	// ADC开始校准
	ADC_StartCalibration(ADC1);
	// 等待校准完成
	while(ADC_GetCalibrationStatus(ADC1));
	
//	ADC_SoftwareStartConvCmd(ADC1,ENABLE);//启动ADC转换
//  Control_Adc_Dma(0);//关闭ADC/DMA
}


void Control_Adc_Dma(FunctionalState NewState)
{
  if(NewState)
	{
		DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE);//开启DMA中断
		DMA_Cmd(DMA1_Channel1,ENABLE);
		ADC_DMACmd(ADC1,ENABLE);
	}
	else
	{
		DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,DISABLE);//DMA中断
		DMA_Cmd(DMA1_Channel1,DISABLE);
		ADC_DMACmd(ADC1,DISABLE);
	}
}


void Adc_Continu_State(FunctionalState NewState)
{
		ADC_InitTypeDef Adc_Init_Structure;  
	if(NewState)
	{
		Adc_Init_Structure.ADC_Mode=ADC_Mode_Independent;//独立
		Adc_Init_Structure.ADC_ContinuousConvMode=ENABLE;// 
		Adc_Init_Structure.ADC_DataAlign=ADC_DataAlign_Right;//右
		Adc_Init_Structure.ADC_ScanConvMode=DISABLE;//单通道
		Adc_Init_Structure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_T3_TRGO;//用外部
		Adc_Init_Structure.ADC_NbrOfChannel=ADC_Channel_1;//通道数量
		ADC_Init(ADC1,&Adc_Init_Structure);
		
  // ADC_SoftwareStartConvCmd(ADC1,ENABLE);//启动ADC转换
	}
	else
	{
		Adc_Init_Structure.ADC_Mode=ADC_Mode_Independent;//独立
		Adc_Init_Structure.ADC_ContinuousConvMode=DISABLE;// 
		Adc_Init_Structure.ADC_DataAlign=ADC_DataAlign_Right;//右
		Adc_Init_Structure.ADC_ScanConvMode=DISABLE;//单通道
		Adc_Init_Structure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_T3_TRGO;//用外部
		Adc_Init_Structure.ADC_NbrOfChannel=ADC_Channel_1;//通道数量
		ADC_Init(ADC1,&Adc_Init_Structure);
		
    //ADC_SoftwareStartConvCmd(ADC1,ENABLE);//启动ADC转换
	}
 

}

void Control_Adc(FunctionalState NewState)
{
  if(NewState)
	{
			ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);//EOC中断
			ADC_Cmd(ADC1,ENABLE);			
	}
	else
	{
			ADC_ITConfig(ADC1,ADC_IT_EOC,DISABLE);//EOC中断
			ADC_Cmd(ADC1,DISABLE);			
	}
}


