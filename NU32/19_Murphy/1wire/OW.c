#include "OW.h"
#include "timing.h"
#include "mutex.h"

// Private variables
static PIN_DEF 	PinOne;
static int		MutexOneWire;

int OW_init(PIN_DEF pin) 
{
	static bool firstTime = true;
	if (firstTime) {
        MutexOneWire = mutex_new();
        firstTime = false;
    }
	PinOne = pin;
	return MutexOneWire;
}

/*******************************************************************************
 End of File
 */
