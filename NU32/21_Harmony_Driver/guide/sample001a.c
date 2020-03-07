/* Multiple Client Static Drivers - 001a.c */

// Example: Static Multiple Client Open Function 

DRV_USART_OBJ drvUsart0; 
DRV_USART_CLIENT_OBJ drvUsartClient[DRV_USART_CLIENTS_NUMBER];

DRV_HANDLE DRV_USART0_Open( const SYS_MODULE_INDEX index, const DRV_IO_INTENT ioIntent ) 
{ 
	int i; 
	DRV_USART_CLIENT_OBJ pClient = (DRV_USART_CLIENT_OBJ *)DRV_HANDLE_INVALID; 
	for (i=0; i < DRV_USART_CLIENTS_NUMBER; i++) { 
		if (drvUsartClient[i].driver == NULL) { 
			pClient = &drvUsartClient[i]; 
			pClient->driver = &drvUsart0; 
			break; 
		} 
	} 
	return (DRV_HANDLE)pClient; 
}


// Example: Multiple Client Dynamic-to-Static Mapping Function 

inline void DRV_USART_BufferAddRead ( const DRV_HANDLE handle, DRV_USART_BUFFER_HANDLE * const bufferHandle, void * buffer, const size_t size ) 
{ 
	DRV_USART_CLIENT_OBJ *client = (DRV_USART_CLIENT_OBJ *)handle; 
	switch (client->driver) { 
		case &drvUsart0:
			DRV_USART0_BufferAddRead(handle, bufferHandle, buffer, size); 
			break; 

		case &drvUsart1:
			DRV_USART1_BufferAddRead(handle, bufferHandle, buffer, size); 
			break;

		default:
			/* invalid instance. */
	} 
	return; 
}