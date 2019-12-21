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

#ifdef MOCK
void        insert_by_index		(NODE_T **, int);
int         remove_by_index		(NODE_T **, int);
void        bubble_sort			(uint8_t*);
void        print_list			(NODE_T *, uint8_t*); 
#endif 
uint8_t 	hash				(const char *);
uint16_t    average				(uint16_t, uint16_t, uint16_t, uint16_t);
bool        isHexadecimal		(uint8_t);
uint8_t 	ascii2decimal		(uint8_t);
char 		ascii2status		(char c);
uint8_t     hexa2ascii 			(uint8_t);
uint16_t    hexa2decimal		(uint8_t*);
bool 		populate_command 	(uint8_t *, COMMAND_DATA*);
bool 		pioMapping_by_name	(COMMAND_DATA*, PORT_DEF *);
bool        crc_update 			(COMMAND_DATA*);
uint32_t    CalculateCrc 		(uint8_t *, uint32_t);
uint16_t    AppendCrc 			(uint8_t*);
int64_t     GetThisBeat			(void);
int64_t     GetBeatsPerSecond	(void);
int64_t     MicrosecondsToBeats	(int64_t);
int64_t     MillisecondsToBeats	(int64_t);
void        DelayMs				(unsigned long int msDelay );
void        Delay10us 			(unsigned long int input );

#endif /* __miscellaneous_h */

/* *****************************************************************************
 End of File
 */
