#ifndef _I2CLIENT_COMMON__H__
#define _I2CLIENT_COMMON__H__

/*!\name      i2client_common.h
 *
 * \brief     Client to communicate with the slave
 *
 * \author    Juan Gago
 *
 */

#include "i2c_common.h"

bool I2CLIENT_Write_Read (I2C_CLIENT*, uint8_t addr, const uint8_t* to_write, uint8_t wlen, const uint8_t* to_read, uint8_t rlen);
bool I2CLIENT_Prom_Setup (I2C_CLIENT*, uint8_t addr, uint16_t page);
bool I2CLIENT_Prom_Read  (I2C_CLIENT*, const uint8_t* to_read, uint8_t rlen);
bool I2CLIENT_Prom_Write (I2C_CLIENT*, const uint8_t* to_write, uint8_t wlen);

/* rlen = 0 at I2CLIENT_Write_Read() */
bool I2CLIENT_Wr_Byte	(I2C_CLIENT*, uint8_t, const uint8_t*); // wlen = 1
bool I2CLIENT_Wr_Block	(I2C_CLIENT*, uint8_t, const uint8_t*, uint8_t wlen);
/* wlen = 1 and rlen > 0 */
bool I2CLIENT_Rd_Byte	(I2C_CLIENT*, uint8_t, const uint8_t*, const uint8_t*); // rlen = 1
bool I2CLIENT_Rd_Word	(I2C_CLIENT*, uint8_t, const uint8_t*, const uint8_t*); // rlen = 2
bool I2CLIENT_Rd_Block	(I2C_CLIENT*, uint8_t, const uint8_t*, const uint8_t*, uint8_t rlen);

#endif