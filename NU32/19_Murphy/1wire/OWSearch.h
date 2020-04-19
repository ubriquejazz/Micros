#ifndef _OW_SEARCH_H
#define _OW_SEARCH_H

/*!\name      OWSearch.h
 *
 * \brief     Search for all devices on the bus. 
 *
 * \author    Juan Gago
 *
 */ 

#define SEARCH_ROM_CMD  0xF0  //!< Search command for all devices

//! Get a pointer to the location of the found address
/*! \param void Nothing passed in
	\return uint8_t pointer to the EEPROM address
*/
uint8_t* OWGetAddresLoc(void);

//! Find the first device on the 1-Wire bus 
/*! \return TRUE - ROM number in \a ROM_NO  FALSE - no device present 
*/ 
int OWFirst(void);

//! Find the next device on the 1-wire bus 
/*! \return TRUE - ROM number in \a ROM_NO  FALSE - no device present 
*/ 
int OWNext(void); 

#endif // _OW_SEARCH_COMMON_H
