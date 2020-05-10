#ifndef _OW_SEARCH__H
#define _OW_SEARCH__H

/*!\name      ow_search.h
 *
 * \brief     Search for all devices on the bus. 
 *
 * \author    Juan Gago
 *
 */ 

// Result of these functions
typedef enum {
	ERR_SEARCH_SUCCESS = 0,
	ERR_SEARCH_FAILURE = -1,
	ERR_SEARCH_UNKNOWN = -99,
} ERR_SEARCH;

uint64_t OW_GetRom64bit(void);
int OW_First(void);
int OW_Next(void); 

#endif // _OW_SEARCH_H