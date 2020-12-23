/**
  ******************************************************************************
  * @file    bqCommand.c
  * @author  IDJGB0
  * @version V1.0.0
  * @date    30 October 2020
  * @brief   Source file of bq76952 driver.
  ******************************************************************************
  */

#include "bq76952.h"
#include "bqcommand.h"
#include "bq76952_hm.h"

/* Defines ------------------------------------------------------------------*/

#define BQ76952_Printf(args ...)		IO_SendString(args)
#define BQ76952_Assert(args ...)
#define BQ76952_ErrorPrintf(args ...)	IO_SendString(args)

/* Private typedef -----------------------------------------------------------*/

// Create Buffers for 2, 3, or 4 bytes of data
uint8_t TX_2Byte [2] = {0x00, 0x00};
uint8_t RX_2Byte [2] ;
uint8_t TX_3Byte [3] = {0x00, 0x00, 0x00};
uint8_t TX_4Byte [4] = {0x00, 0x00, 0x00, 0x00};

idn_RetVal_t BQ76952_AlarmEnable (uint16_t value, uint8_t* result) 
{
	idn_RetVal_t ret = IDN_OK;
	TX_2Byte[0] = LOW_BYTE(value);			// 0x82;	
	TX_2Byte[1] = HIGH_BYTE(value);			// 0xF0;	
	BQ76952_BasicCommand(BQ76952_REG_ALARM_ENABLE, TX_2Byte);
	sprintf(result, "Write Alarm Enable to 0x%04x", TX_2Byte[1] * 256 + TX_2Byte[0]);
	return ret;
}

/* Subcommands (Tx) ------------------------------------------------------------*/

idn_RetVal_t BQ76952_FETEnable (uint8_t* result) 
{
	idn_RetVal_t ret = IDN_OK;
	TX_2Byte[0] = BQ76952_REG_FET_ENABLE;
	TX_2Byte[1] = 0x00;
	ret = BQ76952_SubCommandTransmit(TX_2Byte);
	sprintf(result, "FET enabled");	
	return ret;
}

idn_RetVal_t BQ76952_Reset (uint8_t* result) 
{
	idn_RetVal_t ret = IDN_OK;
	TX_2Byte[0] = BQ76952_REG_RESET;
	TX_2Byte[1] = 0x00;
	ret = BQ76952_SubCommandTransmit(TX_2Byte);
	sprintf(result, "Returned to default settings");	
	return ret;
}

idn_RetVal_t BQ76952_SetConfigUpdateMode (uint8_t mode, uint8_t* result) 
{
	idn_RetVal_t ret = IDN_OK;
	if (mode){
		TX_2Byte[0] = BQ76952_REG_SET_CFGUPDATE;
	} else {
		TX_2Byte[0] = BQ76952_REG_EXIT_CFGUPDATE;
	}
	TX_2Byte[1] = 0x00;	
	ret = BQ76952_SubCommandTransmit(TX_2Byte);
	sprintf(result, "Setup CONFIG_UPDATE mode");	
	return ret;
}

/* Subcommands (TxRx) ----------------------------------------------------------*/

idn_RetVal_t BQ76952_ManufacturerStatus (uint8_t* result) 
{
	idn_RetVal_t ret = IDN_OK;
	TX_2Byte[0] = BQ76952_REG_MANUFACTURER;
	TX_2Byte[1] = 0x00;
	ret = BQ76952_SubCommandTransmit(TX_2Byte);
	BQ76952_SubCommandResponse(RX_2Byte, 2);
	sprintf(result, "Read Manufacturer Status 0x%04x", RX_2Byte[1] * 256 + RX_2Byte[0]);
	return ret;
}

idn_RetVal_t BQ76952_DeviceNumber (uint8_t* result) 
{
	idn_RetVal_t ret = IDN_OK;
	TX_2Byte[0] = BQ76952_REG_DEVICE_NUMBER;
	TX_2Byte[1] = 0x00;
	ret = BQ76952_SubCommandTransmit(TX_2Byte);
	BQ76952_SubCommandResponse(RX_2Byte, 2);
	sprintf(result, "Read Device Number 0x%04x", RX_2Byte[1] * 256 + RX_2Byte[0]);
	return ret;
}

idn_RetVal_t BQ76952_ReadEnableProtection (uint8_t* result) 
{
	idn_RetVal_t ret = IDN_OK;
	TX_2Byte[0] = BQ76952_REG_ENA_PROTECT_L;
	TX_2Byte[1] = BQ76952_REG_ENA_PROTECT_H;
	ret = BQ76952_SubCommandTransmit(TX_2Byte);
	BQ76952_SubCommandResponse(RX_2Byte, 1);
	sprintf(result, "Read Enable Protections A 0x%02x", RX_2Byte[0]);
	return ret;
}

/* Wr Registers --------------------------------------------------------------*/

idn_RetVal_t BQ76952_WriteEnableProtection (uint8_t value, uint8_t* result) 
{
	idn_RetVal_t ret = IDN_OK;

}

idn_RetVal_t BQ76952_WriteEnableProtection2 (uint8_t value, uint8_t* result) 
{
	idn_RetVal_t ret = IDN_OK;
	TX_3Byte[0] = BQ76952_REG_ENA_PROTECT_L;
	TX_3Byte[1] = BQ76952_REG_ENA_PROTECT_H;
	TX_3Byte[2] = value;
	BQ76952_WriteReg(BQ76952_REG_COMMAND, TX_3Byte, 3);
	TX_2Byte[0] = 0x80;
	TX_2Byte[1] = 0x05;
	BQ76952_WriteReg(BQ76952_REG_CHECKSUM, TX_2Byte, 2);	
	sprintf(result, "Write Enable Protections A");
	return ret;
}

idn_RetVal_t BQ76952_VCellMode (uint16_t mode, uint8_t* result) 
{
	idn_RetVal_t ret = IDN_OK;
}

idn_RetVal_t BQ76952_VCellMode2 (uint16_t mode, uint8_t* result) 
{
	idn_RetVal_t ret = IDN_OK;
	TX_4Byte[0] = BQ76952_REG_VCELL_MODE_L;
	TX_4Byte[1] = BQ76952_REG_VCELL_MODE_H;
	TX_4Byte[2] = HIGH_BYTE(mode);
	TX_4Byte[3] = LOW_BYTE(mode);	
	BQ76952_WriteReg(BQ76952_REG_COMMAND, TX_4Byte, 4);
	TX_2Byte[0] = 0xE6;
	TX_2Byte[1] = 0x06;
	BQ76952_WriteReg(BQ76952_REG_CHECKSUM, TX_2Byte, 2);	
	sprintf(result, "Write Cell Mode Register");
	return ret;
}

/* Rd Registers --------------------------------------------------------------*/
