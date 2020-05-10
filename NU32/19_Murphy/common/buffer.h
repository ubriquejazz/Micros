#ifndef __BUFFER_H
#define __BUFFER_H

/*!\name      buffer.h
 *
 * \brief     Circular buffer, server reads while not empty.
 *			  Clients write in at more random rate (faster)
 *
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

#define BUFFER_SIZE	99 	// length of the buffer
#define BUFFER_INST 1	//

typedef struct BASE Base;

struct BASE {
	int 	state;		// write (1); read (0)
	Base* 	self;		// pointer to something
};

int  buffer_init(int);				// initialize state to 0
bool buffer_empty(int);				//
bool buffer_full(int);				//
int  buffer_write(int, Base*);		// add an element to the buffer; assume buffer not full
Base* buffer_read(int);				// read from current location; assume buffe not empty

#endif