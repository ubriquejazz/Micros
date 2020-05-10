#ifndef  _DS2433__H
#define  _DS2433__H

#include "drv_ow.h" 

/*!\name      ds2433.h
 *
 * \brief     Functions of the DS2433 device (L3)
 *            
 * \author    Juan Gago
 *
 */ 

#define EEPROM_FAMILY_CODE  		0x23
#define EEPROM_SIZE         		0x200
#define SCRATCHPAD_SIZE     		32
#define EEPROM_CMD_WRITE_SCRATCHPAD 0x0F
#define EEPROM_CMD_READ_SCRATCHPAD  0xAA
#define EEPROM_CMD_COPY_SCRATCHPAD  0x55
#define EEPROM_CMD_READ_MEMORY      0xF0

typedef union {
    struct {
        unsigned E:5;				// Ending offset
        unsigned PF:1;				// Partial Flag
        unsigned X:1;				// Always zero
        unsigned AA:1;    			// Autorization Accepted
    };
    uint8_t byte;
} ESRegister_t;

// Result of these functions
typedef enum {
	ERR_OWL3_SUCCESS = 0,
	ERR_OWL3_FAILURE = -1,
	ERR_OWL3_NOSPACE = -2,
	ERR_OWL3_UNKNOWN = -99,
} ERR_OWL3;

int ds2433_search(uint64_t*);
int ds2433_write_scratchpad(REQUEST_ARGS, uint16_t addr, uint8_t data);
int ds2433_copy_scratchpad(REQUEST_ARGS, uint16_t addr, uint8_t ES);
int ds2433_read_scratchpad(REQUEST_ARGS, uint16_t addr, uint8_t* ES, uint8_t* result);
uint8_t ds2433_read_memory(REQUEST_ARGS, uint16_t addr, uint8_t* result);

#endif