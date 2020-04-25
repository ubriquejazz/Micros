#ifndef _DS1820_H
#define _DS1820_H

/*!\name      ds1820.h
 *
 * \brief     Functions of the DS18B20 thermometer  
 *			  
 * \author    Juan Gago
 *
 */ 
#define TEMPERATURE_FAMILY_CODE 		 0x28
#define TEMPERATURE_CMD_CONVERT          0x44
#define TEMPERATURE_CMD_WRITE_SCRATCHPAD 0x4E
#define TEMPERATURE_CMD_READ_SCRATCHPAD	 0xBE
#define TEMPERATURE_CMD_COPY_SCRATCHPAD	 0x48
#define TEMPERATURE_CMD_POWER_SUPPLY     0xB4

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

int   ds1820_poll(uint64_t, LED_DEF);

float ds1820_get_temperature(int);

bool  ds1820_get_error();

#endif