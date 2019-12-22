#ifndef __miscellaneous_h    /* Guard against multiple inclusion */
#define __miscellaneous_h

/*!\name      miscellaneous.h
 *
 * \brief     Generic functions 
 *
 * \author    Juan Gago
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "system_config.h"
#include "system_definitions.h"
#include "enum.h"

uint8_t 	hash				(const char *);
uint16_t    average				(uint16_t, uint16_t, uint16_t, uint16_t);
bool        isHexadecimal		(uint8_t);
uint8_t 	ascii2decimal		(uint8_t);
char 		ascii2status		(char c);
uint8_t     hexa2ascii 			(uint8_t);
uint16_t    hexa2decimal		(uint8_t*);
bool 		populate_command 	(uint8_t *, COMMAND_DATA*);
bool 		pioMapping_by_name	(COMMAND_DATA*, PORT_DEF *);

#endif /* __miscellaneous_h */

/* *****************************************************************************
 End of File
 */
