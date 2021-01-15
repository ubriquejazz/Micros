/**
  ******************************************************************************
  * @file    bq_safetyt.c
  * @author  IDJGB0
  * @version V1.0.0
  * @date
  * @brief   permanent fail for current, voltage or temperature
  * 		 others like VSS, 2LVL, LFO or HWMX
  *
  ******************************************************************************
  */

#include "bq_safety.h"

/* OverVoltage  ---------------------------------------------------------------*/

/**
  * @brief  BQSF_Set_UnderVoltage (4150, 1000, NULL)
  *			Set user-defined overvoltage protection
  * @param	mv, sec
  * @param 	log*
  */
idn_RetVal_t BQSF_Set_UnderVoltage (int16_t mv, uint16_t sec, char* log)
{
	idn_RetVal_t ret = IDN_OK;
	int16_t temp = mv;
	if(temp < 0 || temp > 32767) {
		temp = 2200;
		ret = IDN_ERROR;
	}
	if(sec < 0 || sec > 255) {
		sec = 5;
		ret = IDN_ERROR;
	}
	// (ret == IDN_OK)
	{
  		Setter_8Bits(DATA_MEM_ADDR(0xCB, 0), temp);
  		Setter_8Bits(DATA_MEM_ADDR(0xCD, 0), sec);
  		sprintf(log, "[+] SUV => %d, 0x%02x", temp, sec);
	}
  	return ret;
}

/**
  * @brief  BQSF_Set_OverVoltage (4150, 1000, NULL)
  *			Set user-defined overvoltage protection
  * @param	mv, sec
  * @param 	log*
  */
idn_RetVal_t BQSF_Set_OverVoltage (int16_t mv, uint16_t sec, char* log)
{
	idn_RetVal_t ret = IDN_OK;
	int16_t temp = mv;
	if(temp < 0 || temp > 32767) {
		temp = 2200;
		ret = IDN_ERROR;
	}
	if(sec < 0 || sec > 255) {
		sec = 5;
		ret = IDN_ERROR;
	}
	// (ret == IDN_OK)
	{
  		Setter_8Bits(DATA_MEM_ADDR(0xCE, 0), temp);
  		Setter_8Bits(DATA_MEM_ADDR(0xD0, 0), sec);
  		sprintf(log, "[+] SOV => %d, 0x%02x", temp, sec);
	}
  	return ret;
}

/**
  * @brief  BQSF_Set_Set_TopOfStack (4150, 1000, NULL)
  *			Top of Stack versus Cell Sum Permanent Fail (PF).
  *			It compares the measured stack voltage to the sum of individually cells
  *			When the difference is greater than the number of cells times the value of mV
  *			for a time of sec,  the TOSF Permanent Fail is triggered.
  *
  * @param	mv, sec
  * @param 	log*
  */
idn_RetVal_t BQSF_Set_TopOfStack (int16_t mv, uint16_t sec, char* log)
{
	idn_RetVal_t ret = IDN_OK;
	int16_t temp = mv;
	if(temp < 0 || temp > 32767) {
		temp = 500;
		ret = IDN_ERROR;
	}
	if(sec < 0 || sec > 255) {
		sec = 5;
		ret = IDN_ERROR;
	}
	// (ret == IDN_OK)
	{
  		Setter_8Bits(DATA_MEM_ADDR(0xCE, 0), temp);
  		Setter_8Bits(DATA_MEM_ADDR(0xD0, 0), sec);
  		sprintf(log, "[+] TOS => %d, 0x%02x", temp, sec);
	}
  	return ret;
}

/* OverCurrent  ---------------------------------------------------------------*/

/**
  * @brief  BQSF_Set_ChargingOverCurrent (4150, 1000, NULL)
  *
  * @param	mv, sec
  * @param 	log*
  */
idn_RetVal_t BQSF_Set_ChargingOverCurrent (int16_t userA, uint16_t sec, char* log)
{
	idn_RetVal_t ret = IDN_OK;
	int16_t temp = userA;
	if( (temp < -32768) || temp > 32767) {
		temp = 10000;
		ret = IDN_ERROR;
	}
	if(sec < 0 || sec > 255) {
		sec = 5;
		ret = IDN_ERROR;
	}
	// (ret == IDN_OK)
	{
  		Setter_8Bits(DATA_MEM_ADDR(0xD4, 0), temp);			//
  		Setter_8Bits(DATA_MEM_ADDR(0xD6, 0), sec);			//
  		sprintf(log, "[+] SOCC => %d, 0x%02x", temp, sec);
	}
  	return ret;
}

/**
  * @brief  BQSF_Set_DischargingOverCurrent (4150, 1000, NULL)
  *
  * @param	mv, sec
  * @param 	log*
  */
idn_RetVal_t BQSF_Set_DischargingOverCurrent (int16_t userA, uint16_t sec, char* log)
{
	idn_RetVal_t ret = IDN_OK;
	int16_t temp = userA;
	if( (temp < -32768) || temp > 32767) {
		temp = 10000;
		ret = IDN_ERROR;
	}
	if(sec < 0 || sec > 255) {
		sec = 5;
		ret = IDN_ERROR;
	}
	// (ret == IDN_OK)
	{
  		Setter_8Bits(DATA_MEM_ADDR(0xD4, 0), temp);			//
  		Setter_8Bits(DATA_MEM_ADDR(0xD6, 0), sec);			//
  		sprintf(log, "[+] SOCD => %d, 0x%02x", temp, sec);
	}
  	return ret;
}

/* OverTemperature  -----------------------------------------------------------*/

/**
  * @brief  BQSF_Set_OverTemperature (4150, 1000, NULL)
  *			Set user-defined over temperature protection
  * @param	mv, sec
  * @param 	log*
  */
idn_RetVal_t BQSF_Set_OverTemperature (int8_t threshold, uint16_t sec, char* log)
{
	idn_RetVal_t ret = IDN_OK;
	int16_t temp = threshold;
	if(temp < -40 || temp > 120) {
		temp = 65;
		ret = IDN_ERROR;
	}
	if(sec < 0 || sec > 255) {
		sec = 5;
		ret = IDN_ERROR;
	}
	// (ret == IDN_OK)
	{
  		Setter_8Bits(DATA_MEM_ADDR(0xDA, 0), temp);
  		Setter_8Bits(DATA_MEM_ADDR(0xDB, 0), sec);
  		sprintf(log, "[+] SOT => %d, 0x%02x", temp, sec);
	}
  	return ret;
}

/**
  * @brief  BQSF_Set_OverTemperature (4150, 1000, NULL)
  *			Set user-defined over temperature protection
  * @param	mv, sec
  * @param 	log*
  */
idn_RetVal_t BQSF_Set_OverTemperatureFET (int8_t threshold, uint16_t sec, char* log)
{
	idn_RetVal_t ret = IDN_OK;
	int16_t temp = threshold;
	if(temp < 0 || temp > 150) {
		temp = 85;
		ret = IDN_ERROR;
	}
	if(sec < 0 || sec > 255) {
		sec = 5;
		ret = IDN_ERROR;
	}
	// (ret == IDN_OK)
	{
  		Setter_8Bits(DATA_MEM_ADDR(0xDC, 0), temp);
  		Setter_8Bits(DATA_MEM_ADDR(0xDD, 0), sec);
  		sprintf(log, "[+] SOT => %d, 0x%02x", temp, sec);
	}
  	return ret;
}

/*  Voltage Imbalance -----------------------------------------------------------*/

/*  CFET-DFET Fail --------------------------------------------------------------*/

/*  Others ----------------------------------------------------------------------*/

