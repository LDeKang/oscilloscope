#ifndef __SCDPE_API_H
#define __SCDPE_API_H	
#include "stm32f10x.h"

typedef struct scope_mode
{
 u8 Single_Mode_Flag;//���δ���ģʽ
 u8 Pwm_Mode_Flag;//���β���ģʽ
}SCOPE_MODE;

//������Ϣģ��
typedef struct wave
{
	float Frequency;//Ƶ��
	uint16_t Cycle;//����
	uint16_t Pulse_W;//ռ�ձ�
	float Max_Dcvalue;//����
	float Min_Dcvalue;//�ͷ�
	
}WAVEFROM;

//���β�������ģ��
typedef struct contrl_wave
{
	uint16_t Wave_Expand;////���ο�ȱ�������
	uint32_t Hight_Psc;//��Ƶ��Ƶ����
	uint32_t Hight_Range;//�����ʾ������Χ
	uint32_t Low_Range;//��С��ʾ������Χ
	uint32_t To_Expand;//������ʾ��ȿ���
	
}CONTRL_WAVE;

//����ͼ��ģ��
typedef struct wave_image
{
	uint16_t usY;
	uint16_t usX;

	uint16_t Prve_usY;
	uint16_t Prve_usX;

	uint16_t Max_Dc;//��ѹ����ֵ
	uint16_t Min_Dc;

	u8 Finish_Flag;
	
}WAVE_IMAGE;

typedef struct Single_Mode//���δ���ģʽ
{
	uint16_t Fall_Mode;//�½���
	uint16_t Reise_Mode;//������
	
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
