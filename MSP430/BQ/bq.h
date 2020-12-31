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

// Bits in BQ76952 registers
#define BIT_PA_SC_DCHG            7
#define BIT_PA_OC2_DCHG           6
#define BIT_PA_OC1_DCHG           5
#define BIT_PA_OC_CHG             4
#define BIT_PA_CELL_OV            3
#define BIT_PA_CELL_UV            2
#define BIT_PB_OTF                7
#define BIT_PB_OTINT              6
#define BIT_PB_OTD                5
#define BIT_PB_OTC                4
#define BIT_PB_UTINT              2
#define BIT_PB_UTD                1
#define BIT_PB_UTC                0

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
} protection_t;

/* Atomic Commands ----------------------------------------------------------- */

idn_RetVal_t BQ_Set_AlarmEnable (uint16_t alarm_source, char*);
idn_RetVal_t BQ_Get_AlarmEnable (uint16_t* alarm_source, char*);

/* Write only (Tx) ------------------------------------------------------------*/

idn_RetVal_t BQ_Set_FETEnable (char*);
idn_RetVal_t BQ_Set_Reset (char*);
idn_RetVal_t BQ_Set_ConfigUpdateMode (uint8_t mode, char*);

/* Read only (TxRx) -----------------------------------------------------------*/

idn_RetVal_t BQ_Get_ManufacturerStatus (uint16_t* status, char*);
idn_RetVal_t BQ_Get_DeviceNumber (uint16_t* device_number, char*);
idn_RetVal_t BQ_Get_EnableProtection (protection_t, uint8_t* result, char*);
idn_RetVal_t BQ_Get_VCellMode (uint16_t* mode, char*);

/* Wr Registers --------------------------------------------------------------*/

idn_RetVal_t BQ_Set_EnableRegulator (regulator_t, uint8_t value, char*);
idn_RetVal_t BQ_Set_EnableProtection (protection_t, uint8_t value, char*);
idn_RetVal_t BQ_Set_VCellMode (uint16_t mode, char*);

/* System Commands ---------------------------------------------------------- */

idn_RetVal_t BQ_SetTemperatures(void);
idn_RetVal_t BQ_PeriodicMeasurement (void);

#endif /* BQ_COMMAND_H_ */
