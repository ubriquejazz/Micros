#ifndef _LEDCONTROL_H
#define _LEDCONTROL_H

/*!\name      ledcontrol.h
 *
 * \brief     TCP/IP example.
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
#include "ecstypes.h"

typedef enum
{
    /* Application's state machine's initial state. */
    LEDCONTROL_STATE_INIT = 0,
    LEDCONTROL_STATE_SERVICE_TASKS,

} LEDCONTROL_STATES;


typedef struct
{
    /* The application's current state */
    LEDCONTROL_STATES 	state;
    SYS_TMR_HANDLE 		ledFlashTmrHandle;	// Stores the timer handle returned from SYS_TMR_DelayMS() call.

} LEDCONTROL_DATA;

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

void LEDCONTROL_Initialize(void);
void LEDCONTROL_Tasks(void);
void LEDCONTROL_SetCallIndicators(ECS_CALL_BUTTON_STATE_TYPE indicatorState);

#endif /* _LEDCONTROL_H */

/*******************************************************************************
 End of File
 */

