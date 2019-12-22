#ifndef __irda_pic_h    /* Guard against multiple inclusion */
#define __irda_pic_h

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "system_config.h"
#include "system_definitions.h"
#include "enum.h"

/*!\name      irda_pic.h
 *
 * \brief     IRDA Pic - 
 *
 * \author    Juan Gago
 *
 */

bool        IRDA_QueueToSend (uint8_t i, char*);
bool        IRDA_QueueToSend_Beacon (uint8_t i, uint8_t j, char, uint8_t*);
bool        IRDA_QueueToSend_BeaconAndMessage (char, uint8_t i, uint8_t*);
bool        IRDA_MessageSent (uint8_t i, uint32_t, uint32_t);

void		Create_Message (char key, uint8_t i, uint8_t*);

void        IRDA_2channel_Setup(bool);
void        IRDA_3channel_Setup(bool);

#endif /* __irda_pic_h */

/* *****************************************************************************
 End of File
 */
