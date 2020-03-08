/* Buffer Queuing - pag 18 */

// Example: Reading Data Using the Buffer Queuing Model 

DRV_USART_BUFFER_HANDLE handle1; 
DRV_USART_BUFFER_HANDLE handle2; 

char buffer1[BUFFER_1_SIZE]; 
char buffer2[BUFFER_2_SIZE];

DRV_USART_BufferAddRead(myUsart, &handle1, buffer1, BUFFER_1_SIZE); 
DRV_USART_BufferAddRead(myUsart, &handle2, buffer2, BUFFER_2_SIZE);

// Example: Writing Data Using the Buffer Queuing Model 

DRV_USART_BUFFER_HANDLE handle1; 
DRV_USART_BUFFER_HANDLE handle2; 
char buffer1 = “Hello World\n”; 
char buffer2 = “Hello Again\n”;

DRV_USART_BufferAddWrite(myUsart, &handle1, buffer1, strlen(buffer1)); 
DRV_USART_BufferAddWrite(myUsart, &handle2, buffer2, strlen(buffer2));

// Example: Using Callback Notification 

DRV_USART_BUFFER_HANDLE handle1; 
char buffer1 = “Hello World\n”;

void MyUsartCallback ( DRV_USART_BUFFER_EVENT event, 
			DRV_USART_BUFFER_HANDLE bufferHandle, 
			uintptr_t context ) 
{
	switch ( event ) 
	{		
		case DRV_USART_BUFFER_EVENT_COMPLETE:
			if (bufferHandle == handle1) { 
				/* buffer1 data transfer complete */ 
			}
			break;
		
		/* Handle other possible transfer events. */
	}
}

DRV_USART_BufferEventHandlerSet(myUsart, MyUsartCallback, NULL);
DRV_USART_BufferAddWrite(myUsart, &handle1, buffer1, strlen(buffer1));

// Example: Checking for Buffer Status 

DRV_USART_TRANSFER_STATUS status;

status = DRV_USART_BufferStatusGet(myUsart, handle1); 
if (status == DRV_USART_BUFFER_COMPLETE) { 
	/* Buffer Transfer Has Completed */ 
}

// Example: Reading Data Using the File System Style Model 

char buffer[MY_BUFFER_SIZE]; 
size_t count;

count = DRV_USART_Read(myUsart, buffer, MY_BUFFER_SIZE);

// Example: Writing Data Using the File System Style Model 

char buffer = “Hello World\n”; 
size_t count;

count = DRV_USART_Write(myUsart, buffer, strlen(buffer));
