/* File System Read/Write - pag 17 */

// Example: Reading Data Using the File System Style Model 

	char buffer[MY_BUFFER_SIZE]; 
	size_t count;

	count = DRV_USART_Read(myUsart, buffer, MY_BUFFER_SIZE);

// Example: Writing Data Using the File System Style Model 

	char buffer = “Hello World\n”; 
	size_t count;

	count = DRV_USART_Write(myUsart, buffer, strlen(buffer));

// Example: Driver-Client Usage Model 

	DRV_HANDLE myUsart; char *message = “Hello World\n”;

	/* Obtain an open handle to the USART driver. */ 
	myUsart = DRV_USART_Open(MY_UART_INDEX, DRV_IO_INTENT_READWRITE); 

	/* Interact with the USART driver. */ 
	DRV_USART_Write(myUsart, message, sizeof(message)); 

	/* Continue using other USART driver client-interface functions as needed. */ 
	...

	/* Close USART driver if no longer needed. */ 
	DRV_USART_Close(myUsart);
