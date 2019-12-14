/*!\name      i2c.c
 *
 * \brief     I2C library, using driver functions and ring buffer
 *
 * \author    Juan Gago
 *
 */
 
#include "i2c.h"

int buffer_empty(I2C_DRIVER* ptr) {    
  	// return true if the buffer is empty (read = write)
  	return ptr->widx == ptr->ridx; 
}

int buffer_full(I2C_DRIVER* ptr) {     
	// return true if the buffer is full.  
  	return (ptr->widx + 1) % MAX_I2C_CLIENTS == ptr->ridx; 
}

// reads from current buffer location; assumes buffer not empty
I2C_CLIENT buffer_read(I2C_DRIVER* ptr) {     
	I2C_CLIENT val = ptr->data[ptr->ridx];
	++ptr->ridx; // increments read index and wrap around
	if(ptr->ridx >= MAX_I2C_CLIENTS) {  
		ptr->ridx = 0;
	}
	return val;
}

void buffer_write(I2C_CLIENT client, I2C_DRIVER* ptr) {
	// add an element to the buffer; assumes buffer not full 
	ptr->data[ptr->widx] = client;
	++ptr->widx; // increment the write index and wrap around if necessary
	if(ptr->widx >= MAX_I2C_CLIENTS) {  
		ptr->widx = 0;
	}
}

bool I2C_Add (I2C_CLIENT client, I2C_DRIVER* ptr) {
	// if the buffer is full the data is lost
	if(buffer_full(ptr))        
		return false;
	
	buffer_write(client, ptr);
	return true;
}

int8_t I2C_GetStatus (DRV_HANDLE handle, DRV_I2C_BUFFER_HANDLE buffer_handle)
{
	DRV_I2C_BUFFER_EVENT event_handle;
	event_handle = DRV_I2C_TransferStatusGet (handle, buffer_handle);
	if (event_handle == DRV_I2C_BUFFER_EVENT_ERROR)
		return -1;
	if (event_handle == DRV_I2C_BUFFER_EVENT_COMPLETE)
		return 1;
	return 0;
}

I2C_CLIENT* I2C_Tasks (I2C_DRIVER* ptr, uint32_t* milliseconds)
{
    static DRV_HANDLE handle;
    static DRV_I2C_BUFFER_HANDLE buffer_handle;
    static I2C_CLIENT client = {
    	.write = NULL, 	.wlen = 0,
    	.read  = NULL,	.rlen = 0,
    	.state = I2C_CLIENT_NONE,
    };
    int error_code;

	switch(ptr->state)
	{	
		case I2C_DRV_IDLE:
			if (!buffer_empty(ptr)) 
			{
				client = buffer_read(ptr); // first take the driver handle
				client.state = I2C_CLIENT_REQ;
				handle = DRV_I2C_Open (ptr->index, DRV_IO_INTENT_READWRITE);
				ptr->state = I2C_DRV_START;
			}
			break;

		case I2C_DRV_START: // Transmit with static buffer handle

			buffer_handle = DRV_I2C_Transmit (handle, client.address, client.write, client.wlen, NULL);
			if (buffer_handle != NULL)
			{
				*milliseconds = 0;
				ptr->state = I2C_DRV_WRITE;
			}
			break;

		case I2C_DRV_WRITE:

			error_code = I2C_GetStatus(handle, buffer_handle);
			if (error_code < 0)
			{
				printLog("__ERR %d I2C%d Write /r/n", ++ptr->error_count, ptr->index);
				client.state = I2C_CLIENT_ERROR;
				ptr->state = I2C_DRV_CLOSE;
			}
			else if (error_code > 0)
			{
				if (client.rlen > 0)
				{
					ptr->state = I2C_DRV_RESTART;
				}
				else // skip reading states
				{
					client.state = I2C_CLIENT_DONE;
					ptr->state = I2C_DRV_CLOSE;
				}
			}
			else if (!error_code && (milliseconds > TIMEOUT_WRITE_MS))
			{
				client.state = I2C_CLIENT_TIMEOUT;
				ptr->state = I2C_DRV_CLOSE;
			}s
			break;

		case I2C_DRV_RESTART: // Receive with same buffer handle

			buffer_handle = DRV_I2C_Receive (handle, client.address, client.read, client.rlen, NULL);
			if (buffer_handle != NULL)
			{
				*milliseconds = 0;
				ptr->state = I2C_DRV_READ;
			}
			break;

		case I2C_DRV_READ:

			error_code = I2C_GetStatus(handle, buffer_handle);
			if (error_code < 0)
			{
				printLog("__ERR %d I2C%d Read /r/n", ++ptr->error_count, ptr->index);
				client.state = I2C_CLIENT_ERROR;
				ptr->state = I2C_DRV_CLOSE;
			}
			else if (error_code > 0)
			{
				client.state = I2C_CLIENT_DONE;
				ptr->state = I2C_DRV_CLOSE;
			}
			else if (!error_code && (milliseconds > TIMEOUT_READ_MS))
			{
				client.state = I2C_CLIENT_TIMEOUT;
				ptr->state = I2C_DRV_CLOSE;
			}
			break;

		case I2C_DRV_CLOSE:

			DRV_I2C_Close (ptr->index);
			handle = DRV_HANDLE_INVALID;
			ptr->state = I2C_DRV_IDLE;
			break;

		default:
		case I2C_DRV_ERROR:
			break;
	}
	return &client;
}

void I2C_Initialize (I2C_DRIVER* ptr, uint8_t index)
{
	int i; 
	for (i=0; i<MAX_I2C_CLIENTS; i++)
	{
		ptr->data[i].state = I2C_CLIENT_NONE;
	}
	ptr->index = index;
	ptr->state = I2C_DRV_IDLE;
	ptr->widx = 0;
	ptr->ridx = 0;
}

