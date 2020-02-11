/*!\name  		smm_i2c.c
 *
 * \brief     	Defined channels A and B
 *
 * \author    	Juan Gago
 *
 */

#include <stdio.h>
#include "clients.h"
#include "i2client_common.h"

I2C_DRIVER BusA;
I2C_CLIENT client;

void APP_Tasks(void) 
{
	uint16_t temperature;
	static uint32_t tout_a, tout_b;

	switch(appdata.state) 
	{
		case APP_STATE_INIT:
			RFE1600_Rd_Word (&client, 0x2E, 0x98);
			if (I2C_Initialize (&BusA, &client, 1)) {	// BusA.state1 = I2C_CLOSE_IDLE;
				appdata.state = APP_STATE_OPENING;		// BusA.state0 = I2C_OPEN_INIT;
			}
			break;
		  
		case APP_STATE_OPENING:
			if (I2C_OpenDriver (&BusA, &tout_a)) {
				appdata.state = APP_STATE_PENDING;		// BusA.state0 = I2C_OPEN_DONE;
			}
		  	break;

		case APP_STATE_PENDING:
			if (I2C_IsComplete (&BusA, &tout_a)) {
				temperature = RFE1600_Temp (client.read[1], client.read[0]);
				appdata.state = APP_STATE_CLOSING;		// BusA.state1 = I2C_CLOSE_INIT;
			}
		  	break;

		case APP_STATE_CLOSING:
			if (I2C_CloseDriver (&BusA, &tout_a)) {
				appdata.state = APP_STATE_WAIT;			// BusA.state1 = I2C_CLOSE_DONE;
			}
		  	break;

		default:
		  	;// logic error, impossible state!
	}
}
