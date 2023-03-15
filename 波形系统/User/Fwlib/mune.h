#ifndef _MUNE__H
#define _MUNE__H

#include "stm32f10x.h"


typedef struct Mune_St
{
  u8 Next_Offest;//对子菜单的偏移
  u8 Prev_Offest;//对最近主菜单的偏移
  u8 Option_Offest;
  u8 Sub_Num;//菜单下的子菜单数量
  u8 Key_Lock;//菜单按键锁，执行某个功能时与控制菜单的按键不冲突
  u8 Inter_Flag;// 
  u8 Back_Flag;// 
  void (*Fun_Ptr)();//菜单执行函数
}MUNE_ST;


extern MUNE_ST Mune_List[20];


#endif