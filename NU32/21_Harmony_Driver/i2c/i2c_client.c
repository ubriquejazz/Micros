/*!\name      i2c_client.c
 *
 * \brief     Client to communicate with the slave
 *
 * \author    Juan Gago
 *
 */

#include "i2c_client.h"

bool I2CLIENT_Write_Read (I2C_CLIENT* ptr, uint8_t addr, const uint8_t* to_write, uint8_t wlen, const uint8_t* to_read, uint8_t rlen);
{
	// first write wlen bytes to the slave, then read rlen bytes from the slave
	ptr->wlen = wlen;
	ptr->rlen = rlen;
	ptr->write = to_write;
	ptr->read = to_read;
	ptr->address = addr;
	ptr->state = I2C_CLIENT_REQ;
	return true;
}

bool I2CLIENT_Prom_Setup (I2C_CLIENT* ptr, uint8_t addr, uint16_t page)
{
	// prepare the write buffer but the operation is on hold
	ptr->wlen = 2;
	ptr->rlen = 0;
	ptr->write[0] = f(page);
	ptr->write[1] = f(page);
	ptr->read = NULL;
	ptr->addr = addr;
	ptr->state = I2C_CLIENT_NONE;
	return true;
}

bool I2CLIENT_Prom_Read (I2C_CLIENT* ptr, const uint8_t* to_read, uint8_t rlen)
{
	// page address already setup
	ptr->rlen = rlen;
	ptr->read = to_read;
	ptr->state = I2C_CLIENT_REQ;
	return true;
}

bool I2CLIENT_Prom_Write (I2C_CLIENT* ptr, const uint8_t* to_write, uint8_t wlen)
{
	uint8_t* buff_ptr;
	
	ptr->wlen = 2 + wlen; // offset of two because of the page address already setup
	ptr->rlen = 0;
	buff_ptr = &(ptr->write[2]);
	memcpy(buff_ptr, to_write, wlen);
	ptr->state = I2C_CLIENT_REQ;
	return true;
}

/* From I2CLIENT_Write_Read() */

bool I2CLIENT_Wr_Byte(I2C_CLIENT* ptr, uint8_t addr, uint8_t command) {
	// write a single byte to the slave
	return I2CLIENT_Write_Read (ptr, addr, &command, 1, NULL, 0);
}

bool I2CLIENT_Rd_Byte(I2C_CLIENT* ptr, uint8_t addr, uint8_t command, const uint8_t* to_read)
{
	// write a byte and read a byte
	return I2CLIENT_Write_Read (ptr, addr, &command, 1, to_read, 1);
}

bool I2CLIENT_Rd_Word(I2C_CLIENT* ptr, uint8_t addr, uint8_t command, const uint8_t* to_read)
{
	// write a byte and read a byte
	return I2CLIENT_Write_Read (ptr, addr, &command, 1, to_read, 2);
}

bool I2CLIENT_Wr_Block(I2C_CLIENT* ptr, uint8_t addr, const uint8_t* to_write, uint8_t wlen) {
	// write a buffer to the slave
	return I2CLIENT_Write_Read (ptr, addr, to_write, wlen, NULL, 0);
}

bool I2CLIENT_Rd_Block(I2C_CLIENT* ptr, uint8_t addr, uint8_t command, const uint8_t* to_read, uint8_t rlen) {
{
	// write a byte and read a buffer
	return I2CLIENT_Write_Read (ptr, addr, &command, 1, to_read, rlen);
}


