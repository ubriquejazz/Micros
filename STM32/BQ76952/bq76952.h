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

#include "bq_registers.h"
#include "bq76952_hm.h"

/* Drivers */
#include "../IDN/STM32L4xx_IDN_HAL_Driver/stm32l4xx_idn_hal_i2c.h"
#include <../Application/Drivers/IdnDefinitions.h>

/* Exported types ---------------------------------------------------- */

#ifdef BQ76952_ENABLE_EXTERNAL_GPIO_INTERRUPTS

  /*!
   * @brief Rellenar!
   */
  typedef enum Bq76952_AlarmState_e
  {
    /* low */
    ALERT_WAKE      , // Device is wakened from SLEEP mode
    ALERT_ADC_SCAN    , // Voltage ADC Scan Complete.
    ALERT_CBALANCING  , // Cell balancing is active
    ALERT_FUSE_DRIVEN , // FUSE Pin Driven.
    ALERT_SHUT_VOLTAGE  , // Stack voltage is below Power:Shutdown:Shutdown Stack Voltage
    ALERT_DFET_OFF    , // the DSG FET is off.
    ALERT_CFET_OFF    , // the CHG FET is off.
    ALERT_FULL_SCAN   , // Full Voltage Scan Complete
    /* MSK_SF MSK_PF INITSTART INITCOMP */
    ALERT_INIT_START  , // Initialization started
    ALERT_INIT_COMPLETE , // Initialization completed (completed one measurement scan)
    ALERT_MASK_PF   , // Set if a bit in PF Alert A–D() is set
    /* SSBC SSA Breakout */
    FAULT_COW,    // Cell Open Wire
    FAULT_COV,    // Cell Overvoltage 
    ALERT_COV,    // Cell Overvoltage 
    FAULT_CUV,    // Cell Undervoltage 
    ALERT_CUV,    // Cell Undervoltage 
    FAULT_OTD,    // Overtemperature in Discharge 
    ALERT_OTD,    // Overtemperature in Discharge 
    FAULT_UTD,    // Undertemperature in Discharge 
    ALERT_UTD,    // Undertemperature in Discharge
    FAULT_OTC,    // Overtemperature in Charge
    ALERT_OTC,    // Overtemperature in Charge 
    FAULT_UTC,    // Undertemperature in Charge 
    ALERT_UTC,    // Undertemperature in Charge 
    FAULT_SCD,    // Short Circuit in Discharge
    ALERT_SCD,    // Short Circuit in Discharge 
    FAULT_OCD1,   // Overcurrent in Discharge (peak)
    ALERT_OCD1,   // Overcurrent in Discharge (peak)
    FAULT_OCD2,   // Overcurrent in Discharge (continuous)
    ALERT_OCD2,   // Overcurrent in Discharge (continuous)
    FAULT_OCC,    // Overcurrent in Charge (peak)
    ALERT_OCC,    // Overcurrent in Charge (peak)
    /* PF Breakout */
    PFAIL_SOCC,   // Safety Overcurrent in Charge PF
    PFAIL_SOCD,   // Safety Overcurrent in Discharge PF
    PFAIL_TOSF,   // Top of the Stack PF
    PFAIL_SOV,    // Safety Overvoltage PF
    PFAIL_SUV,    // Safety Undervoltage PF
    PFAIL_SOT,    // Safety Overtemperature PF
    PFAIL_SOTF,   // Safety Overtemperature FET PF
    PFAIL_CFETF,  // Charge FET PF
    PFAIL_DFETF,  // Discharge FET PF
    PFAIL_VIMR,   // Voltage Imbalance Relax
    ALERT_UNKNOWN
  } Bq76952_AlarmState_t;

  /*!
   * @brief Rellenar!
   */
  typedef enum Bq76952_FETState_e
  {
    FET_STATE_ALL_FET_OFF     = (uint8_t)0,
    FET_STATE_CFET_OFF        = (uint8_t)1,
    FET_STATE_DFET_OFF        = (uint8_t)2,
    FET_STATE_CFET_ON       = (uint8_t)3,
    FET_STATE_DFET_ON       = (uint8_t)4, // Todo Chequear qué opciones hay!
    FET_STATE_ALL_FET_ON      = (uint8_t)5, // Delmir: no estoy seguros de poder decir ALL_FET_ON/OFF
  } Bq76952_FETState_t;

  typedef struct bq76952_Callbacks_Ext_s
  {
    void (*CbAlarm)(Bq76952_AlarmState_t state);
    void (*CbFETs)(Bq76952_FETState_t state);
  } bq76952_Callbacks_Ext_t;
#endif

typedef struct
{
  I2C_HandleTypeDef*    devHandle;      /*!< device handle      */
  Idn_Hal_I2c_Callbacks_t   callbacks;  /*!< callback structure */
  osTimerId             timerId;        /*!< timer ID           */
  QueueHandle_t         queue;          /*!< alarm queue        */
  osThreadId            taskHandle;     /*!< task Handle        */
  uint8_t               requested;      /*!< user notification  */
  uint8_t               buf[12];        /*!< read buffer        */
  idn_RetVal_t          intResult;      /*!< i2c notification   */

#ifdef BQ76952_ENABLE_EXTERNAL_GPIO_INTERRUPTS
  bq76952_Callbacks_Ext_t   extCallbacks;
#endif
} bq76952_t;

/* Exported constants ------------------------------------------------ */

// Direct Commands
#define BQ76952_REG_VCELL_1       0x14  // R. 16-bit voltage on cell 1
#define BQ76952_REG_TEMPERATURE   0x68  // R. most recent measured internal die temp.
#define BQ76952_REG_CC2           0x3A  // R. 16-bit CC2 current

/* Exported macros -----------------------------------------------------------*/

#define CELL_NO_TO_ADDR(cellNo)   (0x14 + ((cellNo-1)*2))
#define LOW_BYTE(data)            (uint8_t)(data & 0x00FF)
#define HIGH_BYTE(data)            (uint8_t)((data >> 8) & 0x00FF)
#define WORD2B(high, low)         ((uint16_t)high << 8) | low
#define WORD4B(a3,a2,a1,a0)       ((uint32_t)a3 << 24) | ((uint32_t)a2 << 16) | ((uint32_t)a1 << 8)  | a0
#define DATA_MEM_ADDR(x)          (uint16_t) (0x92 << 8) + (uint16_t) (x)
#define READBIT(byte, i)          ((byte >> i) & 1)
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

#ifdef BQ76952_ENABLE_EXTERNAL_GPIO_INTERRUPTS
  /* ALERT Pin */
  idn_RetVal_t BQ76952_RequestAlarm(void (*Callback)(Bq76952_AlarmState_t alarm));
  idn_RetVal_t BQ76952_ReleaseAlarm(void (*Callback)(Bq76952_AlarmState_t alarm));

  /* DDSG, DCHG, DFETOFF, CFETOFF Pins */
  idn_RetVal_t BQ76952_RequestFETState(void (*Callback)(Bq76952_FETState_t state));
  idn_RetVal_t BQ76952_ReleaseFETState(void (*Callback)(Bq76952_FETState_t state));
  idn_RetVal_t BQ76952_SetFETControl(Bq76952_FETControl_t control);

  /* RESET Pin */
  idn_RetVal_t BQ76952_RequestResetPin(void);
  idn_RetVal_t BQ76952_ReleaseResetPin(void);

  // Funcions shoud be called from a Thread
  void BQ76952_PerformReset(void);
  void BQ76952_PerformShutdown(void);

  /* Reset Pin */
  idn_RetVal_t BQ76952_RequestPrecensePin(void);
  idn_RetVal_t BQ76952_ReleasePresencePin(void);
  idn_RetVal_t BQ76952_IsPresencePinAsserted(void);

  /* Boot TS2 Pin */
  idn_RetVal_t BQ76952_RequestBootPin(void);
  idn_RetVal_t BQ76952_ReleaseBootPin(void);
  idn_RetVal_t BQ76952_PerformBoot(void);
#endif

#endif /* BQ76952_H_ */


