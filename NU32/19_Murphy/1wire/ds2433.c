#include "ds2433.h"
#include "OW.h"     // OW_reset_pulse, OW_read_byte, ...
#include "OWRom.h"  // skip_rom(), match_rom(), etc.

/*! \fn         write_scratchpad(int16 addr, uint8_t data)
 * 
 *  \brief      Write a byte of data at a specified location
 *  \param      addr: target address to write to (from 0x000 to 0x1FF)
 *  \param      data: The byte to write to the specified location
 *  \return     none
 */
void write_scratchpad(int16 addr, uint8_t data)
{
  OW_write_byte(0x0F);        // write scratchpad command
  OW_write_byte(addr);
  OW_write_byte(addr >> 8);
  OW_write_byte(data);  
}

/*! \fn         read_scratchpad (uint8_t* ES)
 * 
 *  \brief      Read scratchpad data from target address
  * \param      Pointer to target address
 *  \param      Pointer to E/S byte 
 *  \return     The data in the address of scratchpad
 */
uint8_t read_scratchpad(int16 *addr, uint8_t* ES)
{
  OW_write_byte(0xAA);           // read scratchpad command
  uint8_t addrL = OW_read_byte();
  uint8_t addrH = OW_read_byte(); 
  *addr =  (addrH << 8) + addrL; // target address
  *ES = OW_read_byte();          // ending address and status
  return OW_read_byte();
}

/*! \fn         copy_scratchpad(int16 addr, uint8_t ES)
 * 
 *  \brief      Copy the content of scratchpad to the specified location
 *  \param      addr: The address to write to
 *  \param      ES: The E/S byte to use (ending offset / status)
 *  \return     none
 */
void copy_scratchpad(int16 addr, uint8_t ES)
{
 	OW_write_byte(0x55);        // copy scratchpad command
	OW_write_byte(addr);
	OW_write_byte(addr >> 8);  // Autorization Code
	OW_write_byte(ES);         // TA1, TA2, E/S, in that order
}

/*******************************************************************************
 End of File
 */
