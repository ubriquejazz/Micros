#include "ds1820.h"
#include "OW.h"
#include "OWSearch.h"     
#include "mutex.h"


// Private variables
static uint16_t RawTemperature;	  // Device specific
static int MutexOneWire, Error;   // Generic
static uint64_t Address;          // Rom Code

void ds1820_init(PIN_DEF pin) 
{
    Error = 1;
    RawTemperature = 0;
    MutexOneWire = OW_init(pin);
    return;
}

int ds1820_search(uint64_t* buffer, int length) 
{
    int deviceCount = 0;
    if (!mutex_isLocked(MutexOneWire))
    {
        mutex_lock(MutexOneWire);
        if(OWFirst())
        {
            uint8_t* ptr;
            ptr = OWGetRomPtr();
            if (ptr[0] == TEMPERATURE_FAMILY_CODE){
                buffer[deviceCount] = OWGetRom64bit();
                deviceCount++;
            }
            while(OWNext()) {
                ptr = OWGetRomPtr();
                if (ptr[0] == TEMPERATURE_FAMILY_CODE){
                buffer[deviceCount] = OWGetRom64bit();
                deviceCount++;
            }
            }
        }
        mutex_unlock(MutexOneWire);
        Error = 0;
    }
    return deviceCount;
}

int ds1820_get_error() {
    return Error;
}

int ds1820_set_device(uint64_t rom_code)
{
  Address = rom_code;
  return TEMP_SUCCESS;
}

/* MUTEX (only) IMPLEMENTATION */

void task_phase1(uint64_t address)
{
    if (!mutex_isLocked(MutexOneWire)) {
        mutex_lock(MutexOneWire);
        OW_reset_pulse();
        match_rom(address);  
        OW_write_byte(TEMP_CMD_CONVERT);
        mutex_unlock(MutexOneWire);
        Error = 1;
    }
    return;
}

void task_phase2 (uint64_t address) 
{
    if (!mutex_isLocked(MutexOneWire)) {
        mutex_lock(MutexOneWire);
        OW_reset_pulse();
        match_rom(address);  
        OW_write_byte(TEMP_CMD_READ_SCRATCHPAD); 
        uint8_t aux = OW_read_byte();
        RawTemperature = (uint16_t)(aux << 8) + OW_read_byte();
        mutex_unlock(MutexOneWire);
        Error = 0;
    }
    return;	
}

void task_phase3 () {
}

void task_phase0 () {
}

int ds1820_poll (LED_DEF* led) 
{
    static uint8_t phase = 0;
    if (phase == 1) {
        task_phase1(Address);
        led_on(led);
    }
    else if (phase == 2) {
        task_phase2(Address);
        led_off(led);
    }
    else if (phase == 3) {
        task_phase3();
        led_on(led);
    }
    else {
        task_phase0();
        led_off(led); 
        phase = 0;
    }
    phase++;
    return phase;
}

float ds1820_get_temperature(int resolution) 
{
    float lookup[] = {0.5, 0.25, 0.125, 0.0625};
    uint16_t decimal = RawTemperature & 0x000F;
    uint16_t lsb    = (RawTemperature & 0x00F0) >> 4;
    uint16_t msb    = (RawTemperature & 0x0700) >> 4;
    if (RawTemperature & 0x0800) 
        return (-1) * (msb + lsb) * decimal * lookup[resolution];
    else
        return  (msb + lsb) * decimal * lookup[resolution];
}

/*******************************************************************************
End of File
*/