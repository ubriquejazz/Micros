#ifndef __irda_dummy_h    /* Guard against multiple inclusion */
#define __irda_dummy_h

/*!\name      irda_dummy.h
 *
 * \brief     Master Pic emulation (dummy)
 *			  irda_pic.h should be included
 *
 * \author    Juan Gago
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "system_config.h"
#include "system_definitions.h"
#include "Global.h"

bool DUMMY_BeaconProcessor 	(char* command, uint8_t*);
bool DUMMY_MessageProcessor (char* command, uint8_t*);

void DUMMY_Receive_BT 		(COMMAND_DATA*, uint8_t *);
void DUMMY_Receive_BC 		(COMMAND_DATA*, uint8_t *);
void DUMMY_Receive_MS 		(COMMAND_DATA*, uint8_t *);
void DUMMY_Receive_MR 		(COMMAND_DATA*, uint8_t *);

#endif /* __irda_dummy_h */

/* *****************************************************************************
 End of File
 */
