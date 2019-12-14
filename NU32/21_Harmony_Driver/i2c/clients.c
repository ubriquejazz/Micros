/*!\name      clients.c
 *
 * \brief     	Local clients, MCP9808_{},  RFE1600_{}, etc.
 *
 *				1) How to schedule a read operation?
 *
 *					I2C_CLIENT sensor = {...};
 *					uint8_t buffer[2]; // global
 *					MCP9808_Rd_Word (&sensor, 0x30, 0x05, buffer);
 *					I2C_Add(sensor, &i2cBB);
 *
 *				2) Reading Tasks:
 *
 *					I2C_CLIENT* ptr = I2C_Tasks(&i2cBB, &tout);
 *					if (ptr->state == I2C_CLIENT_DONE)
 *					{
 *						switch (ptr->address)
 *						{
 *							case 0x30:
 *								uint8_t  low = ptr->read[0]; // global buffer
 *								uint8_t high = ptr->read[1]; // global buffer
 *								temperature = MCP9808_Temp (low, high);
 *								break;
 *							...
 *
 * \author    Juan Gago
 *
 */

#include <stdio.h>
#include "clients.h"
#include "i2c_client.h"

/* MCP9808 */

bool MCP9808_Wr_Word (I2C_CLIENT*, MCP9808_ADDR addr, MCP9808_REG sensorReg, uint16_t sensorValue) 
{
	uint8_t buffer[3];
	sprintf(buffer, "%02x %04x", sensorReg, sensorValue);
	return I2CLIENT_Wr_Block(ptr, addr, buffer, 3);
}

/* RFE1600 */

bool MCP9808_Rd_Word (I2C_CLIENT*, MCP9808_ADDR addr, MCP9808_REG sensorReg, const uint8_t* to_read) {
	return I2CLIENT_Rd_Word(ptr, addr, sensorReg, to_read);
}

bool RFE1600_Rd_Word (I2C_CLIENT* ptr, RFE1600_ADDR addr, RFE1600_REG moduleReg, const uint8_t* to_read) {
	return I2CLIENT_Rd_Word(ptr, addr, moduleReg, to_read);
}


