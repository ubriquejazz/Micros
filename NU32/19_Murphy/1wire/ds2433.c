#include "ds2433.h"
#include "OW.h"
#include "OWSearch.h"     
#include "mutex.h"
#include "buffer.h" 

// Private variables
static char Hostname[32];         // Device specific
static int MutexOneWire, Error;   // Generic
static uint64_t Address;          // Rom Code

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
  if (!mutex_isLocked(MutexOneWire)) {
    mutex_lock(MutexOneWire);
    // ...
    mutex_unlock(MutexOneWire);
    Error = 0;
  }
  return deviceCount;
}

int ds2433_get_error() {
  return Error;
}

int ds2433_set_device(uint64_t rom_code)
{
  Address = rom_code;
  return EEPROM_SUCCESS;
}

/* REQ FIFO IMPLEMENTATION */

int ds2433_poll()
{
  OWBus_match_rom(Address);
  OW_poll();
  return 
}

/*! \fn         ds2433_write_scratchpad( addr, data )
* 
*  \brief      Write a byte of data at a specified location
*  \param      addr: target address to write to (from 0x000 to 0x1FF)
*  \param      data: The byte to write to the specified location
*  \return     none
*/
int ds2433_write_scratchpad(uint16_t addr, uint8_t data)
{
  if (buffer_full() == true)
    return EEPROM_NO_ENOUGH;

  OW_REQ req;
  req.to_write = {0x0F, addr, addr>>8, data};
  req.wlen = 1 + 2 + 1;
  req.rlen = 0;
  buffer_write(req);
  return EEPROM_SUCCESS;
}

/*! \fn         read_scratchpad (uint8_t* ES)
* 
*  \brief      Read scratchpad data from target address
*  \param      Pointer to target address
*  \param      Pointer to E/S byte 
*  \return     The data in the address of scratchpad
*/

int read_scratchpad_cb (int a, int b)
{
  // AFter read scratchpad command
  uint8_t addrL = OW_read_byte();
  uint8_t addrH = OW_read_byte(); 
  *addr =  (addrH << 8) + addrL; // target address
  *ES = OW_read_byte();          // ending address and status
   OW_read_byte();
}

uint8_t ds2433_read_scratchpad(uint16_t *addr, uint8_t* ES, uint8_t* result)
{
  if (buffer_full() == true)
    return EEPROM_NO_ENOUGH;

  OW_REQ req;
  req.to_write = {0xAA};
  req.wlen = 1;
  req.rlen = 4;
  req.pfunc = read_scratchpad_cb;
  buffer_write(req);
  return EEPROM_SUCCESS;
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
  if (buffer_full() == true)
    return EEPROM_NO_ENOUGH;

  OW_REQ req;
  req.to_write = {0x55, addr, addr>>8, ES};
  req.wlen = 1 + 2 + 1;  // (TA1, TA2, E/S) in that order
  req.rlen = 0;
  buffer_write(req);
  return EEPROM_SUCCESS;
}

/*******************************************************************************
End of File
*/
