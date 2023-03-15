/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

#ifndef HEART_H_
#define HEART_H_

#define MAX_LEN 500

typedef struct heart_st
{
 u8 Start;//MAX的状态
 u8 Dis_Start;
 uint32_t Ir_Data;
 uint32_t Red_Data;
 uint32_t Cnt;//铺助计数值
 float Data_Buf[MAX_LEN];//数据存储
 float Data_Pix[MAX_LEN];//波峰波谷位置存储
 uint16_t Peak;//波峰数量
}HEART_ST;

extern HEART_ST Heart_Rate;
void Heart_Init(void);
float Heart_Calculate(void);
void Deaw_Heart(void);

void mHeartsOpen(void);

#endif /* SETTINGS_H_ */
