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

/* Exported types ---------------------------------------------------- */

idn_RetVal_t BQ_Set_AlarmEnable (uint16_t alarm_source, char*);
idn_RetVal_t BQ_Get_AlarmEnable (uint16_t* alarm_source, char*);

/* Write only (Tx) ------------------------------------------------------------*/

idn_RetVal_t BQ_Set_FETEnable (char*);
idn_RetVal_t BQ_Set_Reset (char*);
idn_RetVal_t BQ_Set_ConfigUpdateMode (uint8_t mode, char*);

/* Read only (TxRx) -----------------------------------------------------------*/

idn_RetVal_t BQ_Get_ManufacturerStatus (uint16_t* status, char*);
idn_RetVal_t BQ_Get_DeviceNumber (uint16_t* device_number, char*);
idn_RetVal_t BQ_Get_EnableProtection (uint8_t* result, char*);
idn_RetVal_t BQ_Get_VCellMode (uint16_t* mode, char*);

/* Wr Registers --------------------------------------------------------------*/

idn_RetVal_t BQ_Set_EnableProtection (uint8_t value, char*);
idn_RetVal_t BQ_Set_VCellMode (uint16_t mode, char*);

/* System Commands ---------------------------------------------------------- */

idn_RetVal_t BQ_EnableRegulators(void);
idn_RetVal_t BQ_EnableAllProtections(uint8_t a, uint8_t b, uint8_t c);
idn_RetVal_t BQ_SetTemperatures(void);
idn_RetVal_t BQ_PeriodicMeasurement (void);

#endif /* BQ_COMMAND_H_ */
