#ifndef __1WIRECONTROL_H
#define __1WIRECONTROL_H

#include <stdio.h>

/*! \file 	1wirecontrol.h

    \brief 	Control and locate an actual device
	
	 Drive an actual device on the 1-Wire bus by addressing
	 it directly. Search for all devices on the bus. 
*/

// Byte information for the sections in the serial number
#define ID_BYTE                 0  //!< Device ID byte location
#define SERIAL_BYTE_1			1  //!< Byte 1 of address
#define SERIAL_BYTE_2			2  //!< Byte 2 of address
#define SERIAL_BYTE_3			3  //!< Byte 3 of address
#define SERIAL_BYTE_4			4  //!< Byte 4 of address
#define SERIAL_BYTE_5			5  //!< Byte 5 of address
#define SERIAL_BYTE_6			6  //!< Byte 6 of address
#define SERIAL_BYTE_7			7  //!< Byte 7 of address
#define CRC_BYTE_4              8  //!< CRC Byte for ID + address

// All commands used on the 1-wire 
#ifndef SEARCH_ROM_CMD
	#define SEARCH_ROM_CMD		0xF0  //!< Search command for all devices
#endif

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

//! Get a pointer to the location of the found address
/*! \param void Nothing passed in
	\return uint8_t pointer to the EEPROM address
*/
uint8_t* GetAddresLoc(void);

//! Check if a device is on the bus
/*! \return 1 - Device on bus 0 - No Device
*/
unsigned char Detect_Slave_Device(void);

//! Find the first device on the 1-Wire bus
/*! \return TRUE - ROM number in \a ROM_NO  FALSE - no device present
*/
int OWFirst(void);

//! Find the next device on the 1-wire bus
/*! \return TRUE - ROM number in \a ROM_NO  FALSE - no device present
*/
int OWNext(void);

	#if(1)

	/*! \brief Perform the 1-Wire Search Algorithm on the 1-Wire bus using the existing
		search state.
	*/
	/*! \return TRUE - device found, ROM number in \a ROM_NO buffer  FALSE - no device present
	*/
	int OWSearch(void);

	//! 1 byte CRC check	
	/*! @param value Value to add to the byte crc
		 \return last calculated crc
	*/
	unsigned char docrc8(unsigned char value);

	#endif

#endif
