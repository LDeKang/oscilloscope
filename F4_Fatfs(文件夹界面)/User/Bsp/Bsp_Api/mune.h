#ifndef _MUNE__H
#define _MUNE__H

#include "stm32f4xx_hal.h"
#include "sys.h"
#include "stm32f4xx.h"


typedef struct Mune_St
{
  uint8_t Next_Offest;//���Ӳ˵���ƫ��
  uint8_t Prev_Offest;//��������˵���ƫ��
  uint8_t Option_Offest;
  uint8_t Sub_Num;//�˵��µ��Ӳ˵�����
  uint8_t Key_Lock;//�˵���������ִ��ĳ������ʱ����Ʋ˵��İ�������ͻ
  uint8_t Inter_Flag;// 
  uint8_t Back_Flag;// 
  void (*Fun_Ptr)();//�˵�ִ�к���
}MUNE_ST;


extern MUNE_ST Mune_List[20];


#endif