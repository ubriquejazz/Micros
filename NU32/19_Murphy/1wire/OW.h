#ifndef _OW_H
#define _OW_H

#include "pin_common.h"

/*!\name      OW.h
 *
 * \brief     API for PIN controlling (1W line)
 *
 * \author    Juan Gago
 *
 */ 

extern PIN_DEF PinOne;

//! Place a reset pulse on the bus
/*! \a Nothing passed in
    \return If a device is on the bus (1 = Device, 0 = No Device)
*/
uint8_t OW_reset_pulse(void);

//! Read a single bit from the 1-wire
/*! \a Nothing passed in
    \return Value read from the bus (1/0)
*/
uint8_t OW_read_bit (void);

//! Send a single bit to the 1-wire bus
/*! @param write_data bit level that is to be sent (1/0)
    \return Nothing passed back
*/
void OW_write_bit (uint8_t write_data);

//! Send a byte over the 1-wire
/*! @param write_data byte of data to write to the bus
    \return Nothing passed back
*/
void OW_write_byte (uint8_t write_data);

//! Read a byte from the bus
/*! \a Nothing passed in
    \return byte read back from the 1-wire
*/
uint8_t OW_read_byte (void);

#endif /* OW_PIN_COMMON_H */

/*******************************************************************************
 End of File
 */
