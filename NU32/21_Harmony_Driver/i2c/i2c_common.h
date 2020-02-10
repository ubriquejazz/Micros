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
typedef struct NODE node_t;

struct NODE {
       node_t*         self;
       node_t*         owner;
       uint32_t        data;
};

/* States */

typedef enum {	
	I2C_OPEN_INIT=0,
	I2C_OPEN_OPEN,		// open (handle)
	I2C_OPEN_START,		// write 
	I2C_OPEN_RESTART,	// write and read
	I2C_OPEN_DONE,		// first part ending
	I2C_CLOSE_INIT,		// second part begin
	I2C_CLOSE_CLOSE,	// close and deinit
	I2C_CLOSE_DONE
	
} I2C_DRV_STATE;

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
	I2C_DRV_STATE			state;
	I2C_CLIENT*				client;

} I2C_DRIVER;

/* Application Initialization and State Machine Functions */

bool I2C_Initialize 	(I2C_DRIVER*, I2C_CLIENT*, uint8_t);
bool I2C_OpenDriver		(I2C_DRIVER*, uint32_t*);
bool I2C_IsComplete		(I2C_DRIVER*, uint32_t*);
bool I2C_CloseDriver	(I2C_DRIVER*, uint32_t*);

#endif
