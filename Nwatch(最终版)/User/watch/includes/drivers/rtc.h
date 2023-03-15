/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2014 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

#ifndef RTC_H_
#define RTC_H_

typedef enum
{
	RTC_SQW_OFF = 0,
	RTC_SQW_ON = 1
} rtc_sqw_t;

typedef struct
{
	signed char whole;
	byte frac;
}rtc_temperature_s;


#include "devices/ds3231.h"

#define RTCPREFIX(prefix, func)	CONCAT(prefix, func)
#define RTCFUNC(func)			RTCPREFIX(RTCFUNC_PREFIX, func)

#define rtc_init				(RTCFUNC(init))
#define rtc_sqw					(RTCFUNC(sqw))
#define rtc_get					(RTCFUNC(get))
#define rtc_save				(RTCFUNC(save))
#define rtc_tempUpdate			(RTCFUNC(tempUpdate))
#define rtc_tempGet				(RTCFUNC(tempGet))
#define rtc_setUserAlarmWake	(RTCFUNC(setUserAlarmWake))
#define rtc_setSystemAlarmWake	(RTCFUNC(setSystemAlarmWake))
#define rtc_userAlarmState		(RTCFUNC(userAlarmState))
#define rtc_systemAlarmState	(RTCFUNC(systemAlarmState))


#endif /* RTC_H_ */
