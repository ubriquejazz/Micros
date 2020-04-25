s#ifndef __BAR_CODE_DECODE_H
#define __BAR_CODE_DECODE_H

#include <stdio.h>
#include "1wirecontrol.h"   // OWDeviceInfo (OWchipTypes) 

/*! \file   barcode.h

    \brief  Decode the information stored on the 2D barcode or EEPROM

    This is not stored anywhere. The requested section is located and passed back.
    If the information is from an EEPROM then a write can also be performed.
*/   

#define REMOVE_ALL_CRC_CHECKS       1   // 0 if checks are in
/* Set to remove all CRC checking from the 1-wire data received  */
/* This is not a great idea as errors will no be detected but    */
/* unfortunately as the devices are programmed incorrectly this  */
/* has to be done now                                            */

//! These are fixed in all types of EEPROM layouts
#define EEP_FULL_SIG    0x0000  //!< Start of the full signature
#define EEP_SIG_LOW     0x0000  //!< Low byte of the signature
#define EEP_SIG_HIGH    0x0001  //!< High byte of the signature
#define EEP_VERSION     0x0002  //!< EEPROM Layout version information

// ! Other data
#define EEP_BOARD_ID    0x0003  //!< ID of the board
#define EEP_CRC_0       0x001F  //!< CRC for first section
#define EEP_BARCODE     0x0020  //!< Infomation that should be on the barcode
#define EEP_CRC_1       0x007F  //!< CRC for bardcode area
#define EEP_OPTIONAL    0x0080  //!< Optional data sting information
#define EEP_CRC_2       0x01FF  //!< CRC for the optional section

#define MAX_EEPROM_SIZE 0x0200  //!< Max size of the EEPROM on a board

// Maximum number of barcode values
#define BARCODE_MAX     EEP_CRC_1 - EEP_BARCODE

// All the points to calculate the CRC for each section
#define SECTION_1_START 0x00
#define SECTION_1_END   0x1E
#define SECTION_1_CRC   0x1F
#define SECTION_2_START 0x20
#define SECTION_2_END   0x7E
#define SECTION_2_CRC   0x7F
#define SECTION_3_START 0x80
#define SECTION_3_END   0x1FE
#define SECTION_3_CRC   0x1FF
#define BOARD_ID_SEC_1  0x03

// Values to return when ...
#define PACKET_ERROR	1     //!< Return an error found.
#define PACKET_OK	    0     //!< return OK.
#define IGNORE_ERRORS   1     //!< Ignore errors on the 1-wire chipe
#define REPORT_ERRORS   0     //!< Report all errors found on 1-wire

// Values to return when the board is tested for validity
#define EEPROM_ALL_VALID     0
#define EEPROM_INVALID_OK   -1
#define EEPROM_INVALID_ERROR 1
//#define EEPROM_NO_FORMAT   2
#define NO_BOARD_TYPE_FOUND  2
#define NO_VALID_DEVICE      -1
#define FAILED_TO_SET_DATA   -2

//! Look through all the devices on the bus returning the first requested type
/*! @param *DevControl Pointer to pass the found address back to
    @param ModTypeFind The type of module to be locateds
    \return 0 - No device found, 1 - Device found even if not correct
*/
int ModuleBoardLocate(OWDeviceInfo *DevControl, OWchipTypes ModTypeFind);

//! Return a single char from the eeprom at requested address
/*! @param *DevControl Structure of the device to get infomation from
    @param Address of the byte to read
    \return Single char from the barcode section or Null termination
*/
char ModuleBoardByteRead(OWDeviceInfo *DevControl,int Address);

//! Write a single char to the eeprom at the requested address
/*! @param *DevControl Structure of the device to get infomation from
    @param Address of the byte to write
    \return Single char from the barcode section or Null termination
*/
char ModuleBoardByteWrite(OWDeviceInfo *DevControl,int Address, unsigned char);

//! Return a single char from the eeprom section
/*! @param *DevControl Structure of the device to get infomation from
    @param NewContin 1 - Start a new read 0 - Read from last point
    \return Single char from the barcode section or Null termination
*/
char ModuleBoardId(OWDeviceInfo *DevControl,int NewContin);

//! Return a single char from the barcode section
/*! @param *DevControl Structure of the device to get infomation from
    @param NewContin 1 - Start a new read 0 - Read from last point
    \return Single char from the barcode section or Null termination
*/
char ModuleBarCodeGet(OWDeviceInfo *DevControl,int NewContin);

//! Set a single char from the barcode section
/*! @param *DevControl Structure of the device to get infomation from
    @param NewContin 1 - Start a new read 0 - Read from last point
    @param SetVal Value to set the barcode point to
    \return Single char from the barcode section or Null termination
*/
char ModuleBarCodeSet(OWDeviceInfo *DevControl,int NewContin, char SetVal);

//! Remove the gap of chars from the area shifting all left
/*! @param *DevControl Structure of the device to get infomation from
    @param FirstPosChar Position of the first char in the barcode data to remove
    @param CharCount The number of chars to remove from the string
    \return Ok or not ok
*/
int ModuleBarCodeRemove(OWDeviceInfo *DevControl, int FirstPosChar, int CharCount);

//! Get the requested value from barcode data.
/*! @param SectionID is one of the delimited values
    @param *StringData single parameter is returned as a string
    @param ignoreError 1 = ignore the errors 0 = return errors
    \return Pass or fail information about the acction
*/
int GetValue(char SectionID, char *StringData, int ignoreError);

//! Set the requested value to barcode data.
/*! @param SectionID is one of the delimited values
    @param *StringData single parameter is passed as a string
    \return Pass or fail information about the acction
*/
int SetValue(char SectionID, char *StringData, int ignoreError);

//  clear the requested value to barcode data.
/*  @param SectionID is one of the delimited values
    @param *StringData single parameter is passed as a string
    \return Pass or fail information about the acction
*/
int ClearValue(char SectionID);

    #if(1)

    //! Read a device chip signature
    /*! @param *DevControl Structure of the device to get infomation from
         \return The type of device at this address
    */
    OWchipTypes ReadSignature(OWDeviceInfo *DevControl);

    //  Take the data between the two points and check the crc value
    /*  @param start First byte to start the check
        @param end Last byte to check in the list
        @param crcPoint Location the CRC is placed
        \return Pass or fail information about the acction
    */
    int checkCRC(OWDeviceInfo *ActiveDevice, unsigned int start,
                 unsigned int end, unsigned int crcPoint);

    //  Take the data between the two points and check the crc value
    /*  @param start First byte to start the check
        @param end Last byte to check in the list
        @param crcPoint Location the CRC is placed
        \return Pass or fail information about the acction
    */
    int setCRC(OWDeviceInfo *ActiveDevice, unsigned int start,
               unsigned int end, unsigned int crcPoint);

    //! 1 byte CRC check
    /*! @param value Value to add to the byte crc
    	 \return last calculated crc
    */
    unsigned char do_crc8(unsigned char value);
    #endif

#endif

