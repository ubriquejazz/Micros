#ifndef I2C__H__
#define I2C__H__

/*!\name      i2c.h
 *
 * \brief     helps implement I2Cx as a master with driver functions
 *
 * \author    Juan Gago
 *
 */

#define MAX_I2C_CLIENTS		5
#define TIMEOUT_WRITE_MS	100
#define TIMEOUT_READ_MS		100

// buffer pointer type.  The buffer is shared by an ISR and mainline code.
// the pointer to the buffer is also shared by an ISR and mainline code.
// Hence the double volatile qualification
typedef volatile unsigned char * volatile buffer_t;

typedef enum 
{	
	I2C_DRV_IDLE=0, 
	I2C_DRV_ERROR,
	I2C_DRV_START, 
	I2C_DRV_WRITE, 
	I2C_DRV_READ, 
	I2C_DRV_RESTART, 
	I2C_DRV_STOP, 
	I2C_DRV_CLOSE

} I2C_DRV_STATE;

typedef enum 
{
	I2C_CLIENT_NONE=0, 
	I2C_CLIENT_DONE, 
	I2C_CLIENT_REQ, 
	I2C_CLIENT_ERROR, 
	I2C_CLIENT_TIMEOUT

} I2C_CLIENT_STATE;

typedef struct
{
	unsigned int 		address;
    buffer_t			write, read;
    uint8_t				wlen, rlen;
    I2C_CLIENT_STATE	state;

} I2C_CLIENT;

typedef struct
{
	uint8_t				driver_index;
	uint8_t				fifo_index;
    uint32_t     		error_count;
    I2C_DRV_STATE		state;
    /* Clients per driver */
    I2C_CLIENT			data[MAX_I2C_CLIENTS];

} I2C_DATA;

/* Application Initialization and State Machine Functions */

void I2C_Initialize(I2C_DATA*, uint8_t);
void I2C_Tasks(I2C_DATA*, uint32_t*);
void I2C_AddToQueue (I2C_DATA*, I2C_CLIENT);

#endif
