#ifndef _DS1820_H
#define _DS1820_H

/*!\name      ds1820.h
 *
 * \brief     Functions of the DS18B20 thermometer  
 *			   
 *			   write_scratchpad(addr, data)                   
 *			     Used to write data to scratchpad             																	
 *			   data = read_scratchpad(&addr, &ES)             
 *			     Used to verify scratchpad data and target addr
 *			   copy_scratchpad(addr, ES)                      
 *			     Used to copy data from scratchpad to memory  
 *
 *			   convert_temperature()
 *            
 *			   read_power()
 *
 * \author    Juan Gago
 *
 */ 
#define TEMPERATURE_FAMILY_CODE 		 0x28
#define TEMPERATURE_CMD_CONVERT          0x44
#define TEMPERATURE_CMD_WRITE_SCRATCHPAD 0x4E
#define TEMPERATURE_CMD_READ_SCRATCHPAD	 0xBE
#define TEMPERATURE_CMD_COPY_SCRATCHPAD	 0x48

typedef union {
    struct {
        uint8_t temperatureLsb;		// Conversion: 3210. (-1) (-2) (-3) (-4)
        uint8_t temperatureMsb;		// Conversion: SSSS. ( S) ( 6) ( 5) ( 4)
        uint8_t tH;					// Alarm High: S654.3210 (EEPROM)
        uint8_t tL;					// Alarm Low:: S654.3210 (EEPROM)
        uint8_t configuration;		// Resolution (EEPROM)
        uint8_t reserved_0;			// 0xFF
        uint8_t reserved_1;			// 0x00
        uint8_t reserved_2;			// 0x10
        uint8_t crc;
    };
    uint8_t byte[9]; 
}temperatureData_t;

void convert_temperature ();

void read_power ();


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
float read_scratchpad(uint8_t* ES);

/* This will copy the contents of the scratchpad to the specified location
   All parameters passed in must match the DS2493's 3 address registers
PARAM addr: The address to copy to
PARAM ES: The E/S byte to use
*/
void copy_scratchpad(int16 addr, uint8_t ES);

#endif