#include "OWClient.h"
#include "OWSearch.h"
#include "timing.h"

/* Non blocking tasks */

void task_phase0 () {
}

void task_phase1 () {
}

void task_phase2 () {
}

void task_phase3 () {
    char buffer[32];
    // reading the 1W PROM (serial number)
    // GetValue(MODULE_TYPE, buffer, IGNORE_ERRORS);
}

bool one_write_prom (char* payload)
{
    bool retVal=true;
//  SetValue(MODULE_TYPE, payload, IGNORE_ERRORS);
    DPRINT("__1W ROM %s \r\n", payload);
    return retVal; 
}

/*! \fn         one_init (pin)
 * 
 *  \brief      Initialize the driver and search for devices
 * 
 *  \param      pin - index in the static PIN_DEF[] array
 * 
 *  \return     Number of devices in the bus 
 */
int one_init(int pin)
{
    int deviceCount = 0;
    uint8_t* address;
    PIN_DEF* ptr = pin_get(pin);
    PinOne = *ptr;
    
    if(OWFirst())
    {
        address = OWGetAddresLoc();
        if (*address == 0x28){
            deviceCount++;
        }
        while(OWNext())
        {
            address = OWGetAddresLoc();
            if (*address == 0x28){
                deviceCount++;
            }
        }
    }
    elapsed_time(1000 * ms_SCALE);        // lastCoreTime to be initialized
    return deviceCount;
}

/*! \fn         one_pull ()
 * 
 *  \brief      Called periodically (at least every 500ms)
 * 
 *  \return     Current phase
 */
int one_pull()
{
	static uint8_t count = 0;
    if (Elapsed_Time(1000 * ms_SCALE) > 0) {         
        if (count == 1) {
            task_phase1();
            LED_BLUEOn();
        }
        else if (count == 2) {
            task_phase2();
            LED_BLUEOff();
        }
        else if (count == 3) {
            task_phase3();
            LED_BLUEOn();
        }
        else {
            task_phase0();
            LED_BLUEOff(); 
            count = 0;
        }
        count++;  
        WDTCONbits.WDTCLR = 1; // clear the WDT 
	}
	return count;
}
            
/*******************************************************************************
 End of File
 */