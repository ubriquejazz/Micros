#include "OWRom.h"
#include "OW.h"      // OW_reset_pulse, OW_read_bit ...

/*! \fn         skip_rom ()
 * 
 *  \brief      This will save time on single drop bus system by bypassing any
 *              function and heading straight to a memory function
 */
void skip_rom() 
{
	OW_write_byte(0xCC); // command to skip ROM
}

/*! \fn         OW_write_byte (write_data)
 * 
 *  \brief      Send a byte over the 1-wire
 *
 *  \param		write_data - byte of data to write to the bus
 */	
void OW_write_byte (uint8_t write_data)
{
	uint8_t loop;	
	for (loop = 0; loop < 8; loop++) {
		OW_write_bit(write_data & 0x01);    // sending LS-bit first
		write_data >>= 1; // shift the data byte for the next bit to send
	}	
}	

/*! \fn         OW_read_byte ()
 * 
 *  \brief      Read a byte from the bus
 * 
 *  \return     byte read back from the 1-wire
 */	
uint8_t OW_read_byte (void)
{
	uint8_t loop, result=0;
	for (loop = 0; loop < 8; loop++) {		
		result >>= 1; // shift result to get it ready for next bit to receive
		if (OW_read_bit())
            result |= 0x80;    	// if result is one, then set MS-bit
	}
	return result;					
}	

/*******************************************************************************
 End of File
 */