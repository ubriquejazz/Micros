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

/* Exported types ---------------------------------------------------- */

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

/* Direct Commands ----------------------------------------------------------- */
idn_RetVal_t BQPR_Get_SA_Alert (sa_protection_t*, char*);
idn_RetVal_t BQPR_Get_SA_Fault (sa_protection_t*, char*);
idn_RetVal_t BQPR_Get_SB_Alert (sb_protection_t*, char*);
idn_RetVal_t BQPR_Get_SB_Fault (sb_protection_t*, char*);
idn_RetVal_t BQPR_Get_SC_Alert (sc_protection_t*, char*);
idn_RetVal_t BQPR_Get_SC_Fault (sc_protection_t*, char*);

/* OverVoltage Protection --------------------------------------------------------------------------------------- */
idn_RetVal_t BQPR_Set_CellOverVoltageRaw (uint8_t thresh, uint16_t dly, uint8_t hyst, char*); // COV
idn_RetVal_t BQPR_Set_CellOverVoltage (float threshold_mv, uint16_t delay_ms, float hysteresis_mv, char*);
idn_RetVal_t BQPR_Set_CellUnderVoltageRaw (uint8_t thresh, uint16_t dly, uint8_t hyst, char*); // CUV
idn_RetVal_t BQPR_Set_CellUnderVoltage (float threshold_mv, uint16_t delay_ms, float hysteresis_mv, char*);
idn_RetVal_t BQPR_Set_CellOpenWireCheck (uint8_t sec, char*);

/* OverTemperature Protection ------------------------------------------------------------------------------------ */
idn_RetVal_t BQPR_Set_ChargingOverTemperatureRaw (int8_t thresh, int8_t recovery, uint8_t, char*);  // OTC
idn_RetVal_t BQPR_Set_ChargingOverTemperature (int8_t threshold_c, int8_t recovery_c, uint8_t delay_s, char*);
idn_RetVal_t BQPR_Set_DischargingOverTemperatureRaw (int8_t thresh, int8_t recovery, uint8_t, char*);  // OTD
idn_RetVal_t BQPR_Set_DischargingOverTemperature (int8_t threshold_c, int8_t recovery_c, uint8_t delay_s, char*);
idn_RetVal_t BQPR_Set_OverTemperatureIntRaw (int8_t thresh, int8_t recovery, uint8_t, char*);
idn_RetVal_t BQPR_Set_OverTemperatureInt (int8_t threshold_c, int8_t recovery_c, uint8_t delay_s, char*);

/* UnderTemperature Protection ------------------------------------------------------------------------------------ */
idn_RetVal_t BQPR_Set_ChargingUnderTemperatureRaw (int8_t thresh, int8_t recovery, uint8_t, char*);  // UTC
idn_RetVal_t BQPR_Set_ChargingUnderTemperature (int8_t threshold_c, int8_t recovery_c, uint8_t delay_s, char*);
idn_RetVal_t BQPR_Set_DischargingUnderTemperatureRaw (int8_t thresh, int8_t recovery, uint8_t, char*);  // UTD
idn_RetVal_t BQPR_Set_DischargingUnderTemperature (int8_t threshold_c, int8_t recovery_c, uint8_t delay_s, char*);
idn_RetVal_t BQPR_Set_UnderTemperatureIntRaw (int8_t thresh, int8_t recovery, uint8_t, char*);
idn_RetVal_t BQPR_Set_UnderTemperatureInt (int8_t threshold_c, int8_t recovery_c, uint8_t delay_s, char*);

/* OverCurrent Protection ---------------------------------------------------------------------------------------- */
idn_RetVal_t BQPR_Set_ChargingOverCurrentRaw (uint8_t thresh, uint8_t dly, int16_t recovery, int16_t delta, char*);  // OCC
idn_RetVal_t BQPR_Set_ChargingOverCurrent (float threshold_mv, uint8_t delay_ms, int16_t recovery_mA, int16_t delta_mV, char*);
idn_RetVal_t BQPR_Set_DischargingOverCurrentRaw (oc_level_t, uint8_t thresh, uint8_t dly, char*); // OCDx
idn_RetVal_t BQPR_Set_DischargingOverCurrent (oc_level_t, float threshold_mv, uint8_t delay_ms, char*);
idn_RetVal_t BQPR_Set_DischargingShortCircuitRaw (scd_thresh_t, uint8_t dly, uint8_t recovery, char*); //SCD
idn_RetVal_t BQPR_Set_DischargingShortCircuit(scd_thresh_t thresh, uint16_t delay_us, uint8_t recovery_s, char*);

#endif /* BQ_PROTECTION_H_ */
