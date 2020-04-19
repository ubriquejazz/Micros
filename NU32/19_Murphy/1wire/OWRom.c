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

/*******************************************************************************
 End of File
 */