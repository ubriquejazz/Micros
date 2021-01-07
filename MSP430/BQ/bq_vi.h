/**
  ******************************************************************************
  * @file    bqcommand.h
  * @author  
  * @version V1.0.0
  * @date    30 October 2020
  * @brief   Source file of bq76952 driver.
  ******************************************************************************
  */

#ifndef BQ_VI_H_
#define BQ_VI_H_

#include "i2c.h"

/* Exported types ---------------------------------------------------- */

typedef enum {
	OCD1,
	OCD2,
	OCD3,
	NumOfProtectionLevels	
} oc_level_t;

typedef enum {
  SCD_10,
  SCD_20,
  SCD_40,
  SCD_60,
  SCD_80,
  SCD_100,
  SCD_125,
  SCD_150,
  SCD_175,
  SCD_200,
  SCD_250,
  SCD_300,
  SCD_350,
  SCD_400,
  SCD_450,
  SCD_500
} scd_thresh_t;

/* Defines ----------------------------------------------------------- */

idn_RetVal_t BQ_Set_CellOverVoltage (float, uint16_t ms, char*);      // SW2
idn_RetVal_t BQ_Set_CellUnderVoltage (float, uint16_t ms, char*);     // SW2
idn_RetVal_t BQ_Set_CellOpenWireCheck (uint8_t sec, char*);           // SW2.3.3

idn_RetVal_t BQ_Set_ChargingOverCurrent (float, uint16_t ms, char*);              //
idn_RetVal_t BQ_Set_DischargingOverCurrent (oc_level_t, float, uint16_t ms, char*);           //
idn_RetVal_t BQ_Set_DischargingShortCircuit (scd_thresh_t, uint16_t us, char*);   //


#endif /* BQ_VI_H_ */
