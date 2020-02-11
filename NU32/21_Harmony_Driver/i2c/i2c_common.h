#ifndef _I2C_COMMON__H__
#define _I2C_COMMON__H__

/*!\name      i2c.h
 *
 * \brief     I2C Classes
 *
 * \author    Juan Gago
 *
 */

// buffer pointer type.  The buffer is shared by an ISR and mainline code.
// the pointer to the buffer is also shared by an ISR and mainline code.
// Hence the double volatile qualification
typedef volatile uint8_t* volatile buffer_t;

/* States */

typedef enum {	
	I2C_OPEN_IDLE,
	I2C_OPEN_INIT,		// --> OpenDriver()
	I2C_OPEN_CHECK,		//   * drv_init(index)
	I2C_OPEN_OPEN,		//   * drv_open(handle)
	I2C_OPEN_START,		//   * drv_transmit()
	I2C_OPEN_RESTART,	//   * drv_transmitthenreceive()
	I2C_OPEN_DONE,		// <-- OpenDriver()
} I2C_OPEN_STATE;

typedef enum {	
	I2C_CLOSE_IDLE=0,
	I2C_CLOSE_INIT,		// --> CloseDriver()
	I2C_CLOSE_CLOSE,	//   * drv_close(handle) 
						//   * drv_deinit(index)
	I2C_CLOSE_DONE		// <-- CloseDriver()
} I2C_CLOSE_STATE;

/* Driver & Clients */

typedef struct {
	uint8_t 				address;
    buffer_t				write, read;
    uint8_t					wlen, rlen;
} I2C_CLIENT;

typedef struct {		
	uint8_t					index;
	DRV_HANDLE 				handle;
	DRV_I2C_BUFFER_HANDLE 	buffer;
	I2C_OPEN_STATE			state0;
	I2C_CLOSE_STATE			state1;
	I2C_CLIENT*				client;
} I2C_DRIVER;

/* Application Initialization and State Machine Functions */

bool I2C_Initialize 	(I2C_DRIVER*, I2C_CLIENT*, uint8_t);
bool I2C_OpenDriver		(I2C_DRIVER*, uint32_t*);
bool I2C_IsComplete		(I2C_DRIVER*, uint32_t*);
bool I2C_CloseDriver	(I2C_DRIVER*, uint32_t*);

#endif
