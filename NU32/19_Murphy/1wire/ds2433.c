#include "ds2433.h"
#include "mutex.h"

#include "OW.h"     // OW_reset_pulse, OW_read_byte, ...
#include "OWRom.h"  // skip_rom(), match_rom(), etc.

// Private variables
static char Hostname[32];         // Device specific
static int MutexOneWire, Error;   // Generic

void ds2433_init(PIN_DEF pin) 
{
  Error = 1;
  memset(Hostname, 0, 32);
  MutexOneWire = OW_init(pin);
  return;
}

int ds2433_search(uint64_t* buffer, int length) 
{
  int deviceCount = 0;
  return deviceCount;
}

/*! \fn         write_scratchpad(uint16_t addr, uint8_t data)
 * 
 *  \brief      Write a byte of data at a specified location
 *  \param      addr: target address to write to (from 0x000 to 0x1FF)
 *  \param      data: The byte to write to the specified location
 *  \return     none
 */
void ds2433_write_scratchpad(uint16_t addr, uint8_t data)
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
uint8_t ds2433_read_scratchpad(uint16_t *addr, uint8_t* ES)
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
void ds2433_copy_scratchpad(uint16_t addr, uint8_t ES)
{
 	OW_write_byte(0x55);       // copy scratchpad command
	OW_write_byte(addr);
	OW_write_byte(addr >> 8);  // Autorization Code
	OW_write_byte(ES);         // TA1, TA2, E/S, in that order
}


int ds2433_poll (uint64_t address, LED_DEF* led) 
{
  static uint8_t phase = 0;
  return phase;
}

int ds2433_get_error() {
  return Error;
}

/*******************************************************************************
 End of File
 */
