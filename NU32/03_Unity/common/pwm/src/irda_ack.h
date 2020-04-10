#ifndef __irda_ack_h    /* Guard against multiple inclusion */
#define __irda_ack_h

/*!\name      irda_ack.h
 *
 * \brief     IRDA Pic - 
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

bool IRDA_UpdateTable 	(uint8_t, uint8_t, uint16_t, char, uint8_t*);
bool IRDA_ConfirmID 	(uint8_t, uint8_t, uint16_t, char, uint8_t*);
bool IRDA_Uart0Message 	(COMMAND_DATA*, uint8_t, uint8_t*);

#endif /* __irda_ack_h */

/* *****************************************************************************
 End of File
 */
