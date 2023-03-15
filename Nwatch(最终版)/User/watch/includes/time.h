/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

#ifndef TIME_H_
#define TIME_H_

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

typedef enum
{
	RTCWAKE_NONE,
	RTCWAKE_SYSTEM,
	RTCWAKE_USER,
	RTCWAKE_USER_SYSTEM
} rtcwake_t;

extern timeDate_s timeDate;

void time_init(void);
void time_sleep(void);
void time_shutdown(void);
rtcwake_t time_wake(void);
void time_set(timeDate_s*);
bool time_isLeapYear(byte);
day_t time_dow(byte, month_t, byte);
byte time_monthDayCount(month_t, byte);
void time_update(void);
char* time_timeStr(void);
void time_timeMode(time_s*, timemode_t);
//timestamp_t time_getTimestamp(void);
//timestamp_t time_timeDate2TimeStamp(time_s*);

#endif /* TIME_H_ */
