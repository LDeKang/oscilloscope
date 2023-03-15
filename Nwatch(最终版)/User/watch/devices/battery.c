/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

// Battery
// Battery is connected to a P-MOSFET then to a voltage divider. This is so we don't have a load of current being wasted through the voltage divider all the time.
// Polling based, interrupts not needed here since a conversion only takes ~500us and is only done every-so-often

#include "common.h"

static uint voltage;
static byte lastSecs;
static byte changeCount;

void Power_Pin_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;//	 PB12 POWER控制端口
 	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//速度
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_12);	 
}

void Usb_Pin_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//	 PB12 POWER控制端口
 	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IN_FLOATING; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//速度
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
 
}


static void Adc_Delay(void)
{
	uint8_t i;

	/*　
	 	下面的时间是通过逻辑分析仪测试得到的。
    工作条件：CPU主频72MHz ，MDK编译环境，1级优化
  
		循环次数为10时，SCL频率 = 205KHz 
		循环次数为7时，SCL频率 = 347KHz， SCL高电平时间1.5us，SCL低电平时间2.87us 
	 	循环次数为5时，SCL频率 = 421KHz， SCL高电平时间1.25us，SCL低电平时间2.375us 
	*/
	for (i = 0; i < 50; i++);
}
 static void Adc_In_Gpio_Config(void)
{
	
  GPIO_InitTypeDef Adc_Pin_Structure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	Adc_Pin_Structure.GPIO_Pin=GPIO_Pin_0;
	Adc_Pin_Structure.GPIO_Mode=GPIO_Mode_AIN;
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

 

 void Adc_Init_Config(void)
{
	
  ADC_InitTypeDef Adc_Init_Structure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	Adc_In_Gpio_Config();
	
	Adc_Init_Structure.ADC_Mode=ADC_Mode_Independent;//独立
	Adc_Init_Structure.ADC_ContinuousConvMode=ENABLE;//不使用ADC连续触发，不然第一次触发以后会连续触发！
	Adc_Init_Structure.ADC_DataAlign=ADC_DataAlign_Right;//右
	Adc_Init_Structure.ADC_ScanConvMode=DISABLE;//单通道
	Adc_Init_Structure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;//普通软件触发
	Adc_Init_Structure.ADC_NbrOfChannel=ADC_Channel_1;//通道数量
 
	
	ADC_Init(ADC1,&Adc_Init_Structure);
//	Adc_Nvic_Config();//配置ADC中断		

//	ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);//EOC中断

	ADC_Cmd(ADC1,ENABLE);	

	RCC_ADCCLKConfig(RCC_PCLK2_Div8);//ADC时钟设置
	//最大28us 最小0.5us   
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_239Cycles5);//选择通道，通道优先级，采样时间  12MHZ,26个周期,26/12=2.1us采样一次
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	// ADC开始校准
	ADC_StartCalibration(ADC1);
	// 等待校准完成
	while(ADC_GetCalibrationStatus(ADC1));

 
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


void battery_init()
{
  Adc_Init_Config();
}

// Set next update to happen in a x seconds
void battery_setUpdate(byte secs)
{
	changeCount = secs;
	lastSecs = timeDate.time.secs;
}

void battery_update()
{
	// See if seconds has changed
	if(lastSecs == timeDate.time.secs)
		return;
	lastSecs = timeDate.time.secs;

	if(changeCount)
	{
		changeCount--;
		return;
	}

	// Next update in 5 seconds
	battery_setUpdate(5);

	battery_updateNow();
}

// Update voltage
void battery_updateNow()
{
	  uint16_t Adc_Value=ADC_GetConversionValue(ADC1);
	  Adc_Delay();
     voltage = (Adc_Value*1000/4096)*3.3; 
	  
}

// Get voltage
uint battery_voltage()
{
	return voltage;
}
