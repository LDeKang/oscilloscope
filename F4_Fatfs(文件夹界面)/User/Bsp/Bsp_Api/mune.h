#ifndef _MUNE__H
#define _MUNE__H

#include "stm32f4xx_hal.h"
#include "sys.h"
#include "stm32f4xx.h"


typedef struct Mune_St
{
  uint8_t Next_Offest;//对子菜单的偏移
  uint8_t Prev_Offest;//对最近主菜单的偏移
  uint8_t Option_Offest;
  uint8_t Sub_Num;//菜单下的子菜单数量
  uint8_t Key_Lock;//菜单按键锁，执行某个功能时与控制菜单的按键不冲突
  uint8_t Inter_Flag;// 
  uint8_t Back_Flag;// 
  void (*Fun_Ptr)();//菜单执行函数
}MUNE_ST;


extern MUNE_ST Mune_List[20];


#endif