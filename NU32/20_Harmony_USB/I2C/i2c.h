#ifndef I2C_MASTER_INT__H__
#define I2C_MASTER_INT__H__

/*!\name      i2c_master_int.h
 *
 * \brief     helps implement use I2C1 as a master with interrupts
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

typedef enum I2C_STATE
{	
	IDLE=0, START, WRITE, READ, RESTART, STOP, ERROR
};

typedef enum I2C_CLIENT_STATE
{
	INIT=0, DONE, BUSY, 
}

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
    I2C_STATE			state;
    I2C_CLIENT			fifo [MAX_I2C_CLIENTS];
} I2C_DATA;

/* Application Initialization and State Machine Functions */

void I2C_Initialize(I2C_DATA*, uint8_t);
void I2C_Tasks(I2C_DATA*, uint32_t*);
void I2C_AddToQueue (I2C_DATA*, I2C_CLIENT);

#endif
