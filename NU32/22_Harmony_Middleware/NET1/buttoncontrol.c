/*!\name      buttoncontrol.h
 *
 * \brief     TCP/IP example.
 *
 * \author    Juan Gago
 *
 */

#include "buttoncontrol.h"
 
BUTTONCONTROL_DATA buttoncontrolData;
 
#define BUTTON_STABLE_PERIOD_ms 30

/* Callback Functions */

ECS_CALL_BUTTON_STATE_TYPE BUTTONCONTROL_GetCallButtonState()
{
	ECS_CALL_BUTTON_STATE_TYPE callButtonState = ECS_CALL_NONE; //Default return value is ECS_CALL_NONE (no buttons pressed)
 
	if (buttoncontrolData.upCallButton.debounceLatch == true && buttoncontrolData.upCallButton.debouncedValue == BSP_SWITCH_STATE_PRESSED)
	{
		callButtonState += ECS_CALL_UP;
	}
 
	if (buttoncontrolData.downCallButton.debounceLatch == true && buttoncontrolData.downCallButton.debouncedValue == BSP_SWITCH_STATE_PRESSED)
	{
		callButtonState += ECS_CALL_DOWN;
	}
	
	return callButtonState;
}
 
void BUTTONCONTROL_ClearCallButtonLatches()
{
	// Once the client has read the current debounced button state, 
	// the latches are cleared, and the client will not take any further action
	// until the state of the button(s) changes.
	// This prevents multiple call requests being sent 
	// while the button remains in a pressed state.
	buttoncontrolData.upCallButton.debounceLatch = false;
	buttoncontrolData.downCallButton.debounceLatch = false;
}
 
// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************
 
void BUTTONCONTROL_Debounce(SWITCH_DEBOUNCE_TYPE* button)
{
	if (button->currentState == button->previousState)												// The button state has not changed since the last read
	{
		if (button->debounceTime_ms < BUTTON_STABLE_PERIOD_ms)										// Debounce time is less than the "stable period" (30ms)
		{
			button->debounceTime_ms++;																// Increment the debounce time
		}
		else if (button->debounceTime_ms == BUTTON_STABLE_PERIOD_ms)								// Debounce time is equal to the "stable period" (30ms)
		{
			button->debouncedValue = button->currentState;											// Load the current state into the debounced value variable
			button->debounceLatch = true;															// Assert the debounce latch
			button->debounceTime_ms += 1;															// Increment the debounce time to prevent the code within this if 
																									// statement block from being executed again while the button remains 
																									// in the current state
		}
	}
	else																							// The button state has changed
	{
		button->debounceTime_ms = 0;																// The new state will need to be debounced, so reset the debounce time
		button->debounceLatch = false;																// Latch is cleared to prevent the client application
																									// reading the state of the button while its being debounced
	}
	button->previousState = button->currentState;													// Store the current state into the previous state variable
}

void BUTTONCONTROL_DebounceInit(SWITCH_DEBOUNCE_TYPE* button)
{
	button->previousState = BSP_SWITCH_STATE_RELEASED;												//Set the previous, current and debounced state of the button to Released
	button->currentState = BSP_SWITCH_STATE_RELEASED;
	button->debouncedValue = BSP_SWITCH_STATE_RELEASED;
	button->debounceTime_ms = 0;																	//Set the debounce time to 0
	button->debounceLatch = false;																	//Set the debounce latch to false
}
 
// Application Initialization and State Machine Functions

void BUTTONCONTROL_Initialize(void)
{
	/* Place the App state machine in its initial state. */
	buttoncontrolData.state = BUTTONCONTROL_STATE_INIT;
 
	BUTTONCONTROL_DebounceInit(&buttoncontrolData.upCallButton);									// Initialise the debounce variables for the up call button
	BUTTONCONTROL_DebounceInit(&buttoncontrolData.downCallButton);									// Initialise the debounce variables for the down call button
}

void BUTTONCONTROL_Tasks(void)
{
	/* Check the application's current state. */
	switch (buttoncontrolData.state)
	{

		case BUTTONCONTROL_STATE_INIT:
		
			if (SYS_TMR_Status(sysObj.sysTmr) == SYS_STATUS_READY)		  							// Is the system timer ready?
			{
				buttoncontrolData.buttonDebounceTmrHandle = SYS_TMR_DelayMS(1);						// Setup a timer for 1ms, store handle
				if (buttoncontrolData.buttonDebounceTmrHandle != SYS_TMR_HANDLE_INVALID)			// Is the handle valid (timer set)?
				{
					buttoncontrolData.state = BUTTONCONTROL_STATE_SERVICE_TASKS;					// Handle is valid, advance to next state
				}
			}
			break;
 
		case BUTTONCONTROL_STATE_SERVICE_TASKS:
		
			if (SYS_TMR_DelayStatusGet(buttoncontrolData.buttonDebounceTmrHandle) == true)			// Has the timer expired?
			{
				buttoncontrolData.upCallButton.currentState = BSP_SwitchStateGet(BSP_SWITCH_1);		// Read State of Push Button 1, store value in upCallButton structure
				buttoncontrolData.downCallButton.currentState = BSP_SwitchStateGet(BSP_SWITCH_2);	// Read State of Push Button 2, store value in downCallButton structure
 
				BUTTONCONTROL_Debounce(&buttoncontrolData.upCallButton);							// Debounce Push Button 1, pass pointer to upCallButton structure
				BUTTONCONTROL_Debounce(&buttoncontrolData.downCallButton);							// Debounce Push Button 2, pass pointer to downCallButton structure
 
				buttoncontrolData.buttonDebounceTmrHandle = SYS_TMR_DelayMS(1);						// Set another 1ms Timer
				if (buttoncontrolData.buttonDebounceTmrHandle == SYS_TMR_HANDLE_INVALID) 			// Is the handle valid (timer set)?
				{
					buttoncontrolData.state = BUTTONCONTROL_STATE_INIT;								// Timer Failed, try again in init state
				}
			}
			break;
 
		default:
			Nop();
			break;
		
	}
}
 
 
 
/*******************************************************************************
 End of File
 */