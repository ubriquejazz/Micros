#include "ow_search.h"
#include "ow.h"

/*!\name      drv_ow_search.c
 *
 * \brief     OW Driver: search for all devices on the bus 
 *
 * \author    Juan Gago
 *
 */ 

uint8_t 	ROM_NO[8];				// Last device address
static int	LastDiscrepancy;		// Algorithm
static int	LastFamilyDiscrepancy;	// Algorithm
static int	LastDeviceFlag;			// Algorithm
static uint8_t crc8;				// ROM_NO[7]

void reset_counters() {

}

void search_loop() {

}

int OW_Search() {

}

/* API */

uint64_t OW_GetRom64bit() {

}

int OW_First() {

}

int OW_Next() {

}

/*******************************************************************************
End of File
*/