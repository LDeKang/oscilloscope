/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

#ifndef I2C_H_
#define I2C_H_
 


#define I2C_NACK	0
#define I2C_ACK   1
#define I2C_W		0
#define I2C_R		1

#define read(a,b,c) DS3231_Read_Data(a,b,c)
#define write(a,b) DS3231_Write_Data(a,b)

void I2c_Gpio_Init(void);
uint8_t DS3231_Read_Data(uint8_t Address, uint8_t* Data,uint8_t Count);
uint8_t DS3231_Write_Data(uint8_t Address,uint8_t Data);
uint8_t i2c_CheckDevice(uint8_t _Address);
u8 DS3231_RD_Byte(u8 addr);
void DS3231_WR_Byte(u8 addr,u8 bytedata);
 

#endif /* I2C_H_ */
