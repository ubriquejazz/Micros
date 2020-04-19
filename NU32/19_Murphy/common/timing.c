#include "timing.h"

void DelayMs(uint32_t msDelay)
{
    register uint32_t startCnt = _CP0_GET_COUNT();
    register uint32_t waitCnt = msDelay * ms_SCALE;    
    while (_CP0_GET_COUNT() - startCnt < waitCnt);
}

void DelayUs(uint32_t usDelay)
{
    register uint32_t startCnt = _CP0_GET_COUNT();
    register uint32_t waitCnt = usDelay * us_SCALE;    
    while (_CP0_GET_COUNT() - startCnt < waitCnt);
}

/*! \fn         elapsed_time_100ms (void)
 * 
 *  \brief      Read the core timer and indicate if the 100ms has already 
 *              occurred from the last time the function was called. 
 * 
 *  \remark     Called periodically at least 50% faster (50ms)
 *              The first time, it initializes an internal static variable
 *  
 *  \return     Core timer ticks up to the next 100ms tick. 
 *              Negative if it is too soon, positive if it has already happened 
 */
static int Elapsed_Time_100ms ()
{ 
    static uint32_t lastCoreTime; // keep track of where core timer was at
    static bool firstTime = true; // to initialize lastCoreTime
    uint32_t thisCoreTime = _CP0_GET_COUNT(); // current core timer tick
    
    if (firstTime) {
        lastCoreTime = thisCoreTime;
        firstTime = false;
    }

    int distance; // number of ticks since the last call    
    if (thisCoreTime > lastCoreTime) {
        distance = thisCoreTime - lastCoreTime;
    }
    else {  
        // Overflow state will require 'distance' to be adjusted 
        distance = (MAX_CORE_TIMER - lastCoreTime) + thisCoreTime;
    }
    
    // If the error is over 0 then the 100ms has passed
    int error = distance - MS100_CORE_TICKS;
    if (error > 0) {
         // Any difference is carried forward to the next 100ms interval 
        lastCoreTime = thisCoreTime + error;
    }
    return error;
}

/*! \fn         elapsed_time_100ms (void)
 * 
 *  \brief      Read the core timer and indicate if the 100ms has already 
 *              occurred from the last time the function was called. 
 * 
 *  \remark     Called periodically at least 50% faster (50ms)
 *              The first time, it initializes an internal static variable
 *  
 *  \return     Core timer ticks up to the next 100ms tick. 
 *              Negative if it is too soon, positive if it has already happened 
 */
static int Elapsed_Time (uint32_t interval)
{ 
    static uint32_t lastCoreTime; // keep track of where core timer was at
    static bool firstTime = true; // to initialize lastCoreTime
    uint32_t thisCoreTime = _CP0_GET_COUNT(); // current core timer tick
    
    if (firstTime) {
        lastCoreTime = thisCoreTime;
        firstTime = false;
    }

    int distance; // number of ticks since the last call    
    if (thisCoreTime > lastCoreTime) {
        distance = thisCoreTime - lastCoreTime;
    }
    else {  
        // Overflow state will require 'distance' to be adjusted 
        distance = (MAX_CORE_TIMER - lastCoreTime) + thisCoreTime;
    }
    
    // If the error is over 0 then the 100ms has passed
    int error = distance - interval;
    if (error > 0) {
         // Any difference is carried forward to the next 100ms interval 
        lastCoreTime = thisCoreTime + error;
    }
    return error;
}

          
/*******************************************************************************
 End of File
 */
