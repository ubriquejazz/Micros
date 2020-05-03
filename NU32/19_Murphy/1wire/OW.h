#ifndef _OW_H
#define _OW_H

#include "pin_common.h"

/*!\name      OW.h
 *
 * \brief     Library to control bit/byte
 *
 * \author    Juan Gago
 *
 */ 

#define OW_SUCCESS		0
#define OW_FAILURE		-1

int 	OW_init (PIN_DEF);
uint8_t OW_reset_pulse (void);
uint8_t OW_read_bit (void);
int 	OW_write_bit (uint8_t write_data);
int 	OW_write_byte (uint8_t write_data);
uint8_t OW_read_byte (void);

#endif /* _OW_H */

/*******************************************************************************
 End of File
 */
