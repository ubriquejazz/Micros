#ifndef __BUFFER_H
#define __BUFFER_H

/*!\name      buffer.h
 *
 * \brief     Circular buffer, one server reads while not empty.
 *			  Clients write in at more random rate (faster)
 *
 *			  
 * \author    Juan Gago
 *
 */ 

#define BUFFER_OK		0
#define BUFFER_ERR		-1
#define BUFFER_LEN  	1024		// length of the buffer

// buffer pointer type.  The buffer is shared by an ISR and mainline code.
// the pointer to the buffer is also shared by an ISR and mainline code.
// Hence the double volatile qualification
typedef volatile unint8_t * volatile buffer_t;
typedef int (*callback_t) (int, int);

// Request
typedef struct 
{
	buffer_t	to_write;	// address + command + ...
	buffer_t	to_read;	// NULL for a write request
	int 		wlen, rlen;	// rlen = 0 for a write request
	callback_t	pfunc;		// pointer to a function
} OW_REQ;

bool buffer_init();
bool buffer_empty();
bool buffer_full();
int  buffer_write(int);
int  buffer_read(int*);

#endif