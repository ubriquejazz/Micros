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
#include "bqcommand.h"

/* Defines ------------------------------------------------------------------*/

#define BQ76952_Printf(args ...)		IO_SendString(args)
#define BQ76952_Assert(args ...)
#define BQ76952_ErrorPrintf(args ...)	IO_SendString(args)

/* Private typedef -----------------------------------------------------------*/

typedef enum
{
	BQ76952_STATE_POWERDOWN		= 0x00U,	/*!< Init		*/
	BQ76952_STATE_ERROR			= 0x01U,	/*!< Error 		*/
	BQ76952_STATE_IDLE			= 0x02U,	/*!< Wait		*/
	BQ76952_STATE_REQUESTED		= 0x03U,	/*!< Request	*/
	BQ76952_STATE_RELEASED		= 0x04U,	/*!< Release	*/
	BQ76952_STATE_EXEC			= 0x05U,	/*!< Operation	*/
} bq76952_state_t;

typedef struct 
{
	I2C_HandleTypeDef* 			devHandle;			/*!< device handle 		*/
	Idn_Hal_I2c_Callbacks_t		callbacks;			/*!< callback structure */
	osTimerId					timerId;			/*!< timer ID 			*/
	osThreadId 					taskHandle;			/*!< task Handle		*/
	int32_t						intSignal;			/*!< i2c notification 	*/
	int32_t						userSignal;			/*!< user notification	*/

	bq76952_state_t				state;				/*!< internal state		*/
	bq76952_op_t 				operation;			/*!< action, value, cb	*/ 
	uint8_t 					buf[12];			/*!< read buffer 		*/
	uint16_t 					value;				/*!< read buffer 		*/
	volatile uint8_t			timerFlag;			/*!< flag when timer  	*/
	volatile idn_RetVal_t		intResult;			/*!< internal i2c result */
} bq76952_t;

/* Global variables ----------------------------------------------------------*/

static bq76952_t 	bq76952;

static char* ResultString[] = {
		"Bq76952_Op Completed\r\n",	// IDN_OK
		"Bq76952_Op Nack\r\n",		// IDN_ERROR
		"Bq76952_Op Busy\r\n",		// IDN_BUSY
		"Bq76952_Op Timeout\r\n" };	// IDN_TIMEOUT

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
	bq76952.intResult = IDN_OK;
	BQ76952_SignalSet(bq76952.intSignal);
}

static void BQ76952_RxCallback (I2C_HandleTypeDef *hi2c) {
	bq76952.intResult = IDN_OK;
	BQ76952_SignalSet(bq76952.intSignal);
}

static void BQ76952_ErrorCallback (I2C_HandleTypeDef *hi2c) {
	bq76952.intResult = IDN_ERROR;
	BQ76952_SignalSet(bq76952.intSignal);
}

static void BQ76952_TimerCallback(void const *arg)
{
	bq76952.intResult = IDN_TIMEOUT;
	BQ76952_SignalSet(bq76952.intSignal);
}

/* Private functions ---------------------------------------------------------*/

idn_RetVal_t I2C_Master_Request()
{
	HAL_StatusTypeDef ret;
	ret = IDN_HAL_I2C_Request (BQ76952_Hm_GetPeripheralAddr(), &(bq76952.devHandle), &(bq76952.callbacks), BQ76952_TIMEOUT_MS);
	if(ret != HAL_OK) {
		BQ76952_Printf("HAL Request function error!\r\n");
	}
	else {

		/* Configure I2C */
		BQ76952_Hm_ConfigureDeviceHandler(bq76952.devHandle);

		// Init structure from CubeMX
		IDN_HAL_I2C_Config(bq76952.devHandle);

		/* Start timeout */
		bq76952.intResult = IDN_BUSY;
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
idn_RetVal_t I2C_Master_WriteReg(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint8_t count)
{
	HAL_StatusTypeDef ret;

	if (I2C_Master_Request() == IDN_OK)
	{
		ret = IDN_HAL_I2C_Mem_Write_IT (bq76952.devHandle, dev_addr, reg_addr, 1, reg_data, count);
		if(ret != HAL_OK) {
			BQ76952_Printf("HAL write function error!\r\n");
		}
		else {
			BQ76952_SignalWait(bq76952.intSignal, osWaitForever);
			BQ76952_Printf ( ResultString[bq76952.intResult] );
		}
		I2C_Master_Realease();
	}
	return bq76952.intResult;
}

/**
  * @brief  For slave device with dev_addr, read the data specified in slaves reg_addr.
  * 		The received data is available in bq76952.buf
  * @param  			dev_addr: 	The slave device address. Example: SLAVE_ADDR
  * @param  			reg_addr: 	The register or command to send to the slave.
  * @param  			count: 		The length of data to read
  * @retval Idn return value
  *           + IDN_OK    if Ok
  *           + IDN_ERROR otherwise
  */
idn_RetVal_t I2C_Master_ReadReg(uint8_t dev_addr, uint8_t reg_addr, uint8_t count)
{
	HAL_StatusTypeDef ret;

	if (I2C_Master_Request() == (idn_RetVal_t)HAL_OK)
	{
		ret = IDN_HAL_I2C_Mem_Read_IT (bq76952.devHandle, dev_addr, reg_addr, 1, bq76952.buf, count);
		if(ret != HAL_OK) {
			BQ76952_Printf("HAL read function error!\r\n");
		}
		else {
			BQ76952_SignalWait(bq76952.intSignal, osWaitForever);
			BQ76952_Printf ( ResultString[bq76952.intResult] );
		}
		I2C_Master_Realease();
	}
	return bq76952.intResult;
}

/**
  * @brief  Write Register.
  */
idn_RetVal_t BQ76952_WriteReg(uint8_t reg_addr, uint8_t *reg_data, uint8_t count)
{
	idn_RetVal_t ret = IDN_ERROR;
	#if CRC_Mode
	#else
		ret = I2C_Master_WriteReg(BQ76952_I2C_ADDR_WRITE, reg_addr, reg_data, count);
	#endif		
	return ret;
}

/**
  * @brief  Read Register.
  */
idn_RetVal_t BQ76952_ReadReg(uint8_t reg_addr, uint8_t count)
{
	idn_RetVal_t ret = IDN_OK;
	uint8_t RX_CRC_Fail = 0;  // reset to 0. If in CRC Mode and CRC fails, this will be incremented.
	#if CRC_Mode
	#else
		I2C_Master_ReadReg(BQ76952_I2C_ADDR_READ, reg_addr, count);
	#endif
	if (RX_CRC_Fail > 0)
		ret = IDN_ERROR;
	return ret;
}

/**
  * @brief  Direct Command = Wr(1), Rd(2)
  */
idn_RetVal_t BQ76952_DirectCommand(uint8_t command, uint16_t* data) 
{
	idn_RetVal_t ret = IDN_OK;
	BQ76952_ReadReg(command, 2);
	BQ76952_Printf("[+] Direct Cmd SENT ->");	// command
	BQ76952_Printf("[+] Direct Cmd RESP <-");	// bq76952.buf[i]
	// strcpy(data, bq76952.buf, 2);
	return ret;
}

/**
  * @brief Command = Wr(3)
  */
idn_RetVal_t BQ76952_BasicCommand(uint8_t reg_addr, uint16_t data) 
{
	idn_RetVal_t ret = IDN_OK;
	BQ76952_WriteReg(reg_addr, data, 2);			//
	BQ76952_Printf("[+] Cmd SENT to 0x%02x ->");	//
	return ret;
}

/**
  * @brief  Sub Command = Wr(3)
  */
idn_RetVal_t BQ76952_SubCommandTransmit(uint16_t data) 
{
	idn_RetVal_t ret = IDN_OK;
	ret = BQ76952_BasicCommand(BQ76952_REG_COMMAND, data);
	return ret;
}

/**
  * @brief  Sub Command Response = Wr(1) + Rd(2)
  */
idn_RetVal_t BQ76952_SubCommandResponse(uint8_t* data, uint8_t count) 
{
	idn_RetVal_t ret = IDN_OK;
	BQ76952_WriteReg(BQ76952_REG_RESPONSE, count);		// 0x40

	BQ76952_Printf("[+] Sub Cmd RESP at 0x40 ->");		// bq76952.buf[i]
	strcpy(data, bq76952.buf, count);
	return ret;
}

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Helper to execute actions.
  */
void BQ76952_ExecuteOperation_Helper(bq76952_op_t op, char* result)
{
	uint16_t value;	// Tempertature, Voltage or Current
	#warning "The argument op.value is ignored!!"

	switch(op.action)
	{
		case BQ76952_OP_ALARM_ENABLE:
			BQ76952_AlarmEnable(0xF082, result),
			break;

		case BQ76952_OP_FET_ENABLE:
			BQ76952_FETEnable(result);
			break;

		case BQ76952_OP_RESET:
			BQ76952_Reset(result);
			break;

		case BQ76952_OP_MANUFACTURER:
			BQ76952_ManufacturerStatus(result),
			break;

		case BQ76952_OP_DEVICE_NUMBER:
			BQ76952_DeviceNumber(result);
			break;

		case BQ76952_OP_ENA_PROTECT:
			BQ76952_ReadEnableProtection(result);
			break;

		case ZZZZ:
			BQ76952_SetConfigUpdateMode(1, NULL);				// Enter CONFIG_UPDATE mode
			BQ76952_WriteEnableProtection(0x8C, result);
			BQ76952_SetConfigUpdateMode(0, NULL);				// Exit CONFIG_UPDATE mode
			break;

		case BQ76952_OP_VCELL_MODE:
			BQ76952_SetConfigUpdateMode(1, NULL);			// Enter CONFIG_UPDATE mode
			BQ76952_VCellMode(0x37F, result);				// 9-cell battery
			BQ76952_SetConfigUpdateMode(0, NULL);			// Exit CONFIG_UPDATE mode
			break;

		case BQ76952_OP_READ_VCELL:
			BQ76952_DirectCommand(BQ76952_REG_VCELL_1, &value);
			sprintf(result, "Read Voltage on Cell #1 0x%04x", value);
			break;

		case BQ76952_OP_READ_CC:
			BQ76952_DirectCommand(BQ76952_REG_CC2, &value);
			sprintf(result, "Read CC2 Current 0x%04x", value);
			break;

		case BQ76952_OP_READ_TEMP:
			BQ76952_DirectCommand(BQ76952_REG_TEMPERATURE, &value);
			sprintf(result, "Read Internal Temperature 0x%04x", value);
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

		switch (bq76952.state)
		{
			case BQ76952_STATE_POWERDOWN:
				bq76952.state = BQ76952_STATE_IDLE;
				break;

			case BQ76952_STATE_IDLE:
				BQ76952_SignalWait(bq76952.userSignal, osWaitForever);
				break;

			case BQ76952_STATE_REQUESTED:
				BQ76952_SignalWait(bq76952.userSignal, osWaitForever);
				break;

			case BQ76952_STATE_EXEC:
				BQ76952_ExecuteOperation_Helper(bq76952.operation, result);
				BQ76952_Printf(result);
				bq76952.state = BQ76952_STATE_IDLE;
				break;

			case BQ76952_STATE_RELEASED:
				// Enter into Low Power mode (optional)
				bq76952.state = BQ76952_STATE_POWERDOWN;
				break;

			default:
			case BQ76952_STATE_ERROR:
				break;
		}
	}
}

/* Public functions ---------------------------------------------------------*/

/**
  * @brief	Initialize the global structure (timer, callbacks, etc).
  * @retval	Idn return value
  * 					+ IDN_OK		if Ok
  * 					+ IDN_ERROR	otherwise
  */
idn_RetVal_t BQ76952_Init() 
{
	/* Initialize structure */
	bq76952.intResult 	= IDN_ERROR;
	bq76952.state 		= BQ76952_STATE_POWERDOWN;
	bq76952.devHandle 	= BQ76952_Hm_GetDeviceHandler();

	/* Create timers */
	BQ76952_TimerCreate();

	/* -- Create and launch the Task -- */
	/* Thread definition OnlyTask. */
	osThreadDef( BQ76952_Task, BQ76952_Task, osPriorityNormal, 0, (1*configMINIMAL_STACK_SIZE) );

	/* Create the task. */
	bq76952.taskHandle = osThreadCreate( osThread(BQ76952_Task), NULL );

	/* Configure callbacks */
	bq76952.callbacks.AbortCpltCallback 	= NULL;
	bq76952.callbacks.AddrCallback			= NULL;
	bq76952.callbacks.ListenCpltCallback	= NULL;
	bq76952.callbacks.MemTxCpltCallback		= NULL;
	bq76952.callbacks.MemRxCpltCallback		= NULL;
	bq76952.callbacks.SlaveRxCpltCallback	= NULL;
	bq76952.callbacks.SlaveTxCpltCallback	= NULL;
	bq76952.callbacks.MasterTxCpltCallback 	= &BQ76952_TxCallback;
	bq76952.callbacks.MasterRxCpltCallback 	= &BQ76952_RxCallback;
	bq76952.callbacks.ErrorCallback 		= &BQ76952_ErrorCallback;
	
	/* Operation */
	bq76952.operation.action = BQ76952_OP_UNKNOWN;
	bq76952.operation.value = 0x0000;
	bq76952.operation.callback = NULL;
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
	if (bq76952.state == BQ76952_STATE_IDLE){
		bq76952.state = BQ76952_STATE_REQUESTED;
		BQ76952_SignalSet(bq76952.userSignal);
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
	if (bq76952.state == BQ76952_STATE_IDLE){
		bq76952.state = BQ76952_STATE_RELEASED;
		BQ76952_SignalSet(bq76952.userSignal);
		ret = IDN_OK;
	}
	return ret;
}

/* Execute Operations ----------------------------------------------------*/

/**
  * @brief  Helper for the callback.
  */
uint8_t BQ76952_GetLastResult(void* output)
{
	uint8_t	 length;
	switch(bq76952.operation.action)
	{
		case BQ76952_OP_MANUFACTURER:
		case BQ76952_OP_DEVICE_NUMBER:
			break;

		case BQ76952_OP_READ_TEMP:
		case BQ76952_OP_READ_CC:
		case BQ76952_OP_READ_VCELL:
			bq76952.value = bq76952.buf[1] * 256 +  bq76952.buf[0];
			output = (void*) &bq76952.value;
			length = 2;
			break;

		case BQ76952_OP_ENA_PROTECT:
			output = (void*) &bq76952.buf[0];
			length = 1;
			break;

		default:
		case BQ76952_OP_UNKNOWN:
			break;
	}
	return length;
}

/**
  * @brief  Execute an operation.
  */
idn_RetVal_t BQ76952_ExecuteOperation(bq76952_operation_t action, uint16_t value, bq76952_callback_t func)
{
	idn_RetVal_t ret = IDN_ERROR;
	if (bq76952.state == BQ76952_STATE_REQUESTED){
		bq76952.state = BQ76952_STATE_EXEC;
		bq76952.operation.action = action;
		bq76952.operation.value = value;
		bq76952.operation.callback = func;
		BQ76952_SignalSet(bq76952.userSignal);
		ret = IDN_OK;
	}
	return ret;
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
	bq76952.timerId = osTimerCreate(osTimer(none), osTimerOnce, NULL);

	if(bq76952.timerId == NULL)
	{
		BQ76952_Printf("Create master timer fail");
		BQ76952_Assert(bq76952.timerId);
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
	if (osTimerStart(bq76952.timerId, millisec) != osOK)
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
	if (osTimerStop(bq76952.timerId) != osOK)
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
	if (osTimerDelete(bq76952.timerId) != osOK)
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
	if(osSignalSet (bq76952.taskHandle, signals) != osOK)
	{
		BQ76952_ErrorPrintf("BQ76952_Signal: SetSignal KO");
	}
}
