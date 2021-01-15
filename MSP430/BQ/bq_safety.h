/*
 * bq_safetyt.h
 *
 *  Created on: 13 ene. 2021
 *      Author: IDJGB0
 */

#ifndef BQ_SAFETY_H_
#define BQ_SAFETY_H_

#include <stdio.h>
#include "cmsis_os.h"
#include "string.h"
#include "bq76952.h"
#include "IdnDefinitions.h"

/* Functions ----------------------------------------------------------- */

idn_RetVal_t BQSF_Set_UnderVoltage (int16_t, uint16_t, char*);     	// SUV
idn_RetVal_t BQSF_Set_OverVoltage (int16_t, uint16_t, char*);		// SOV
idn_RetVal_t BQSF_Set_TopOfStack (int16_t, uint16_t, char*);		// TOS

idn_RetVal_t BQSF_Set_ChargingOverCurrent (int16_t, uint16_t, char*);		// SOCC
idn_RetVal_t BQSF_Set_DischargingOverCurrent (int16_t, uint16_t, char*);	// SOCD

idn_RetVal_t BQSF_Set_OverTemperature (int8_t, uint16_t, char*);		// SOT
idn_RetVal_t BQSF_Set_OverTemperatureFET (int8_t, uint16_t, char*);		// SOTF

#endif /* BQ_SAFETY_H_ */
