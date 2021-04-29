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
  I2C_HandleTypeDef*    devHandle;      /*!< device handle      */
  Idn_Hal_I2c_Callbacks_t   callbacks;  /*!< callback structure */
  osTimerId             timerId;        /*!< timer ID           */
  QueueHandle_t         queue;          /*!< alarm queue        */
  osThreadId            taskHandle;     /*!< task Handle        */
  uint8_t               requested;      /*!< user notification  */
  uint8_t               buf[32];        /*!< read buffer        */
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

extern uint8_t TX_2Byte[], TX_3Byte[], TX_4Byte[], TX_6Byte[];
extern bq76952_t Bq76952;

/* Operation functions -------------------------------------------------------*/

static inline void BQ76952_GetBuffer(uint8_t* buf, uint8_t bytes_to_read) {
  buf = &Bq76952.buf[0];
}

/**
  * @brief  Initialize the global structure
  */
idn_RetVal_t BQ76952_Init(void);

/**
  * @brief  Request the module (thread-safe not implemented)
  */
idn_RetVal_t BQ76952_Request(void);

/**
  * @brief  Release the module (thread-safe not implemented)
  */
idn_RetVal_t BQ76952_Release(void);

/**
  * @brief  Write Register.
  */
idn_RetVal_t BQ76952_WriteReg(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint8_t count);

/**
  * @brief  Read Register.
  */
idn_RetVal_t BQ76952_ReadReg(uint8_t dev_addr, uint8_t reg_addr, uint8_t count);

/**
  * @brief  Compute the Checksum used in "Data R-W Access"
  */
uint8_t BQ76952_Checksum(uint8_t *ptr, uint8_t len);

/* Inline functions ****************************************************/

/**
  * @brief  BQ76952_Setter_8Bits (0xABCD, 0xFF)
  * @param  addr  16 bit address (subcommand)
  * @param  value   8 bits
**/
static inline void BQ76952_Setter_8Bits(uint16_t addr, uint8_t value)
{
  TX_3Byte[0] = LOW_BYTE(addr);
  TX_3Byte[1] = HIGH_BYTE(addr);
  TX_3Byte[2] = value;
  BQ76952_WriteReg(BQ76952_SLAVE_ADDR, 0x3E, TX_3Byte, 3);
  BQ76952_I2C_WAIT(1);
  TX_2Byte[0] = BQ76952_Checksum(TX_3Byte, 3);
  TX_2Byte[1] = 0x05; // Length
  BQ76952_WriteReg(BQ76952_SLAVE_ADDR, 0x60, TX_2Byte, 2);
  BQ76952_I2C_WAIT(1);
}

/**
  * @brief  BQ76952_Setter_2Bytes (0xABCD, 0xFFFF)
  * @param  addr  16 bit address (subcommand)
  * @param  value   16 bits
**/
static inline void BQ76952_Setter_2Bytes(uint16_t addr, uint16_t value)
{
  TX_4Byte[0] = LOW_BYTE(addr);
  TX_4Byte[1] = HIGH_BYTE(addr);
  TX_4Byte[2] = LOW_BYTE(value);
  TX_4Byte[3] = HIGH_BYTE(value);
  BQ76952_WriteReg(BQ76952_SLAVE_ADDR, 0x3E, TX_4Byte, 4);
  BQ76952_I2C_WAIT(1);
  TX_2Byte[0] = BQ76952_Checksum(TX_4Byte, 4);
  TX_2Byte[1] = 0x06;  // Length
  BQ76952_WriteReg(BQ76952_SLAVE_ADDR, 0x60, TX_2Byte, 2);
  BQ76952_I2C_WAIT(1);
}

/**
  * @brief  BQ76952_Setter_2Bytes (0xABCD, 0xFFFF)
  * @param  addr  16 bit address (subcommand)
  * @param  high  16 bits
  * @param  low   16 bits
**/
static inline void BQ76952_Setter_4Bytes(uint16_t addr, uint16_t high, uint16_t low)
{
  TX_6Byte[0] = LOW_BYTE(addr);
  TX_6Byte[1] = HIGH_BYTE(addr);
  TX_6Byte[2] = LOW_BYTE(high);
  TX_6Byte[3] = HIGH_BYTE(high);
  TX_6Byte[4] = LOW_BYTE(low);
  TX_6Byte[5] = HIGH_BYTE(low);
  BQ76952_WriteReg(BQ76952_SLAVE_ADDR, 0x3E, TX_6Byte, 6);
  BQ76952_I2C_WAIT(1);
  TX_2Byte[0] = BQ76952_Checksum(TX_6Byte, 6);
  TX_2Byte[1] = 0x08;  // Length
  BQ76952_WriteReg(BQ76952_SLAVE_ADDR, 0x60, TX_2Byte, 2);
  BQ76952_I2C_WAIT(1);
}

/**
  * @brief  BQ76952_Setter_Direct (0xAB, 0xFF)
  * @param  addr  8 bit address (direct command)
  * @param  value   16 bits
**/
static inline void BQ76952_Setter_Direct(uint8_t addr, uint16_t value)
{
  TX_2Byte[0] = LOW_BYTE(value);
  TX_2Byte[1] = HIGH_BYTE(value);
  BQ76952_WriteReg(BQ76952_SLAVE_ADDR, addr, TX_2Byte, 2);
  BQ76952_I2C_WAIT(1);
}

/**
  * @brief  BQ76952_Getter (0xABCD, 1)
  * @param  addr  16 bit address (subcommand)
  * @param  count   number of bytes to get {1,2}
**/
static inline void BQ76952_Getter(uint16_t addr, uint8_t count)
{
  TX_2Byte[0] = LOW_BYTE(addr);
  TX_2Byte[1] = HIGH_BYTE(addr);
  BQ76952_WriteReg(BQ76952_SLAVE_ADDR, 0x3E, TX_2Byte, 2);
    BQ76952_I2C_WAIT(1);
    BQ76952_ReadReg(BQ76952_SLAVE_ADDR, 0x40, count);
    BQ76952_I2C_WAIT(2);
}

#endif /* BQ76952_H_ */