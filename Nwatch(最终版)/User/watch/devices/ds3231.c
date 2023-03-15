/*
*��Ŀ:N |�ֱ�
*����:Zak Kemble, contact@zakkemble.co.uk
*��Ȩ:(C) 2013��Zak Kemble
*���:GNU GPL v3(�μ�License.txt)
*��վ:http://blog.zakkemble.co.uk/diy-digital-wristwatch/
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

	//����
	//ʹ����
	read(DS3231_ADDR_CTRL, (byte*)&data, 1);
	CLEAR_BITS(data, DS3231_BIT_EOSC, DS3231_BIT_RS1, DS3231_BIT_RS2);
	write(DS3231_ADDR_CTRL, data);

	//״̬
	//�ر�32KHz���
	//ȡ�����ø澯1��2�ж�
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
	
	//����1Hz���
//	rtc_sqw(RTC_SQW_ON);
}

//��1Hz����͸澯�ж����֮���л�
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
//��ʼ��Ϊ0
//�������RTCû�����ӣ���ô���ǽ��Գ���ʹ��������ݽ���
	byte data[7] = {0,0,0};

	read(DS3231_ADDR_SECS, data, 7);

	timeData->time.secs		= bcd2dec(data[0]);
	timeData->time.mins		= bcd2dec(data[1]);
	timeData->time.hour		= bcd2dec(data[2]);
	timeData->date.day		= data[3]; //����Ҫת��ΪDEC����Ϊ��ֻ�洢0 - 6
	timeData->date.date		= bcd2dec(data[4]);
	timeData->date.month	= bcd2dec(data[5]);
	timeData->date.year		= bcd2dec(data[6]);

	//�º��մ�1��ʼ�����������0��ʼ
	if(timeData->date.day > 0)
		timeData->date.day--;

	if(timeData->date.month > 0)
		timeData->date.month--;
 
}

 
void ds3231_save(timeDate_s* timeData)
{
//���ڳ����д�0��ʼ�洢����RTC��1��ʼ�洢
// byte month = timeData->month + 1;
// if(month == 0b00001010) // invalid BCD, added 1 to 9 = 10 = bad . //
//��= 0b00010000;//���6���޸�����������ӳ���Ĵ�С������ֻ���ֶ�����
// TODO:һ��д����������д��

	write(DS3231_ADDR_SECS,	dec2bcd(timeData->time.secs));
	write(DS3231_ADDR_MINS,	dec2bcd(timeData->time.mins));
	write(DS3231_ADDR_HRS,	dec2bcd(timeData->time.hour));
	write(DS3231_ADDR_DAY,	timeData->date.day + 1); //����Ҫת��ΪBCD����Ϊ��ֻ�洢0 - 6���ڳ�����DayҲ��0��ʼ�洢����RTC��1��ʼ
	write(DS3231_ADDR_DATE,	dec2bcd(timeData->date.date));
	write(DS3231_ADDR_MONTH,	dec2bcd(timeData->date.month + 1));
	write(DS3231_ADDR_YEAR,	dec2bcd(timeData->date.year));
}

// �¶ȸ���
void ds3231_tempUpdate()
{
	read(DS3231_ADDR_TEMPH, &temperature.whole, sizeof(temperature.whole));
	read(DS3231_ADDR_TEMPL, &temperature.frac, sizeof(temperature.frac));
}

 
void ds3231_tempGet(rtc_temperature_s* temp)
{
	memcpy(temp, &temperature, sizeof(rtc_temperature_s));
}

//�û��澯ʹ�ø澯1
//���ա�ʱ���֡���ƥ��ʱ����
void ds3231_setUserAlarmWake(alarm_s* alarm)
{
	//  �����־
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

//ϵͳ����ʹ�ø澯2
//ע��:�澯2û����
//������ƥ��ʱ����
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

//��ȡ�û��澯״̬
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

//��ȡϵͳ�澯״̬
bool ds3231_systemAlarmState()
{
	byte data;
	read(DS3231_ADDR_STATUS, &data, 1);
	bool state = data & _BV(DS3231_BIT_A2F);
	if(state) //��������������״̬
	{
		CLEAR_BITS(data, DS3231_BIT_A2F);
		write(DS3231_ADDR_STATUS, data);
	}

	return state;
}

#endif
