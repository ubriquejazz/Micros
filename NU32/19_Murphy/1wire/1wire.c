/*!\name      1wire.h
 *
 * \brief     1W functions
 *
 * \author    Juan Gago
 *
 */

#include "1wire.h"
#include "led_common.h" // scales

PIN_DEF PinOne;

void DelayMs (uint32_t delay)
{
	uint32_t startCnt = _CP0_GET_COUNT();
	uint32_t waitCnt = delay * ms_SCALE;
	while (_CP0_GET_COUNT() - startCnt > waitCnt);
}

void DelayUs (uint32_t delay)
{
	uint32_t startCnt = _CP0_GET_COUNT();
	uint32_t waitCnt = delay * us_SCALE;
	while (_CP0_GET_COUNT() - startCnt > waitCnt);
}

//! Place a reset pulse on the bus
/*! Reset all devices on the bus by sending a long low.
	 over the 1-wire bus followed by a presence pulse
*/ 
unsigned char OW_reset_pulse(void)
{
	unsigned char presence_detect;
	DelayMs(1);
	return presence_detect;
}	

//! Send a single bit to the 1-wire bus
void OW_write_bit (unsigned char write_bit)
{
	DelayUs(10);                       // tREC = 50us
}	

//! Read a single bit from the 1-wire
unsigned char OW_read_bit (void)
{
	unsigned char read_data; 
    return read_data;
}

//! Send a byte over the 1-wire
void OW_write_byte (unsigned char write_data)
{
	unsigned char loop;	
	for (loop = 0; loop < 8; loop++) {
		OW_write_bit(write_data & 0x01); 	//Sending LS-bit first
		write_data >>= 1;					// shift the data byte for the next bit to send
	}	
}	

//! Read a byte from the bus
unsigned char OW_read_byte (void)
{
	unsigned char loop, result=0;
	for (loop = 0; loop < 8; loop++)
	{		
		result >>= 1; 				// shift the result to get it ready for the next bit to receive
		if (OW_read_bit())
		result |= 0x80;				// if result is one, then set MS-bit
	}
	return result;					
}	

/*******************************************************************************
 End of File
 */
