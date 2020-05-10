# 1W Driver

## Low level library (L1)
- ow.h definition of the OW_REQUEST structure, rom commands
- src/ow.c implementation 
- src/ow_search.{c,h}

## Static driver (L2)
- drv_ow.h prototypes of the driver API
- src/drv_ow_static.c implementation

## Devices (L3)
 - ds1820.h	 functions for the thermometer
 - ds2433.h	 functions for the EEPROM

## Dependencies
 - mutex.h	for OW_init()
 - buffer.h for OW_REQUEST