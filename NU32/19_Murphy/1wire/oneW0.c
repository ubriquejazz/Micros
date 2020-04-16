/*!\name      oneW0.c
 *
 * \brief     Client for the 1W driver
 *
 * \author    Juan Gago
 */

#include "oneW0.h"
#include "led_common.h"
#include "1wire_common.h"   // OW_read_byte(), etc.
#include "1wire_system.h"   // System1WireCount
#include "barcode.h"        // SetValue, GetValue

static int elapsed_time(uint32_t interval)
{
    static uint32_t lastCoreTime;
    static bool firstTime = true;
    uint32_t thisCoreTime = _CP0_GET_COUNT();

    if(firstTime) {
        lastCoreTime = thisCoreTime;
        firstTime = false;
    }

    int distance; // ticks since the last call
    if (thisCoreTime > lastCoreTime) {
        distance = thisCoreTime - lastCoreTime;
    }
    else {
        distance = 0xFFFFFFFF - lastCoreTime + thisCoreTime;
    }

    int error = distance - interval; // if over 0 then 100ms has passed
    if (error > 0) {
        lastCoreTime = thisCoreTime + error; // carried fowared in the next interval
    }
    return error;
}

float one_read_temperature ()
{
    uint8_t tempL, tempH;
    float aux;
    tempL = OW_read_byte();
    tempH = OW_read_byte();
    aux = ((uint8_t)tempH << 8 ) + (uint8_t)tempL;
    return 6.25 * aux;    
}

void task_phase0() {

}

void task_phase1() {    // Conversion DS18B20 device
    OW_reset_pulse();
    OW_write_byte(DS18B20_Skip_ROM);        
    OW_write_byte(DS18B20_Convert_T);  
}

void task_phase2(char* result) {    // Reading DS18B20 device
    OW_reset_pulse();
    OW_write_byte(DS18B20_Skip_ROM);        
    OW_write_byte(DS18B20_Read_Scratchpad); 
    sprintf(result, "%.2f", one_read_temperature()); 
}

void task_phase3(char* buffer) {    // Reading the 1W PROM (serial number)
    OW_reset_pulse();
    GetValue(MODULE_TYPE, buffer, IGNORE_ERRORS);
}

// Application Initialization and State Machine Functions

int one_init ( int pin )
{
    int deviceCnt = 0;
    PIN_DEF* ptr = ping_get(pin);
    PinOne = *ptr;
    elapsed_time(1000 * ms_SCALE);  // lastCoreTime initialized
    deviceCnt += System1WireCount(TYPE_ID_DS18B20Z);
    deviceCnt += System1WireCount(TYPE_ID_MEMORY_1);
    return deviceCnt;
}

int one_pull ( uint32_t* milliseconds )
{
    static int count = 0;
    char buffer[32];

    // (*milliseconds > 1000)
    if (elapsed_time(1000 * ms_SCALE) > 0)
    {
        if (count == 1) {
            task_phase1();
        }
        else if (count == 2) {
            task_phase2(buffer);
        }
        else if (count == 3) {
            task_phase3(buffer);
        }
        else {
            task_phase0();
            count = 0;
        }
        count++;
        // *milliseconds = 0;

    }
    return count;
}

/*******************************************************************************
 End of File
 */
