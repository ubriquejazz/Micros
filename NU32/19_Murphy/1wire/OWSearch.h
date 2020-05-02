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

uint8_t* OWGetRomPtr(void);
uint64_t OWGetRom64bit(void);

int OWFirst(void);
int OWNext(void); 

#endif // _OW_SEARCH_H
