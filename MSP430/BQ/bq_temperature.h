/*
 * bq_protection.h
 *
 *  Created on: 13 ene. 2021
 *      Author: IDJGB0
 */

#ifndef BQ_PROTECTION_H_
#define BQ_PROTECTION_H_

#include <stdio.h>
#include "cmsis_os.h"
#include "string.h"
#include "bq76952.h"
#include "IdnDefinitions.h"

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

typedef enum {
	OCD1,
	OCD2,
	OCD3,
	NumOfProtectionLevels
} oc_level_t;

/* OverVoltage Functions --------------------------------------------------------------- */

idn_RetVal_t BQ_Set_CellOverVoltage (float, uint16_t ms, char*);      // SW2
idn_RetVal_t BQ_Set_CellUnderVoltage (float, uint16_t ms, char*);     // SW2
idn_RetVal_t BQ_Set_CellOpenWireCheck (uint8_t sec, char*);           // SW2.3.3

/* OverTemperature Functions ----------------------------------------------------------- */

idn_RetVal_t BQ_Set_ChargingOverTemperature (int16_t, uint8_t sec, char*);        // SW3.3
idn_RetVal_t BQ_Set_DischargingOverTemperature (int16_t, uint8_t sec, char*);     // SW3.4
idn_RetVal_t BQ_Set_CellOverTemperature (int8_t, char*);        	  			  // SW4.3
idn_RetVal_t BQ_Set_InternalOverTemperature (int16_t, uint8_t sec, char*);        // SW3.7

idn_RetVal_t BQ_Set_ChargingUnderTemperature (int16_t, uint8_t sec, char*);       // SW3.5
idn_RetVal_t BQ_Set_DischargingUnderTemperature (int16_t, uint8_t sec, char*);    // SW3.6
idn_RetVal_t BQ_Set_CellUnderTemperature (int8_t, char*);       	  			  // SW4.3
idn_RetVal_t BQ_Set_InternalUnderTemperature (int16_t, uint8_t sec, char*);       // SW3.8

/* OverCurrent Functions -------------------------------------------------------------- */

idn_RetVal_t BQ_Set_ChargingOverCurrent (float, uint16_t ms, char*);              		//
idn_RetVal_t BQ_Set_DischargingShortCircuit (scd_thresh_t, uint16_t us, char*);   		//
idn_RetVal_t BQ_Set_DischargingOverCurrent (oc_level_t, float, uint16_t ms, char*);     //

#endif /* BQ_PROTECTION_H_ */
