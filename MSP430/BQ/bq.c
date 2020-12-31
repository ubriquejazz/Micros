/**
  ******************************************************************************
  * @file    bqcommand.h
  * @author  
  * @version V1.0.0
  * @date    30 October 2020
  * @brief   Source file of bq76952 driver.
  ******************************************************************************
  */
#include "bq.h"

// Direct Commands
#define	BQ76952_REG_ALARM_ENABLE	0x66	// R/W. By default is set to 0xF800
#define BQ76952_REG_VCELL_1			0x14	// R. 16-bit voltage on cell 1
#define BQ76952_REG_TEMPERATURE		0x68	// R. most recent measured internal die temp.
#define BQ76952_REG_CC2				0x3A	// R. 16-bit current (Filter 2)

// Subcommands (0x3E, 0x40)
#define BQ76952_REG_DEVICE_NUMBER 	0x01	// R. Identifies the product (7695x)
#define BQ76952_REG_MANUFACTURER	0x57	// R. Provides flags for use during manufacturing
#define BQ76952_REG_FET_ENABLE		0x22	// W. FET_EN = 0 means Test Mode. FET_EN = 1 means FW Control
#define BQ76952_REG_RESET 		  	0x12	// W. Reset the device
#define BQ76952_REG_SET_CFGUPDATE	0x90	// W. Enters CONFIG_UPDATE mode
#define BQ76952_REG_EXIT_CFGUPDATE	0x92	// W. Also clears the Battery Status() [POR] and Battery Status()[WD] bits.

/* TX_*Byte are example buffers initialized in the master, they will be
 * sent by the master to the slave.
 * RX_*Byte are example buffers initialized in the slave, they will be
 * sent by the slave to the master.
 * */
// Create Buffers for 2, 3, or 4 bytes of data
uint8_t TX_2Byte [2] = {0x00, 0x00};
uint8_t TX_3Byte [3] = {0x00, 0x00, 0x00};
uint8_t TX_4Byte [4] = {0x00, 0x00, 0x00, 0x00};
uint8_t TX_Buffer [MAX_BUFFER_SIZE] = {0};

uint8_t RX_2Byte [2] = {0x00, 0x00};
uint8_t RX_3Byte [3] = {0x00, 0x00, 0x00};
uint8_t RX_4Byte [4] = {0x00, 0x00, 0x00, 0x00};
uint8_t RX_Buffer [MAX_BUFFER_SIZE] = {0};

unsigned char Checksum(unsigned char *ptr, unsigned char len)
{
	unsigned char i;
	unsigned char checksum = 0;
	
	for(i=0; i<len; i++)
		checksum += ptr[i];
	checksum = 0xff & ~checksum;
	return(checksum);
}

//******************************************************************************
// Basic Commands 
//******************************************************************************

idn_RetVal_t BQ_Set_AlarmEnable (uint16_t alarm_source, char* log) 
{
	idn_RetVal_t ret = IDN_OK;
	TX_2Byte[0] = LOW_BYTE(alarm_source);						// 0x82;	
	TX_2Byte[1] = HIGH_BYTE(alarm_source);						// 0xF0;	
	I2C_WriteReg(0x08, BQ76952_REG_ALARM_ENABLE, TX_2Byte, 2);
	sprintf(log, "Set Alarm Enable to 0x%04x", TX_2Byte[1] * 256 + TX_2Byte[0]);
	return ret;
}

idn_RetVal_t BQ_Get_AlarmEnable (uint16_t* alarm_source, char* log) 
{
	idn_RetVal_t ret = IDN_OK;
	*alarm_source = RX_2Byte[1] * 256 + RX_2Byte[0];
	sprintf(log, "Get Alarm Enable : 0x%04x", RX_2Byte[1] * 256 + RX_2Byte[0]);
	return ret;
}

/* Subcommands (Tx) ------------------------------------------------------------*/

/**
  * @brief  Toggle FET_EN in Manufacturing Status (see below)
  *			FET_EN = 0 means FET Test Mode. FET_EN = 1 means Firmware FET Control
  * @retval Idn return value
**/
idn_RetVal_t BQ_Set_FETEnable (char* log) 
{
	idn_RetVal_t ret = IDN_OK;
	TX_2Byte[0] = BQ76952_REG_FET_ENABLE;
	TX_2Byte[1] = 0x00;
	I2C_WriteReg(0x08, 0x3E, TX_2Byte, 2);
    wait(1);
	sprintf(log, "Toggle FET_EN in Manufacturing Status");	
	return ret;
}

idn_RetVal_t BQ_Set_Reset (char* log) 
{
	idn_RetVal_t ret = IDN_OK;
	TX_2Byte[0] = BQ76952_REG_RESET;
	TX_2Byte[1] = 0x00;
	I2C_WriteReg(0x08, 0x3E, TX_2Byte, 2);
    wait(2);
	sprintf(log, "Returned to default settings");	
	return ret;
}

idn_RetVal_t BQ_Set_ConfigUpdateMode (uint8_t mode, char* log) 
{
	idn_RetVal_t ret = IDN_OK;
	if (mode){
		TX_2Byte[0] = BQ76952_REG_SET_CFGUPDATE;
	} else {
		TX_2Byte[0] = BQ76952_REG_EXIT_CFGUPDATE;
	}
	TX_2Byte[1] = 0x00;	
	I2C_WriteReg(0x08, 0x3E, TX_2Byte, 2);
	sprintf(log, "Set CONFIG_UPDATE mode");	
	return ret;
}

/* Subcommands (TxRx) ----------------------------------------------------------*/

idn_RetVal_t BQ_Get_ManufacturerStatus (uint16_t* status, char* log) 
{
	idn_RetVal_t ret = IDN_OK;
	TX_2Byte[0] = BQ76952_REG_MANUFACTURER;
	TX_2Byte[1] = 0x00;
	I2C_WriteReg(0x08, 0x3E, TX_2Byte, 2);
    wait(1);
    I2C_ReadReg(0x08, 0x40, 2);
    wait(2);
	*status = RX_2Byte[1] * 256 + RX_2Byte[0];
	sprintf(log, "Get Manufacturer Status : 0x%04x", RX_2Byte[1] * 256 + RX_2Byte[0]);
	return ret;
}

idn_RetVal_t BQ_Get_DeviceNumber (uint16_t* device_number, char* log) 
{
	idn_RetVal_t ret = IDN_OK;
	TX_2Byte[0] = BQ76952_REG_DEVICE_NUMBER;
	TX_2Byte[1] = 0x00;
    I2C_WriteReg(0x08, 0x3E, TX_2Byte, 2);
    wait(1);
    I2C_ReadReg(0x08, 0x40, 2);
    wait(2);
	*device_number = RX_2Byte[1] * 256 + RX_2Byte[0];
	sprintf(log, "Get Device Number : 0x%04x", RX_2Byte[1] * 256 + RX_2Byte[0]);
	return ret;
}

/**
  * @brief 	To verify that default settings have COV (over-voltage) and SCD (short-circuit) 
  *			protections enable. 
**/
idn_RetVal_t BQ_Get_EnableProtection (protection_t protect, uint8_t* result, char* log) 
{
	idn_RetVal_t ret = IDN_OK;
	TX_2Byte[0] = 0x61 + protect;
	TX_2Byte[1] = 0x92;
    I2C_WriteReg(0x08, 0x3E, TX_2Byte, 2);
    wait(1);
    I2C_ReadReg(0x08, 0x40, 1);
    wait(2);
	*result = RX_2Byte[0];
	sprintf(log, "Get Enable Protections : 0x%02x", RX_2Byte[0]);
	return ret;
}

idn_RetVal_t BQ76952_Get_VCellMode (uint16_t* mode, char* log) 
{
	idn_RetVal_t ret = IDN_OK;
	return ret;
}

/* Wr Registers --------------------------------------------------------------*/

/**
  * @brief  BQ_Set_EnableRegulator()
  * 		It is recommended to first enter CONFIG_UPDATE mode
  */
idn_RetVal_t BQ_Set_EnableRegulator (regulator_t regx, uint8_t value, char* log) 
{
	idn_RetVal_t ret = IDN_OK;
	TX_3Byte[0] = 0x36 + regx;		// settings::configuration
	TX_3Byte[1] = 0x92;
	TX_3Byte[2] = value;
	I2C_WriteReg(0x08, 0x3E, TX_3Byte, 3);
	wait(1);
	TX_2Byte[0] = Checksum(TX_3Byte, 3);
	TX_2Byte[1] = 0x05;
	I2C_WriteReg(0x08, 0x60, TX_2Byte, 2);	
	sprintf(log, "Set Regulator %d to 0x%02x", regx, value);
	return ret;
}

idn_RetVal_t BQ_Set_EnableProtection (protection_t abc, uint8_t value, char* log) 
{
	idn_RetVal_t ret = IDN_OK;
	TX_3Byte[0] = 0x61 + abc;			// settings::configuration
	TX_3Byte[1] = 0x92;
	TX_3Byte[2] = value;
	I2C_WriteReg(0x08, 0x3E, TX_3Byte, 3);
	wait(1);
	TX_2Byte[0] = Checksum(TX_3Byte, 3);
	TX_2Byte[1] = 0x05;
	I2C_WriteReg(0x08, 0x60, TX_2Byte, 2);	
	sprintf(log, "Set Enable Protections %d to 0x%02x", abc, value);
	return ret;
}

/**
  * @brief  BQ76952_Set_VCellMode()
  * 		It is recommended to first enter CONFIG_UPDATE mode
  */
idn_RetVal_t BQ76952_Set_VCellMode (uint16_t mode, char* log) 
{
	idn_RetVal_t ret = IDN_OK;
	TX_4Byte[0] = 0x04;				// settings::configuration
	TX_4Byte[1] = 0x93;
	TX_4Byte[2] = HIGH_BYTE(mode);
	TX_4Byte[3] = LOW_BYTE(mode);
	I2C_WriteReg(0x08, 0x3E, TX_4Byte, 4); 	
	wait(1);
	TX_2Byte[0] = Checksum(TX_4Byte, 4); 
	TX_2Byte[1] = 0x06;  // Length
    I2C_WriteReg(0x08, 0x60, TX_2Byte, 2);
	sprintf(log, "Set Cell Mode Register to 0x%04x", mode);
	return ret;
}

//******************************************************************************
// System Commands 
//******************************************************************************

idn_RetVal_t BQ_SetTemperatures(void)
{
	idn_RetVal_t ret = IDN_OK;

	// Set TS1 to measure Cell Temperature - 0x92FD = 0x07
	TX_3Byte[0] = 0xFD; TX_3Byte[1] = 0x92; TX_3Byte[2] = 0x07;
    I2C_WriteReg(0x08, 0x3E, TX_3Byte, 3); 
    wait(1);
	TX_2Byte[0] = Checksum(TX_3Byte, 3); TX_2Byte[1] = 0x05;
    I2C_WriteReg(0x08, 0x60, TX_2Byte, 2);	
	
	// Set TS3 to measure FET Temperature - 0x92FF = 0x0F
	TX_3Byte[0] = 0xFF; TX_3Byte[1] = 0x92; TX_3Byte[2] = 0x0F;
    I2C_WriteReg(0x08, 0x3E, TX_3Byte, 3); 
    wait(1);
	TX_2Byte[0] = Checksum(TX_3Byte, 3); TX_2Byte[1] = 0x05;
    I2C_WriteReg(0x08, 0x60, TX_2Byte, 2);	
    return ret;
}

idn_RetVal_t BQ_PeriodicMeasurement(void)
{
	idn_RetVal_t ret = IDN_OK;

	// Read Voltage on cells
	I2C_ReadReg(0x08, 0x14, 2); // Cell 1
	I2C_ReadReg(0x08, 0x16, 2); // Cell 2
	I2C_ReadReg(0x08, 0x18, 2); // Cell 3
	I2C_ReadReg(0x08, 0x1A, 2); // Cell 4
	I2C_ReadReg(0x08, 0x1C, 2); // Cell 5
	I2C_ReadReg(0x08, 0x1E, 2); // Cell 6 (VC6)
	I2C_ReadReg(0x08, 0x26, 2); // Cell 7 (VC10)
	// Read CC2 Current
	I2C_ReadReg(0x08, 0x3A, 2);
	// Read Temperatures
	I2C_ReadReg(0x08, 0x70, 2); // Cell temp on TS1
	I2C_ReadReg(0x08, 0x74, 2); // FET temp on TS3
	return ret;
}