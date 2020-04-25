#ifndef _OW_H
#define _OW_H

#include "pin_common.h"

/*!\name      OW.h
 *
 * \brief     Library to control 1W bit
 *
 * \author    Juan Gago
 *
 */ 

int OW_init(PIN_DEF);

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

#endif /* _OW_H */

/*******************************************************************************
 End of File
 */
