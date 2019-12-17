/*!\name  		smm_i2c.c
 *
 * \brief     	Defined two I2C drivers: A and B
 *
 * \author    Juan Gago
 *
 */

#include <stdio.h>
#include "clients.h"
#include "i2c_client.h"
#include "smm_i2c.h"

I2C_DRIVER DriverA, DriverB;
uint8_t LastMessageA[8];
uint8_t LastMessageB[8];

void SMM_I2C_Add_Prom (bool contex) {
	I2C_CLIENT client;
	uint8_t rxBuff[2];
	I2CLIENT_Prom_Setup (&client, 0xA0, 0x05);
	if (context)
		I2CLIENT_Prom_Read (&client, rxBuff, 2);
	else
		I2CLIENT_Prom_Write (&client, "12", 2);
	I2C_Add (client, &DriverA);
}

void SMM_I2C_Add_Sensor () {
	I2C_CLIENT client;
	MCP9808_Rd_Word (&client, 0x30, 0x05);
	I2C_Add (client, &DriverA);
}

void SMM_I2C_Add_PSU () {
	I2C_CLIENT client;
	RFE1600_Rd_Word (&client, 0x98);
	I2C_Add (client, &DriverB);
}

void SMM_I2C_Initialize(){
	I2C_Initialize(&DriverA, 0);
	I2C_Initialize(&DriverB, 1);
}

void SMM_I2C_Tasks(uint32_t* tout_a, uint32_t* tout_b)
{
	uint16_t temperature;
	uint8_t hostname[8]

	I2C_CLIENT* ptra = I2C_Tasks(&DriverA, &tout_a, LastMessageA);
	I2C_CLIENT* ptrb = I2C_Tasks(&DriverB, &tout_b, LastMessageB);

	if (ptra->state == I2C_CLIENT_DONE) {
		// this check avoid problem of reentrance
		if (ptra->address == 0x30) {
			temperature = MCP9808_Temp (LastMessageA[1], LastMessageA[0]);
		}
		else if (ptra->address == 0xA0) {
			memcpy(hostname, LastMessageA, 8);
		}
		memset(LastMessageA, 0, 8);
		ptra->state = I2C_CLIENT_NONE;
	}

	if (ptrb->state == I2C_CLIENT_DONE) {
		// this check avoid problem of reentrance
		if (ptrb->address == 0x2F) {
			temperature = RFE1600_Temp (LastMessageB[1], LastMessageB[0]);
		}
		memset(LastMessageB, 0, 8);
		ptrb->state = I2C_CLIENT_NONE;
	}
}
