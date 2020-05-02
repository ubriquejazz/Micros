#ifndef _DS1820_H
#define _DS1820_H

/*!\name      ds1820.h
 *
 * \brief     Functions of the DS18B20 thermometer  
 *			  
 * \author    Juan Gago
 *
 */ 

// Functions result
#define TEMP_SUCCESS              +1
#define TEMP_FAILURE              -1
#define TEMP_NO_ENOUGH            -2

#define TEMP_FAMILY_CODE 		  0x28
#define TEMP_CMD_CONVERT          0x44
#define TEMP_CMD_WRITE_SCRATCHPAD 0x4E
#define TEMP_CMD_READ_SCRATCHPAD  0xBE
#define TEMP_CMD_COPY_SCRATCHPAD  0x48
#define TEMP_CMD_POWER_SUPPLY     0xB4

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

void  ds1820_init(PIN_DEF);

int   ds1820_search(uint64_t*, int);

int   ds1820_set_device(uint64_t);

bool  ds1820_get_error();

/* Implementation with MUTEX */

int   ds1820_poll(LED_DEF);

float ds1820_get_temperature(int);

#endif