#ifndef _I2C_CLIENT__H__
#define _I2C_CLIENT__H__

/*!\name      i2c_client.h
 *
 * \brief     Client to communicate with the slave
 *
 * \author    Juan Gago
 *
 */

#include "i2c.h"

bool I2CLIENT_Write_Read 	(I2C_CLIENT*, uint8_t addr, const uint8_t* to_write, uint8_t wlen, const uint8_t* to_read, uint8_t rlen);
bool I2CLIENT_Prom_Setup 	(I2C_CLIENT*, uint8_t addr, uint16_t page);
bool I2CLIENT_Prom_Read  	(I2C_CLIENT*, const uint8_t* to_read, uint8_t rlen);
bool I2CLIENT_Prom_Write 	(I2C_CLIENT*, const uint8_t* to_write, uint8_t wlen);

/* From I2CLIENT_Write_Read() */
bool I2CLIENT_Wr_Byte	(I2C_CLIENT*, uint8_t addr, uint8_t command);
bool I2CLIENT_Rd_Byte	(I2C_CLIENT*, uint8_t addr, uint8_t command, const uint8_t* to_read);
bool I2CLIENT_Rd_Word	(I2C_CLIENT*, uint8_t addr, uint8_t command, const uint8_t* to_read);
//bool I2CLIENT_Wr_Block	(I2C_CLIENT*, uint8_t addr, const uint8_t* to_write, uint8_t wlen);
bool I2CLIENT_Rd_Block	(I2C_CLIENT*, uint8_t addr, uint8_t command, const uint8_t* to_read, uint8_t rlen);

#endif