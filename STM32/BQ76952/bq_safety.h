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

/* Direct Commands --------------------------------- */
idn_RetVal_t BQSF_Get_PFA_Alert (pfa_safety_t*, char*);
idn_RetVal_t BQSF_Get_PFA_Fault (pfa_safety_t*, char*);
idn_RetVal_t BQSF_Get_PFB_Alert (pfb_safety_t*, char*);
idn_RetVal_t BQSF_Get_PFB_Fault (pfb_safety_t*, char*);
idn_RetVal_t BQSF_Get_PFC_Alert (pfc_safety_t*, char*);
idn_RetVal_t BQSF_Get_PFC_Fault (pfc_safety_t*, char*);
idn_RetVal_t BQSF_Get_PFD_Alert (pfd_safety_t*, char*);
idn_RetVal_t BQSF_Get_PFD_Fault (pfd_safety_t*, char*);

/* OverVoltage Permanent Fail ------------------------------------------------- */
idn_RetVal_t BQSF_Set_UnderVoltage (int16_t threshold_mv, uint8_t delay_s, char*);		// SUV
idn_RetVal_t BQSF_Set_OverVoltage (int16_t threshold_mv, uint8_t delay_s, char*);		// SOV
idn_RetVal_t BQSF_Set_TopOfStack (int16_t threshold_mv, uint8_t delay_s, char*);		// TOS

/* OverTemperature Permanent Fail ------------------------------------------------*/
idn_RetVal_t BQSF_Set_OverTemperature (int8_t threshold_c, uint8_t delay_s, char*);		// SOT
idn_RetVal_t BQSF_Set_OverTemperatureFET (int8_t threshold_c, uint8_t delay_s, char*);	// SOTF

/* OverCurrent PF Permanent Fail ----------------------------------------------------------- */
idn_RetVal_t BQSF_Set_ChargingOverCurrent (int16_t threshold_userA, uint8_t delay_s, char*);	// SOCC
idn_RetVal_t BQSF_Set_DischargingOverCurrent (int16_t threshold_userA, uint8_t delay_s, char*);	// SOCD
idn_RetVal_t BQSF_Set_ChargeFETFailCurrent (int16_t threshold_mA, uint8_t delay_s, char*);		// CFETF
idn_RetVal_t BQSF_Set_DischargeFETFailCurrent (int16_t threshold_mA, uint8_t delay_s, char*); 	// DFETF

/* Others ---------------------------------------------------------------------------- */
idn_RetVal_t BQSF_Set_VoltageImbalanceRelax (int16_t threshold_mv, uint8_t delay_s,
			int16_t checkVoltage_mv, int16_t maxCurrent_ma,  uint16_t duration_s, char*);

#endif /* BQ_SAFETY_H_ */
