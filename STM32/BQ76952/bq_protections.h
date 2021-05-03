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
  idn_RetVal_t ret = IDN_BUSY;

  if( xSemaphoreTake( Bq76952.rd.mutex, ( TickType_t ) 10 ) == pdTRUE )
  {
      ret = TICOMM_ReadReg(BQ76952_SLAVE_ADDR, 0x05, Bq76952.rd.buf, 1);
      BQREG_Process_SB_Protection(Bq76952.rd.buf[0], temp);
      sprintf(log, "Get Safety Status (SB) : 0x%02x", buf[0]);
      xSemaphoreGive(  Bq76952.wr.mutex );
  }
  else {
    sprintf(log, "Cannot access the shared resource safely");
  }
  return ret;
}

/* OverVoltage Protection --------------------------------------------------------------------------------------- */

inline idn_RetVal_t BQPR_Set_CUV_Raw (uint8_t thresh, uint16_t dly, uint8_t hyst, char*)
{
  idn_RetVal_t ret = IDN_OK;
  if( xSemaphoreTake( Bq76952.wr.mutex, ( TickType_t ) 10 ) == pdTRUE )
  {
  	Bq76952.wr.buf[0] =	0x92
  	Bq76952.wr.buf[1] = 0x75
	Bq76952.wr.buf[2] = (thresh); 			// Protections:CUV:Threshold
	Bq76952.wr.buf[3] = LOW_BYTE(dly);    	// Protections:CUV:Delay
	Bq76952.wr.buf[4] = HIGH_BYTE(dly);    	// Protections:CUV:Delay
    Bq76952.wr.buf[5] = (hyst);       		// Protections:COV:Recovery H.
	ret = TIComm_WriteFlash(BQ76952_SLAVE_ADDR, NO_CRC, Bq76952.wr.buf, 6);
    sprintf(log, BQPR_Voltage_LogFormat, "CUV", thresh, dly, hyst);
  }
  return ret;
}

inline idn_RetVal_t BQPR_Set_SF_Alert_Mask (sa_protection_t prota, sb_protection_t protb, sc_protection_t protc, char* log)
{
  idn_RetVal_t ret = IDN_BUSY;

  if( xSemaphoreTake( Bq76952.wr.mutex, ( TickType_t ) 10 ) == pdTRUE )
  {
  	Bq76952.wr.buf[0] =	0x92
  	Bq76952.wr.buf[1] = 0x62
    Bq76952.wr.buf[2] = (prota.value);      // Settings:Alarm
    Bq76952.wr.buf[3] = (protb.value);      // Settings:Alarm
    Bq76952.wr.buf[4] = (protc.value);      // Settings:Alarm
    ret = TIComm_WriteFlash(BQ76952_SLAVE_ADDR, NO_CRC, Bq76952.wr.buf, 5);
    sprintf(log, "SF Alert Masks (ABC): 0x%02x, 0x%02x, 0x%02x", prota.value, protb.value, protc.value);
    xSemaphoreGive(  Bq76952.wr.mutex );
  }
  else {
    sprintf(log, "Cannot access the shared resource safely");
  }
  return ret;
}


#endif /* BQ_PROTECTION_H_ */
