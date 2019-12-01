#ifndef _BUTTONCONTROL_H
#define _BUTTONCONTROL_H
 
/*!\name      buttoncontrol.h
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

typedef struct
{
	 BSP_SWITCH_STATE 	currentState;		// Current state of the button
	 BSP_SWITCH_STATE 	previousState;		// State of the button from the previous read
	 uint16_t 			debounceTime_ms;	// Time period (in ms) the button has been in the current state
	 BSP_SWITCH_STATE 	debouncedValue; 	// Final state of the button after the debounce period has elapsed
	 bool 				debounceLatch;		// True if debouncedValue has been read and acted upon by the application
} SWITCH_DEBOUNCE_TYPE;
 
typedef enum
{
	/* Application's state machine's initial state. */
	BUTTONCONTROL_STATE_INIT = 0,
	BUTTONCONTROL_STATE_SERVICE_TASKS,

} BUTTONCONTROL_STATES;
 
typedef struct
{
	/* The application's current state */
	BUTTONCONTROL_STATES 	state;
	SYS_TMR_HANDLE 			buttonDebounceTmrHandle;

	SWITCH_DEBOUNCE_TYPE upCallButton;		//Context Structure for the up call button
	SWITCH_DEBOUNCE_TYPE downCallButton;	//Context Structure for the down call button
} BUTTONCONTROL_DATA;
 
/* Application Initialization and State Machine Functions */

ECS_CALL_BUTTON_STATE_TYPE BUTTONCONTROL_GetCallButtonState();

void BUTTONCONTROL_ClearCallButtonLatches();
 
void BUTTONCONTROL_Initialize(void);
 
void BUTTONCONTROL_Tasks(void);
 
#endif /* _BUTTONCONTROL_H */
 
/*******************************************************************************
 End of File
 */