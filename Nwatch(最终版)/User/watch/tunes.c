/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2014 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

#include "common.h"
#define TUNEMEM 
// Remove const when TUNEMEM is EEPMEM?

const tune_t tuneUsbPlugin[] TUNEMEM = {
	TONE_2KHZ,
	TONE_PAUSE,
	TONE_2_5KHZ,
	TONE_PAUSE,
	TONE_3KHZ,
	TONE_STOP
};

const tune_t tuneUsbUnplug[] TUNEMEM = {
	TONE_3KHZ,
	TONE_PAUSE,
	TONE_2_5KHZ,
	TONE_PAUSE,
	TONE_2KHZ,
	TONE_STOP
};

const tune_t tuneUsbCharged[] TUNEMEM = {
	TONE_2KHZ,
	TONE_PAUSE,
	TONE_2KHZ,
	TONE_PAUSE,
	TONE_4KHZ,
	TONE_4KHZ,
	TONE_STOP
};

const tune_t tuneHour[] TUNEMEM = {
	TONE_2_5KHZ,
	TONE_PAUSE,
	TONE_2_5KHZ,
	TONE_STOP
};

const tune_t tuneAlarm[] TUNEMEM = {
	TONE_2KHZ,
	TONE_PAUSE,
	TONE_2KHZ,
	TONE_PAUSE,
	TONE_3KHZ,
	TONE_PAUSE,
	TONE_3KHZ,
	TONE_PAUSE,
	TONE_REPEAT
};

const tune_t tuneBtn1[] TUNEMEM = {
	TONE_2KHZ,
	TONE_PAUSE,
	TONE_3KHZ,
	TONE_STOP
};

const tune_t tuneBtn2[] TUNEMEM = {
	TONE_3KHZ,
	TONE_PAUSE,
	TONE_3KHZ,
	TONE_STOP
};

const tune_t tuneBtn3[] TUNEMEM = {
	TONE_3KHZ,
	TONE_PAUSE,
	TONE_2KHZ,
	TONE_STOP
};
