#ifndef  _DS2433_H
#define  _DS2433_H

/*!\name      ds2433.h
 *
 * \brief     Functions of the DS2433 SRAM   
 *            
 * \author    Juan Gago
 *
 */ 

// Functions result
#define EEPROM_SUCCESS				+1
#define EEPROM_FAILURE				-1
#define EEPROM_NO_ENOUGH			-2

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

void  	ds2433_init(PIN_DEF);

int   	ds2433_search(uint64_t*, int);

int 	ds2433_set_device(uint64_t);

int  	ds2433_get_error();

/* Implementation with Request FIFO */

int 	ds2433_poll();

bool 	ds2433_write_scratchpad(uint16_t addr, uint8_t data);

bool 	ds2433_read_scratchpad(uint8_t* ES, uint8_t* result);

bool 	ds2433_copy_scratchpad(uint16_t addr, uint8_t ES);

uint8_t ds2433_read_memory(uint16_t addr, uint8_t* result);

#endif

