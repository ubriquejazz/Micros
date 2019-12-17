/*!\name      clients.c
 *
 * \brief     Possible I2C clients, MCP9808_{},  RFE1600_{}, etc.
 *
 *
 * \author    Juan Gago
 *
 */

#include <stdio.h>
#include "clients.h"
#include "i2c_client.h"

uint8_t sensor_rxBuff[3];
uint8_t sensor_txBuff[3];

uint8_t module_rxBuff[3];
uint8_t module_txBuff[3];

bool MCP9808_Wr_Word (I2C_CLIENT*, MCP9808_ADDR addr, MCP9808_REG sensorReg, uint16_t sensorValue) 
{
	uint8_t buffer[3];
	sprintf(buffer, "%02x %04x", sensorReg, sensorValue);
	buffer[0] = (uint8_t) sensorReg;
	buffer[1] = 0x00FF & (sensorValue << 8);
	buffer[2] = sensorValue & 0x0FF;
	return I2CLIENT_Wr_Block (ptr, addr, buffer, 3);
}

bool MCP9808_Rd_Word (I2C_CLIENT*, MCP9808_ADDR addr, MCP9808_REG sensorReg) {
	uint8_t buffer[1];
	buffer[0] = (uint8_t) sensorReg;
	return I2CLIENT_Rd_Word (ptr, addr, buffer, sensor_rxBuff);
}

bool RFE1600_Rd_Word (I2C_CLIENT*, RFE1600_ADDR, RFE1600_REG moduleReg) {
	uint8_t buffer[1];
	buffer[0] = (uint8_t) moduleReg;
	return I2CLIENT_Rd_Word (ptr, addr, buffer, module_rxBuff);
}
