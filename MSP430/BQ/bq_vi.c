/**
  ******************************************************************************
  * @file    bqcommand.h
  * @author  
  * @version V1.0.0
  * @date    30 October 2020
  * @brief   Source file of bq76952 driver.
  ******************************************************************************
  */
#include "bq_vi.h"

/* Cells  ---------------------------------------------------------------------*/

/**
  * @brief  BQ_Set_CellOverVoltage (4150, 1000, NULL)
  *			Set user-defined overvoltage protection
  * @param	mv, ms
  * @param 	log*		
  */
idn_RetVal_t BQ_Set_CellOverVoltage (float mv, uint16_t ms, char* log) 
{
	idn_RetVal_t ret;
	uint16_t dly;
	ret = MillisecondsU2(ms, &dly);

	uint8_t thresh = (uint8_t) (mv/50.6);
	if(thresh < 20 || thresh > 110) {
		thresh = 86;
		ret = IDN_ERROR;
	}
	// (ret == IDN_OK) 
	{
		Setter_8Bits (DATA_MEM_ADDR(0x78, 0), thresh);		// Protections:COV:Threshold
		Setter_2Bytes(DATA_MEM_ADDR(0x78, 1), dly);			// Protections:COV:Delay
		sprintf(log, "[+] COV => 0x%02x, 0x%04x", thresh, dly);
	}
	return ret;
}

/**
  * @brief  BQ_Set_CellUnderVoltage (2880, 1000, NULL)
  *			Set user-defined undervoltage protection (SW3)
  * @param	mv, ms
  * @param 	log*		
  */
idn_RetVal_t BQ_Set_CellUnderVoltage (float mv, uint16_t ms, char* log) 
{
	idn_RetVal_t ret;
	uint16_t dly;
	ret = MillisecondsU2(ms, &dly);

	uint8_t thresh = (uint8_t) (mv/50.6);
	if(thresh < 20 || thresh > 90) {
		thresh = 50;
		ret = IDN_ERROR;
	}
	// (ret == IDN_OK) 
	{
		Setter_8Bits (DATA_MEM_ADDR(0x75, 0), thresh);		// Protections:CUV:Threshold
		Setter_2Bytes(DATA_MEM_ADDR(0x75, 1), dly);			// Protections:CUV:Delay
		sprintf(log, "[+] CUV => 0x%02x, 0x%04x", thresh, dly);
	}
	return ret;
}

/**
  * @brief  BQ_Set_CellOpenWireCheck (5, NULL)
  *			Set user-defined undervoltage protection (SW3)
  * @param	sec  - 0 = cell open-wire check is disabled.
  * @param 	log*		
  */
idn_RetVal_t BQ_Set_CellOpenWireCheck (uint8_t sec, char* log) 
{
	idn_RetVal_t ret;
	if(sec < 0 || sec > 255) {
		sec = 5;
		ret = IDN_ERROR;
	}
	// (ret == IDN_OK) 
	{
		Setter_8Bits (0x9314, sec);				// settings:cell open wire
		sprintf(log, "[+] => 0x%02x s", sec);
	}
	return ret;
}

/* OverCurrent ----------------------------------------------------------------*/

/**
  * @brief  BQ_Set_ChargingOverCurrent (20, 10, NULL)
  *			Set user-defined charging current protection
  * @param	amps, ms
  * @param 	log*		
  */
idn_RetVal_t BQ_Set_ChargingOverCurrent (float amps, uint16_t ms, char* log) 
{
	idn_RetVal_t ret;
	uint8_t dly;
	ret = MillisecondsU1(ms, 4, &dly);

	uint8_t thresh = (uint8_t) (amps/2.0);
	if(thresh < 2 || thresh > 62) {
		thresh = 2;
		ret = IDN_ERROR;
	}
	// (ret == IDN_OK) 
	{
		Setter_8Bits(DATA_MEM_ADDR(0x80, 0), thresh);		// Protections:OCC:Threshold
		Setter_8Bits(DATA_MEM_ADDR(0x80, 1), dly);			// Protections:OCC:Delay
		sprintf(log, "[+] OCC => 0x%02x, 0x%04x", thresh, dly);
	}
	return ret;
}

/**
  * @brief  BQ_Set_DischargingOverCurrent (OCD1, 20, 10, NULL)
  *			Set user-defined discharging current protection
  * @param	amps, ms
  * @param 	log*		
  */
idn_RetVal_t BQ_Set_DischargingOverCurrent (oc_level_t level, float amps, uint16_t ms, char* log) 
{
	idn_RetVal_t ret = IDN_OK;
	uint8_t dly;

	uint8_t thresh = (uint8_t) (amps/2.0);
	if(thresh < 2 || thresh > 100) {
		thresh = 4;
		ret = IDN_ERROR;
	}
	// (ret == IDN_OK) 
	{
		if (level == OCD1) {	
			ret = MillisecondsU1(ms, 1, &dly);
			Setter_8Bits(DATA_MEM_ADDR(0x82, 0), thresh);		// Protections:OCD1:Threshold
			Setter_8Bits(DATA_MEM_ADDR(0x82, 1), dly);			// Protections:OCD1:Delay
			sprintf(log, "[+] OCD1 => 0x%02x, 0x%04x", thresh, dly);
		}
		else if (level == OCD2) {
			ret = MillisecondsU1(ms, 7, &dly);
			Setter_8Bits(DATA_MEM_ADDR(0x84, 0), thresh);		// Protections:OCD2:Threshold
			Setter_8Bits(DATA_MEM_ADDR(0x84, 1), dly);			// Protections:OCD2:Delay
			sprintf(log, "[+] OCD2 => 0x%02x, 0x%04x", thresh, dly);
		}
	}
	return ret;
}

/**
  * @brief  BQ_Set_DischargingShortCircuit (SCD_40, 100, NULL)
  *			Set user-defined discharging current protection
  * @param	thresh, us
  * @param 	log*		
  */
idn_RetVal_t BQ_Set_DischargingShortCircuit (scd_thresh_t thresh, uint16_t us, char* log) 
{
	idn_RetVal_t ret = IDN_OK;	
	float delay = us/15.0 + 1.0;
	uint8_t dly = (uint8_t) delay;
	if(dly < 1 || dly > 31) {	
		dly = 2;
		ret = IDN_ERROR;
	}
	// (ret == IDN_OK) 
	{
  		Setter_8Bits(DATA_MEM_ADDR(0x86, 0), thresh);		// Protections:SCD:Threshold
  		Setter_8Bits(DATA_MEM_ADDR(0x86, 1), dly);			// Protections:SCD:Delay
		sprintf(log, "[+] SCD => 0x%02x, 0x%02x (us)", thresh, dly);
  	}
  	return ret;
}