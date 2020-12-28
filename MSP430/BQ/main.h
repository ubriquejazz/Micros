/*
 * bq769x2_Ctrl_FR2355.h
 *
 * bq769x2 CRC code example header file
 *
 * Copyright (C) 2020 Texas Instruments Incorporated - http://www.ti.com/ 
 *
*/


#ifndef APP_H__
#define APP__H__

#define I2C_ADDR 			0x10
#define SLAVE_ADDR 		 	0x08  	// 0x10 including R/W bit or 0x8 as 7-bit address
#define CRC_Mode 			0  		// 0 for disabled, 1 for enabled
#define CRC_KEY 			7		//
#define MAX_BUFFER_SIZE     20		//

unsigned char CRC8 (unsigned char *ptr, unsigned char len,unsigned char key);

int I2CWriteRegisterByteWithCRC(unsigned char I2CSlaveAddress, unsigned char Register, unsigned char Data);

int I2CReadRegisterWordWithCRC(unsigned char I2CSlaveAddress, unsigned char Register, unsigned int *Data);

#endif