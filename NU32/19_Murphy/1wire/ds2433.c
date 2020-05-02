#include "ds2433.h"
#include "OW.h"
#include "OWSearch.h"     
#include "mutex.h"
#include "buffer.h" 

// Private variables
static char Hostname[32];         // Device specific
static int MutexOneWire, Error;   // Generic
static OW_REQ Request;            // 

int read_scratchpad_cb (uint16_t addr, uint16_t tail);

void ds2433_init(PIN_DEF pin) {
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
  Request.rom_code = rom_code;
  Request.pfunc = NULL;
  return EEPROM_SUCCESS;
}

/* REQ FIFO IMPLEMENTATION */

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

  Request.to_write = {0x0F, addr, addr>>8, data};
  Request.wlen = 1 + 2 + 1;
  Request.rlen = 0;
  buffer_write(Request);
  return EEPROM_SUCCESS;
}

/*! \fn        ds2433_copy_scratchpad(int16 addr, uint8_t ES)
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

  Request.to_write = {0x55, addr, addr>>8, ES};
  Request.wlen = 1 + 2 + 1;  // (TA1, TA2, E/S) in that order
  Request.rlen = 0;
  buffer_write(Request);
  return EEPROM_SUCCESS;
}

/*! \fn        ds2433_read_scratchpad (uint8_t* ES)
* 
*  \brief      Read scratchpad data from target address
*  \param      Pointer to target address
*  \param      Pointer to E/S byte 
*  \return     The data in the address of scratchpad
*/
int ds2433_read_scratchpad(uint16_t *addr, uint8_t* ES, uint8_t* result)
{
  if (buffer_full() == true)
    return EEPROM_NO_ENOUGH;
  Request.to_write = {0xAA};
  Request.wlen = 1;
  Request.rlen = 4;
  Request.pfunc = read_scratchpad_cb;
  buffer_write(Request);
  return EEPROM_SUCCESS;
}

int read_scratchpad_cb (uint16_t addr, uint16_t tail) {
  uint8_t readValue = (tail & 0xFF00) >> 8;
  return ds2433_copy_scratchpad(addr, tail & 0x00FF);
}

/*******************************************************************************
End of File
*/
