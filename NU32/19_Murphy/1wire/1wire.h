#ifndef _1WIRE_H
#define _1WIRE_H

#include <stdio.h>
#include "pin_common.h"

extern PIN_DEF PinOne;

/*! \file 	1wire.h

    \brief 	IO functions
*/

#define  READ_COMMAND_SERIAL_NUMBER     0x33

//! Place a reset pulse on the bus
/*! Reset all devices on the bus by sending a long low.
     over the 1-wire bus followed by a presense pulse
*/ 
/*! \a Nothing passed in
    \return If a device is on the bus (1 = Device, 0 = No Device)
*/
unsigned char OW_reset_pulse(void);

//! Read a single bit from the 1-wire
/*! \a Nothing passed in
    \return Value read from the bus (1/0)
*/
unsigned char OW_read_bit (void);

//! Send a byte over the 1-wire
/*! @param write_data byte of data to write to the bus
    \return Nothing passed back
*/
void OW_write_byte (unsigned char write_data);

//! Read a byte from the bus
/*! \a Nothing passed in
    \return byte read back from the 1-wire
*/
unsigned char OW_read_byte (void);

#endif /* _1WIRE_H */

/*******************************************************************************
 End of File
 */

