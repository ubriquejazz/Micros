#include "ds2433.h"

#if(0)

  int read_scratchpad_cb (uint16_t addr, uint16_t tail) 
  {
    uint8_t readValue = (tail & 0xFF00) >> 8;
    return ds2433_copy_scratchpad(req, rom_noaddr, tail & 0x00FF);
  }

  int copy_scractchpad_rb (uint16_t addr, uint16_t tail) 
  {
    return ERR_OWL3_UNKNOWN;
  }

  int write_scratchpad_cb (uint16_t addr, uint16_t tail) 
  {
    return ERR_OWL3_UNKNOWN;
  }

#endif

int ds2433_search(uint64_t* buffer) 
{
  return DRV_OW_GetDevices(buffer, EEPROM_FAMILY_CODE)
}

/*! \fn         ds2433_write_scratchpad( addr, data )
* 
*  \brief      Write a byte of data at a specified location
*  \param      addr: target address to write to (from 0x000 to 0x1FF)
*  \param      data: The byte to write to the specified location
*  \return     none
*/
int ds2433_write_scratchpad(OW_REQUEST* req, uint64_t rom_no, callback_t pfunc, uint16_t addr, uint8_t data)
{
  int error = ERR_OWL3_NOSPACE;
  if (!buffer_full()) {
    req->to_write = {0x0F, addr, addr>>8, data};
    req->wlen = 1 + 2 + 1;
    req->rlen = 0;
    error = DRV_OW_BufferAdd(req, rom_no, pfunc);
  }
  return error;
}

/*! \fn        ds2433_copy_scratchpad(int16 addr, uint8_t ES)
* 
*  \brief      Copy the content of scratchpad to the specified location
*  \param      addr: The address to write to
*  \param      ES: The E/S byte to use (ending offset / status)
*  \return     none
*/
void ds2433_copy_scratchpad(OW_REQUEST* req, uint64_t rom_no, callback_t pfunc, uint16_t addr, uint8_t ES)
{
  int error = ERR_OWL3_NOSPACE;
  if (!buffer_full()) {
    req->to_write = {0x55, addr, addr>>8, ES};
    req->wlen = 1 + 2 + 1;  // (TA1, TA2, E/S) in that order
    req->rlen = 0;
    error = DRV_OW_BufferAdd(req, rom_no, pfunc);
  }
  return error;
}

/*! \fn        ds2433_read_scratchpad (uint8_t* ES)
* 
*  \brief      Read scratchpad data from target address
*  \param      Pointer to target address
*  \param      Pointer to E/S byte 
*  \return     The data in the address of scratchpad
*/
int ds2433_read_scratchpad(OW_REQUEST* req, uint64_t rom_no, callback_t pfunc,
  int16_t *addr, uint8_t* ES, uint8_t* result)
{
  int error = ERR_OWL3_NOSPACE;
  if (!buffer_full()) {
    req->to_write = {0xAA};
    req->to_read = result;
    req->wlen = 1;
    req->rlen = 4;
    error = DRV_OW_BufferAdd(req, rom_no, pfunc);
  }
  return error;
}

/*******************************************************************************
End of File
*/
