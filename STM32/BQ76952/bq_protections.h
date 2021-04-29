/**
  ******************************************************************************
  * @file    bq_protection.h
  * @author  
  * @version V1.0.0
  * @date    30 October 2020
  * @brief   tier-1 protections (BQPR) and tier-2 protections (BQPR2)
  **********
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
idn_RetVal_t BQPR_Get_SB_Alert (sb_protection_t*, char*);

inline idn_RetVal_t BQPR_Get_SB_Fault (sb_protection_t* temp, char* log)
{
  idn_RetVal_t ret = IDN_OK;
  ret = BQ76952_ReadReg(BQ76952_SLAVE_ADDR, 0x05, 1);
  BQ76952_I2C_WAIT(2);
  BQREG_Process_SB_Protection(Bq76952.buf[0], temp);
  sprintf(log, "Get Safety Status (SB) : 0x%02x", Bq76952.buf[0]);
  return ret;
}

/* OverVoltage Protection --------------------------------------------------------------------------------------- */

inline idn_RetVal_t BQPR_Set_CUV_Raw (uint8_t thresh, uint16_t dly, uint8_t hyst, char*)
{
  idn_RetVal_t ret = IDN_OK;
  {
    ret = BQ76952_Setter_8Bits (DATA_MEM_ADDR(0x75 + 0), thresh); // Protections:CUV:Threshold
    ret = BQ76952_Setter_2Bytes(DATA_MEM_ADDR(0x75 + 1), dly);    // Protections:CUV:Delay
    ret = BQ76952_Setter_8Bits (DATA_MEM_ADDR(0x7B), hyst);       // Protections:COV:Recovery H.
    sprintf(log, BQPR_Voltage_LogFormat, "CUV", thresh, dly, hyst);
  }
  return ret;
}

inline idn_RetVal_t BQPR_Set_SF_Alert_Mask (sa_protection_t prota, sb_protection_t protb, sc_protection_t protc, char* log)
{
  idn_RetVal_t ret = IDN_OK;
  ret = BQ76952_Setter_8Bits (DATA_MEM_ADDR(0x6F + 0), prota.value);      // Settings:Alarm
  ret = BQ76952_Setter_8Bits (DATA_MEM_ADDR(0x6F + 1), protb.value);      // Settings:Alarm
  ret = BQ76952_Setter_8Bits (DATA_MEM_ADDR(0x6F + 2), protc.value);      // Settings:Alarm
  sprintf(log, "SF Alert Masks (ABC): 0x%02x, 0x%02x, 0x%02x", prota.value, protb.value, protc.value);
  return ret;
}
#endif /* BQ_PROTECTION_H_ */
