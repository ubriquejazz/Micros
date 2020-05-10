#ifndef _DRV_OW__H
#define _DRV_OW__H

#include "ow.h"	 // OW_REQUEST, buffer

/*!\name      drv_ow.h
 *
 * \brief     OW Driver (L2)
 *			  Definition of DRV_DATA
 *
 * \author    Juan Gago
 *
 */ 

#define REQUEST_ARGS	OW_REQUEST*, uint64_t, callback_t

typedef enum 
{
	DRV_STATE_INIT,
	DRV_STATE_WRITE,
	DRV_STATE_READ,
	DRV_STATE_CALLBACK,
	DRV_STATE_ERROR
} DRV_STATES;

typedef struct 
{
	DRV_STATES 		state,			//
	OW_REQUEST*		ptr2req;		// one reques at the time
	int 			mutex;			// 
} DRV_DATA;

extern DRV_DATA driver; 

// Result of these functions
typedef enum {
	ERR_DRV_SUCCESS = 0,
	ERR_DRV_FAILURE = -1,
	ERR_DRV_UNKNOWN = -99,
} ERR_DRV;

int	DRV_OW_Init(PIN_DEF);
int DRV_OW_Tasks();
int DRV_OW_GetDevices(uint64_t*, uint8_t);
int DRV_OW_SendCommand(uint64_t, uint8_t);
int	DRV_OW_BufferAdd(REQUEST_ARGS);

#endif