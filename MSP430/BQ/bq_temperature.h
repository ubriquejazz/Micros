/**
  ******************************************************************************
  * @file    bqcommand.h
  * @author  
  * @version V1.0.0
  * @date    30 October 2020
  * @brief   Source file of bq76952 driver.
  ******************************************************************************
  */

#ifndef BQ_TEMPERATURE_H_
#define BQ_TEMPERATURE_H_

#include "i2c.h"

/* Defines ----------------------------------------------------------- */

idn_RetVal_t BQ_Set_ChargingOverTemperature (int16_t, uint8_t sec, char*);        // SW3.3
idn_RetVal_t BQ_Set_DischargingOverTemperature (int16_t, uint8_t sec, char*);     // SW3.4
idn_RetVal_t BQ_Set_CellOverTemperature (int8_t, char*);        	  			  // 
idn_RetVal_t BQ_Set_InternalOverTemperature (int16_t, uint8_t sec, char*);        // SW3.7

idn_RetVal_t BQ_Set_ChargingUnderTemperature (int16_t, uint8_t sec, char*);       // SW3.5
idn_RetVal_t BQ_Set_DischargingUnderTemperature (int16_t, uint8_t sec, char*);    // SW3.6
idn_RetVal_t BQ_Set_CellUnderTemperature (int8_t, char*);       	  			  // 
idn_RetVal_t BQ_Set_InternalUnderTemperature (int16_t, uint8_t sec, char*);       // SW3.8

#endif /* BQ_TEMPERATURE_H_ */
