/**
  ******************************************************************************
  * @file    bqcommand.h
  * @author  
  * @version V1.0.0
  * @date    30 October 2020
  * @brief   Source file of bq76952 driver.
  ******************************************************************************
  */
#include "bq_temperature.h"

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
idn_RetVal_t BQ_Set_InternalOverTemperature (int16_t, uint8_t sec, char*)
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

/**
  * @brief  BQ_Set_CellOverTemperature (60, NULL)
  *			Set user-defined cel over-temperature protection
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
  		sprintf(log, "[+] UTINT => %d, 0x%02x", temp, sec);
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

/**
  * @brief  BQ_Set_CellUnderTemperature (-20, NULL)
  *			Set user-defined cel under-temperature protection
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
  		sprintf(log, "[+] UTINT => %d, 0x%02x", temp, sec);
	}
  	return ret;
}