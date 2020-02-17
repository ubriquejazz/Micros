
/* structure containing the values to be loaded into the SPI */

const static DRV_SPI_INIT drvSpi0InitData = {
   	/* these values are defined in system_config.h */
};


void SYS_Initialize ( void )
{
	/* SPI Driver Index 0 initialization*/
   	SYS_INT_VectorPrioritySet(DRV_SPI_INT_VECTOR_IDX0, DRV_SPI_INT_PRIORITY_IDX0);
   	SYS_INT_VectorSubprioritySet**(DRV_SPI_INT_VECTOR_IDX0, DRV_SPI_INT_SUB_PRIORITY_IDX0);
	sysObj.spiObjectIdx0 = DRV_SPI_Initialize(0, (const SYS_MODULE_INIT * const)&drvSpi0InitData);

	APP_Initialize();
}

/* system_interrupt.c */

void __ISR(_SPI_2_VECTOR, ipl3AUTO) _IntHandlerSPIInstance0(void)
{
    DRV_SPI_Tasks(sysObj.spiObjectIdx0);
}

/* system_tasks.c */

void SYS_Tasks ( void )
{
    /* Maintain system services */
    SYS_DEVCON_Tasks(sysObj.sysDevcon);
    
    /* Maintain the application's state machine. */
    APP_Tasks();
}

/* app.c */

APP_STATES state;

void APP_Tasks ( void )
{
	state = APP_STATE_INIT;
}

void APP_Tasks ( void )
{
	/* SPI Driver Handle */
	static DRV_HANDLE SPIHandle;
	/* Write buffer handle */
	static DRV_SPI_BUFFER_HANDLE Write_Buffer_Handle;
	static DRV_SPI_BUFFER_HANDLE Read_Buffer_Handle;
	/* SPI Driver TX buffer */
	static SPI_DATA_TYPE TXbuffer[6];
	static SPI_DATA_TYPE RXbuffer[6];

	switch(state)
	{
      	case APP_STATE_INIT: 

			SPIHandle = DRV_SPI_Open (DRV_SPI_INDEX_0, DRV_IO_INTENT_READWRITE );
			if(SPIHandle != (uintptr_t)NULL) {
				APP_SPI_CS_SELECT();
				state = APP_SEND_READ_CMD;
			}
			break;
  
      	case APP_SEND_READ_CMD: /* Loads the transmit buffer with the Read command
                              (0x03), the 16-bit address to read ( 0x0000),
                              and one byte of don't cares. ( 0x00). */
         
			TXbuffer[0] = 3; /* EEPROM Read Opcode */
			TXbuffer[1] = 0; /* Address - LSB */
			TXbuffer[2] = 0; /* Address - MSB */
			TXbuffer[3] = 0; /* Dummy byte */

			Write_Buffer_Handle = DRV_SPI_BufferAddWrite(SPIHandle, (SPI_DATA_TYPE *)&TXbuffer[0], 4, 0, 0);
			state = APP_WAIT_FOR_REPLY;
			break;

		case APP_WAIT_FOR_REPLY: 

			if(DRV_SPI_BUFFER_EVENT_COMPLETE & DRV_SPI_BufferStatus(Write_Buffer_Handle)) {
				state = APP_GET_DATA;
			}
			break;
 
      	case APP_GET_DATA: /* Sends out 16 clock signals (4 bytes of don't care data) to
                          receive the contents of the address 0x00, and 0x01 from the EEPROM.
                          Increments the state upon return from DRV_SPI_BufferAddRead */
         
			Read_Buffer_Handle = DRV_SPI_BufferAddRead( SPIHandle, (SPI_DATA_TYPE *)&RXbuffer[0], 4, 0, 0);
			state = APP_WAIT_FOR_DATA;
			break;
 
      	case APP_WAIT_FOR_DATA: 

			if(DRV_SPI_BUFFER_EVENT_COMPLETE & DRV_SPI_BufferStatus (Read_Buffer_Handle)) {
				APP_SPI_CS_DESELECT();
				state = APP_READ_COMPLETE;
			}
			break;

		default:	// APP_READ_COMPLETE:
         	break; /* end of function */
   	}
 }
