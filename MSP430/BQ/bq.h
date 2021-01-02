/**
  ******************************************************************************
  * @file    bqcommand.h
  * @author  
  * @version V1.0.0
  * @date    30 October 2020
  * @brief   Source file of bq76952 driver.
  ******************************************************************************
  */

#ifndef BQ_COMMAND_H_
#define BQ_COMMAND_H_

#include "i2c.h"

/* Defines ----------------------------------------------------------- */

// Direct Commands
#define BQ76952_REG_VCELL_1			0x14	// R. 16-bit voltage on cell 1
#define BQ76952_REG_TEMPERATURE		0x68	// R. most recent measured internal die temp.
#define BQ76952_REG_CC2				0x3A	// R. 16-bit current (Filter 2)
#define BQ76952_REG_FET_STAT        0x7F
// Subcommands (0x3E, 0x40)
#define BQ76952_REG_FET_ENABLE		0x22	// W. FET_EN = 0 means Test Mode. FET_EN = 1 means FW Control
#define BQ76952_REG_RESET 		  	0x12	// W. Reset the device
#define BQ76952_REG_SET_CFGUPDATE	0x90	// W. Enters CONFIG_UPDATE mode
#define BQ76952_REG_EXIT_CFGUPDATE	0x92	// W. Also clears the Battery Status() [POR] and Battery Status()[WD] bits.

// Bits in BQ76952 registers
#define BIT_PA_SC_DCHG            	7
#define BIT_PA_OC2_DCHG           	6
#define BIT_PA_OC1_DCHG           	5
#define BIT_PA_OC_CHG             	4
#define BIT_PA_CELL_OV            	3
#define BIT_PA_CELL_UV            	2
#define BIT_PB_OTF                	7
#define BIT_PB_OTINT              	6
#define BIT_PB_OTD                	5
#define BIT_PB_OTC                	4
#define BIT_PB_UTINT              	2
#define BIT_PB_UTD                	1
#define BIT_PB_UTC                	0

/* Macros ---------------------------------------------------- */

#define CELL_NO_TO_ADDR(cellNo) 	(0x14 + ((cellNo-1)*2))
#define LOW_BYTE(data) 				(uint8_t)(data & 0x00FF)
#define HIGH_BYTE(data) 			(uint8_t)((data >> 8) & 0x00FF)
#define DATA_MEM_ADDR(x, y)			(uint16_t) (0x92 << 8) + (uint16_t) (x + y)		

/* Exported types ---------------------------------------------------- */

typedef enum {
    PROTECTION_A,		// SCD, OCD2, OCD1, OCC, COV, CUV
    PROTECTION_B,		// OTF, OTINT, OTD, OTC, UTINT, UTD, UTC
    PROTECTION_C,		// OCD3, SCDL, OCDL, COVL, PTO, HWDF
    NumOfProtections
} protection_t;

typedef enum {
	REG12,				// 7:4 REG2 ; 3:0 REG1
	REG0,				// REG0_EN only (bit 0)
	NumOfRegulators
} regulator_t;

typedef enum {
	CFETOFF,
	DFETOFF,
	ALERT,				// 
	NumOfOuputPins
} output_pin_t;

typedef enum {
	TS1,				//
	TS2,
	TS3,				//
	NumOfThermistors
} thermistor_t;

/* Direct Commands ----------------------------------------------------------- */

idn_RetVal_t BQ_Set_AlarmStatus (uint16_t alarm_source, char*);
idn_RetVal_t BQ_Set_RawAlarmStatus (uint16_t alarm_source, char*);
idn_RetVal_t BQ_Set_AlarmEnable (uint16_t alarm_source, char*);

/* Write only (Tx) ------------------------------------------------------------*/

idn_RetVal_t BQ_Set_FETEnable (char*);
idn_RetVal_t BQ_Set_Reset (char*);
idn_RetVal_t BQ_Set_ConfigUpdateMode (uint8_t mode, char*);

/* Read only (TxRx) -----------------------------------------------------------*/

idn_RetVal_t BQ_Get_ManufacturerStatus (uint16_t* status, char*);
idn_RetVal_t BQ_Get_DeviceNumber (uint16_t* device_number, char*);

/* Setter 8 Bits --------------------------------------------------------------*/

idn_RetVal_t BQ_Get_EnableRegulator (regulator_t, uint8_t* result, char*);
idn_RetVal_t BQ_Set_EnableRegulator (regulator_t, uint8_t value, char*);

idn_RetVal_t BQ_Get_EnableProtection (protection_t, uint8_t* result, char*);
idn_RetVal_t BQ_Set_EnableProtection (protection_t, uint8_t value, char*);

idn_RetVal_t BQ_Get_ThermistorConfig (thermistor_t, uint8_t* result, char*);
idn_RetVal_t BQ_Set_ThermistorConfig (thermistor_t, uint8_t value, char*);

idn_RetVal_t BQ_Get_OutputPinConifg (output_pin_t, uint8_t* result, char*); 
idn_RetVal_t BQ_Set_OutputPinConifg (output_pin_t, uint8_t value, char*); 

/* Setter 2 Bytes ------------------------------------------------------------ */

idn_RetVal_t BQ_Get_VCellMode (uint16_t* mode, char*);
idn_RetVal_t BQ_Set_VCellMode (uint16_t mode, char*);

idn_RetVal_t BQ_Get_AlarmMask (uint16_t* mask, char*);
idn_RetVal_t BQ_Set_AlarmMask (uint16_t mask, char*);

/* System Commands ---------------------------------------------------------- */

idn_RetVal_t BQ_PeriodicMeasurement (char*);

#endif /* BQ_COMMAND_H_ */
