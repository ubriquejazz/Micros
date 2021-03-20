/*
 * bq_config.h
 *
 */
#ifndef BQ_CONFIG_H_
#define BQ_CONFIG_H_

#include <stdio.h>
#include "cmsis_os.h"
#include "string.h"
#include "bq76952.h"

/* Exported types ---------------------------------------------------- */

typedef enum {
  PROTECTION_A,     // Table 13-18: SCD, OCD2, OCD1, OCC, COV, CUV
  PROTECTION_B,   // Table 13-19: OTF, OTINT, OTD, OTC, UTINT, UTD, UTC
  PROTECTION_C,   // Table 13-20: OCD3, SCDL, OCDL, COVL, PTO, HWDF
  NumOfProtections
} type_protection_t;

typedef enum {
  PFAIL_A,      // Table 13-18: SCD, OCD2, OCD1, OCC, COV, CUV
  PFAIL_B,    // Table 13-19: OTF, OTINT, OTD, OTC, UTINT, UTD, UTC
  PFAIL_C,    // Table 13-20: OCD3, SCDL, OCDL, COVL, PTO, HWDF
  PFAIL_D,
  NumOfPFails
} type_pfail_t;

typedef enum {
  REG12,             // 7:4 REG2 ; 3:0 REG1
  REG0,            // REG0_EN only (bit 0)
  NumOfRegulators
} regulator_t;

typedef enum {
  CFETOFF,           // BAT_CFETOFF (in)
  DFETOFF,           // BAT_DFETOFF (in)
  ALERT,           // BAT_ALERT (output)
  TS1,           // Thermistor NTC1 (analog)
  TS2,               // Thermistor NTC2 (analog)
  TS3,           // Thermistor NTC3 (analog)
  HDQ,               // Thermistor for the FET temp. control
  DCHG,              // BAT_DDSG (output)
  DDSG,              // BAT_DCHG (output)
  NumOfPins
} bq_pin_t;

/* Regulator Sub-commands ---------------------------------------------------------*/
idn_RetVal_t BQCFG_Get_EnableRegulator (regulator_t, uint8_t* result, char*);
idn_RetVal_t BQCFG_Set_EnableRegulator (regulator_t, uint8_t value, char*);

/* Pin Config Sub-commands ---------------------------------------------------------*/
idn_RetVal_t BQCFG_Get_PinConifg (bq_pin_t, uint8_t* result, char*);
idn_RetVal_t BQCFG_Set_PinConifg (bq_pin_t, uint8_t value, char*);

/* Balance Config Sub-commands -----------------------------------------------------*/
idn_RetVal_t BQCFG_Set_BalancingConfig (uint8_t mode, char*);
idn_RetVal_t BQCFG_Get_BalancingConfig (balance_config_t*, char*);

/* Mfg Status Init Sub-commands -----------------------------------------------------*/
idn_RetVal_t BQCFG_Set_Mfg_Status_Init (uint8_t mode, char*);
idn_RetVal_t BQCFG_Get_Mfg_Status_Init (mfg_status_init_t*, char*);

/* FET Options Sub-commands --------------------------------------------------------------*/
idn_RetVal_t BQCFG_Set_FET_Options (uint8_t mode, char* log);
idn_RetVal_t BQCFG_Get_FET_Options (fet_options_t* fet, char* log);
idn_RetVal_t BQCFG_Set_CHG_Pump_Control (uint8_t mode, char* log);
idn_RetVal_t BQCFG_Get_CHG_Pump_Control (chg_pump_ctrl_t* chg, char* log);

/* VCell Sub-commands --------------------------------------------------------------*/
idn_RetVal_t BQCFG_Set_VCellMode (uint16_t mode, char*);
idn_RetVal_t BQCFG_Get_VCellMode (uint16_t* mode, char*);

/* Protection Sub-commands --------------------------------------------------------*/
idn_RetVal_t BQCFG2_Get_EnableProtection (type_protection_t, uint8_t* result, char*);
idn_RetVal_t BQCFG2_Set_EnableProtection (type_protection_t, uint8_t value, char*);
idn_RetVal_t BQCFG2_Get_CFET_Protection (type_protection_t, uint8_t* result, char*);
idn_RetVal_t BQCFG2_Set_CFET_Protection (type_protection_t, uint8_t value, char*);
idn_RetVal_t BQCFG2_Get_DFET_Protection (type_protection_t, uint8_t* result, char*);
idn_RetVal_t BQCFG2_Set_DFET_Protection (type_protection_t, uint8_t value, char*);

/* PFs Sub-commands ---------------------------------------------------------------*/
idn_RetVal_t BQCFG2_Get_EnablePFail (type_pfail_t, uint8_t* result, char*);
idn_RetVal_t BQCFG2_Set_EnablePFail (type_pfail_t, uint8_t value, char*);

/* Alarm Management ----------------------------------------------------------------*/
idn_RetVal_t BQCFG2_Set_AlarmStatus (uint16_t status, char*);
idn_RetVal_t BQCFG2_Get_AlarmStatus (alarm_hgh_t*, alarm_low_t*, char*);
idn_RetVal_t BQCFG2_Get_RawAlarmStatus (alarm_hgh_t*, alarm_low_t*, char*);
idn_RetVal_t BQCFG2_Set_AlarmEnable (uint16_t enable, char*);
idn_RetVal_t BQCFG2_Get_AlarmEnable (alarm_hgh_t*, alarm_low_t*, char*);
idn_RetVal_t BQCFG2_Set_DefaultAlarmMask (uint16_t mask, char*);
idn_RetVal_t BQCFG2_Get_DefaultAlarmMask (alarm_hgh_t*, alarm_low_t*, char*);

/* Basic Functions -------------------------------------------------------------------*/
idn_RetVal_t BQCFG2_Set_PowerConfig (uint16_t mode, char*);
idn_RetVal_t BQCFG2_Get_PowerConfig (pwr_cfg_hgh_t* h, pwr_cfg_low_t* l, char*);
idn_RetVal_t BQCFG2_Set_RecoveryTime (uint8_t time, char*);
idn_RetVal_t BQCFG2_Set_CCGain (floatIdn_t value, char* log);
idn_RetVal_t BQCFG2_Set_CapacityGain (floatIdn_t value, char* log);

/* Initial Setup -------------------------------------------------------------------*/
idn_RetVal_t BQCFG2_Set_LowCurrent (uint16_t sleep_current, char*);
idn_RetVal_t BQCFG2_Set_LowVbat (float stack_voltage_mv, char*);
idn_RetVal_t BQCFG2_Set_ChargerAttached(float delta_mv, float thresh_mv, char*);
idn_RetVal_t BQCFG2_Set_RelaxMode (uint16_t dsg, uint16_t chg, char*);


#endif /* BQ_CONFIG_H_ */
