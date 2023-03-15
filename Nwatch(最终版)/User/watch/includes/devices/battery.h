/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

#ifndef BATTERY_H_
#define BATTERY_H_

#define USB_START   PAin(6)

void battery_init(void);
void battery_setUpdate(byte);
void battery_update(void);
void battery_updateNow(void);
uint battery_voltage(void);
void Power_Pin_Init(void);
void Usb_Pin_Init(void);

#endif /* BATTERY_H_ */
