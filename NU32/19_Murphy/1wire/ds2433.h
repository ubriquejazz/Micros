#ifndef  __DS2433_H
#define  __DS2433_H

/*!\name      ds2433.h
 *
 * \brief     Functions of the DS2433 SRAM  
 *			   
 *			   write_scratchpad(addr, data)                   
 *			     Used to write data to scratchpad             																	
 *			   data = read_scratchpad(&addr, &ES)             
 *			     Used to verify scratchpad data and target addr
 *			   copy_scratchpad(addr, ES)                      
 *			     Used to copy data from scratchpad to memory  
 *
 *			   data = read_memory(addr)                       
 *			     Used to read a place in memory   
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
    struct{
        unsigned E:5;				// Ending offset
        unsigned PF:1;				// Partial Flag
        unsigned X:1;				// Always zero
        unsigned AA:1;    			// Autorization Accepted
    };
    uint8_t byte;
} ESRegister_t;

/* This will write a byte of data to the scratchpad at a specified location
PARAM addr: The address to write to
PARAM data: The byte to write to the specified location
RETURNS: none
*/
void write_scratchpad(int16 addr, uint8_t data);

/* This will read scratchpad data from a specified address
PARAM addr: The address to read from
NOTE: In order to copy data from the scratchpad, the ES byte must be known
RETURNS: The data in the address of scratchpad
*/
uint8_t read_scratchpad(uint8_t* ES);

/* This will copy the contents of the scratchpad to the specified location
   All parameters passed in must match the DS2493's 3 address registers
PARAM addr: The address to copy to
PARAM ES: The E/S byte to use
*/
void copy_scratchpad(int16 addr, uint8_t ES);

/* This will read one byte from memory at a specified address
PARAM: The address to read from
RETURNS: The data at the address
*/
uint8_t read_memory(int16 addr);

#endif

