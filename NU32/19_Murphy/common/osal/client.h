#ifndef __CLIENT_H
#define __CLIENT_H

#include "request.h" 

/*!\name      client.h
 *
 * \brief     Driver client (Harmony only) 
 *			  
 * \author    Juan Gago
 *
 */ 

typedef enum 
{
	CLIENT_ST_INVALID = 0,
	CLIENT_ST_OPENED,
	CLIENT_ST_ERROR
} CLIENT_ST;

typedef struct 
{
	DRV_HANDLE		handle,					//
	CLIENT_ST		state;					// 
	DRV_CLIENT_DATA	data;					// Params for ClientConfigure()
	BUFFER_REQUEST	buffer[REQ_PER_CLI];	//
	uint64_t		baudrate;
} CLIENT_OBJ;

// Result of these functions
typedef enum {
	ERR_CLIENT_SUCCESS = 0,
	ERR_CLIENT_FAILURE = -1,
	ERR_CLIENT_UNKNOWN = -99,
} ERR_CLIENT;

int client_open(int, int, uint64_t);
int client_close(int);
BUFFER_REQUEST* client_get(int, int);
int client_enable(int, int);

#endif


static CLIENT_OBJ Client[CLIENT_COUNT];

/*******************************************************************************
 End of File
 */
