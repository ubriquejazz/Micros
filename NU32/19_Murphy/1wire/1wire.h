#ifndef _1WIRE_H
#define _1WIRE_H

#include <stdio.h>

/*! \file 	1wire.h

    \brief 	IO functions
*/

#define  READ_COMMAND_SERIAL_NUMBER     0x33

//! Drive 1-wire pin low
/*! Set pin to be an output and drive it low
*/ 
/*! \a Nothing passed in
    \return Nothing passed back
*/
void drive_OW_low (void);

//! Drive 1-wire pin high
/*! Set pin to be an output and drive it high
*/ 
/*! \a Nothing passed in
    \return Nothing passed back
*/
void drive_OW_high (void);

//unsigned char read__one_wire (void);

//! Send a single bit to the 1-wire bus
/*! @param write_data bit level that is to be sent (1/0)
    \return Nothing passed back
*/
void OW_write_bit (unsigned char write_data);

//! Read the 1-wire state
/*! Set pin to be an input and read the value
*/ 
/*! \a Nothing passed in
    \return The pin value found
*/
unsigned char read_OW (void);

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

