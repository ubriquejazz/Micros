/*!\name      i2c.c
 *
 * \brief     I2C library, using driver functions and ring buffer
 *
 * \author    Juan Gago
 *
 */
 
#include "i2c_common.h"

int buffer_empty(I2C_DRIVER* drv) {    
  	// return true if the buffer is empty (read = write)
  	return drv->widx == drv->ridx; 
}

int buffer_full(I2C_DRIVER* drv) {     
	// return true if the buffer is full.  
  	return (drv->widx + 1) % MAX_I2C_CLIENTS == drv->ridx; 
}

// reads from current buffer location; assumes buffer not empty
I2C_CLIENT* buffer_read (I2C_DRIVER* drv) {     
	I2C_CLIENT* client = drv->data[drv->ridx];
	++drv->ridx; // increments read index and wrap around
	if(drv->ridx >= MAX_I2C_CLIENTS) {  
		drv->ridx = 0;
	}
	return client;
}

void buffer_write (I2C_CLIENT client, I2C_DRIVER* drv) {
	// add an element to the buffer; assumes buffer not full 
	drv->data[drv->widx] = client;
	++drv->widx; // increment the write index and wrap around if necessary
	if(drv->widx >= MAX_I2C_CLIENTS) {  
		drv->widx = 0;
	}
}

bool I2C_Add (I2C_CLIENT* client, I2C_DRIVER* drv) {
	// if the buffer is full the data is lost
	if(buffer_full(drv))        
		return false;
	
	buffer_write(*client, drv);
	client->base = drv;
	return true;
}

I2C_CLIENT* I2C_Get (I2C_DRIVER* drv) {  
	if (buffer_empty(drv)) {
		return NULL;
	}
	return drv->data[drv->ridx];
}

I2C_CLIENT* I2C_Pop (I2C_DRIVER* drv) {  
	if (buffer_empty(drv)) {
		return NULL;
	}
	return buffer_read (drv);
}

DRV_I2C_BUFFER_EVENT I2C_GetStatus (I2C_CLIENT* client)
{
	I2C_DRIVER* drv; /* handle is global*/
	event_handle = DRV_I2C_TransferStatusGet (handle, buffer_handle);
	return event_handle;
}

/* State Machine */

void I2C_Initialize (I2C_DRIVER* drv, uint8_t index)
{
	int i; 
	for (i=0; i<MAX_I2C_CLIENTS; i++)
	{
		drv->data[i].state = I2C_CLIENT_NONE;
	}
	drv->index = index;
	drv->handle = DRV_HANDLE_INVALID;

	drv->tx_handle = NULL;
	drv->rx_handle = NULL;

	drv->state = I2C_DRV_IDLE;
	drv->widx = 0;
	drv->ridx = 0;
}

bool I2C_Tasks (I2C_CLIENT* client, uint32_t* milliseconds, uint8_t* result)
{   
    DRV_I2C_BUFFER_EVENT error_code;
	I2C_DRIVER* drv; /* handle is global*/

    if(client)
    {
    	drv = client->base;
	    switch(drv->state)
		{	
			case I2C_DRV_IDLE:
				if (client->state == I2C_CLIENT_REQ) {
					handle = DRV_I2C_Open (ptr->index, DRV_IO_INTENT_READWRITE);
					if (handle == DRV_HANDLE_INVALID) { 
						ptr->state = I2C_DRV_ERROR;
					}
					else {
						ptr->state = I2C_DRV_START;
					}
				}
				else{

				}
				break

			default:
			case I2C_DRV_DONE:
			case I2C_DRV_ERROR:
				drv->state = I2C_DRV_IDLE;
				break;
		}
    }	
	return true;
}


