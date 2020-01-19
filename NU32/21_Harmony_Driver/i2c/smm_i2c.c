/*!\name  		smm_i2c.c
 *
 * \brief     	Defined channels A and B
 *
 * \author    	Juan Gago
 *
 */

#include <stdio.h>
#include "clients.h"
#include "smm_i2c.h"
#include "i2client_common.h"

I2C_OBJECT BusA, BusB;

uint8_t LastMessageA[8];
uint8_t LastMessageB[8];
uint32_t tout_a, tout_b;

I2C_CLIENT* ptra = NULL, ptrb = NULL;

void SMM_I2C_Add_Sensor () {
	I2C_CLIENT client;
	MCP9808_Rd_Word (&client, 0x30, 0x05);
	I2C_Add (&client, &BusA);
}

void SMM_I2C_Add_PSU () {
	I2C_CLIENT client;
	RFE1600_Rd_Word (&client, 0x2E, 0x98);
	I2C_Add (&client, &BusB);
}

void SMM_I2C_Initialize(){
	I2C_Initialize (&BusA, 0);
	I2C_Initialize (&BusB, 1);
}

int SMM_I2C_Tasks ()
{
	uint16_t temperature;

	if ( ptra && (ptra->state != I2C_CLIENT_NONE) ) {
		if (ptra->state == I2C_CLIENT_REQ) {
			I2C_Tasks(ptra, &tout_a, LastMessageA);
		}
		else if (ptra->state == I2C_CLIENT_COMPLETE) {
			/* Process */
			if (ptra->address == 0x30) {
				temperature = MCP9808_Temp (LastMessageA[1], LastMessageA[0]);
			}
			else if (ptra->address == 0xA0) {
				memcpy(hostname, LastMessageA, 8);
			}
			memset(LastMessageA, 0, 8);
			ptra->state = I2C_CLIENT_USED;
		}
		else {	// ERROR or TIMEOUT
			I2C_Tasks(ptra, &tout_a, LastMessageA);
			if (BusA.drv.state == I2C_DRV_DONE) {
				ptra->state = I2C_CLIENT_NONE;
				BusA.drv.state = I2C_DRV_IDLE;
			}
		}
	}

	if ( ptrb && (ptrb->state != I2C_CLIENT_NONE) ) {
		if (ptrb->state == I2C_CLIENT_REQ) {
			I2C_Tasks(ptrb, &tout_b, LastMessageB); 
		}
		else if (ptrb->state == I2C_CLIENT_COMPLETE) {
			/* Process */
			if (ptrb->address == 0x2E) {
				temperature = RFE1600_Temp (LastMessageB[1], LastMessageB[0]);
			}
			memset(LastMessageB, 0, 8);
			ptrb->state = I2C_CLIENT_USED;
		}
		else {	// ERROR or TIMEOUT
			I2C_Tasks(ptrb, &tout_b, LastMessageB); 
			if (BusB.drv.state == I2C_DRV_DONE) {
				ptrb->state = I2C_CLIENT_NONE;
				BusB.drv.state = I2C_DRV_IDLE;
			}
		}
	}

	// reentrance issue: ptra and ptrb reference to the same client?
	ptra = I2C_Get (&BusA);
	ptrb = I2C_Get (&BusB);
	return 0;
}
