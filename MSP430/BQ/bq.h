/*
 * bq76952_hm.h
 *
 *  Created on: 26 oct. 2020
 *      Author: IDJGB0
 */

#ifndef BQ76952_H_
#define BQ76952_H_

#include <stdio.h>
#include "cmsis_os.h"
#include "string.h"
#include "IdnDefinitions.h"
/* Drivers */
#include "../IDN/STM32L4xx_IDN_HAL_Driver/stm32l4xx_idn_hal_i2c.h"

/** @addtogroup IDNEO
  * @{
  */

/** @addtogroup BQ76952 Peripheral
  * @{
  */

/* Exported types ---------------------------------------------------- */

typedef enum {
  BQ76952_OP_VCELL_MODE,      /*!< R/W. VCell Mode              */
  BQ76952_OP_ENA_PROTECT,       /*!< R/W. Enabled Protections A     */
  BQ76952_OP_RESET,       /*!< W. Reset the device            */
  BQ76952_OP_FET_ENABLE,      /*!< W. Test Mode or FW Control   */
  BQ76952_OP_MANUFACTURER,      /*!< R. Flags for use during manufacturing  */
  BQ76952_OP_DEVICE_NUMBER,     /*!< R. Identifies the product (7695x)    */
  BQ76952_OP_ALARM_ENABLE,      /*!< R/W. By default is set to 0xF800     */
  BQ76952_OP_READ_VCELL,        /*!< R. 16-bit voltage on cell        */
  BQ76952_OP_READ_CC,           /*!< R. 16-bit CC2 current          */
  BQ76952_OP_READ_TEMP,         /*!< R. most recent measured internal temp. */
  BQ76952_OP_UNKNOWN,
  BQ76952_OP_NumOfOperations
} bq76952_operation_t;

typedef void (*bq76952_callback_t)(void);

typedef struct
{
  bq76952_operation_t     action;     /*!< operation to execute */
  uint16_t          value;      /*!< input for operation  */
  bq76952_callback_t      callback;   /*!< to print the result  */
} bq76952_op_t;

typedef enum
{
  BQ76952_STATE_INIT      = 0x00U,  /*!< Init   */
  BQ76952_STATE_ERROR     = 0x01U,  /*!< Error    */
  BQ76952_STATE_IDLE      = 0x02U,  /*!< Wait   */
  BQ76952_STATE_REQUESTED   = 0x03U,  /*!< Request  */
  BQ76952_STATE_RELEASED    = 0x04U,  /*!< Release  */
  BQ76952_STATE_EXEC      = 0x05U,  /*!< Operation  */
} bq76952_state_t;

typedef struct
{
  I2C_HandleTypeDef*      devHandle;      /*!< device handle    */
  Idn_Hal_I2c_Callbacks_t   callbacks;      /*!< callback structure */
  osTimerId         timerId;      /*!< timer ID       */
  bq76952_state_t       state;        /*!< internal state   */
  bq76952_op_t        operation;      /*!< action, value, cb  */
  osThreadId          taskHandle;     /*!< task Handle    */
  int32_t           intSignal;      /*!< internal i2c notification */
  int32_t           userSignal;     /*!< user notification  */
  uint8_t           buf[12];      /*!< read buffer    */

  uint16_t          value;
  volatile idn_RetVal_t   intResult;      /*!< internal i2c result*/
} bq76952_t;

/* Exported constants ------------------------------------------------ */

#define BQ76952_SLAVE_ADDR        0x08    // 0x10 including R/W bit or 0x8 as 7-bit address
#define BQ76952_TIMEOUT_MS        100   // Software timer (FreeRTOS)

// Direct Commands
#define BQ76952_REG_VCELL_1     0x14  // R. 16-bit voltage on cell 1
#define BQ76952_REG_TEMPERATURE   0x68  // R. most recent measured internal die temp.
#define BQ76952_REG_CC2       0x3A  // R. 16-bit CC2 current

/* Exported macros -----------------------------------------------------------*/

#define CELL_NO_TO_ADDR(cellNo)   (0x14 + ((cellNo-1)*2))
#define LOW_BYTE(data)              (uint8_t)(data & 0x00FF)
#define HIGH_BYTE(data)             (uint8_t)((data >> 8) & 0x00FF)
#define DATA_MEM_ADDR(x, y)         (uint16_t) (0x92 << 8) + (uint16_t) (x + y)
#define READBIT(byte, i)            ((byte >> i) & 1)
#define I2C_WAIT(x)         while (Bq76952.intResult == IDN_BUSY)

/* Exported functions --------------------------------------------------------*/

extern uint8_t TX_2Byte[], TX_3Byte[], TX_4Byte[];
extern bq76952_t Bq76952;

/* Operation functions ****************************************************/

/** @addtogroup Operation functions
  * @{
  */

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
  * @brief  Execute an operation.
  */
idn_RetVal_t BQ76952_ExecuteOperation(bq76952_operation_t action, uint16_t value, bq76952_callback_t callback);

/**
  * @brief  Helper for the callback.
  */
uint8_t BQ76952_GetLastResult(void* output);

/* Aux functions *******************************************************/

uint8_t Checksum(uint8_t *ptr, uint8_t len);

idn_RetVal_t MillisecondsU2 (uint16_t ms, uint16_t* result);

idn_RetVal_t MillisecondsU1 (uint16_t ms, uint8_t offset, uint8_t* result);

/* Inline functions ****************************************************/

/**
  * @brief  Setter_8Bits (0xABCD, 0xFF)
  * @param  addr  16 bit address (subcommand)
  * @param  value   8 bits
**/
static inline void Setter_8Bits(uint16_t addr, uint8_t value)
{
  TX_3Byte[0] = LOW_BYTE(addr);
  TX_3Byte[1] = HIGH_BYTE(addr);
  TX_3Byte[2] = value;
  BQ76952_WriteReg(0x08, 0x3E, TX_3Byte, 3);
  I2C_WAIT(1);
  TX_2Byte[0] = Checksum(TX_3Byte, 3);
  TX_2Byte[1] = 0x05; // Length
  BQ76952_WriteReg(0x08, 0x60, TX_2Byte, 2);
  I2C_WAIT(1);
}

/**
  * @brief  Setter_2Bytes (0xABCD, 0xFFFF)
  * @param  addr  16 bit address (subcommand)
  * @param  value   16 bits
**/
static inline void Setter_2Bytes(uint16_t addr, uint16_t value)
{
  TX_4Byte[0] = LOW_BYTE(addr);
  TX_4Byte[1] = HIGH_BYTE(addr);
  TX_4Byte[2] = HIGH_BYTE(value);
  TX_4Byte[3] = LOW_BYTE(value);
    BQ76952_WriteReg(0x08, 0x3E, TX_4Byte, 4);
    I2C_WAIT(1);
  TX_2Byte[0] = Checksum(TX_4Byte, 4);
  TX_2Byte[1] = 0x06;  // Length
    BQ76952_WriteReg(0x08, 0x60, TX_2Byte, 2);
    I2C_WAIT(1);
}

/**
  * @brief  Setter_Direct (0xAB, 0xFF)
  * @param  addr  8 bit address (direct command)
  * @param  value   16 bits
**/
static inline void Setter_Direct(uint8_t addr, uint16_t value)
{
  TX_2Byte[0] = LOW_BYTE(value);
  TX_2Byte[1] = HIGH_BYTE(value);
  BQ76952_WriteReg(0x08, addr, TX_2Byte, 2);
  I2C_WAIT(1);
}

/**
  * @brief  Getter (0xABCD, 1)
  * @param  addr  16 bit address (subcommand)
  * @param  count   number of bytes to get {1,2}
**/
static inline void Getter(uint16_t addr, uint8_t count)
{
  TX_2Byte[0] = LOW_BYTE(addr);
  TX_2Byte[1] = HIGH_BYTE(addr);
  BQ76952_WriteReg(0x08, 0x3E, TX_2Byte, 2);
    I2C_WAIT(1);
    BQ76952_ReadReg(0x08, 0x40, count);
    I2C_WAIT(2);
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#endif /* BQ76952_H_ */
