/*! \file 1wirecontrol.c

    \brief Control and locate an actual device
	
	 Drive an actual device on the 1-Wire bus by addressing
	 it directly. Search for all devices on the bus. 
*/

#include "1wire_system.h"
#include "1wire_search.h"   // OW_reset_pulse, OW_read_bit ...
#include "ds2433.h"         // match_rom, read_scratchpad, copy, etc. 

//! Count the number of a selected device type is on the bus
/*! Loop counting the number of devices on the bus
	@param OWdeviceIds Device ID to be counted
*/
int System1WireCount(OWdeviceIds DevType)
{
    int DevCount = 0;

    if(OWFirst())
    {
        if (ROM_NO[0] == DevType){
            DevCount++;
        }
        while(OWNext())
        {
            if (ROM_NO[0] == DevType){
                DevCount++;
            }
        }
    }
    return DevCount;
}

//! Write a single byte to the given device address and byte location
/*! @param Device[] Address of device as loaded into \a Address of OWDeviceInfo
	 @param MemLoc Location within the memory device to write to
	 @param Data Single byte of data to be writen to the location
	 \return Status of the request (1 - Pass, 0 - fail)
*/ 
int System1WireWriteByte(unsigned char Device[], unsigned int MemLoc, 
        unsigned char Data)
{
    int err = 1;
    char ESData;

    // Detect a slave device and read from it
    if(Detect_Slave_Device())
    {
        match_rom(Device);
	    write_scratchpad(MemLoc, Data);
        Detect_Slave_Device();          		        // Redetect the device
        match_rom(Device);
        if (Data == read_scratchpad((uint8_t *)&ESData))
        {
            Detect_Slave_Device();
            match_rom(Device);
            copy_scratchpad(MemLoc, ESData);
            DelayMs(15);
        }
        else
        {
            err = 0;
        }
    }
    else
    {
        err = 0;
    }
    return err;
}

//! Read a single byte from a given device address and byte location
/*! @param Device[] Address of device as loaded into \a Address of OWDeviceInfo
	 @param MemLoc Location within the memory device to read from
	 @param *Data Location to place the single byte of data read
	 \return Status of the request (1 - Pass, 0 - fail)
*/
int System1WireReadByte(unsigned char Device[], unsigned int MemLoc,
        unsigned char *Data)
{
    int err = 1;

    // Detect a slave device and read from it
    if(Detect_Slave_Device())
    {
        match_rom(Device);
        *Data = read_memory(MemLoc);
    }
    else
    {
    	err = 0;
    }
    return err;
}

/*******************************************************************************
 End of File
 */
