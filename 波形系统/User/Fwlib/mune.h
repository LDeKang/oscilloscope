#ifndef _MUNE__H
#define _MUNE__H

#include "stm32f10x.h"


typedef struct Mune_St
{
  u8 Next_Offest;//���Ӳ˵���ƫ��
  u8 Prev_Offest;//��������˵���ƫ��
  u8 Option_Offest;
  u8 Sub_Num;//�˵��µ��Ӳ˵�����
  u8 Key_Lock;//�˵���������ִ��ĳ������ʱ����Ʋ˵��İ�������ͻ
  u8 Inter_Flag;// 
  u8 Back_Flag;// 
  void (*Fun_Ptr)();//�˵�ִ�к���
}MUNE_ST;


extern MUNE_ST Mune_List[20];


#endif