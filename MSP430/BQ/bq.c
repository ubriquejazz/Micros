/**
  ******************************************************************************
  * @file    bq76952.h
  * @author  IDJGB0
  * @version V1.0.0
  * @date    30 October 2020
  * @brief   Source file of bq76952 driver.
  ******************************************************************************
  */

#include "bq76952.h"
#include "bq76952_hm.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "IO.h"
#include "PortMap.h"

/* Defines -------------------------------------------------------------------*/

#define BQ76952_Printf(args ...)		IO_SendString(args)
#define BQ76952_Assert(args ...)
#define BQ76952_ErrorPrintf(args ...)	IO_SendString(args)

/* Global variables ----------------------------------------------------------*/

bq76952_t Bq76952;

static char* ResultString[] = {
		"Bq76952_Op Completed\r\n",	// IDN_OK
		"Bq76952_Op Nack\r\n",		// IDN_ERROR
		"Bq76952_Op Busy\r\n",		// IDN_BUSY
		"Bq76952_Op Timeout\r\n" };	// IDN_TIMEOUT

// Create Buffers for 2, 3, or 4 bytes of data
uint8_t TX_2Byte [2] = {0x00, 0x00};
uint8_t TX_3Byte [3] = {0x00, 0x00, 0x00};
uint8_t TX_4Byte [4] = {0x00, 0x00, 0x00, 0x00};

/* Private function prototypes -----------------------------------------------*/

static void  BQ76952_Task(void const *pvParameters);

/**
  * @brief  Timer functions to create, launch and delete
  */
static void BQ76952_TimerCreate(void);
static void BQ76952_TimerLaunch(uint32_t millisec);
static void BQ76952_TimerStop(void);

/**
  * @brief  Os Notification/Signal functions
  */
static void BQ76952_SignalSet(int32_t signals);
static void BQ76952_SignalWait(int32_t signals, uint32_t);

/* Callback functions --------------------------------------------------------*/

static void BQ76952_TxCallback (I2C_HandleTypeDef *hi2c) {
	Bq76952.intResult = IDN_OK;
	BQ76952_SignalSet(Bq76952.intSignal);
}

static void BQ76952_RxCallback (I2C_HandleTypeDef *hi2c) {
	Bq76952.intResult = IDN_OK;
	BQ76952_SignalSet(Bq76952.intSignal);
}

static void BQ76952_ErrorCallback (I2C_HandleTypeDef *hi2c) {
	Bq76952.intResult = IDN_ERROR;
	BQ76952_SignalSet(Bq76952.intSignal);
}

static void BQ76952_TimerCallback(void const *arg)
{
	Bq76952.intResult = IDN_TIMEOUT;
	BQ76952_SignalSet(Bq76952.intSignal);
}

/* Private functions ---------------------------------------------------------*/

uint8_t Checksum(uint8_t *ptr, uint8_t len)
{
	uint8_t i, checksum = 0;
	for(i=0; i<len; i++)
		checksum += ptr[i];
	checksum = 0xff & ~checksum;
	return(checksum);
}

idn_RetVal_t MillisecondsU2 (uint16_t ms, uint16_t* result)
{
	idn_RetVal_t ret = IDN_OK;
	float delay = (float)ms/3.3 - 2.0;
	if(delay < 1.0 || delay > 2047.0) {
		*result = 74;
		ret = IDN_ERROR;
	}
	else {
		*result = (uint16_t) delay;
	}
	return ret;
}

idn_RetVal_t MillisecondsU1 (uint16_t ms, uint8_t offset, uint8_t* result)
{
	idn_RetVal_t ret = IDN_OK;
	float delay = (float)ms/3.3 - 2.0;
	if(delay < 1.0 || delay > 127.0) {
		*result = offset;
		ret = IDN_ERROR;
	}
	else {
		*result = (uint8_t) delay;
	}
	return ret;
}

/* I2C Write and Read Functions (PRIVATE) */

idn_RetVal_t I2C_Master_Request()
{
	HAL_StatusTypeDef ret;
	ret = IDN_HAL_I2C_Request (BQ76952_Hm_GetPeripheralAddr(), &(Bq76952.devHandle), &(Bq76952.callbacks), BQ76952_TIMEOUT_MS);
	if(ret != HAL_OK) {
		BQ76952_Printf("HAL Request function error!\r\n");
	}
	else {

		/* Configure I2C */
		BQ76952_Hm_ConfigureDeviceHandler(Bq76952.devHandle);

		// Init structure from CubeMX
		IDN_HAL_I2C_Config(Bq76952.devHandle);

		/* Start timeout */
		Bq76952.intResult = IDN_BUSY;
		BQ76952_TimerLaunch(BQ76952_TIMEOUT_MS);
	}
	return (idn_RetVal_t)ret;
}

idn_RetVal_t I2C_Master_Realease()
{
	BQ76952_TimerStop();
	IDN_HAL_I2C_Release (BQ76952_Hm_GetPeripheralAddr());
	return IDN_OK;
}

/* Register functions -------------------------------------------------------*/

/**
  * @brief  For slave device with dev_addr, writes the data specified in *reg_data
  * @param  			dev_addr: 	The slave device address. Example: SLAVE_ADDR
  * @param  			reg_addr: 	The register or command to send to the slave.
  * @param  			*reg_data: 	The buffer to write
  * @param  			count: 		The length of *reg_data
  * @retval Idn return value
  *           + IDN_OK    if Ok
  *           + IDN_ERROR otherwise
  */
idn_RetVal_t BQ76952_WriteReg(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint8_t count)
{
	HAL_StatusTypeDef ret;

	if (I2C_Master_Request() == IDN_OK)
	{
		ret = IDN_HAL_I2C_Mem_Write_IT (Bq76952.devHandle, dev_addr, reg_addr, 1, reg_data, count);
		if(ret != HAL_OK) {
			BQ76952_Printf("HAL write function error!\r\n");
		}
		else {
			BQ76952_SignalWait(Bq76952.intSignal, osWaitForever);
			BQ76952_Printf (ResultString[Bq76952.intResult] );
		}
		I2C_Master_Realease();
	}
	return Bq76952.intResult;
}

/**
  * @brief  For slave device with dev_addr, read the data specified in slaves reg_addr.
  * 		The received data is available in Bq76952.buf
  * @param  			dev_addr: 	The slave device address. Example: SLAVE_ADDR
  * @param  			reg_addr: 	The register or command to send to the slave.
  * @param  			count: 		The length of data to read
  * @retval Idn return value
  *           + IDN_OK    if Ok
  *           + IDN_ERROR otherwise
  */
idn_RetVal_t BQ76952_ReadReg(uint8_t dev_addr, uint8_t reg_addr, uint8_t count)
{
	HAL_StatusTypeDef ret;

	if (I2C_Master_Request() == (idn_RetVal_t)HAL_OK)
	{
		ret = IDN_HAL_I2C_Mem_Read_IT (Bq76952.devHandle, dev_addr, reg_addr, 1, Bq76952.buf, count);
		if(ret != HAL_OK) {
			BQ76952_Printf("HAL read function error!\r\n");
		}
		else {
			BQ76952_SignalWait(Bq76952.intSignal, osWaitForever);
			BQ76952_Printf (ResultString[Bq76952.intResult] );
		}
		I2C_Master_Realease();
	}
	return Bq76952.intResult;
}

/* Standard functions ------------------------------------------------------*/

/**
  * @brief	Initialize the global structure (timer, callbacks, etc).
  * @retval	Idn return value
  * 					+ IDN_OK		if Ok
  * 					+ IDN_ERROR	otherwise
  */
idn_RetVal_t BQ76952_Init() 
{
	/* Initialize structure */
	Bq76952.intResult 	= IDN_ERROR;
	Bq76952.state 		= BQ76952_STATE_INIT;
	Bq76952.devHandle 	= BQ76952_Hm_GetDeviceHandler();

	/* Create timers */
	BQ76952_TimerCreate();

	/* -- Create and launch the Task -- */
	/* Thread definition OnlyTask. */
	osThreadDef( BQ76952_Task, BQ76952_Task, osPriorityNormal, 0, (1*configMINIMAL_STACK_SIZE) );

	/* Create the task. */
	Bq76952.taskHandle = osThreadCreate( osThread(BQ76952_Task), NULL );

	/* Configure callbacks */
	Bq76952.callbacks.AbortCpltCallback 	= NULL;
	Bq76952.callbacks.AddrCallback			= NULL;
	Bq76952.callbacks.ListenCpltCallback	= NULL;
	Bq76952.callbacks.MemTxCpltCallback		= NULL;
	Bq76952.callbacks.MemRxCpltCallback		= NULL;
	Bq76952.callbacks.SlaveRxCpltCallback	= NULL;
	Bq76952.callbacks.SlaveTxCpltCallback	= NULL;
	Bq76952.callbacks.MasterTxCpltCallback 	= &BQ76952_TxCallback;
	Bq76952.callbacks.MasterRxCpltCallback 	= &BQ76952_RxCallback;
	Bq76952.callbacks.ErrorCallback 		= &BQ76952_ErrorCallback;
	
	/* Operation */
	Bq76952.operation.action = BQ76952_OP_UNKNOWN;
	Bq76952.operation.value = 0x0000;
	Bq76952.operation.callback = NULL;
	return IDN_OK;
}

/**
  * @brief	Requests the device (thread-safe not implemented)
  * @retval	Idn return value
  * 					+ IDN_OK		if Ok
  * 					+ IDN_ERROR	otherwise
  */
idn_RetVal_t BQ76952_Request()
{
	idn_RetVal_t ret = IDN_ERROR;
	if (Bq76952.state == BQ76952_STATE_IDLE){
		Bq76952.state = BQ76952_STATE_REQUESTED;
		BQ76952_SignalSet(Bq76952.userSignal);
		ret = IDN_OK;
	}
	return ret;
}

/**
  * @brief  Execute an operation.
  */
idn_RetVal_t BQ76952_ExecuteOperation(bq76952_operation_t action, uint16_t value, bq76952_callback_t func)
{
	idn_RetVal_t ret = IDN_ERROR;
	if (Bq76952.state == BQ76952_STATE_REQUESTED){
		Bq76952.state = BQ76952_STATE_EXEC;
		Bq76952.operation.action = action;
		Bq76952.operation.value = value;
		Bq76952.operation.callback = func;
		BQ76952_SignalSet(Bq76952.userSignal);
		ret = IDN_OK;
	}
	return ret;
}

/**
  * @brief	Releases the device (thread-safe not implemented)
  * @retval	Idn return value
  * 					+ IDN_OK		if Ok
  * 					+ IDN_ERROR	otherwise
  */
idn_RetVal_t BQ76952_Release()
{
	idn_RetVal_t ret = IDN_ERROR;
	if (Bq76952.state == BQ76952_STATE_EXEC){
		Bq76952.state = BQ76952_STATE_RELEASED;
		BQ76952_SignalSet(Bq76952.userSignal);
		ret = IDN_OK;
	}
	return ret;
}

/**
  * @brief  Helper for the callback.
  */
uint8_t BQ76952_GetLastResult(void* output)
{
	uint8_t	 length;
	switch(Bq76952.operation.action)
	{
		case BQ76952_OP_MANUFACTURER:
		case BQ76952_OP_DEVICE_NUMBER:
			break;

		case BQ76952_OP_READ_TEMP:
		case BQ76952_OP_READ_CC:
		case BQ76952_OP_READ_VCELL:
			Bq76952.value = Bq76952.buf[1] * 256 +  Bq76952.buf[0];
			output = (void*) &Bq76952.value;
			length = 2;
			break;

		case BQ76952_OP_ENA_PROTECT:
			output = (void*) &Bq76952.buf[0];
			length = 1;
			break;

		default:
		case BQ76952_OP_UNKNOWN:
			break;
	}
	return length;
}

/* Private functions -------------------------------------------------------*/

/**
  * @brief  Helper to execute actions.
  */
void BQ76952_ExecuteOperation_Helper(bq76952_op_t op, char* result)
{
	#warning "The argument op.value is ignored!!"

	switch(op.action)
	{
		case BQ76952_OP_ALARM_ENABLE:
			Bq76952.value = 0xF082;
			BQCFG_Set_AlarmEnable (Bq76952.value , result);
			break;

		case BQ76952_OP_READ_VCELL:
			BQ76952_ReadReg(BQ76952_SLAVE_ADDR, BQ76952_REG_VCELL_1, 2);
			I2C_WAIT(1);
			sprintf(result, "Read Voltage on Cell #1 0x%04x", Bq76952.buf[1] * 256 +  Bq76952.buf[0]);
			break;

		case BQ76952_OP_READ_CC:
			BQ76952_ReadReg(BQ76952_SLAVE_ADDR, BQ76952_REG_CC2, 2);
			I2C_WAIT(1);
			sprintf(result, "Read CC2 Current 0x%04x", Bq76952.buf[1] * 256 +  Bq76952.buf[0]);
			break;

		case BQ76952_OP_READ_TEMP:
			BQ76952_ReadReg(BQ76952_SLAVE_ADDR, BQ76952_REG_TEMPERATURE, 2);
			I2C_WAIT(1);
			sprintf(result, "Read Internal Temperature 0x%04x", Bq76952.buf[1] * 256 +  Bq76952.buf[0]);
			break;

		// ############## Subcommand ##############

		case BQ76952_OP_MANUFACTURER:
			BQCFG_Get_ManufacturerStatus(&Bq76952.value, result);
			break;

		case BQ76952_OP_DEVICE_NUMBER:
			BQCFG_Get_DeviceNumber(&Bq76952.value, result);
			break;

		case BQ76952_OP_FET_ENABLE:
			BQCFG_Set_FETEnable(result);
			break;

		case BQ76952_OP_RESET:
			BQCFG_Set_Reset(result);
			break;

		// ############## Wr-Rd Subcommand ##############

		case BQ76952_OP_VCELL_MODE:
			BQCFG_Set_ConfigUpdateMode(1, NULL);
			BQCFG_Set_VCellMode (&Bq76952.value, result);
			BQCFG_Set_ConfigUpdateMode(0, NULL);
			break;

		default:
		case BQ76952_OP_UNKNOWN:
			break;
	}
	if (op.callback != NULL) {
		op.callback();
	}
}

/**
  * @brief  Main task of the driver
  */
static void BQ76952_Task(void const *pvParameters)
{
	char result[20] = "Operation Unknown";

	for (;;) {

		switch (Bq76952.state)
		{
			case BQ76952_STATE_INIT:
				Bq76952.state = BQ76952_STATE_IDLE;
				break;

			case BQ76952_STATE_IDLE:
				BQ76952_SignalWait(Bq76952.userSignal, osWaitForever);
				break;

			case BQ76952_STATE_REQUESTED:
				BQ76952_SignalWait(Bq76952.userSignal, osWaitForever);
				break;

			case BQ76952_STATE_EXEC:
				BQ76952_ExecuteOperation_Helper(Bq76952.operation, result);
				BQ76952_Printf(result);
				BQ76952_SignalWait(Bq76952.userSignal, osWaitForever);
				break;

			case BQ76952_STATE_RELEASED:
				// Enter into Low Power mode (optional)
				Bq76952.state = BQ76952_STATE_IDLE;
				break;

			default:
			case BQ76952_STATE_ERROR:
				break;
		}
	}
}







/* ------- Timer Functions ------- */
/**
* @brief Create the driver timer
*
* @param none
* @return none
*/
static void BQ76952_TimerCreate(void)
{
	/* Create the Timer */
	osTimerDef(	none, /* Timer Name (name facilitate when debugging). */
				BQ76952_TimerCallback); /* CallBack Function Timer. */
	Bq76952.timerId = osTimerCreate(osTimer(none), osTimerOnce, NULL);

	if(Bq76952.timerId == NULL)
	{
		BQ76952_Printf("Create master timer fail");
		BQ76952_Assert(Bq76952.timerId);
	}
}

/**
* @brief Starts the driver timer
*
* @param millisec Timeout milliseconds
* @return none
*/
static void BQ76952_TimerLaunch(uint32_t millisec)
{
	if (osTimerStart(Bq76952.timerId, millisec) != osOK)
	{
		BQ76952_Printf("Start master timer fail");
		/* Task in NOT Blocked */
	}
}

/**
* @brief Stops the driver timer
*
* @param none
* @return none
*/
static void BQ76952_TimerStop(void)
{
	if (osTimerStop(Bq76952.timerId) != osOK)
	{
		BQ76952_Printf("Stops master timer fail");
		/* Task in NOT Blocked */
	}
}

/**
* @brief Delete the driver timer
*
* @param none
* @return none
*/
static void BQ76952_TimerDelete(void)
{
	/* Cannot be called in an IRQ!! */
	if (osTimerDelete(Bq76952.timerId) != osOK)
	{
		BQ76952_Printf("Delete master timer fail");
		/* Task in NOT Blocked */
	}
}

/* -------- Os Notification/Signal functions -------- */
static void BQ76952_SignalWait(int32_t signals, uint32_t millisec)
{
	osEvent signalEvent;

	signalEvent = osSignalWait (signals, millisec);
	if(signalEvent.status == osEventSignal)
	{
		BQ76952_Assert("BQ76952_Signal: Returned OK");
	}
	else
	{
		BQ76952_ErrorPrintf("BQ76952_Signal: Returned KO");
	}
}

static void BQ76952_SignalSet(int32_t signals)
{
	if(osSignalSet (Bq76952.taskHandle, signals) != osOK)
	{
		BQ76952_ErrorPrintf("BQ76952_Signal: SetSignal KO");
	}
}
