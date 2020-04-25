#ifndef __CRC8_H
#define __CRC8_H

extern uint8_t crc8;

//! 1 byte CRC check	
/*! @param value Value to add to the byte crc
    \return last calculated crc
*/
uint8_t do_crc8(uint8_t);

bool check_crc8(uint8_t *);

bool reset_crc8(void);

#endif // __CRC8_H
