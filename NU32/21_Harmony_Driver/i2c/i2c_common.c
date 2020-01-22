/*!\name      i2c.c
 *
 * \brief     I2C library, using driver functions and ring buffer
 *
 * \author    Juan Gago
 *
 */
 
#include "i2c_common.h"

int buffer_empty(I2C_OBJECT* bus) {    
  	// return true if the buffer is empty (read = write)
  	return bus->widx == bus->ridx; 
}

int buffer_full(I2C_OBJECT* bus) {     
	// return true if the buffer is full.  
  	return (bus->widx + 1) % MAX_I2C_CLIENTS == bus->ridx; 
}

// reads from current buffer location; assumes buffer not empty
I2C_CLIENT* buffer_read (I2C_OBJECT* bus) {     
	I2C_CLIENT* client = bus->users[bus->ridx];
	++bus->ridx; // increments read index and wrap around
	if(bus->ridx >= MAX_I2C_CLIENTS) {  
		bus->ridx = 0;
	}
	return client;
}

void buffer_write (I2C_CLIENT client, I2C_OBJECT* bus) {
	// add an element to the buffer; assumes buffer not full 
	bus->users[bus->widx] = client;
	++bus->widx; // increment the write index and wrap around if necessary
	if(bus->widx >= MAX_I2C_CLIENTS) {  
		bus->widx = 0;
	}
}

uint8_t I2C_Add (I2C_CLIENT* client, I2C_OBJECT* bus) {
	// if the buffer is full the data is lost
	if(buffer_full(bus))        
		return false;
	
	buffer_write(*client, bus);
	(client->base).owner = (node_t*) bus;
	return bus->wdix;
}

I2C_CLIENT* I2C_Get (I2C_OBJECT* bus) {  
	if (buffer_empty(bus)) {
		return NULL;
	}
	return bus->users[bus->ridx];
}

DRV_I2C_BUFFER_EVENT I2C_GetStatus (device_h, buffer_h) {
	return DRV_I2C_TransferStatusGet (device_h, buffer_h);
}

/* State Machine */

void I2CBUS_Init (I2C_OBJECT* bus) {
	int i; 
	for (i=0; i<MAX_I2C_CLIENTS; i++)
	{
		bus->users[i].state = I2C_CLIENT_NONE;
		(bus->users[i].base).self = (node_t*) & (bus->users[i]);
	}
	bus->widx = 0;
	bus->ridx = 0;	
}

void I2C_Initialize (I2C_OBJECT* bus, uint8_t index)
{
	I2CBUS_Init(bus);

	(bus->drv).index = index;
	(bus->drv).handle = DRV_HANDLE_INVALID;
	(bus->drv).txbuf_h = NULL;
	(bus->drv).rxbuf_h = NULL;
	(bus->drv).state = I2C_DRV_IDLE;
	(bus->drv).base.owner = (node_t*) bus;
}

int I2C_Tasks (I2C_CLIENT* client, uint32_t* milliseconds, uint8_t* result)
{   
	int retVal = 0;
    DRV_I2C_BUFFER_EVENT error_code;
    I2C_OBJECT* bus = (I2C_OBJECT*) (client->base).owner;

    switch( (bus->drv).state )
	{	
		case I2C_DRV_IDLE:
			if (!buffer_empty(bus) ) {
				client = buffer_read (bus);
				if (client->state == I2C_CLIENT_REQ) {
					(bus->drv).state = I2C_DRV_OPEN;
				}
			}
			break

		case I2C_DRV_OPEN:
			(bus->drv).handle = DRV_I2C_Open ((bus->drv).index, DRV_IO_INTENT_READWRITE);
			if (handle != DRV_HANDLE_INVALID) { 
				(bus->drv).state = (client->rlen > 0)?  I2C_DRV_START_RD : I2C_DRV_START_WR;
			}
			break;

		case I2C_DRV_START_WR: // Transmit with static buffer handle		 		
  			(bus->drv).txbuf_h = DRV_I2C_Transmit ((bus->drv).handle, client->address, client->write, client->wlen, NULL);
 			if ((bus->drv).txbuf_h != NULL) {
 				*milliseconds = 0;
 				(bus->drv).state = I2C_DRV_WRITE;
 			}		 		
 			break;

 		case I2C_DRV_WRITE:
 			client->state == I2C_CLIENT_COMPLETE;
 			(bus->drv).state = I2C_DRV_CLOSE;
 			break;

		case I2C_DRV_CLOSE:
			DRV_I2C_Close((bus->drv).handle);
			*milliseconds = 0;
			(bus->drv).state = I2C_DRV_INIT;
			break;

		case I2C_DRV_INIT:
			if (*milliseconds > 800) {
				(bus->drv).state = I2C_DRV_DONE;
			}
			break;

		case I2C_DRV_DONE:
			retVal = 1;
			break;

		case I2C_DRV_ERROR:
			retVal = -1;
			break;
	} 
	return retVal;
}


