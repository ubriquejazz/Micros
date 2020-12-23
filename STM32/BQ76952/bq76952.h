/*
 * bq76952_hm.h
 *
 *  Created on: 26 oct. 2020
 *      Author: IDJGB0
 *
 *
  @verbatim
  ==============================================================================
                        ##### How to use this driver #####
  ==============================================================================
	[..]
	The BQ manager can be used as follows:

	(#) Initialize the module by calling the d().

	(#) In order to operate, a Bq_Request() must be satisfactory.

	(#) BQ76952_ExecuteOperation() with the following options

			- BQ76952_OP_READ_VCELL, BQ76952_OP_READ_CC, BQ76952_OP_READ_TEMP
			- BQ76952_OP_MANUFACTURER, BQ76952_OP_FET_ENABLE, BQ76952_OP_FET_RESET
			- BQ76952_OP_VCELL_MODE

  @endverbatim
  ******************************************************************************
  *
  ******************************************************************************
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
	BQ76952_OP_WR_VCELL_MODE,		/*!< R/W. VCell Mode 	           			*/
	BQ76952_OP_RD_VCELL_MODE,
	BQ76952_OP_WR_ENA_PROTECT,	   	/*!< R/W. Enabled Protections A   			*/
	BQ76952_OP_RD_ENA_PROTECT,	
	BQ76952_OP_WR_ALARM_ENABLE,    	/*!< R/W. By default is set to 0xF800   	*/
	BQ76952_OP_RD_ALARM_ENABLE,
	/* Write only */	
	BQ76952_OP_WR_RESET,			/*!< W. Reset the device          			*/
	BQ76952_OP_WR_FET_ENABLE,		/*!< W. Test Mode or FW Control 			*/
	/* Direct () */
	BQ76952_OP_RD_VCELL,     		/*!< R. 16-bit voltage on cell				*/
	BQ76952_OP_RD_CC,        		/*!< R. 16-bit CC2 current					*/
	BQ76952_OP_RD_TEMP,      		/*!< R. most recent measured internal temp. */
	/* Read only */
	BQ76952_OP_RD_MANUFACTURER,   	/*!< R. Flags for use during manufacturing 	*/
	BQ76952_OP_RD_DEVICE_NUMBER,  	/*!< R. Identifies the product (7695x) 		*/
	BQ76952_OP_UNKNOWN,
	BQ76952_OP_NumOfOperations
} bq76952_operation_t;

typedef void (*bq76952_callback_t)(void);

typedef struct
{
	bq76952_operation_t 		action;			/*!< operation to execute	*/
	uint16_t					value;			/*!< input for operation	*/
	bq76952_callback_t  		callback;		/*!< to print the result 	*/
} bq76952_op_t;

/* Exported types ---------------------------------------------------- */

typedef union protection {
	struct {
		uint8_t SC_DCHG            :1;		/*!< R/W.  Short Circuit Discharge */
		uint8_t OC2_DCHG           :1;		/*!< R/W.  Mode */
		uint8_t OC1_DCHG           :1;		/*!< R/W.  Mode */
		uint8_t OC_CHG             :1;		/*!< R/W.  Mode */
		uint8_t CELL_OV            :1;		/*!< R/W.  Mode */
		uint8_t CELL_UV            :1;		/*!< R/W.  Mode */
	} bits;
} bq76952_protection_t;

typedef union temperatureProtection {
	struct {
		uint8_t OVERTEMP_FET		:1;		/*!< R/W.  Mode */
		uint8_t OVERTEMP_INTERNAL	:1;		/*!< R/W.  Mode */
		uint8_t OVERTEMP_DCHG		:1;		/*!< R/W.  Mode */
		uint8_t OVERTEMP_CHG		:1;		/*!< R/W.  Mode */
		uint8_t UNDERTEMP_INTERNAL	:1;		/*!< R/W.  Mode */
		uint8_t UNDERTEMP_DCHG		:1;		/*!< R/W.  Mode */
		uint8_t UNDERTEMP_CHG		:1;		/*!< R/W.  Mode */
	} bits;
} bq76952_temperature_t;

/* Exported constants ------------------------------------------------ */

#define BQ76952_I2C_ADDR_WRITE  	0x10	// 0x10 including R/W bit or 0x8 as 7-bit address
#define BQ76952_I2C_ADDR_READ    	0x11	//
#define BQ76952_BAUD_KHZ			400		//
#define BQ76952_TIMEOUT_MS    		100		// Software timer (FreeRTOS)
#define CRC_Mode 					0  		// 0 for disabled, 1 for enabled

// Direct Commands
#define	BQ76952_REG_ALARM_ENABLE	0x66	// R/W. By default is set to 0xF800
#define BQ76952_REG_VCELL_1			0x14	// R. 16-bit voltage on cell 1
#define BQ76952_REG_TEMPERATURE		0x68	// R. most recent measured internal die temp.
#define BQ76952_REG_CC2				0x3A	// R. 16-bit CC2 current


#define BQ76952_REG_COMMAND			0x3E 	// W. Subcommand
#define BQ76952_REG_RESPONSE		0x04 	// R. Subcommand
#define BQ76952_REG_CHECKSUM		0x60 	// W. RAM Registers

// Subcommands (0x3E, 0x40)
#define BQ76952_REG_DEVICE_NUMBER 	0x01	// R. Identifies the product (7695x)
#define BQ76952_REG_MANUFACTURER	0x57	// R. Provides flags for use during manufacturing
#define BQ76952_REG_FET_ENABLE		0x22	// W. FET_EN = 0 means Test Mode. FET_EN = 1 means FW Control
#define BQ76952_REG_RESET 		  	0x12	// W. Reset the device
#define BQ76952_REG_SET_CFGUPDATE	0x90	// W. Enters CONFIG_UPDATE mode
#define BQ76952_REG_EXIT_CFGUPDATE	0x92	// W. Also clears the Battery Status() [POR] and Battery Status()[WD] bits.
#define BQ76952_REG_ENA_PROTECT_L	0x61
#define BQ76952_REG_ENA_PROTECT_H	0x92	// R/W. Enabled Protections A register

// RAM Registers (0x3E, 0x40, 0x60)
// - It is recommended to first enter CONFIG_UPDATE mode
// - Write the address to 0x3E followed by the data
// - Write the checksum of the address and data
#define BQ76952_REG_VCELL_MODE_L	0x04
#define BQ76952_REG_VCELL_MODE_H	0x93	// R/W. VCell Mode registers

// Alert Bits in BQ76952 registers
#define BIT_SA_SC_DCHG            7
#define BIT_SA_OC2_DCHG           6
#define BIT_SA_OC1_DCHG           5
#define BIT_SA_OC_CHG             4
#define BIT_SA_CELL_OV            3
#define BIT_SA_CELL_UV            2

#define BIT_SB_OTF                7
#define BIT_SB_OTINT              6
#define BIT_SB_OTD                5
#define BIT_SB_OTC                4
#define BIT_SB_UTINT              2
#define BIT_SB_UTD                1
#define BIT_SB_UTC                0

/* Exported macros -----------------------------------------------------------*/

#define CELL_NO_TO_ADDR(cellNo) (0x14 + ((cellNo-1)*2))
#define LOW_BYTE(data) 			(byte)(data & 0x00FF)
#define HIGH_BYTE(data) 		(byte)((data >> 8) & 0x00FF)

/* Exported functions --------------------------------------------------------*/

/* Operation functions ****************************************************/

/** @addtogroup Operation functions
  * @{
  */

/**
  * @brief	Initialize the global structure
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

/* Command Level --------------------------------------------------------*/

idn_RetVal_t BQ76952_DirectCommand(uint8_t command, uint16_t* data);

idn_RetVal_t BQ76952_BasicCommand(uint8_t reg_addr, uint16_t data);

idn_RetVal_t BQ76952_SubCommandTransmit(uint16_t data);

idn_RetVal_t BQ76952_SubCommandResponse(uint8_t* data, uint8_t count);

/* Register Level --------------------------------------------------------*/

idn_RetVal_t BQ76952_ReadReg(uint8_t reg_addr, uint8_t count);

idn_RetVal_t BQ76952_WriteReg(uint8_t reg_addr, uint8_t *reg_data, uint8_t count);

/* Execute Operations ----------------------------------------------------*/

/**
  * @brief  Helper for the callback.
  */
uint8_t BQ76952_GetLastResult(void* output);

/**
  * @brief  Execute an operation.
  */
idn_RetVal_t BQ76952_ExecuteOperation(bq76952_operation_t action, uint16_t value, bq76952_callback_t callback);


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
