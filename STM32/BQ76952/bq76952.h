/**
  ******************************************************************************
  * @file    bq76952.h
  * @author  IDJGB0
  * @version V1.0.0
  * @date    13 abr. 2021
  * @brief   Header file of bq76952.c
  *          This file provides firmware functions to manage the following
  *          functionalities of the Batt. Monitor:
  *           + Initialization functions
  *           + Request and Release
  *
  @verbatim
  ==============================================================================
                        ##### How to use this module #####
  ==============================================================================
  [..]

  The Batt. Monitor manager can be used as follows:

  (#) Initialize the module by calling the BQ76952_Init().

  (#) In order to operate, a BQ76952_Request() must be satisfactory.

  (#) After operation the function BQ76952_Release() must be called.

  @endverbatim
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */

#ifndef BQ76952_H_
#define BQ76952_H_

/* Exported types ---------------------------------------------------- */

/*!
 * @brief Rellenar!
 */
typedef enum Bq76952_AlarmState_e
{
  /* low */
  ALERT_WAKE          , // Device is wakened from SLEEP mode
  ALERT_ADC_SCAN      , // Voltage ADC Scan Complete.
  ALERT_CBALANCING    , // Cell balancing is active
  ALERT_FUSE_DRIVEN   , // FUSE Pin Driven.
  ALERT_SHUT_VOLTAGE  , // Stack voltage is below Power:Shutdown:Shutdown Stack Voltage
  ALERT_DFET_OFF      , // the DSG FET is off.
  ALERT_CFET_OFF      , // the CHG FET is off.
  ALERT_FULL_SCAN     , // Full Voltage Scan Complete
  ALERT_INIT_START    , // Initialization started
  ALERT_INIT_COMPLETE , // Initialization completed (completed one measurement scan)
  ALERT_MASK_SF       , // Set if a bit in SF Alert A–C() is set
  ALERT_MASK_PF       , // Set if a bit in PF Alert A–D() is set
  ALERT_UNKNOWN
} Bq76952_AlarmState_t;

/*!
 * @brief Rellenar!
 */
typedef enum Bq76952_FETState_e
{
  FET_STATE_ALL_FET_OFF   = (uint8_t)0,
  FET_STATE_CFET_OFF      = (uint8_t)1,
  FET_STATE_DFET_OFF      = (uint8_t)2,
  FET_STATE_CFET_ON       = (uint8_t)3,
  FET_STATE_DFET_ON       = (uint8_t)4, // Todo Chequear qué opciones hay!
  FET_STATE_ALL_FET_ON    = (uint8_t)5, // Delmir: no estoy seguros de poder decir ALL_FET_ON/OFF
} Bq76952_FETState_t;

/*!
 * @brief Rellenar!
 */
typedef struct bq76952_Callbacks_Ext_s
{
  void (*CbAlarm)(Bq76952_AlarmState_t state);
  void (*CbFETs)(Bq76952_FETState_t state);
} bq76952_Callbacks_Ext_t;

/*!
 * @brief Rellenar!
 */
typedef struct
{
  uint8_t               buf[32];
  uint8_t               crc[2];
  SemaphoreHandle_t     mutex;
} bq76952_buf_t;

/*!
 * @brief Rellenar!
 */
typedef struct
{
  I2C_HandleTypeDef*    devHandle;      /*!< device handle      */
  Idn_Hal_I2c_Callbacks_t   callbacks;  /*!< callback structure */
  osTimerId             timerId;        /*!< timer ID           */
  QueueHandle_t         queue;          /*!< alarm queue        */
  osThreadId            taskHandle;     /*!< task Handle        */
  uint8_t               requested;      /*!< user notification  */
  bq76952_buf_t         rd, wr;
  idn_RetVal_t          intResult;      /*!< i2c notification   */

  bq76952_Callbacks_Ext_t   extCallbacks;
} bq76952_t;

/* Exported constants ------------------------------------------------ */

// Direct Commands
#define BQ76952_REG_VCELL_1       0x14  // R. 16-bit voltage on cell 1
#define BQ76952_REG_TEMPERATURE   0x68  // R. most recent measured internal die temp.
#define BQ76952_REG_CC2           0x3A  // R. 16-bit CC2 current

/* Exported macros -----------------------------------------------------------*/

#define CELL_NO_TO_ADDR(cellNo)   (0x14 + ((cellNo-1)*2))
#define BQ76952_I2C_WAIT(x)       __NOP()

/* Exported functions --------------------------------------------------------*/

extern bq76952_t Bq76952;

/* Operation functions -------------------------------------------------------*/

/**
  * @brief  Initialize the global structure
  */
idn_RetVal_t BQ76952_Init(void)
{

  /* Create the semaphore to guard a shared resource.  As we are using
    the semaphore for mutual exclusion we create a mutex semaphore
    rather than a binary semaphore. */
    Bq76952.wr.mutex  = xSemaphoreCreateMutex();
    Bq76952.rd.mutex  = xSemaphoreCreateMutex();
}

/**
  * @brief  Request the module (thread-safe not implemented)
  */
idn_RetVal_t BQ76952_Request(void);

/**
  * @brief  Release the module (thread-safe not implemented)
  */
idn_RetVal_t BQ76952_Release(void);

/* Subcommand and direct-commands for CasandrAPP ----------------------------*/


static inline idn_RetVal_t BQ76952_Get_DirectCommand(uint8_t command, uint8_t bytes_to_read, void *variable)
{
  idn_RetVal_t ret = IDN_OK;

  ret = TICOMM_ReadReg(BQ76952_SLAVE_ADDR, command, bytes_to_read);
  if(bytes_to_read == 2) {
    *(uint16_t*)variable = WORD2B (Bq76952.buf[1], Bq76952.buf[0]);
    // FS_WriteR(file_id, *(uint16_t*)variable, 0);
  }
  else if (bytes_to_read == 1) {
    *(uint8_t*)variable = Bq76952.buf[0];
    // FS_WriteR(file_id, *(uint8_t*)variable, 0);
  }
  else {
    ret = IDN_ERROR;
  }
  return ret;
}

static inline idn_RetVal_t BQ76952_Get_SubCommand(uint16_t command, uint8_t bytes_to_read, void *variable)
{
  idn_RetVal_t ret = IDN_OK;

  if(bytes_to_read == 4) {
    ret = BQ76952_Getter(command, 4);
    *(uint32_t*)variable = WORD4B (Bq76952.buf[3], Bq76952.buf[2], Bq76952.buf[1], Bq76952.buf[0]);
  }
  if(bytes_to_read == 2) {
    ret = BQ76952_Getter(command, 2);
    *(uint16_t*)variable = WORD2B (Bq76952.buf[1], Bq76952.buf[0]);
  }
  else if (bytes_to_read == 1) {
    ret = BQ76952_Getter(command, 1);
    *(uint8_t*)variable = Bq76952.buf[0];
  }
  else {
    ret = IDN_ERROR;
  }
  return ret;
}

#endif /* BQ76952_H_ */