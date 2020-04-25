#include "OWSearch.h"
#include "OW.h"   // OW_reset_pulse, OW_read_bit ...
#include "crc8.h"

uint8_t   ROM_NO[8];
int       LastDiscrepancy;
int       LastFamilyDiscrepancy;
int       LastDeviceFlag;

/*! \fn         OWGetRomPtr ()
 * 
 *  \brief      Get a pointer to the location of the found address
 *  \return     pointer to the EEPROM address
 *
 */ 
uint8_t* OWGetRomPtr(void) {
    return &ROM_NO[0];
}

/*******************************************************************************
 End of File
 */
