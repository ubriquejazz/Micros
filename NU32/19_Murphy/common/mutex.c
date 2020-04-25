#include "mutex.h"

bool mutex_init()
{
	for (i=0; i < MUTEX_COUNT; i++) {
		Mutex[i] = UNLOCKED;
		Availabe[i] = true;
	}
	return true;	
}

int mutex_new()
{
	while(!Availabe[i]){
		i++;
	}
	return i; // handle to a free location
}

bool mutex_lock(int)
{}		// attempt to lock the resource

bool mutex_unlock(int)
{}		// release the lock

bool mutex_isLocked(int)
{}	// 
          
/*******************************************************************************
 End of File
 */
