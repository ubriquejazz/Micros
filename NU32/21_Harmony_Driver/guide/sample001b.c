/* Multiple Client Drivers 001b.c */

// Example: Driver Open Function 

DRV_HANDLE DRV_USART_Open( const SYS_MODULE_INDEX index, const DRV_IO_INTENT ioIntent ) 
{ 
	int i; 
	DRV_USART_CLIENT_OBJ pClient = (DRV_USART_CLIENT_OBJ *)DRV_HANDLE_INVALID; 
	for (i=0; i < DRV_USART_CLIENTS_NUMBER; i++) { 
		if (drvUsartClient[i].driver == NULL) { 
			pClient = &drvUsartClient[i]; 
			pClient->driver = &drvUsart[index]; 
			break; 
		} 
	} 
	return (DRV_HANDLE)pClient;
}

// Example: Client API Function Implementation 

void DRV_USART_BufferAddRead ( const DRV_HANDLE handle, DRV_USART_BUFFER_HANDLE * const bufferHandle, void * buffer, const size_t size ) 
{
	DRV_USART_CLIENT_OBJ *client = (DRV_USART_CLIENT_OBJ *)handle;
	DRV_USART_OBJ *driver = (DRV_USART_OBJ *)client->driver;

	if (driver->buffer == NULL) 
	{
		driver->buffer = buffer; 
		driver->bufferSize = size; 
		bufferHandle = buffer;
		/* Start the data transfer process. */
	} 
	else 
	{
		*bufferHandle = DRV_USART_BUFFER_HANDLE_INVALID;
	}

	return; 
}