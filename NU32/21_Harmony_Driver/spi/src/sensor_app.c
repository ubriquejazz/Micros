#include "sensor_app.h"
#include "fram_app.h"

SENSOR_APP_DATA sensor_appData;

/* Populate the sensor request objects */

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* Application's SPI Operation Starting Callback Function */
static void SENSOR_APP_OpStartingHandler(
    DRV_SPI_BUFFER_EVENT event,
    DRV_SPI_BUFFER_HANDLE bufferHandle,
    void* context
)
{
	/* Chip Select Sensor here ----------> (Step #3) */
    SENSOR_CSOn();
}

/* Application's SPI Operation Ending Callback Function */
static void SENSOR_APP_OpEndingHandler(
    DRV_SPI_BUFFER_EVENT event,
    DRV_SPI_BUFFER_HANDLE bufferHandle,
    void* context
)
{
	/* De-select Sensor */
    SENSOR_CSOff();
}

/* Application's SPI Buffer Event Callback Function */
static void SENSOR_APP_BufferEventHandler(
    DRV_SPI_BUFFER_EVENT event,
    DRV_SPI_BUFFER_HANDLE bufferHandle,
    void * context
)
{
    switch(event)
    {
        case DRV_SPI_BUFFER_EVENT_COMPLETE:
            if (context)
            {
				/* Copy the buffer status here ----------> (Step #7) */
                *((SENSOR_APP_SPI_BUFFER_STATUS*)context) = SENSOR_APP_SPI_BUFFER_STATUS_COMPLETE;
            }
            break;

        case DRV_SPI_BUFFER_EVENT_ERROR:
        default:
            if (context)
            {
				*((SENSOR_APP_SPI_BUFFER_STATUS*)context) = SENSOR_APP_SPI_BUFFER_STATUS_ERROR;
            }
            break;
    }
}

/* Application's Timer Callback Function */
static void SENSOR_APP_PeriodicTimerCallback(uintptr_t context, uint32_t currTick)
{
    /* Set temperature measurement request here ---------> (Step #5) */
	/* Comment the below line of code for Lab 3 ---------> (Lab 3_3) */
    //sensor_appData.readTemperatureReq = true;

	/* Un-comment the below line of code for Lab 3 ---------> (Lab 3_3) */

	xSemaphoreGive(sensor_appData.xSemaphore);
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


static void SENSOR_APP_SaveCalibValues(const uint8_t* const pCalibValues)
{
    uint8_t i;

    for(i = 0; i < sizeof (sensor_appData.sensorCalibValues); i++)
    {
        sensor_appData.sensorCalibValues[i] = pCalibValues[i];
    }
}

static float SENSOR_APP_CalcTemperature(
    const uint8_t* const pTemperatureRawValue,
    const uint8_t* const pCalibrationValues
)
{
    int32_t  temp1, temp2;
    int32_t  adc_t;
    uint16_t dig_T1 = *((uint16_t*)&pCalibrationValues[0]);
    int16_t  dig_T2 = *((int16_t*)&pCalibrationValues[2]);
    int16_t  dig_T3 = *((int16_t*)&pCalibrationValues[4]);
    float degC;

    adc_t  = (unsigned long)pTemperatureRawValue[2] >> 4;
    adc_t |= (unsigned long)pTemperatureRawValue[1] << 4;
    adc_t |= (unsigned long)pTemperatureRawValue[0] << 12;

    temp1 = ((((adc_t>>3) -((int32_t)dig_T1<<1))) * ((int32_t)dig_T2)) >> 11;
    temp2 = (((((adc_t>>4) - ((int32_t)dig_T1)) * ((adc_t>>4) - ((int32_t)dig_T1))) >> 12) * ((int32_t)dig_T3)) >> 14;
    degC = ((int32_t) (((temp1 + temp2) * 5 + 128) >> 8))/(float)100.0;
    return (degC * 9)/5 + 32;
}

/* Application function to return temperature value */
float SENSOR_APP_GetTemperature(void)
{
    return sensor_appData.temperature;
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

void SENSOR_APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    sensor_appData.state = SENSOR_APP_STATE_INIT;

    sensor_appData.readTemperatureReq = false;

	/* Un-comment the below lines of code for Lab 3 ---------> (Lab 3_2) */


	sensor_appData.xSemaphore = xSemaphoreCreateBinary();

	if (NULL == sensor_appData.xSemaphore)
	{
		sensor_appData.state = SENSOR_APP_STATE_ERROR;
	}

}

void SENSOR_APP_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( sensor_appData.state )
    {
        /* Application's initial state. */
        case SENSOR_APP_STATE_INIT:
        {
            DRV_SPI_CLIENT_DATA clientData = {0};


			/* Open SPI driver (Instance 0, SPI ID 2) here ----------> (Step #1) */

            sensor_appData.handle = DRV_SPI_Open( DRV_SPI_INDEX_0, DRV_IO_INTENT_READWRITE );


			if (DRV_HANDLE_INVALID != sensor_appData.handle)
			{

				/* Set up client event handlers for operation starting and ending events*/

				//clientData.baudRate = DRV_SPI_BAUD_RATE_IDX0;
				clientData.operationStarting = SENSOR_APP_OpStartingHandler;
				clientData.operationEnded = SENSOR_APP_OpEndingHandler;

				/* Set client configuration here ----------> (Step #2) */

                DRV_SPI_ClientConfigure(sensor_appData.handle, &clientData );




				sensor_appData.state = SENSOR_APP_STATE_SENSOR_INIT;
			}
			else
			{
				sensor_appData.state = SENSOR_APP_STATE_ERROR;
			}

            break;
        }

        case SENSOR_APP_STATE_SENSOR_INIT:
        {
            DRV_SPI_BUFFER_HANDLE handle;

			//Queue up multiple SPI requests to initialize the sensor

			/* +Read Calibration values */

            sensor_appData.calibrationData.wrBuffer[0]    = RD(BME280_TEMPERATURE_CALIB_DIG_T1_LSB_REG),
            sensor_appData.calibrationData.nWrBytes       = 1,
            sensor_appData.calibrationData.nRdBytes       = 1+6,
            sensor_appData.calibrationData.bufferStatus   = SENSOR_APP_SPI_BUFFER_STATUS_INVALID,

            handle = DRV_SPI_BufferAddWriteRead2(
                sensor_appData.handle,
                (void*)sensor_appData.calibrationData.wrBuffer,
                sensor_appData.calibrationData.nWrBytes,
                (void*)sensor_appData.calibrationData.rdBuffer,
                sensor_appData.calibrationData.nRdBytes,
                SENSOR_APP_BufferEventHandler,
                (void*)&sensor_appData.calibrationData.bufferStatus,
                &sensor_appData.calibrationData.bufferHandle
            );

            if (DRV_SPI_BUFFER_HANDLE_INVALID == handle)
            {
                sensor_appData.state = SENSOR_APP_STATE_ERROR;
            }

			// Configure sensor

            sensor_appData.sensorControlData.wrBuffer[0]    = WR(BME280_CTRL_MEAS_REG),
            sensor_appData.sensorControlData.wrBuffer[1]    = BME280_OVERSAMP_1X + BME280_NORMAL_MODE,
            sensor_appData.sensorControlData.nBytes         = 2,
            sensor_appData.sensorControlData.bufferStatus   = SENSOR_APP_SPI_BUFFER_STATUS_INVALID,

			/* +Write Oversampling Rate */

			handle = DRV_SPI_BufferAddWrite2(
				sensor_appData.handle,
				(void*)sensor_appData.sensorControlData.wrBuffer,
				sensor_appData.sensorControlData.nBytes,
				SENSOR_APP_BufferEventHandler,
				(void*)&sensor_appData.sensorControlData.bufferStatus,
				&sensor_appData.sensorControlData.bufferHandle
			);

            if (DRV_SPI_BUFFER_HANDLE_INVALID == handle)
            {
                sensor_appData.state = SENSOR_APP_STATE_ERROR;
            }

            sensor_appData.state = SENSOR_APP_STATE_WAIT_INIT_COMPLETE;
            break;
        }

        case SENSOR_APP_STATE_WAIT_INIT_COMPLETE:

        	/* Check the buffer status which is updated in the registered callback */

			if (SENSOR_APP_SPI_BUFFER_STATUS_COMPLETE == sensor_appData.calibrationData.bufferStatus &&
					SENSOR_APP_SPI_BUFFER_STATUS_COMPLETE == sensor_appData.sensorControlData.bufferStatus)
			{
				SENSOR_APP_SaveCalibValues(&sensor_appData.calibrationData.rdBuffer[1]);

				/* Start Periodic Timer here using Timer System Service here -----------> (Step #4) */

                SYS_TMR_CallbackPeriodic (
                    TEMPERATURE_READ_PERIOD,
                    NULL,
                    SENSOR_APP_PeriodicTimerCallback
                );

				sensor_appData.state = SENSOR_APP_STATE_WAIT_MEAS_REQ;
            }
        	break;

		case SENSOR_APP_STATE_WAIT_MEAS_REQ:
        {
            DRV_SPI_BUFFER_HANDLE handle;

			/* Comment the below line of code for Lab 3 ---------> (Lab 3_4) */
			//if (true == sensor_appData.readTemperatureReq)

			/* Un-comment the below line of code for Lab 3 ---------> (Lab 3_4) */
			xSemaphoreTake( sensor_appData.xSemaphore, portMAX_DELAY);
			{
                sensor_appData.temperatureData.wrBuffer[0]    = RD(BME280_TEMP_REG_ADDR);
                sensor_appData.temperatureData.nWrBytes       = 1;
                sensor_appData.temperatureData.nRdBytes       = 1+3;
                sensor_appData.temperatureData.bufferStatus   = SENSOR_APP_SPI_BUFFER_STATUS_INVALID;

				/* Queue Temperature Measurement Request here -----------> (Step #6) */

                handle = DRV_SPI_BufferAddWriteRead2 (
                    sensor_appData.handle,
                    (void*)sensor_appData.temperatureData.wrBuffer,
                    sensor_appData.temperatureData.nWrBytes,
                    (void*)sensor_appData.temperatureData.rdBuffer,
                    sensor_appData.temperatureData.nRdBytes,
                    SENSOR_APP_BufferEventHandler,
					(void*)&sensor_appData.temperatureData.bufferStatus,
                    &sensor_appData.temperatureData.bufferHandle
                );

                if (DRV_SPI_BUFFER_HANDLE_INVALID == handle)
                {
                    sensor_appData.state = SENSOR_APP_STATE_ERROR;
                }

				sensor_appData.readTemperatureReq = false;
				sensor_appData.state = SENSOR_APP_STATE_WAIT_MEAS_COMPLETE;
            }

			break;
        }

		case SENSOR_APP_STATE_WAIT_MEAS_COMPLETE:

			/* Poll the status of the submitted buffer */

			if (DRV_SPI_BUFFER_EVENT_COMPLETE & DRV_SPI_BufferStatus(sensor_appData.temperatureData.bufferHandle))
			{
				sensor_appData.temperature = SENSOR_APP_CalcTemperature(
                        &sensor_appData.temperatureData.rdBuffer[1],
                        sensor_appData.sensorCalibValues);

				/* Print to console using System Console Service here ----------> (Step #8) */
                SYS_PRINT("Temperature:%.2f\r", sensor_appData.temperature);



				/* Un-comment below line for ---------> (Lab 2) */
				FRAM_APP_SetWriteReq();

				sensor_appData.state = SENSOR_APP_STATE_WAIT_MEAS_REQ;
            }
			break;


        /* The default state should never be executed. */
        case SENSOR_APP_STATE_ERROR:
		default:
			SYS_PRINT("Sensor Task Error\r\n");

			break;
    }
}

/*******************************************************************************
 End of File
 */
