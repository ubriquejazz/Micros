#ifndef _ONEW0_H
#define _ONEW0_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "system_config.h"
#include "system_definitions.h"

/*!\name      oneW0.c
 *
 * \brief     State machine for the 1W driver
 *
 * \author    Juan Gago
 *
 */

#define ONEW0_DISCOVER_START            1000  
#define ONEW0_DISCOVER_TIMEOUT          100  
#define ONEW0_SENSOR_DELAY              750

// Temperature Sensor Commands
#define DS18B20_Skip_ROM                0xCC
#define DS18B20_Convert_T               0x44
#define DS18B20_Read_Scratchpad         0xBE

//__TRACE(id);
#define MODULE_TYPE     '['   //!< Module type delimiter.
#define MODULE_SERIAL   '$'   //!< Module serial delimiter.
#define STOCK_CODE      '%'   //!< Module stock code delimiter.
#define WORK_ORDER      '}'   //!< Work order info delimiter.
#define MANUFACTURER    '{'   //!< Manufactured by delimiter.
#define CHIP_ID         '!'   //!< Module chip ID delimiter.
#define PROCESS         '*'   //!< Process type for the Module delimiter.
#define TERMINATOR      ']'   //!< Terminator of any field ID delimiter (This has no data attaced to it).
#define CR_VALUE        '\r'  //!< Caraige return to mark end of data.
#define LF_VALUE        '\n'  //!< Caraige return to mark end of data.

typedef enum
{
	ONEW0_STATE_INIT=0,            
	ONEW0_STATE_SETUP_FREQ,
	ONEW0_STATE_INIT_1WIRE,            
    ONEW0_STATE_IDLE,
    ONEW0_STATE_CONVERT_T

} ONEW0_STATES;

typedef struct
{
    ONEW0_STATES state;   
    char        rxbuff[64];     // buffer from the 1W Prom
    uint8_t     device_id;      // Device ID to be read
    bool        readen;         // read Enable    

} ONEW0_DATA;
	
// Application Initialization and State Machine Functions

void ONEW0_Initialize ( void );

void ONEW0_Tasks( uint32_t* );

#endif /* _ONEW0_H */

/*******************************************************************************
 End of File
 */
