#ifndef CDC__H__
#define CDC__H__

/*!\name      cdc.h
 *
 * \brief     common code to all cdc examples
 * 			  you must define the following variables and macros in system_config.h
 *
 * \author    Juan Gago
 *
 */
#include <stdbool.h>      // bool type with true, false
#include <stdint.h>       // uint8_t
#include "system_config.h"

// initialize the cdc device 
void cdc_setup(void);

// attempt to open the cdc device, return true when ...
bool cdc_open(void);

// update the necessary harmony state machines, call from the main loop
void cdc_update(void);

#endif