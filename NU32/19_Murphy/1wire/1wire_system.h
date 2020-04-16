#ifndef __1WIRE_SYSTEM_H
#define __1WIRE_SYSTEM_H

#include <stdio.h>

//! Max number of devices can be found and tracked
/*! If more than this number are on the bus then only
	 up to this value will be returned the rest will be lost
	 The first devices found will be used. If more that the max 
	 are on the bus it's no garantee that the same ones will always be found
*/
#define MAX_1_WIRE_DEVICES		3

//! Types of device on the bus
/*! The types are for module id not 1-wire device.
	 This information is found from the first 2 bytes read 
	 from the memory device
*/
typedef enum _OWchipTypes
{
	INACTIVE_ADDRESS = 0,  	//!< Slot not in use
	ACTIVE_MODULE,		//!< Is a testable module
	ACTIVE_ADDAPTER,	//!< Is a addapter for the system
	SLOT_DETECTOR,		//!< Module fitted detector system card
	UNKNOWN_DEVICE,		//!< Device is not recognised
} OWchipTypes;

//! 1-Wire device type ID
/*! List of all the valid device types on the bus
*/
typedef enum _OWdeviceIds
{
	TYPE_ID_MEMORY_1 		= 0x23,
    TYPE_ID_MEMORY_2 		= 0x2D,
    TYPE_ID_DS18B20Z        = 0x28,
	TYPE_ID_SLOT_CHECK  	= 0x10,
}OWdeviceIds;

//! Track active devices in the system
/*! \todo  Change type value ot OWchipTypes
*/
typedef struct _OWDeviceInfo
{
	unsigned char Address[8];  //!< Address of the device
	unsigned char Type;        //!< Type of device
} OWDeviceInfo;


//! Count the number of a selected device type is on the bus
/*! Loop counting the number of devices on the bus
	@param OWdeviceIds Device ID to be counted
*/
int System1WireCount(OWdeviceIds DevType);

//! Write a single byte to the given device address and byte location
/*! @param Device[] Address of device as loaded into \a Address of OWDeviceInfo
	 @param MemLoc Location within the memory device to write to
	 @param Data Single byte of data to be writen to the location
	 \return Status of the request (1 - Pass, 0 - fail)
*/ 
int System1WireWriteByte(unsigned char Device[], unsigned int MemLoc, unsigned char Data);

//! Read a single byte from a given device address and byte location
/*! @param Device[] Address of device as loaded into \a Address of OWDeviceInfo
	 @param MemLoc Location within the memory device to read from
	 @param *Data Location to place the single byte of data read
	 \return Status of the request (1 - Pass, 0 - fail)
*/
int System1WireReadByte(unsigned char Device[], unsigned int MemLoc, unsigned char *Data);

#endif
