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

bool mutex_init();
int  mutex_new();			// return a handle to a free location
bool mutex_destroy();		// free an used mutex

bool mutex_lock(int);		// attempt to lock the resource
bool mutex_unlock(int);		// release the lock
bool mutex_isLocked(int);	// 

#endif