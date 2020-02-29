#include "fram_app.h"
#include "sensor_app.h"

FRAM_APP_DATA fram_appData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* Application's SPI Instance 1 Operation Starting Callback */
static void FRAM_APP_OpStartingHandler(
    DRV_SPI_BUFFER_EVENT event,
    DRV_SPI_BUFFER_HANDLE bufferHandle,
    void* context
)
{
	/* Chip Select FRAM */
    FRAM_CSOn();
}

/* Application's SPI Instance 1 Operation Ending Callback */
static void FRAM_APP_OpEndingHandler(
    DRV_SPI_BUFFER_EVENT event,
    DRV_SPI_BUFFER_HANDLE bufferHandle,
    void* context
)
{
	/* De-select FRAM */
    FRAM_CSOff();
}

/* Application's SPI Instance 1 Buffer Event Callback */
static void FRAM_APP_BufferEventHandler(
    DRV_SPI_BUFFER_EVENT event,
    DRV_SPI_BUFFER_HANDLE bufferHandle,
    void * context
)
{
    switch(event)
    {
        case DRV_SPI_BUFFER_EVENT_COMPLETE:
            if (context) {
                *((BUFFER_STATUS*)context) = BUFFER_STATUS_COMPLETE;
            }
            break;

        case DRV_SPI_BUFFER_EVENT_ERROR:
        default:
            if (context) {
                *((BUFFER_STATUS*)context) = BUFFER_STATUS_ERROR;
            }
            break;
    }
}

/* Buffer to print temperature logs to console */
static uint8_t temperatureLogBuffer[MAX_TEMPERATURE_LOGS * sizeof(float)] __attribute__ ((aligned (4))) = {0};

/* Application function to print temperature to console */
static void FRAM_APP_PrintTemperatureToConsole(uint8_t* const pBuffer)
{
    uint8_t i = 0;
    for (i = 0; i < MAX_TEMPERATURE_LOGS * sizeof(float); i++)
    {
        temperatureLogBuffer[i] = pBuffer[i];
    }
    SYS_PRINT("\nFRAM Readout:\r\n");

    /* Print the logs with the oldest log first and the latest log in the end*/
    for (i = fram_appData.wrIndex; i < MAX_TEMPERATURE_LOGS; i++)
    {
        uint8_t* ptr = &temperatureLogBuffer[i*4];
        SYS_PRINT("%.2f\r\n", *((float*)ptr));
    }
    if (fram_appData.wrIndex > 0)
    {
        for (i = 0; i < fram_appData.wrIndex; i++)
        {
            uint8_t* ptr = &temperatureLogBuffer[i*4];
            SYS_PRINT("%.2f\r\n", *((float*)ptr));
        }
    }
}

/* Application function to set FRAM write request */
void FRAM_APP_SetWriteReq(void)
{
    fram_appData.isWriteReq = true;
}

/* Application function to get next FRAM address to write temperature to */
static uint16_t FRAM_APP_GetNextFramAddrToWrite(void)
{
    uint16_t framAddr = TEMPERATURE_LOG_START_ADDR;

    framAddr += fram_appData.wrIndex * sizeof(float);

    fram_appData.wrIndex++;

    if (fram_appData.wrIndex >= MAX_TEMPERATURE_LOGS)
    {
        fram_appData.wrIndex = 0;
    }

    return framAddr;
}

/* Application function to write enable the FRAM */
static bool FRAM_APP_WriteEnable(void)
{
    DRV_SPI_BUFFER_HANDLE handle;

    fram_appData.framWriteEnableData.wrBuffer[0] = FRAM_CMD_WREN;
    fram_appData.framWriteEnableData.nBytes = 1;
    fram_appData.framWriteEnableData.bufferStatus = BUFFER_STATUS_INVALID;

    handle = DRV_SPI_BufferAddWrite2(
                fram_appData.handle,
        (void*) fram_appData.framWriteEnableData.wrBuffer,
                fram_appData.framWriteEnableData.nBytes,
                FRAM_APP_BufferEventHandler,
        (void*) &fram_appData.framWriteEnableData.bufferStatus,
                &fram_appData.framWriteEnableData.bufferHandle
    );

    if (DRV_SPI_BUFFER_HANDLE_INVALID == handle)
    {
        return false; //Handle error condition
    }
    return true;
}

/* Application function to write to FRAM */
static bool FRAM_APP_Write(uint16_t startAddr, const uint8_t* pWrBuffer, uint8_t nWrBytes)
{
    uint8_t i;
    DRV_SPI_BUFFER_HANDLE handle;

    FRAM_APP_WriteEnable();

    fram_appData.framWriteData.wrBuffer[0] = FRAM_CMD_WRITE;
    fram_appData.framWriteData.wrBuffer[1] = (uint8_t)(startAddr >> 8);
    fram_appData.framWriteData.wrBuffer[2] = (uint8_t)(startAddr);

    /* Copy the data to be written to the framWrite request object */
    for (i = 0; i < nWrBytes; i++)
    {
        fram_appData.framWriteData.wrBuffer[3+i] = *pWrBuffer++;
    }

    fram_appData.framWriteData.nBytes = 3 + nWrBytes;
    fram_appData.framWriteData.bufferStatus = BUFFER_STATUS_INVALID;

    handle = DRV_SPI_BufferAddWrite2(
                fram_appData.handle,
        (void*) fram_appData.framWriteData.wrBuffer,
                fram_appData.framWriteData.nBytes,
                FRAM_APP_BufferEventHandler,
        (void*) &fram_appData.framWriteData.bufferStatus,
                &fram_appData.framWriteData.bufferHandle
    );

    if (DRV_SPI_BUFFER_HANDLE_INVALID == handle)
    {
        return false; //Handle error condition
    }
    return true;
}

/* Application function to read from FRAM */
static bool FRAM_APP_Read(uint16_t startAddr, uint8_t nRdBytes)
{
    DRV_SPI_BUFFER_HANDLE handle;

    fram_appData.framReadData.wrBuffer[0] = FRAM_CMD_READ;
    fram_appData.framReadData.wrBuffer[1] = (uint8_t)(startAddr >> 8);
    fram_appData.framReadData.wrBuffer[2] = (uint8_t)(startAddr);
    fram_appData.framReadData.nWrBytes = 3;

    /* Number of bytes to read must include the dummy bytes received during the
       write operation. */
    fram_appData.framReadData.nRdBytes = 3 + nRdBytes;
    fram_appData.framReadData.bufferStatus = BUFFER_STATUS_INVALID;

    handle = DRV_SPI_BufferAddWriteRead2(
                fram_appData.handle,
        (void*) fram_appData.framReadData.wrBuffer,
                fram_appData.framReadData.nWrBytes,
        (void*) fram_appData.framReadData.rdBuffer,
                fram_appData.framReadData.nRdBytes,
                FRAM_APP_BufferEventHandler,
        (void*) &fram_appData.framReadData.bufferStatus,
                &fram_appData.framReadData.bufferHandle
    );

    if (DRV_SPI_BUFFER_HANDLE_INVALID == handle)
    {
        return false; //Handle error condition
    }
    return true;
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

void FRAM_APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    fram_appData.state = FRAM_APP_STATE_INIT;
    fram_appData.keyValue = 0;
    fram_appData.isWriteReq = false;
    fram_appData.wrIndex = 0;
}

void FRAM_APP_Tasks ( void )
{
    switch ( fram_appData.state )
    {
        /* Application's initial state. */
        case FRAM_APP_STATE_INIT:

			/* Open SPI driver (Instance 1, SPI ID 1) here  */
            fram_appData.handle = DRV_SPI_Open (DRV_SPI_INDEX_1, DRV_IO_INTENT_READWRITE);

			if (DRV_HANDLE_INVALID != fram_appData.handle)
			{
				/* Set up client event handlers for operation starting and ending events*/
                DRV_SPI_CLIENT_DATA clientData = {0};
				clientData.operationStarting = FRAM_APP_OpStartingHandler;
                //clientData.baudRate = DRV_SPI_BAUD_RATE_IDX1;
				clientData.operationEnded = FRAM_APP_OpEndingHandler;
                DRV_SPI_ClientConfigure (fram_appData.handle, &clientData);

                /* Clear the FRAM temperature log memory locations */
                FRAM_APP_Write(TEMPERATURE_LOG_START_ADDR, 
                    temperatureLogBuffer, sizeof(temperatureLogBuffer));
                
				fram_appData.state = FRAM_APP_STATE_WRITE_REQ_STATUS;
			}
			else
			{
				fram_appData.state = FRAM_APP_STATE_ERROR;
            }
            break;

        case FRAM_APP_STATE_WRITE_REQ:

			if (fram_appData.isWriteReq)
			{
                /* Next FRAM address to write temperature to */
				uint16_t framAddr = FRAM_APP_GetNextFramAddrToWrite();
				/* Get Temperature and write to FRAM here */
                float temp = SENSOR_APP_GetTemperature();

                if (FRAM_APP_Write (framAddr, (uint8_t*)&temp, sizeof(temp)) == false) 
                {
                    fram_appData.state = FRAM_APP_STATE_ERROR;
                }
                else 
                {
                    fram_appData.state = FRAM_APP_STATE_WRITE_REQ_STATUS;
                }
                fram_appData.isWriteReq = false;
			}
            else
            {
                fram_appData.state = FRAM_APP_STATE_READ_REQ;
            }
            break;

        case FRAM_APP_STATE_READ_REQ:

            if (SYS_STATUS_READY == SYS_CONSOLE_Status(sysObj.sysConsole0))
			{
				if (0x0d == fram_appData.keyValue)
				{
					/* Read back saved temperature value from FRAM here */
                    if (FRAM_APP_Read(TEMPERATURE_LOG_START_ADDR, MAX_TEMPERATURE_LOGS * sizeof(float)) == false)
                    {
                        fram_appData.state = FRAM_APP_STATE_ERROR;
                    }
                    else
                    {
					   fram_appData.state = FRAM_APP_STATE_READ_REQ_STATUS;
                    }
				}
                else
                {
                    fram_appData.state = FRAM_APP_STATE_WRITE_REQ;
                }
                SYS_CONSOLE_Read( SYS_CONSOLE_INDEX_0, 0, &fram_appData.keyValue, 1 );
            }
            else
            {
                fram_appData.state = FRAM_APP_STATE_WRITE_REQ;
            }
            break;

        case FRAM_APP_STATE_WRITE_REQ_STATUS:

			/* Check the buffer status which is updated in the registered callback */
			if (BUFFER_STATUS_COMPLETE == fram_appData.framWriteData.bufferStatus
			&&  BUFFER_STATUS_COMPLETE == fram_appData.framWriteEnableData.bufferStatus) 
            {
				fram_appData.state = FRAM_APP_STATE_WRITE_REQ;
			}
            break;

		case FRAM_APP_STATE_READ_REQ_STATUS:

			/* Check the buffer status which is updated in the registered callback */
			if (BUFFER_STATUS_COMPLETE == fram_appData.framReadData.bufferStatus)
			{
				/* Print read value to console here ---------> (Step #7) */
                FRAM_APP_PrintTemperatureToConsole ((uint8_t*)&fram_appData.framReadData.rdBuffer[3]);
                fram_appData.state = FRAM_APP_STATE_WRITE_REQ;
			}
            break;

        /* The default state should never be executed. */
        case FRAM_APP_STATE_ERROR:
		default:
			SYS_PRINT("FRAM App Error\r\n");
			break;
    }
}

/*******************************************************************************
 End of File
 */
