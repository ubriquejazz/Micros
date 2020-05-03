#ifndef __OW_REQUEST_H
#define __OW_REQUEST_H

#include "buffer.h"

// buffer pointer type.  The buffer is shared by an ISR and mainline code.
// the pointer to the buffer is also shared by an ISR and mainline code.
// Hence the double volatile qualification
typedef volatile unint8_t * volatile buffer_t;
typedef int (*callback_t) (uint16_t, uint16_t);

typedef enum 
{
	OW_REQ_STATE_INIT,
	OW_REQ_STATE_MATCH,
	OW_REQ_STATE_WRITE,
	OW_REQ_STATE_READ,
	OW_STATE_CALLBACK,
	OW_REQ_STATE_ERROR,
} OW_REQ_STATES;

typedef struct 
{
	Base*		base;
	unint64_t	rom_no;		// address
	buffer_t	to_write;	// data + command + ...
	buffer_t	to_read;	// NULL for a write request
	uint8_t		wlen, rlen;	// rlen = 0 for a write request
	callback_t	pfunc;		// pointer to a function
} OW_Request;

int OW_new_request(OW_Request* req, ...);

OW_Request* OW_request_read();

int OW_request_poll();

#endif;