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
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "IO.h"
#include "PortMap.h"

#include "bq_config2.h"
#include "bq_safety.h"
#include "bq_protection.h"

/* Defines -------------------------------------------------------------------*/

#define BQ76952_Printf(args ...)		IO_SendString(args)
#define BQ76952_Assert(args ...)
#define BQ76952_ErrorPrintf(args ...)	IO_SendString(args)
#define BQ76952_SIGNALWAIT_STATE		1

/* Global variables ----------------------------------------------------------*/

bq76952_t Bq76952 = {
		.taskHandle = NULL,
		.queue = NULL
};
static char* ResultString[] = {
		"Bq76952_Op Completed\r\n",	// IDN_OK
		"Bq76952_Op Nack\r\n",		// IDN_ERROR
		"Bq76952_Op Busy\r\n",		// IDN_BUSY
		"Bq76952_Op Timeout\r\n" };	// IDN_TIMEOUT

// Create Buffers for 2, 3, or 4 bytes of data
uint8_t TX_2Byte [2] = {0x00, 0x00};
uint8_t TX_3Byte [3] = {0x00, 0x00, 0x00};
uint8_t TX_4Byte [4] = {0x00, 0x00, 0x00, 0x00};
uint8_t TX_6Byte [6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/* Private function prototypes -----------------------------------------------*/

static void  BQ76952_Task(void const *pvParameters);

#ifdef BQ76952_ENABLE_EXTERNAL_GPIO_INTERRUPTS
	static void Bq76952_CallbackInt(bq76952_extIntName_t extInt);
#endif
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
	BQ76952_TimerStop();
//	BQ76952_SignalSet(BQ76952_SIGNALWAIT_STATE);
}

static void BQ76952_RxCallback (I2C_HandleTypeDef *hi2c) {
	Bq76952.intResult = IDN_OK;
	BQ76952_TimerStop();
//	BQ76952_SignalSet(BQ76952_SIGNALWAIT_STATE);
}

static void BQ76952_ErrorCallback (I2C_HandleTypeDef *hi2c) {
	Bq76952.intResult = IDN_ERROR;
	BQ76952_TimerStop();
//	BQ76952_SignalSet(BQ76952_SIGNALWAIT_STATE);
}

static void BQ76952_TimerCallback(void const *arg)
{
	Bq76952.intResult = IDN_TIMEOUT;
//	BQ76952_SignalSet(BQ76952_SIGNALWAIT_STATE);
}

/* Private functions ---------------------------------------------------------*/

static idn_RetVal_t __i2c_master_request(void)
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
	}
	return (idn_RetVal_t)ret;
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

	if (__i2c_master_request() == IDN_OK)
	{
		ret = IDN_HAL_I2C_Mem_Write_IT (Bq76952.devHandle, dev_addr, reg_addr, 1, reg_data, count);
		if(ret != HAL_OK) {
			BQ76952_Printf("HAL write function error!\r\n");
		}
		else {
			/* Start timeout */
			BQ76952_TimerLaunch(BQ76952_TIMEOUT_MS);
			Bq76952.intResult = IDN_BUSY;
//			BQ76952_SignalWait(BQ76952_SIGNALWAIT_STATE, osWaitForever);
			while(Bq76952.intResult == IDN_BUSY) {
				taskYIELD();
			}
			BQ76952_Printf (ResultString[Bq76952.intResult] );
		}
		IDN_HAL_I2C_Release (BQ76952_Hm_GetPeripheralAddr());
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

	if (__i2c_master_request() == (idn_RetVal_t)HAL_OK)
	{
		ret = IDN_HAL_I2C_Mem_Read_IT (Bq76952.devHandle, dev_addr, reg_addr, 1, Bq76952.buf, count);
		if(ret != HAL_OK) {
			BQ76952_Printf("HAL read function error!\r\n");
		}
		else {
			/* Start timeout */
			BQ76952_TimerLaunch(BQ76952_TIMEOUT_MS);
			Bq76952.intResult = IDN_BUSY;
//			BQ76952_SignalWait(BQ76952_SIGNALWAIT_STATE, osWaitForever);
			while(Bq76952.intResult == IDN_BUSY) {
				taskYIELD();
			}
			BQ76952_Printf (ResultString[Bq76952.intResult] );
		}
		IDN_HAL_I2C_Release (BQ76952_Hm_GetPeripheralAddr());
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
	if (Bq76952.taskHandle == NULL)
	{
		/* Initialize structure */
		Bq76952.intResult 	= IDN_ERROR;
		Bq76952.requested	= 0;
		Bq76952.devHandle 	= BQ76952_Hm_GetDeviceHandler();

	#ifdef BQ76952_ENABLE_EXTERNAL_GPIO_INTERRUPTS
		Bq76952.extCallbacks.CbAlarm = NULL;
		Bq76952.extCallbacks.CbFETs = NULL;
	#endif
		/* Create timers */
		BQ76952_TimerCreate();

		/* -- Create and launch the Task -- */
		/* Thread definition OnlyTask. */
		osThreadDef( BQ76952_Task, BQ76952_Task, osPriorityNormal, 0, (3*configMINIMAL_STACK_SIZE) );

		/* Create the task. */
		Bq76952.taskHandle = osThreadCreate( osThread(BQ76952_Task), NULL );

		/* Queue creation */
		Bq76952.queue = xQueueCreate( 1, sizeof( unsigned short ) );

		/* Configure callbacks */
		Bq76952.callbacks.AbortCpltCallback 	= NULL;
		Bq76952.callbacks.AddrCallback			= NULL;
		Bq76952.callbacks.ListenCpltCallback	= NULL;
		Bq76952.callbacks.MemTxCpltCallback		= &BQ76952_TxCallback;
		Bq76952.callbacks.MemRxCpltCallback		= &BQ76952_RxCallback;
		Bq76952.callbacks.SlaveRxCpltCallback	= NULL;
		Bq76952.callbacks.SlaveTxCpltCallback	= NULL;
		Bq76952.callbacks.MasterTxCpltCallback 	= NULL;
		Bq76952.callbacks.MasterRxCpltCallback 	= NULL;
		Bq76952.callbacks.ErrorCallback 		= &BQ76952_ErrorCallback;
	}

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
	if (Bq76952.requested == 0){
		Bq76952.requested = 1;
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
	if (Bq76952.requested == 1){
		Bq76952.requested = 0;
		ret = IDN_OK;
	}
	return ret;
}

/**
  * @brief  Compute the Checksum used in "Data R-W Access"
  */
uint8_t BQ76952_Checksum(uint8_t *ptr, uint8_t len)
{
	uint8_t i, checksum = 0;
	for(i=0; i<len; i++)
		checksum += ptr[i];
	checksum = 0xff & ~checksum;
	return(checksum);
}

#ifdef BQ76952_ENABLE_EXTERNAL_GPIO_INTERRUPTS

idn_RetVal_t BQ76952_RequestAlarm(void (*Callback)(Bq76952_AlarmState_t))
{
	idn_RetVal_t retVal = IDN_ERROR;

	if(Bq76952.requested == 1)
	{
		if(Bq76952.extCallbacks.CbAlarm == NULL)	/* Not requested */
		{
			if(BQ76952_Hm_RequestAlert(Bq76952_CallbackInt) == IDN_OK)
			{
				BQCMD2_Enable_ALERT_Pin(NULL);
				Bq76952.extCallbacks.CbAlarm = Callback;
				retVal = IDN_OK;
			}
		}
	}
	return retVal;
}

idn_RetVal_t BQ76952_ReleaseAlarm(void (*Callback)(Bq76952_AlarmState_t))
{
	idn_RetVal_t retVal = IDN_ERROR;

	if(Bq76952.extCallbacks.CbAlarm != NULL)	/* Not requested */
	{
		BQ76952_Hm_ReleaseAlert();
		BQCMD2_Disable_ALERT_Pin(NULL);

		Bq76952.extCallbacks.CbAlarm = NULL;

		retVal = IDN_OK;
	}
	return retVal;
}

idn_RetVal_t BQ76952_RequestFETState(void (*Callback)(Bq76952_FETState_t))
{
	idn_RetVal_t retVal = IDN_ERROR;

	if(Bq76952.requested == 1)
	{
		if(Bq76952.extCallbacks.CbFETs == NULL)	/* Not requested */
		{
			if(BQ76952_Hm_RequestFETState(Bq76952_CallbackInt) == IDN_OK)
			{
				if(BQ76952_Hm_RequestFETControl() == IDN_OK)
				{
					BQCMD2_Enable_FETS_Pin(NULL);
					BQCMD2_Enable_STATUS_Pin(NULL);
					Bq76952.extCallbacks.CbFETs = Callback;
					retVal = IDN_OK;
				}
			}
		}
	}
	return retVal;
}

idn_RetVal_t BQ76952_ReleaseFETState(void (*Callback)(Bq76952_FETState_t))
{
	idn_RetVal_t retVal = IDN_ERROR;

	if(Bq76952.extCallbacks.CbFETs != NULL)	/* Not requested */
	{
		BQ76952_Hm_ReleaseFETState();
		BQ76952_Hm_ReleaseFETControl();
		BQCMD2_Disable_FETS_Pin(NULL);
		BQCMD2_Disable_STATUS_Pin(NULL);

		Bq76952.extCallbacks.CbFETs = NULL;

		retVal = IDN_OK;
	}
	return retVal;
}

static void Bq76952_CallbackInt(bq76952_extIntName_t extInt)
{
	if(osMessagePut(Bq76952.queue, extInt, 0) != osOK){
		IO_SendString("Event dropped: ");
		IO_SendByte((uint8_t)extInt);
		IO_SendString("\n");
	}
}

idn_RetVal_t BQ76952_SetFETControl(Bq76952_FETControl_t control)
{
	idn_RetVal_t retVal = IDN_ERROR;

	if(Bq76952.extCallbacks.CbFETs != NULL)	/* Not requested */
	{
		retVal = BQ76952_hm_SetFETControl(control);
	}
	return retVal;
}

idn_RetVal_t BQ76952_RequestResetPin(void)
{
	return BQ76952_Hm_RequestResetPin();
}

idn_RetVal_t BQ76952_ReleaseResetPin(void)
{
	return BQ76952_Hm_ReleaseResetPin();
}

void BQ76952_PerformReset(void)
{
	/*	When the pin is	driven high, the device will immediately reset most of the digital logic,
	 * 	including that associated with the serial communications bus. However, it does not reset the
	 *  logic that holds the state of the protection FETs and FUSE, these remain as they were before
	 *  the pin was driven high. If the pin continues to be driven high for 1 second, the device will
	 *  then transition into SHUTDOWN mode, which involves disabling external protection FETs, and
	 *  powering off the internal oscillators, the REG18 LDO, the on-chip preregulator, and the REG1 and REG2 LDOs.
	*/

	BQ76952_Hm_SetPinReset();
	osDelay(200U);	// 200ms
	BQ76952_Hm_ResetPinReset();
}

void BQ76952_PerformShutdown(void)
{
	/*	When the pin is	driven high, the device will immediately reset most of the digital logic,
	 * 	including that associated with the serial communications bus. However, it does not reset the
	 *  logic that holds the state of the protection FETs and FUSE, these remain as they were before
	 *  the pin was driven high. If the pin continues to be driven high for 1 second, the device will
	 *  then transition into SHUTDOWN mode, which involves disabling external protection FETs, and
	 *  powering off the internal oscillators, the REG18 LDO, the on-chip preregulator, and the REG1 and REG2 LDOs.
	*/

	BQ76952_Hm_SetPinReset();
	osDelay(1300U);	// 1.3s
	BQ76952_Hm_ResetPinReset();
}


idn_RetVal_t BQ76952_RequestPrecensePin(void)
{
	return BQ76952_Hm_RequestPresencePin();
}

idn_RetVal_t BQ76952_ReleasePresencePin(void)
{
	return BQ76952_Hm_ReleasePresencePin();
}

idn_RetVal_t BQ76952_IsPresencePinAsserted(void)
{
	return BQ76952_Hm_IsPresencePinAsserted();
}


idn_RetVal_t BQ76952_RequestBootPin(void)
{
	return BQ76952_Hm_RequestBootPin();
}

idn_RetVal_t BQ76952_ReleaseBootPin(void)
{
	return BQ76952_Hm_ReleaseBootPin();
}

idn_RetVal_t BQ76952_PerformBoot(void)
{
	/*  Falling edge on TS2, with BAT already in valid range.
	 *  TS2 will be weakly driven with a aprox 5V level during shutdown.
	 *  If the TS2 pin is pulled low, such as by a switch to VSS the
	 *  device will exit SHUTDOWN mode
     */
	BQ76952_Hm_SetBootPin();
	osDelay(300U);	// 300ms  Min time not found in datasheet
	BQ76952_Hm_ResetBootPin();
	return IDN_OK;
}

#endif


/* Private functions -------------------------------------------------------*/

/**
  * @brief  Main task of the driver
  */
static void BQ76952_Task(void const *pvParameters)
{
	alarm_hgh_t high;
	alarm_low_t low;
	osEvent event;
	for (;;) {
		event = osMessageGet(Bq76952.queue, osWaitForever);
		if((event.status == osEventMessage) && (Bq76952.requested == 1))
		{
			switch(event.value.v){

				case BQ76952_EXT_INT_ALERT:

					BQCFG2_Get_AlarmStatus (&high, &low, NULL);
					if (low.bits.FULLSCAN) {
						Bq76952.extCallbacks.CbAlarm(ALERT_FULL_SCAN);
					}
					if (low.bits.ADSCAN) {
						Bq76952.extCallbacks.CbAlarm(ALERT_ADC_SCAN);
					}
					if (low.bits.WAKE) {
						Bq76952.extCallbacks.CbAlarm(ALERT_WAKE);
					}
					if (low.bits.CB) {
						Bq76952.extCallbacks.CbAlarm(ALERT_CBALANCING);
					}
					if (low.bits.XDSG) {
						Bq76952.extCallbacks.CbAlarm(ALERT_DFET_OFF);
					}
					if (low.bits.XCHG) {
						Bq76952.extCallbacks.CbAlarm(ALERT_CFET_OFF);
					}
					if (low.bits.SHUTV) {
						Bq76952.extCallbacks.CbAlarm(ALERT_SHUT_VOLTAGE);
					}
					/* MSK_SF MSK_PF INITSTART INITCOMP */
					if (high.bits.MSK_SFALERT) {
						BQCMD2_Alert_SA_SB_SC(Bq76952.extCallbacks.CbAlarm);
//						Bq76952.extCallbacks.CbAlarm(ALERT_MASK_SF);
					}
					if (high.bits.MSK_PFALERT) {
						Bq76952.extCallbacks.CbAlarm(ALERT_MASK_PF);
					}
					if (high.bits.INITCOMP) {
						Bq76952.extCallbacks.CbAlarm(ALERT_INIT_COMPLETE);
					}
					if (high.bits.INITSTART) {
						Bq76952.extCallbacks.CbAlarm(ALERT_INIT_START);
					}
					if (high.bits.SSA) {
						BQCMD2_Fault_SA(Bq76952.extCallbacks.CbAlarm);
					}
					if (high.bits.SSBC) {
						BQCMD2_Fault_SB_SC(Bq76952.extCallbacks.CbAlarm);
					}
					if (high.bits.PF) {
						BQCMD2_Fault_PFA_PFB_PFC_PFD(Bq76952.extCallbacks.CbAlarm);
					}
					else {
						Bq76952.extCallbacks.CbAlarm(ALERT_UNKNOWN);
					}
					break;

		//		case BQ76952_EXT_INT_DCHG:
		//				Bq76952.extCallbacks.CbFETs(ALERT_CFET_OFF);
		//			break;
		//		case BQ76952_EXT_INT_DDSH:
		//				Bq76952.extCallbacks.CbFETs(ALERT_DFET_OFF);
		//			break;

			}
		}
	}
	osThreadTerminate(NULL); // Just in case the loop is exited
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
