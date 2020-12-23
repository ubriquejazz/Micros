/*
 * bqCommand.h
 *
 *  Created on: 26 oct. 2020
 *      Author: IDJGB0
 *
 *
  @verbatim
  */

#ifndef BQ7COMMAND_H_
#define BQ7COMMAND_H_

/** @addtogroup IDNEO
  * @{
  */

/** @addtogroup BQ76952 Peripheral
  * @{
  */

idn_RetVal_t BQ76952_AlarmEnable (uint8_t* result);

/* Subcommands (Tx) ------------------------------------------------------------*/

idn_RetVal_t BQ76952_FETEnable (uint8_t* result) 

idn_RetVal_t BQ76952_Reset (uint8_t* result);

idn_RetVal_t BQ76952_SetConfigUpdateMode (uint8_t mode, uint8_t* result);

/* Subcommands (TxRx) ----------------------------------------------------------*/

idn_RetVal_t BQ76952_ManufacturerStatus (uint8_t* result);

idn_RetVal_t BQ76952_DeviceNumber (uint8_t* result);

idn_RetVal_t BQ76952_ReadEnableProtection (uint8_t* result);

/* Wr Registers --------------------------------------------------------------*/

idn_RetVal_t BQ76952_WriteEnableProtection (uint8_t value, uint8_t* result);

idn_RetVal_t BQ76952_VCellMode (uint16_t mode, uint8_t* result);


#endif /* BQ7COMMAND_H_ */