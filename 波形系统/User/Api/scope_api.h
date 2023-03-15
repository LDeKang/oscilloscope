#ifndef __SCDPE_API_H
#define __SCDPE_API_H	
#include "stm32f10x.h"

typedef struct scope_mode
{
 u8 Single_Mode_Flag;//单次触发模式
 u8 Pwm_Mode_Flag;//波形捕获模式
}SCOPE_MODE;

//波形信息模块
typedef struct wave
{
	float Frequency;//频率
	uint16_t Cycle;//周期
	uint16_t Pulse_W;//占空比
	float Max_Dcvalue;//顶峰
	float Min_Dcvalue;//低缝
	
}WAVEFROM;

//波形参数控制模块
typedef struct contrl_wave
{
	uint16_t Wave_Expand;////波形宽度比例控制
	uint32_t Hight_Psc;//高频分频设置
	uint32_t Hight_Range;//最大显示数量范围
	uint32_t Low_Range;//最小显示数量范围
	uint32_t To_Expand;//波形显示宽度控制
	
}CONTRL_WAVE;

//波形图像模块
typedef struct wave_image
{
	uint16_t usY;
	uint16_t usX;

	uint16_t Prve_usY;
	uint16_t Prve_usX;

	uint16_t Max_Dc;//电压顶峰值
	uint16_t Min_Dc;

	u8 Finish_Flag;
	
}WAVE_IMAGE;

typedef struct Single_Mode//单次触发模式
{
	uint16_t Fall_Mode;//下降延
	uint16_t Reise_Mode;//上升沿
	
	uint16_t Fall_Finish;
	uint16_t Reise_Finish;
	uint16_t Count;
}SINGLE_MODE;


extern WAVEFROM Wave_Mag;
extern CONTRL_WAVE Con_Shape;
extern WAVE_IMAGE Wave_Image;
extern SINGLE_MODE Single_Mode;
extern SCOPE_MODE Scope_Mode;

void Scope_Init(void);
void Scope_Shut(void);

void Scope_Wave_Refresh(void);
void Scope_Win_Refresh(void);
void Scope_Data_Dispaly(FunctionalState NewState);
void Scope_Shut_Cap(FunctionalState NewState);
void Scope_Open_Process(FunctionalState NewState);

void Scope_Single_Operate(void);
void Scope_Single_Init(void);
void Scope_Single_Control(FunctionalState NewState);

#endif
