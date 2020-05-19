#ifndef __BUFFER_H
#define __BUFFER_H

#include "request.h"

/*!\name      buffer.h
 *
 * \brief     Driver pseudo-client (not Harmony) 
 *
 *
 * \remark    Circular buffer, server reads while not empty.
 *			  Clients write in at more random rate (faster)
 *			  Server (DRV_X_Init, DRV_X_Task); Client (DRV_X_Transmit)
 *			  are part of the System Level API
 *			  
 * \author    Juan Gago
 *
 */ 

// Result of these functions
typedef enum {
	ERR_BUFF_SUCCESS = 0,
	ERR_BUFF_FAILURE = -1,
	ERR_BUFF_UNKNOWN = -99,
} ERR_BUFF;

int  buffer_init(int);				// initialize state to 0
bool buffer_empty(int);				//
bool buffer_full(int);				//
int  buffer_write(int, BUFFER_REQUEST);	// add an element to the buffer; assume not full
BUFFER_REQUEST* buffer_read(int);		// read from current location; assume not empty

#endif

#include "buffer.h"

static uint8_t read[DRIVER_INST];    					// read buf index
static uint8_t write[DRIVER_INST];   					// write buf index
static BUFFER_REQUEST Driver[DRIVER_INST][REQ_PER_DRV]; // data

int buffer_empty(int i) {    // return true if the buffer is empty (read = write)
  return read[i] == write[i]; 
}

int buffer_full(int i) {     // return true if the buffer is full.  
  return (write[i] + 1) % REQ_PER_DRV == read[i]; 
}

/*******************************************************************************
 End of File
 */