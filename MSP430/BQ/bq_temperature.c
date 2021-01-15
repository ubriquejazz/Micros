/**
  ******************************************************************************
  * @file    bq_protection.c
  * @author  IDJGB0
  * @version V1.0.0
  * @date
  * @brief   under and over-temperature protection CUV, COV
  ******************************************************************************
  */

#include "bq_protection.h"

/* OverVoltage  ---------------------------------------------------------------*/

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
  *			When a cell is detached, that node voltage is discharged, and an under-voltage
  *			event will be triggered on that cell. The current source is enabled for the
  *			duration of an ADC measurement (3 ms) once per the interval defined by *sec*
  *
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

/* OverTemperature ------------------------------------------------------------*/

/**
  * @brief  BQ_Set_ChargingOverTemperature (45, 1, NULL)
  *			Set user-defined charging over-temperature protection
  * @param	temp, sec
  * @param 	log*
  */
idn_RetVal_t BQ_Set_ChargingOverTemperature (int16_t temp, uint8_t sec, char* log)
{
	idn_RetVal_t ret = IDN_OK;
	if(temp < -40 || temp > 120) {
		temp = 55;
		ret = IDN_ERROR;
	}
	if(sec < 0 || sec > 255) {
		sec = 1;
		ret = IDN_ERROR;
	}
	// (ret == IDN_OK)
	{
  		Setter_8Bits(DATA_MEM_ADDR(0x9A, 0), temp);			// Protections:OTC:Threshold
  		Setter_8Bits(DATA_MEM_ADDR(0x9A, 1),sec);			// Protections:OTC:Delay
  		sprintf(log, "[+] OTC => %d, 0x%02x", temp, sec);
	}
  	return ret;
}

/**
  * @brief  BQ_Set_DischargingOverTemperature (65, 1, NULL)
  *			Set user-defined discharging over-temperature protection
  * @param	temp, sec
  * @param 	log*
  */
idn_RetVal_t BQ_Set_DischargingOverTemperature (int16_t temp, uint8_t sec, char* log)
{
	idn_RetVal_t ret = IDN_OK;
	if(temp < -40 || temp > 120) {
		temp = 60;
		ret = IDN_ERROR;
	}
	if(sec < 0 || sec > 255) {
		sec = 2;
		ret = IDN_ERROR;
	}
	// (ret == IDN_OK)
	{
  		Setter_8Bits(DATA_MEM_ADDR(0x9D, 0), temp);			// Protections:OTD:Threshold
  		Setter_8Bits(DATA_MEM_ADDR(0x9D, 1), sec);			// Protections:OTD:Threshold
  		sprintf(log, "[+] OTD => %d, 0x%02x", temp, sec);
	}
  	return ret;
}

/**
  * @brief  BQ_Set_InternalOverTemperature (50, 1, NULL)
  *			Set user-defined discharging over-temperature protection
  * @param	temp, sec
  * @param 	log*
  */
idn_RetVal_t BQ_Set_InternalOverTemperature (int16_t temp, uint8_t sec, char* log)
{
	idn_RetVal_t ret = IDN_OK;
	if(temp < -40 || temp > 120) {
		temp = 85;
		ret = IDN_ERROR;
	}
	if(sec < 0 || sec > 255) {
		sec = 2;
		ret = IDN_ERROR;
	}
	// (ret == IDN_OK)
	{
  		Setter_8Bits(DATA_MEM_ADDR(0xA3, 0), temp);			// Protections:OTINT:Threshold
  		Setter_8Bits(DATA_MEM_ADDR(0xA3, 1), sec);			// Protections:OTINT:Threshold
  		sprintf(log, "[+] OTINT => %d, 0x%02x", temp, sec);
	}
  	return ret;
}

/* UnderTemperature ------------------------------------------------------------*/

/**
  * @brief  BQ_Set_ChargingUnderTemperature (0, 1, NULL)
  *			Set user-defined charging under-temperature protection
  * @param	temp, sec
  * @param 	log*
  */
idn_RetVal_t BQ_Set_ChargingUnderTemperature (int16_t temp, uint8_t sec, char* log)
{
	idn_RetVal_t ret = IDN_OK;
	if(temp < -40 || temp > 120) {
		temp = 0;
		ret = IDN_ERROR;
	}
	if(sec < 0 || sec > 255) {
		sec = 2;
		ret = IDN_ERROR;
	}
	// (ret == IDN_OK)
	{
  		Setter_8Bits(DATA_MEM_ADDR(0xA6, 0), temp);			// Protections:UTC:Threshold
  		Setter_8Bits(DATA_MEM_ADDR(0xA6, 1),sec);			// Protections:UTC:Delay
  		sprintf(log, "[+] UTC => %d, 0x%02x", temp, sec);
	}
  	return ret;
}

/**
  * @brief  BQ_Set_DischargingUnderTemperature (-25, 1, NULL)
  *			Set user-defined discharging Under-temperature protection
  * @param	temp, sec
  * @param 	log*
  */
idn_RetVal_t BQ_Set_DischargingUnderTemperature (int16_t temp, uint8_t sec, char* log)
{
	idn_RetVal_t ret = IDN_OK;
	if(temp < -40 || temp > 120) {
		temp = 0;
		ret = IDN_ERROR;
	}
	if(sec < 0 || sec > 255) {
		sec = 2;
		ret = IDN_ERROR;
	}
	// (ret == IDN_OK)
	{
  		Setter_8Bits(DATA_MEM_ADDR(0xA9, 0), temp);			// Protections:UTD:Threshold
  		Setter_8Bits(DATA_MEM_ADDR(0xA9, 1), sec);			// Protections:UTD:Threshold
  		sprintf(log, "[+] UTD => %d, 0x%02x", temp, sec);
	}
  	return ret;
}

/**
  * @brief  BQ_Set_InternalUnderTemperature (-20, 1, NULL)
  *			Set user-defined discharging under-temperature protection
  * @param	temp, sec
  * @param 	log*
  */
idn_RetVal_t BQ_Set_InternalUnderTemperature (int16_t temp, uint8_t sec, char* log)
{
	idn_RetVal_t ret = IDN_OK;
	if(temp < -40 || temp > 120) {
		temp = 0;
		ret = IDN_ERROR;
	}
	if(sec < 0 || sec > 255) {
		sec = 2;
		ret = IDN_ERROR;
	}
	// (ret == IDN_OK)
	{
  		Setter_8Bits(DATA_MEM_ADDR(0xAC, 0), temp);			// Protections:UTINT:Threshold
  		Setter_8Bits(DATA_MEM_ADDR(0xAC, 1), sec);			// Protections:UTINT:Threshold
  		sprintf(log, "[+] UTINT => %d, 0x%02x", temp, sec);
	}
  	return ret;
}

/* Cell Balancing ------------------------------------------------------------*/

/**
  * @brief  BQ_Set_CellOverTemperature (60, NULL)
  *			Set user-defined cell over-temperature protection
  * @param	temp
  * @param 	log*
  */
idn_RetVal_t BQ_Set_CellOverTemperature (int8_t temp, char* log)
{
	idn_RetVal_t ret = IDN_OK;
	if(temp < -128 || temp > 127) {
		temp = 60;
		ret = IDN_ERROR;
	}
	// (ret == IDN_OK)
	{
  		Setter_8Bits(0x9337, temp);			// settings::cell_balance_config
  		sprintf(log, "[+] Max Cell T. => %d, 0x%02x", temp);
	}
  	return ret;
}

/**
  * @brief  BQ_Set_CellUnderTemperature (-20, NULL)
  *			Set user-defined cell under-temperature protection
  * @param	temp
  * @param 	log*
  */
idn_RetVal_t BQ_Set_CellUnderTemperature (int8_t temp, char* log)
{
	idn_RetVal_t ret = IDN_OK;
	if(temp < -128 || temp > 127) {
		temp = -20;
		ret = IDN_ERROR;
	}
	// (ret == IDN_OK)
	{
  		Setter_8Bits(0x9336, temp);			// settings::cell_balance_config
  		sprintf(log, "[+] Min Cell T. => %d, 0x%02x", temp);
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
  * @param	thresh	- threshold for the sense resistor voltage.
  * @param	us		- delay before the fault is triggered in units of us
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
