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
#define BUFFER_LEN  	99 // length of the buffer

typedef struct BASE Base;

struct BASE {
	int data;
	Base* self;
};

bool buffer_init();
bool buffer_empty();
bool buffer_full();
int  buffer_write(Base);
int  buffer_read(Base*);

#endif