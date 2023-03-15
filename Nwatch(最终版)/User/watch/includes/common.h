/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

#ifndef COMMON_H_
#define COMMON_H_

//#include "arm_math.h"
//#include "arm_const_structs.h"
#include "util.h"
#include "tim2.h"
#include "config.h"
#include "typedefs.h"
#include "stm32f10x.h"
#include "debug.h"
#include "systick.h"
#include "oled.h"
#include "sys.h"
#include "wq_flash.h"
#include "MAX1302.h"
#include "DEV_IIC.h"
#include "MPU6050.h"
#include "usart.h"
#include "BMP280.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "drivers/spi.h"
#include "drivers/rtc.h"
#include "drivers/i2c.h"
#include "devices/buttons.h"
#include "devices/battery.h"
#include "devices/buzzer.h"
#include "devices/led.h"

#include "devices/ds3231.h"
#include "menus/alarms.h"
#include "menus/diag.h"
#include "menus/m_display.h"
#include "menus/games.h"
#include "menus/timedate.h"
#include "menus/settings.h"
#include "menus/heart.h"
#include "menus/letter.h"
#include "menus/pressure.h"
#include "menus/gyroscope.h"
#include "menus/sleep.h"
#include "menus/sound.h"
#include "menus/functions.h"
#include "menus/m_main.h"
#include "games/game1.h"
#include "games/game2.h"
#include "apps/stopwatch.h"
#include "apps/torch.h"
#include "apps/screenshot.h"
#include "watchfaces/normal.h"
#include "watchfaces/lowbatt.h"
#include "watchfaces/ui.h"

#include "system.h"
#include "global.h"
#include "display.h"
#include "time.h"
#include "alarm.h"
#include "pwrmgr.h"
#include "appconfig.h"
#include "disco.h"
#include "tune.h"
#include "animation.h"
#include "draw.h"
#include "menu.h"

#include "lang.h"
#include "tunes.h"
#include "discos.h"
#include "resources.h"

#endif /* COMMON_H_ */
