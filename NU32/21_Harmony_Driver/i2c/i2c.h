#ifndef _I2C__H__
#define _I2C__H__

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
typedef volatile uint8_t * volatile buffer_t;

typedef enum {	
	I2C_DRV_IDLE=0, 
	I2C_DRV_ERROR,
	I2C_DRV_START, 
	I2C_DRV_WRITE, 
	I2C_DRV_READ, 
	I2C_DRV_RESTART, 
	I2C_DRV_STOP, 
	I2C_DRV_CLOSE
} I2C_DRV_STATE;

typedef enum {
	I2C_CLIENT_NONE=0, 
	I2C_CLIENT_DONE, 
	I2C_CLIENT_REQ, 
	I2C_CLIENT_ERROR, 
	I2C_CLIENT_TIMEOUT
} I2C_CLIENT_STATE;

/* Driver & Clients */

typedef struct {
	uint8_t 			address;
    buffer_t			write, read;
    uint8_t				wlen, rlen;
    I2C_CLIENT_STATE	state;
} I2C_CLIENT;

typedef struct {
	I2C_DRV_STATE		state;
	uint8_t				index;
    uint32_t     		error_count;
    uint8_t				widx, ridx;
    I2C_CLIENT			data[MAX_I2C_CLIENTS];
} I2C_DRIVER;


/* Application Initialization and State Machine Functions */

void		I2C_Initialize 	(I2C_DRIVER*, uint8_t);
bool		I2C_Add 		(I2C_CLIENT,  I2C_DRIVER*);
I2C_CLIENT* I2C_Tasks 		(I2C_CLIENT*, I2C_DRIVER*, uint32_t*);

#endif
