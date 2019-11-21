/*!\name      i2c.c
 *
 * \brief     I2C Master utilities, using interrupts
 *
 * \author    Juan Gago
 *
 */
 
#include "i2c.h"

int8_t _Error(DRV_HANDLE handle, DRV_I2C_BUFFER_HANDLE buffer_handle)
{
	DRV_I2C_BUFFER_EVENT event_handle;

	event_handle = DRV_I2C_TransferStatusGet (handle, buffer_handle);
	if (event_handle == DRV_I2C_BUFFER_EVENT_ERROR)
		return -1;
	if (event_handle == DRV_I2C_BUFFER_EVENT_COMPLETE)
		return 1;
	return 0;
}

void I2C_Initialize(I2C_DATA* ptr, uint8_t index)
{
	int i; 
	for (i=0; i<MAX_I2C_CLIENTS; i++)
	{
		ptr->fifo[i].state = INIT;
	}
	ptr->driver_index = index;
	ptr->state = IDLE;
	ptr->fifo_index = 0;
}

bool I2C_AddToQueue (I2C_DATA* ptr, I2C_CLIENT client)
{
	(ptr->fifo [ptr->fifo_index]).wlen = client.wlen;
	(ptr->fifo [ptr->fifo_index]).rlen = client.rlen;
	(ptr->fifo [ptr->fifo_index]).write = client.write;
	(ptr->fifo [ptr->fifo_index]).read = client.read;
	(ptr->fifo [ptr->fifo_index]).state = BUSY;
	ptr->fifo_index += 1;
	if (ptr->fifo_index > MAX_I2C_CLIENTS)
		return false;
	return true;
}

void I2C_Tasks (I2C_DATA* ptr, uint32_t*)
{
    static DRV_HANDLE handle;
    static DRV_I2C_BUFFER_HANDLE buffer_handle;

	if (ptr->fifo_index)
	{
		client = ptr->fifo[ptr->fifo_index];
		handle = DRV_I2C_Open (ptr->driver_index, DRV_IO_INTENT_READWRITE);

		buffer_handle = DRV_I2C_Transmit (handle, client.address, client.write, client.wlen, NULL);
    	_Error(handle, buffer_handle)

	    buffer_handle = DRV_I2C_Receive (handle, client.address, client.read, client.rlen, NULL);
	    _Error(handle, buffer_handle)

		DRV_I2C_Close (ptr->driver_index);

    	client.state = DONE;
    	ptr->fifo_index -= 1;
	}
}

