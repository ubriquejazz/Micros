/*!\name      ledcontrol.h
 *
 * \brief     TCP/IP example.
 *
 * \author    Juan Gago
 *
 */

#include "ledcontrol.h"
 
LEDCONTROL_DATA ledcontrolData;

/* Functions */

int LEDCONTROL_SetCallIndicators(ECS_CALL_BUTTON_STATE_TYPE indicatorState)
{
	switch (indicatorState)
	{
		case ECS_CALL_DOWN:
			//Down Call Indicator ON, Up Call Indicator OFF
			BSP_LEDStateSet(BSP_LED_1, BSP_LED_STATE_OFF);
			BSP_LEDStateSet(BSP_LED_2, BSP_LED_STATE_ON);
			break;
 
		case ECS_CALL_UP:
			//Down Call Indicator OFF, Up Call Indicator ON
			BSP_LEDStateSet(BSP_LED_1, BSP_LED_STATE_ON);
			BSP_LEDStateSet(BSP_LED_2, BSP_LED_STATE_OFF);
			break;
 
		case ECS_CALL_BOTH:
			//Down Call Indicator ON, Up Call Indicator ON
			BSP_LEDStateSet(BSP_LED_1, BSP_LED_STATE_ON);
			BSP_LEDStateSet(BSP_LED_2, BSP_LED_STATE_ON);
			break;
 
		case ECS_CALL_NONE:
		default:
			//Down Call Indicator OFF, Up Call Indicator OFF
			BSP_LEDStateSet(BSP_LED_1, BSP_LED_STATE_OFF);
			BSP_LEDStateSet(BSP_LED_2, BSP_LED_STATE_OFF);
			break;
	}
}
 
/* Application Initialization and State Machine Functions */

void LEDCONTROL_Initialize(void)
{
	/* Place the App state machine in its initial state. */
	ledcontrolData.state = LEDCONTROL_STATE_INIT;
}
 
void LEDCONTROL_Tasks(void)
{
 
	/* Check the application's current state. */
	switch (ledcontrolData.state)
	{
		
		case LEDCONTROL_STATE_INIT:
			if (SYS_TMR_Status(sysObj.sysTmr) == SYS_STATUS_READY) // Is the system timer ready?
			{
				ledcontrolData.ledFlashTmrHandle = SYS_TMR_DelayMS(500); // Setup a timer for 500ms
				if (ledcontrolData.ledFlashTmrHandle != SYS_TMR_HANDLE_INVALID) // Is the timer handle valid?
				{
					ledcontrolData.state = LEDCONTROL_STATE_SERVICE_TASKS; // Handle is valid, advance to next state
				}
			}
			break;
		
 
		case LEDCONTROL_STATE_SERVICE_TASKS:
			if (SYS_TMR_DelayStatusGet(ledcontrolData.ledFlashTmrHandle) == true)	// Has the timer expired?
			{
				ledcontrolData.ledFlashTmrHandle = SYS_TMR_DelayMS(500);			// Start another 500ms timer
				SYS_CMD_READY_TO_READ();											// Accept new data

				if (ledcontrolData.ledFlashTmrHandle != SYS_TMR_HANDLE_INVALID)		// Is the handle valid (timer set)?
				{													   
					BSP_LEDToggle(BSP_LED_3);		// Toggle LED3
					SYS_CMD_READY_TO_READ();		// Tell the system console to accept new data
				}
				else	// Timer Failed, try again in init state
				{
					ledcontrolData.state = LEDCONTROL_STATE_INIT;
				}
			}
			break;
		

		/* The default state should never be executed. */
		default:
		
			/* TODO: Handle error in application's state machine. */
			break;
		
	}
}
 
 
 
/*******************************************************************************
 End of File
 */