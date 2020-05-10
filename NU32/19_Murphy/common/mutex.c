#include "mutex.h"

static int  mutex_state[MUTEX_SIZE];	// lock or unlock
static bool mutex_avail[MUTEX_SIZE];	// available or not

int mutex_init()
{
	for (i=0; i < MUTEX_SIZE; i++) {
		mutex_state[i] = UNLOCKED;
		mutex_avail[i] = true;
	}
	return ERR_MUTEX_SUCCESS;	
}
          
/*******************************************************************************
 End of File
 */
