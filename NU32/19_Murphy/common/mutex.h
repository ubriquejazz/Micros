#ifndef __MUTEX_H
#define __MUTEX_H

/*!\name      mutex.h
 *
 * \brief     Mutual exclusion of a shared resource  
 *			  
 * \author    Juan Gago
 *
 */ 

#define LOCKED		1
#define UNLOCKED	0

// Result of these functions
typedef enum {
	ERR_MUTEX_SUCCESS = 0,
	ERR_MUTEX_FAILURE = -1,
	ERR_MUTEX_UNKNOWN = -99,
} ERR_MUTEX;

#define MUTEX_SIZE			// length of mutex array

int  mutex_init();			// initialize of mutex array
int  mutex_new();			// handle to a free location
int  mutex_destroy();		// free an used mutex
int  mutex_lock(int);		// attempt to lock the resource
int  mutex_unlock(int);		// release the lock (handle)
bool mutex_isLocked(int);	// status of the lock (handle)

#endif