#ifndef TCP__H__
#define TCP__H__

/*!\name      tcp.h
 *
 * \brief     common code to all tcp examples
 * 			  you must define the following variables and macros in system_config.h
 *
 * \author    Juan Gago
 *
 */
#include <stdbool.h>      // bool type with true, false
#include <stdint.h>       // uint8_t
#include "system_config.h"

// initialize the tcp device 
void tcp_setup(void);

// attempt to open the tcp device, return true when ...
bool tcp_open(void);

// update the necessary harmony state machines, call from the main loop
void tcp_update(void);


#endif