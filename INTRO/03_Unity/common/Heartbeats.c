/*!\name      Heartbeats.c
 *
 * \brief     Heartbeats, depends on Blade.c
 *
 * \author    Juan Gago, 2019
 */

#include <stdio.h>

uint8_t HB_Beacons[] = {0, 0, 0, 0, 0};    // issue in i = 2
uint8_t HB_Counter[] = {0, 0, 0, 0, 0};
        
bool HB_Reset (void)
{
    bool retVal = true;
    uint8_t i;
    for (i=0; i<5; i++)
    {
        HB_Beacons[i] = 0;
        HB_Counter[i] = 0;
    }    
    return retVal;
}

uint8_t HB_ReadValue (uint8_t channel)
{
    uint8_t i = channel - 1;
    if (i <= 4)
    {
        return HB_Beacons[i];
    }
    return 0;
}

uint8_t HB_ReadCounter (uint8_t channel)
{
    uint8_t i = channel - 1;
    if (i <= 4)
    {
        return HB_Counter[i];
    }
    return 0;
}

bool HB_NewCallback(uint8_t channel)
{
    bool retVal = false;
    volatile uint8_t i = channel-1;
    
    HB_Beacons[i] = 0;
    HB_Counter[i] = 0;
    return retVal;
}

bool HB_NewBeacon_Simple (uint8_t channel)
{
    bool retVal = false;
    volatile uint8_t i = channel-1;
    HB_Beacons[i] = 1;
    return retVal;
}

bool HB_NewBeacon (uint8_t channel)
{
    bool retVal = false;
    volatile uint8_t i = channel-1;
     
    if (HB_Beacons[i] == 0) // zero means callback
    {
        retVal = true;
    }  
    else // there was no callback in the last iteration
    {
        HB_Counter[i] = HB_Counter[i] + 1;
        if (HB_Counter[i] > HB_NUM_RETRIES)
        {
            BLADE_Uncalibrate(channel);
            HB_Counter[i] = HB_NUM_RETRIES;
            retVal = true;
        }
    }
    HB_Beacons[i] = 1;
    return retVal;
}

/* Unit Test */

uint8_t HB_ConsecutiveBeacons (uint8_t channel, uint8_t iterations)
{
    uint8_t slot;        
    uint8_t i = channel-1;

    HB_Beacons[i] = 0;  // HB_Reset();
    HB_Counter[i] = 0;

    for (slot=0; slot<iterations; slot++)
    {        
        HB_NewBeacon(channel); 
    }
    return HB_ReadCounter(channel);
}

/* Integration Test */

uint8_t HB_Every100ms_Task()
{
    static uint8_t channel = 1;     // channel scanning
    if (channel <= 5)
    {
        HB_NewBeacon(channel);     
        HB_NewCallback(channel);
    }
    else
    {
        channel = 0;
    }                            
    return channel++;
}

/* *****************************************************************************
 * End of File
 */