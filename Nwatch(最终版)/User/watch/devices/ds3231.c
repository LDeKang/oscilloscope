/*
*项目:N |手表
*作者:Zak Kemble, contact@zakkemble.co.uk
*版权:(C) 2013由Zak Kemble
*许可:GNU GPL v3(参见License.txt)
*网站:http://blog.zakkemble.co.uk/diy-digital-wristwatch/
*/

#include "common.h"
#include "devices/ds3231_defs.h"

#if RTC_SRC == RTC_SRC_DS3231

#include "devices/rtc_funcs.h"

static rtc_temperature_s temperature;

void ds3231_init()
{
 
   timeDate_s timeData;
	
	byte data;

	//控制
	//使振荡器
	read(DS3231_ADDR_CTRL, (byte*)&data, 1);
	CLEAR_BITS(data, DS3231_BIT_EOSC, DS3231_BIT_RS1, DS3231_BIT_RS2);
	write(DS3231_ADDR_CTRL, data);

	//状态
	//关闭32KHz输出
	//取消设置告警1和2中断
	read(DS3231_ADDR_STATUS, &data, 1);
	CLEAR_BITS(data, DS3231_BIT_EN32KHZ, DS3231_BIT_A2F, DS3231_BIT_A1F);
	write(DS3231_ADDR_STATUS, data);

//   timeData.time.secs=0;
//	timeData.time.mins=3;
//	timeData.time.hour=22;
//	timeData.date.day=4;	 
//	timeData.date.date=10;	 
//	timeData.date.month=11;
//	timeData.date.year=1974;	 
//	ds3231_save(&timeData);
	
	//设置1Hz输出
//	rtc_sqw(RTC_SQW_ON);
}

//在1Hz输出和告警中断输出之间切换
void ds3231_sqw(rtc_sqw_t enable)
{
	byte data;
	read(DS3231_ADDR_CTRL, &data, 1);
	CLEAR_BITS(data, DS3231_BIT_1HZ, DS3231_BIT_INTCN);
	data |= (enable == RTC_SQW_ON) ? _BV(DS3231_BIT_1HZ) : _BV(DS3231_BIT_INTCN);
	write(DS3231_ADDR_CTRL, data);
}

 
void ds3231_get(timeDate_s* timeData)
{
//初始化为0
//否则，如果RTC没有连接，那么我们将以尝试使用随机数据结束
	byte data[7] = {0,0,0};

	read(DS3231_ADDR_SECS, data, 7);

	timeData->time.secs		= bcd2dec(data[0]);
	timeData->time.mins		= bcd2dec(data[1]);
	timeData->time.hour		= bcd2dec(data[2]);
	timeData->date.day		= data[3]; //不需要转换为DEC，因为它只存储0 - 6
	timeData->date.date		= bcd2dec(data[4]);
	timeData->date.month	= bcd2dec(data[5]);
	timeData->date.year		= bcd2dec(data[6]);

	//月和日从1开始，但我们想从0开始
	if(timeData->date.day > 0)
		timeData->date.day--;

	if(timeData->date.month > 0)
		timeData->date.month--;
 
}

 
void ds3231_save(timeDate_s* timeData)
{
//月在程序中从0开始存储，但RTC从1开始存储
// byte month = timeData->month + 1;
// if(month == 0b00001010) // invalid BCD, added 1 to 9 = 10 = bad . //
//月= 0b00010000;//添加6来修复，但这会增加程序的大小，所以只需手动设置
// TODO:一次写入而不是逐个写入

	write(DS3231_ADDR_SECS,	dec2bcd(timeData->time.secs));
	write(DS3231_ADDR_MINS,	dec2bcd(timeData->time.mins));
	write(DS3231_ADDR_HRS,	dec2bcd(timeData->time.hour));
	write(DS3231_ADDR_DAY,	timeData->date.day + 1); //不需要转换为BCD，因为它只存储0 - 6，在程序中Day也从0开始存储，但RTC从1开始
	write(DS3231_ADDR_DATE,	dec2bcd(timeData->date.date));
	write(DS3231_ADDR_MONTH,	dec2bcd(timeData->date.month + 1));
	write(DS3231_ADDR_YEAR,	dec2bcd(timeData->date.year));
}

// 温度更新
void ds3231_tempUpdate()
{
	read(DS3231_ADDR_TEMPH, &temperature.whole, sizeof(temperature.whole));
	read(DS3231_ADDR_TEMPL, &temperature.frac, sizeof(temperature.frac));
}

 
void ds3231_tempGet(rtc_temperature_s* temp)
{
	memcpy(temp, &temperature, sizeof(rtc_temperature_s));
}

//用户告警使用告警1
//当日、时、分、秒匹配时触发
void ds3231_setUserAlarmWake(alarm_s* alarm)
{
	//  清除标志
	rtc_userAlarmState();

	byte data;
	read(DS3231_ADDR_CTRL, &data, 1);
	if(alarm != NULL)
	{
		write(DS3231_ADDR_ALARM1_SECS,	0);
		write(DS3231_ADDR_ALARM1_MINS,	dec2bcd(alarm->min));
		write(DS3231_ADDR_ALARM1_HRS,	dec2bcd(alarm->hour));
		write(DS3231_ADDR_ALARM1_DAYDATE, 0x40 | alarm->days);
		SET_BITS(data, DS3231_BIT_A1IE);
	}
	else // Alarm disabled
		CLEAR_BITS(data, DS3231_BIT_A1IE);
	write(DS3231_ADDR_CTRL, data);
}

//系统唤醒使用告警2
//注意:告警2没有秒
//当分钟匹配时触发
void ds3231_setSystemAlarmWake(alarm_s* alarm)
{
	// Clear flag
	rtc_systemAlarmState();

	byte data;
	read(DS3231_ADDR_CTRL, &data, 1);
	if(alarm != NULL)
	{
		write(DS3231_ADDR_ALARM2_MINS,dec2bcd(alarm->min));
		write(DS3231_ADDR_ALARM2_HRS,	0x80 | dec2bcd(alarm->hour));
		write(DS3231_ADDR_ALARM2_DAYDATE,0x80 | dec2bcd(alarm->days));
		SET_BITS(data, DS3231_BIT_A2IE);
	}
	else // Alarm disabled
		CLEAR_BITS(data, DS3231_BIT_A2IE);
	write(DS3231_ADDR_CTRL, data);
}

//获取用户告警状态
bool ds3231_userAlarmState()
{
	byte data;
	read(DS3231_ADDR_STATUS, &data, 1);
	bool state = data & _BV(DS3231_BIT_A1F);
	if(state) // Clear state if its set
	{
		CLEAR_BITS(data, DS3231_BIT_A1F);
		write(DS3231_ADDR_STATUS, data);
	}

	return state;
}

//获取系统告警状态
bool ds3231_systemAlarmState()
{
	byte data;
	read(DS3231_ADDR_STATUS, &data, 1);
	bool state = data & _BV(DS3231_BIT_A2F);
	if(state) //如果设置了则清除状态
	{
		CLEAR_BITS(data, DS3231_BIT_A2F);
		write(DS3231_ADDR_STATUS, data);
	}

	return state;
}

#endif
