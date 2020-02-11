#ifndef _SPI_COMMON__H__
#define _SPI_COMMON__H__

/*!\name      SPI.h
 *
 * \brief     SPI Classes
 *
 * \author    Juan Gago
 *
 */

#define TIMEOUT_SPI_MS    100

// buffer pointer type.  The buffer is shared by an ISR and mainline code.
// the pointer to the buffer is also shared by an ISR and mainline code.
// Hence the double volatile qualification
typedef volatile uint8_t* volatile buffer_t;

/* States */

typedef enum {	
	SPI_OPEN_IDLE,
	SPI_OPEN_INIT,		// --> OpenDriver()
	SPI_OPEN_CHECK,		//   * drv_init(index)
	SPI_OPEN_OPEN,		//   * drv_open(handle)
	SPI_OPEN_START,		//   * drv_transmit()
	SPI_OPEN_RESTART,	//   * drv_transmitthenreceive()
	SPI_OPEN_DONE,		// <-- OpenDriver()
} SPI_OPEN_STATE;

typedef enum {	
	SPI_CLOSE_IDLE=0,
	SPI_CLOSE_INIT,		// --> CloseDriver()
	SPI_CLOSE_CLOSE,	//   * drv_close(handle) 
						//   * drv_deinit(index)
	SPI_CLOSE_DONE		// <-- CloseDriver()
} SPI_CLOSE_STATE;

/* Driver & Clients */

typedef struct {
	uint8_t 				address;
    buffer_t				write, read;
    uint8_t					wlen, rlen;
} SPI_CLIENT;

typedef struct {		
	uint8_t					index;
	DRV_HANDLE 				handle;
	DRV_SPI_BUFFER_HANDLE 	buffer;
	SPI_OPEN_STATE			state0;
	SPI_CLOSE_STATE			state1;
	SPI_CLIENT*				client;
} SPI_DRIVER;

/* Application Initialization and State Machine Functions */

bool SPI_Initialize 	(SPI_DRIVER*, SPI_CLIENT*, uint8_t);
bool SPI_OpenDriver		(SPI_DRIVER*, uint32_t*);
bool SPI_IsComplete		(SPI_DRIVER*, uint32_t*);
bool SPI_CloseDriver	(SPI_DRIVER*, uint32_t*);

#endif
