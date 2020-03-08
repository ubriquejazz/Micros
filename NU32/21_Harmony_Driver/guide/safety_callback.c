/* Safety Callbacks - pag 27 */

// Example: Registering a Callback Function 

	#define PERIOD 1000 
	#define NO_REPEAT false

	if (DRV_TMR_AlarmRegister(pObj->tmrHandle, PERIOD, NO_REPEAT, pObj, MyCallback)) 
	{ 
		/* Successfully registered “MyCallback” function */ 
	}

// Example: Callback Function 

	void MyCallback ( uintptr_t context, uint32_t alarmCount ) 
	{
		MY_DATA_OBJECT *pObj = (MY_DATA_OBJECT *)context;
		pObj->alarmCount += alarmCount; 
	}

// Example: Temporarily Disabling a Callback 

	{
		bool previousAlarmEnable;

		alarmWasDisabled = DRV_TMR_AlarmDisable(pObj->myTimer); 
		if (pObj->alarmCount > MY_MAX_ALARM_COUNT) 
		{
			/* Reset my alarm count. */
			pObj->alarmCount = 0; 
		}
		DRV_TMR_AlarmEnable(pObj->myTimer, previousAlarmEnable);
	}

// Example: Transfer Synchronization Callback 

	void MyBufferEventHandler ( DRV_USART_BUFFER_EVENT event, 
								DRV_USART_BUFFER_HANDLE bufferHandle, 
								uintptr_t context ) 
	{
		MY_OBJ *pObj = (MY_OBJ *)context;

		if (pObj->myBufferHandle == bufferHandle)
		{
			switch(event)
			{ 
				case DRV_USART_BUFFER_EVENT_COMPLETE:
				{ 
					/* Clean up after my buffer transfer is complete. */ 
				}
				break; 

				/* Handle other events for my buffer */
			}
		}
	} 

// Example: Interface With an Intrinsic Race Condition 

	char buffer[] = “Hello World\n”;
	pObj->bufferHandle = DRV_USART_BufferAddWrite(pObj->myUsart, buffer, strlen(buffer));
	
// Example: Interface Without an Intrinsic Race Condition 

	char buffer[] = “Hello World\n”;
	DRV_USART_BufferAddWrite(pObj->myUsart, buffer, strlen(buffer), &pObj->bufferHandle);

