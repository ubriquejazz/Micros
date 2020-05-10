#ifndef _OW__H
#define _OW__H

#include "pin_common.h"
#include "buffer.h"

/*!\name      OW.h
 *
 * \brief     Low level library (L1)
 *			  Definition of basic commands
 *			  Type definiton of OW_REQUEST
 *
 * \author    Juan Gago
 *
 */ 

#define ROM_CMD_SEARCH  0xF0  // Search command for all devices
#define ROM_CMD_SKIP	0xF0  // Skip command for all devices
#define ROM_CMD_MATCH	0xF0  // Match command for all devices
#define ROM_CMD_READ	0xF0  // Read command for all devices

typedef int (*callback_t) (uint16_t, uint16_t);

typedef struct 
{
	Base*		base;				// 
	uint64_t	rom_no				// after search algorithm
	uint8_t		to_write[8];		// write request
	uint8_t		to_read[8];			// read request
	int			wlen, rlen;			// rlen = 0 (write req)
	callback_t	pfunc;				// pointer to a function
	
} OW_REQUEST;

// Result of these functions
typedef enum {
	ERR_OWL1_SUCCESS = 0,
	ERR_OWL1_FAILURE = -1,
	ERR_OWL1_UNKNOWN = -99,
} ERR_OWL1;

int	OW_init(PIN_DEF);			// return mutex of the driver
int OW_reset_pulse();			// presence (1) not detected (0)
int OW_write_bit(int);			// value to be sent (1/0)
int OW_read_bit();				// value read from the bus (1/0)

int OW_write_byte(uint8_t);		// byte to be sent (0xFF)
uint8_t OW_read_byte();			// byte read back from the bus

int OW_match_rom(uint64_t);
int OW_skip_rom();
int OW_read_rom(uint64_t*);

#endif