#include "buffer.h"

// buffer pointer type.  The buffer is shared by an ISR and mainline code.
// the pointer to the buffer is also shared by an ISR and mainline code.
// Hence the double volatile qualification
typedef volatile unint8_t * volatile buffer_t;
typedef int (*callback_t) (uint16_t, uint16_t);

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

int OW_request_read(OW_Request* value);

int OW_request_write(OW_Request value);

int OW_request_poll();