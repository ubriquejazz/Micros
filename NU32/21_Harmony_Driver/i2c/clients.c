
/*!\name      clients.c
 *
 * \brief     	How to schedule a read operation?
 *
 *				I2C_CLIENT sensor = {...};
 *				MCP9808_Read(&sensor, 0x30);
 *				I2C_Add(sensor, &i2cBB);
 *
 *				Reading Tasks:
 *
 *				I2C_CLIENT* ptr = I2C_Tasks(&i2cBB, &tout);
 *				if (ptr->state == I2C_CLIENT_DONE)
 *				{
 *					switch (ptr->address)
 *					{
 *						case 0x30:
 *							uint8_t  low = ptr->read[0];
 *							uint8_t high = ptr->read[1];
 *							temperature = MCP9808_Read(low, high);
 *							break;
 *						...
 *
 * \author    Juan Gago
 *
 */

#include "clients.h"
#include "i2c_client.h"

/* MCP9808 */

bool MCP9808_Write (I2C_CLIENT* ptr, MCP9808_ADDR, MCP9808_REG, uint16_t)
{
	return true;
}

bool MCP9808_Read (I2C_CLIENT* ptr, MCP9808_ADDR, MCP9808_REG)
{
	return true;
}

/* RFE1600 */

bool RFE1600_Read (I2C_CLIENT* ptr, RFE1600_ADDR, RFE1600_REG)
{
	return true;
}



