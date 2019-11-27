/*!\name      i2c.c
 *
 * \brief     I2C Master, using driver functions
 *
 * \author    Juan Gago
 *
 */
 
#include "i2c.h"

int8_t _GetStatus (DRV_HANDLE handle, DRV_I2C_BUFFER_HANDLE buffer_handle)
{
	DRV_I2C_BUFFER_EVENT event_handle;
	event_handle = DRV_I2C_TransferStatusGet (handle, buffer_handle);
	if (event_handle == DRV_I2C_BUFFER_EVENT_ERROR)
		return -1;
	if (event_handle == DRV_I2C_BUFFER_EVENT_COMPLETE)
		return 1;
	return 0;
}

void I2C_Initialize (I2C_DATA* ptr, uint8_t index)
{
	int i; 
	for (i=0; i<MAX_I2C_CLIENTS; i++)
	{
		ptr->fifo[i].state = I2C_CLIENT_NONE;
	}
	ptr->driver_index = index;
	ptr->state = I2C_DRV_IDLE;
	ptr->fifo_index = 0;
}

bool I2C_AddToQueue (I2C_DATA* ptr, I2C_CLIENT client)
{
	(ptr->fifo [ptr->fifo_index]).wlen = client.wlen;
	(ptr->fifo [ptr->fifo_index]).rlen = client.rlen;
	(ptr->fifo [ptr->fifo_index]).write = client.write;
	(ptr->fifo [ptr->fifo_index]).read = client.read;
	(ptr->fifo [ptr->fifo_index]).state = I2C_CLIENT_REQ;
	ptr->fifo_index += 1;
	if (ptr->fifo_index > MAX_I2C_CLIENTS)
		return false;
	return true;
}

void I2C_Tasks (I2C_DATA* ptr, uint32_t* milliseconds)
{
    static DRV_HANDLE handle;
    static DRV_I2C_BUFFER_HANDLE buffer_handle;

	switch(ptr->state)
	{	
		case I2C_DRV_IDLE:
			if (ptr->fifo_index && (ptr->fifo[fifo_index].state == I2C_CLIENT_REQ))
			{
				// first take static driver handle
				handle = DRV_I2C_Open (ptr->driver_index, DRV_IO_INTENT_READWRITE);
				ptr->state = I2C_DRV_START;
			}
			break;

		case I2C_DRV_START: // Transmit with static buffer handle

			I2C_CLIENT client = ptr->fifo[ptr->fifo_index];
			buffer_handle = DRV_I2C_Transmit (handle, client.address, client.write, client.wlen, NULL);
			if (buffer_handle != NULL)
			{
				*milliseconds = 0;
				ptr->state = I2C_DRV_WRITE;
			}
			break;

		case I2C_DRV_WRITE:

			int error_code = _GetStatus(handle, buffer_handle);
			if (error_code < 0)
			{
				(ptr->fifo [ptr->fifo_index]).state = I2C_CLIENT_ERROR;
				ptr->error_count += 1;
				ptr->state = I2C_DRV_CLOSE;
			}
			else if (error_code > 0)
			{
				if ((ptr->fifo [ptr->fifo_index]).rlen > 0)
				{
					ptr->state = I2C_DRV_RESTART;
				}
				else // skip reading states
				{
					(ptr->fifo [ptr->fifo_index]).state = I2C_CLIENT_DONE;
					ptr->state = I2C_DRV_CLOSE;
				}
			}
			else if (!error_code && (milliseconds > TIMEOUT_WRITE_MS))
			{
				(ptr->fifo [ptr->fifo_index]).state = I2C_CLIENT_TIMEOUT;
				ptr->state = I2C_DRV_CLOSE;
			}s
			break;

		case I2C_DRV_RESTART: // Receive with same buffer handle

			I2C_CLIENT client = ptr->fifo[ptr->fifo_index];
			buffer_handle = DRV_I2C_Receive (handle, client.address, client.read, client.rlen, NULL);
			if (buffer_handle != NULL)
			{
				*milliseconds = 0;
				ptr->state = I2C_DRV_READ;
			}
			break;

		case I2C_DRV_READ:

			int error_code = _GetStatus(handle, buffer_handle);
			if (error_code < 0)
			{
				(ptr->fifo [ptr->fifo_index]).state = I2C_CLIENT_ERROR;
				ptr->error_count += 1;
				ptr->state = I2C_DRV_CLOSE;
			}
			else if (error_code > 0)
			{
				(ptr->fifo [ptr->fifo_index]).state = I2C_CLIENT_DONE;
				ptr->state = I2C_DRV_CLOSE;
			}
			else if (!error_code && (milliseconds > TIMEOUT_READ_MS))
			{
				(ptr->fifo [ptr->fifo_index]).state = I2C_CLIENT_TIMEOUT;
				ptr->state = I2C_DRV_CLOSE;
			}
			break;

		case I2C_DRV_CLOSE:

			DRV_I2C_Close (ptr->driver_index);
			handle = DRV_HANDLE_INVALID;
			ptr->fifo_index -= 1;
			ptr->state = I2C_DRV_IDLE;
			break;

		default:
		case I2C_DRV_ERROR:
			break;
	}
}

