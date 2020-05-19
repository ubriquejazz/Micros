#ifndef __REQ_H
#define __REQ_H

/*!\name      request.h
 *
 * \brief     Driver request (Harmony or not) 
 *			  
 * \author    Juan Gago
 *
 */ 

typedef enum 
{
	BUFFER_ST_INVALID = 0,
	BUFFER_ST_COMPLETE,
	BUFFER_ST_ERROR
} BUFFER_ST;

typedef struct 
{
	BUFFER_HANDLE	bhandle,		// extern definition
	BUFFER_ST 		state;			// 
	BUFFER_HANDLER 	pfunc;			// extern definition
	int 			wlen, rlen;		//
	uint8_t*		write;			//
	uint8_t*		read;			//
} BUFFER_REQUEST;

// Result of these functions
typedef enum {
	ERR_REQ_SUCCESS = 0,
	ERR_REQ_FAILURE = -1,
	ERR_REQ_UNKNOWN = -99,
} ERR_REQUEST;

int request_init(BUFFER_REQUEST*, uint8_t*, int, uint8_t*, int);
int request_valid(BUFFER_HANDLE);
int request_complete(BUFFER_ST);

#endif