#include "led_common.h"
#include "timing.h"

// Private variables
static LED_DEF LedState[LED_COUNT]; // Copy of the static array (local))

/*! \fn         led_on (led)
 * 
 *  \brief      Set a led in ON state according to their 
 *              polarization (active low - active high)  
 * 
 *  \param      led     pointer to LED_DEF
 * 
 *  \return     SUCCESS 
*/ 
int led_on (LED_DEF* led)
{
	led->state = LEVEL_HIGH;
    if (led->pol == ACTIVE_HIGH)
		SYS_PORTS_PinSet(PORTS_ID_0, led->channel, led->bitPos);
	else
		SYS_PORTS_PinClear(PORTS_ID_0, led->channel, led->bitPos);
	return LED_SUCCESS;
}

/*! \fn         led_off (LED_DEF*)
 * 
 *  \brief      Set a led in OFF state according to their 
 *              polarization (active low - active high)  
 * 
 *  \param      led         pointer to LED_DEF
 * 
 *  \return     SUCCESS 
 */ 
int led_off (LED_DEF* led)
{
    led->state = LEVEL_LOW;
	if (led->pol == ACTIVE_HIGH)
		SYS_PORTS_PinClear(PORTS_ID_0, led->channel, led->bitPos);
	else
		SYS_PORTS_PinSet(PORTS_ID_0, led->channel, led->bitPos);
	return LED_SUCCESS;
}

/*! \fn         led_get(index)
 * 
 *  \brief      Returns the led indicated by the index
 * 
 *  \param      index       index in the array LedState[]
 * 
 *  \return     Pointer to LED_DEF
 */
LED_DEF* led_get (int index) 
{
    return &LedState[index];
}

/*! \fn         led_status(led, state)
 * 
 *  \brief      Returns the status of the LED through pointer value
 * 
 *  \param      led         pointer to LED_DEF
 *  \param      state       status of the led (pointer)
 * 
 *  \return     SUCCESS if some conditions
 */
int led_status (LED_DEF* led, uint8_t* state) 
{
    if ( state != NULL ) {
        *state = led->state;
        return LED_SUCCESS;
    }
    return LED_FAILURE;
}

/*! \fn         led_status_i(index, state)
 * 
 *  \brief      Returns the status of the LED through pointer value
 * 
 *  \param      index       index in the array LedState[]
 *  \param      state       status of the led (pointer)
 * 
 *  \return     SUCCESS if some conditions
 */
int led_status_i (int index, uint8_t* state) 
{
    if (index < LED_COUNT) {
        LED_DEF led = LedState[index];
        return led_status(&led, state);
    }
    return LED_FAILURE;
}

/*! \fn         led_init(LedStateFlash)
 * 
 *  \brief      Initilize all LED's to the OFF state according to their
 *              polarization (active low - active high) 
 *
 *	\param		Static array pointer
 * 
 *  \return     SUCCESS
 */
int led_init (const LED_DEF* led)
{
	int i;
	for (i=0; i<LED_COUNT; i++) {  
        LedState[i] = led[i];    // i * sizeof(LED_DEF);
        led_off (&LedState[i]);
	}      
    elapsed_time_100ms();        // lastCoreTime to be initialized
	return LED_SUCCESS;
}

/*! \fn         led_update_counter(led)
 * 
 *  \brief      Update internal counter of the led
 * 
 *  \param      led         pointer to LED_DEF
 * 
 *  \return     counter value (on multiple of 100ms)
 */

uint8_t led_update_counter (LED_DEF* led)
{   
    // check if the led is active, otherwise don't update
    if (led->period.high && led->period.low) {
        led->count++;  // new 100ms tick
    }
    if (led->period.offset > 0) { // initial delay 
        if (led->count >= led->period.offset) {
            led->count = 0;
            led->period.offset = 0;
        }
    }
    else { // led->period.offset == 0
        if (led->state == LEVEL_LOW) {
            if (led->count > led->period.low) {
                led->count = 0;
                led_on(led);
            }
        }
        else {  // (led->state == LEVEL_HIGH) 
            if (led->count > led->period.high) {
                led->count = 0;
                led_off(led);
            }
        }
    }
    return (led->count);
}

/*! \fn         led_poll(void)
 * 
 *  \brief      Called periodically (at least every 50ms)
 * 
 *  \return     SUCCESS every 100ms
 */

int led_poll ()
{   
    int i, retVal = LED_WAITING;
	if (Elapsed_Time_100ms() > 0) {   
        // Update all leds in LedState[] with the current tick
        for (i=0; i<LED_COUNT; i++) {
            led_update_counter (&LedState[i]);
        }
        retVal = LED_100MS;
	}
	return retVal;
}

int led_tasks (LED_DEF* led)
{
    int retVal = LED_WAITING;
	if (elapsed_time_100ms() > 0 ) {   
        led_update_counter(led);
        retVal = LED_100MS;
	}
	return retVal;
}

// *****************************************************************************
// End of File
//