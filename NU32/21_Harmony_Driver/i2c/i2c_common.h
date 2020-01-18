#ifndef _I2C_COMMON__H__
#define _I2C_COMMON__H__

/*!\name      i2c.h
 *
 * \brief     I2C library
 *
 * \author    Juan Gago
 *
 */

#define MAX_I2C_CLIENTS		32		// length of the buffer
#define TIMEOUT_WRITE_MS	100
#define TIMEOUT_READ_MS		100

// buffer pointer type.  The buffer is shared by an ISR and mainline code.
// the pointer to the buffer is also shared by an ISR and mainline code.
// Hence the double volatile qualification
typedef volatile uint8_t* volatile buffer_t;

typedef uint32_t object_t;
typedef struct NODE node_t;

struct NODE {
	node_t*		self;
	node_t*		owner;
	object_t 	data;
};

typedef enum {	
	I2C_DRV_IDLE=0, 
	I2C_DRV_START, 			// open (handle)
	I2C_DRV_WRITE, 
	I2C_DRV_CLOSE,			// close and deinit
	I2C_DRV_INIT, 			// initialize again
	I2C_DRV_DONE,
	I2C_DRV_ERROR,			// handle invalid
	
} I2C_DRV_STATE;

typedef enum {
	I2C_CLIENT_NONE=0, 
	I2C_CLIENT_REQ, 
	I2C_CLIENT_OK,			// Complete
	I2C_CLIENT_USED, 		// Used
	I2C_CLIENT_ERROR, 		// Error
	I2C_CLIENT_TIMEOUT		// Pending
} I2C_CLIENT_STATE;

/* Driver & Clients */

typedef struct {
	node_t					base;
	uint8_t 				address;
    buffer_t				write, read;
    uint8_t					wlen, rlen;
    I2C_CLIENT_STATE		state;
} I2C_CLIENT;

typedef struct {
	node_t					base;			
	I2C_DRV_STATE			state;
	uint8_t					index;
    uint32_t     			error_count;
	DRV_HANDLE 				handle;
	DRV_I2C_BUFFER_HANDLE 	tx_handle;
	DRV_I2C_BUFFER_HANDLE	rx_handle;

} I2C_DRIVER;

typedef struct {
	uint8_t					widx, ridx;
	I2C_CLIENT				users[MAX_I2C_CLIENTS];
	I2C_DRIVER				drv;
} I2C_BUS;


/* Application Initialization and State Machine Functions */

void		I2C_Initialize 	(I2C_DRIVER*, uint8_t);
bool		I2C_Add 		(I2C_CLIENT*, I2C_DRIVER*);
I2C_CLIENT* I2C_Pop			(I2C_DRIVER*);
I2C_CLIENT* I2C_Get			(I2C_DRIVER*);

bool		I2C_Tasks 		(I2C_CLIENT*, uint32_t*, uint8_t*);

#endif
