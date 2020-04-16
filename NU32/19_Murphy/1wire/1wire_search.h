#ifndef __1WIRE_SEARCH_H
#define __1WIRE_SEARCH_H

#include <stdio.h>

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

/*! \file 	1wire_search.h

    \brief 	Control and locate an actual device
	
	 Drive an actual device on the 1-Wire bus by addressing
	 it directly. Search for all devices on the bus. 
*/

//! Get a pointer to the location of the found address
/*! \param void Nothing passed in
	\return uint8_t pointer to the EEPROM address
*/
uint8_t* GetAddresLoc(void);

//! Find the first device on the 1-Wire bus
/*! \return TRUE - ROM number in \a ROM_NO  FALSE - no device present
*/
int OWFirst(void);

//! Find the next device on the 1-wire bus
/*! \return TRUE - ROM number in \a ROM_NO  FALSE - no device present
*/
int OWNext(void);

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
