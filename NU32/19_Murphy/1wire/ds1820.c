#include "ds1820.h"
#include "OW.h"     // OW_reset_pulse, OW_read_byte, ...
#include "OWRom.h"  // skip_rom(), match_rom(), etc.

void convert_temperature () 
{
    OW_reset_pulse();
    skip_rom();  
    OW_write_byte(0x44);    // convert command
}


void read_power () 
{
    OW_reset_pulse();
    skip_rom();  
    OW_write_byte(0xB4);    // power-supply command
}

/*******************************************************************************
 End of File
 */