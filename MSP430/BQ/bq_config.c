/**
  ******************************************************************************
  * @file   bq_config.c
  * @author  IDJGB0
  * @version V1.0.0
  * @date
  * @brief   selection of functions for configuration
  ******************************************************************************
  */

#include "bq_config.h"

//******************************************************************************
// Direct Commands
//******************************************************************************

/**
  * @brief  BQCFG_Get_PrimaryProtection (&prot, NULL)
  *			Table 5-2 Ref. Manual
  *	@param	prot* 	Pimary Protection - Safety Status A Register (SA)
  * @param	log*
**/
idn_RetVal_t BQCFG_Get_PrimaryProtection (sa_protection_t* prot, char* log)
{
	idn_RetVal_t ret = IDN_OK;
	BQ76952_ReadReg(BQ76952_SLAVE_ADDR, 0x03, 1);
	I2C_WAIT(2);
	prot->bits.SC_DCHG  = READBIT(Bq76952.buf[0], BIT_SA_SC_DCHG);		// Short Circuit in Discharge Protection
	prot->bits.OC2_DCHG = READBIT(Bq76952.buf[0], BIT_SA_OC2_DCHG);		// Overcurrent in Discharge 2nd Tier Protection
	prot->bits.OC1_DCHG = READBIT(Bq76952.buf[0], BIT_SA_OC1_DCHG);		// Overcurrent in Discharge 1st Tier Protection
	prot->bits.OC_CHG   = READBIT(Bq76952.buf[0], BIT_SA_OC_CHG);		// Overcurrent in Charge Protection
	prot->bits.CELL_OV  = READBIT(Bq76952.buf[0], BIT_SA_CELL_OV);		// Cell Overvoltage Protection
	prot->bits.CELL_UV  = READBIT(Bq76952.buf[0], BIT_SA_CELL_UV);		// Cell Undervoltage Protection
	sprintf(log, "Get Protection Status (SA) : 0x%02x", Bq76952.buf[0]);
	return ret;
}

/**
  * @brief  BQCFG_Get_TemperatureProtection (&temp, NULL)
  *			Table 5-4 Ref. Manual
  *	@param	temp* 	Temperature Protection - Safety Status B Register (SB)
  * @param	log*
**/
idn_RetVal_t BQCFG_Get_TemperatureProtection (sb_protection_t* temp, char* log)
{
	idn_RetVal_t ret = IDN_OK;
	BQ76952_ReadReg(BQ76952_SLAVE_ADDR, 0x05, 1);
	I2C_WAIT(2);
	temp->bits.OVERTEMP_FET 	 = READBIT(Bq76952.buf[0], BIT_SB_OTF);		// FET Overtemperature
	temp->bits.OVERTEMP_INTERNAL = READBIT(Bq76952.buf[0], BIT_SB_OTINT);	// Internal Die Overtemperature
	temp->bits.OVERTEMP_DCHG	 = READBIT(Bq76952.buf[0], BIT_SB_OTD);	// Overtemperature in Discharge
	temp->bits.OVERTEMP_CHG 	 = READBIT(Bq76952.buf[0], BIT_SB_OTC);	// Overtemperature in Charge
	temp->bits.UNDERTEMP_INTERNAL = READBIT(Bq76952.buf[0], BIT_SB_UTINT);
	temp->bits.UNDERTEMP_DCHG 	 = READBIT(Bq76952.buf[0], BIT_SB_UTD);	// Undertemperature in Discharge
	temp->bits.UNDERTEMP_CHG 	 = READBIT(Bq76952.buf[0], BIT_SB_UTC);	// Undertemperature in Charge
	sprintf(log, "Get Temperature Status (SB) : 0x%02x", Bq76952.buf[0]);
	return ret;
}

/**
  * @brief  BQCFG_Set_AlarmStatus()
  * @param 	status	bit description in Alarm Status Register
  * @param	log*
**/
idn_RetVal_t BQCFG_Set_AlarmStatus(uint16_t status, char* log)
{
	idn_RetVal_t ret = IDN_OK;
	Setter_Direct(0x62, status);
	sprintf(log, "Set Alarm Status to 0x%04x", TX_2Byte[1] * 256 + TX_2Byte[0]);
	return ret;
}

idn_RetVal_t BQCFG_Get_AlarmStatus (uint16_t* status, char* log)
{
	idn_RetVal_t ret = IDN_OK;
	BQ76952_ReadReg(BQ76952_SLAVE_ADDR, 0x62, 2);
	I2C_WAIT(2);
	*status = Bq76952.buf[1] * 256 + Bq76952.buf[0];
	sprintf(log, "Get Alarm Status : 0x%04x", Bq76952.buf[1] * 256 + Bq76952.buf[0]);
	return ret;
}

/**
  * @brief  BQCFG_Set_RawAlarmStatus()
  * @param 	raw_status	bit description in Alarm Raw Status Register
  * @param	log*
**/
idn_RetVal_t BQCFG_Set_RawAlarmStatus(uint16_t raw_status, char* log)
{
	idn_RetVal_t ret = IDN_OK;
	Setter_Direct(0x64, raw_status);
	sprintf(log, "Set Alarm Raw Status to 0x%04x", TX_2Byte[1] * 256 + TX_2Byte[0]);
	return ret;
}

idn_RetVal_t BQCFG_Get_RawAlarmStatus (uint16_t* raw_status, char* log)
{
	idn_RetVal_t ret = IDN_OK;
	BQ76952_ReadReg(BQ76952_SLAVE_ADDR, 0x64, 2);
	I2C_WAIT(2);
	*raw_status = Bq76952.buf[1] * 256 + Bq76952.buf[0];
	sprintf(log, "Get Alarm Raw Status : 0x%04x", Bq76952.buf[1] * 256 + Bq76952.buf[0]);
	return ret;
}

/**
  * @brief  BQCFG_Set_AlarmEnable(0xF082, NULL)
  * @param 	enable	bit description in Alarm Enable Register (default is 0xF800)
  * @param	log*
**/
idn_RetVal_t BQCFG_Set_AlarmEnable (uint16_t enable, char* log)
{
	idn_RetVal_t ret = IDN_OK;
	Setter_Direct(0x66, enable);
	sprintf(log, "Set Alarm Enable to 0x%04x", TX_2Byte[1] * 256 + TX_2Byte[0]);
	return ret;
}

idn_RetVal_t BQCFG_Get_AlarmEnable (uint16_t* enable, char* log)
{
	idn_RetVal_t ret = IDN_OK;
	BQ76952_ReadReg(BQ76952_SLAVE_ADDR, 0x66, 2);
	I2C_WAIT(2);
	*enable = Bq76952.buf[1] * 256 + Bq76952.buf[0];
	sprintf(log, "Get Alarm Enable : 0x%04x", Bq76952.buf[1] * 256 + Bq76952.buf[0]);
	return ret;
}

//******************************************************************************
// 8-Bit Setter (Tx) SubCommands
// It is recommended to first enter CONFIG_UPDATE mode
//******************************************************************************

/**
  * @brief  BQCFG_Set_EnableRegulator(REG0, 0x01, NULL);
  *
  */
idn_RetVal_t BQCFG_Set_EnableRegulator (regulator_t regx, uint8_t value, char* log)
{
	idn_RetVal_t ret = IDN_OK;
	uint16_t addr = DATA_MEM_ADDR(0x36, regx);				// settings::configuration
	Setter_8Bits(addr, value);
	sprintf(log, "Set Regulator %d to 0x%02x", regx, value);
	return ret;
}

/**
  * @brief  BQCFG_Set_EnableProtection(A, 0x8C, NULL);
  *
**/
idn_RetVal_t BQCFG_Set_EnableProtection (type_protection_t abc, uint8_t value, char* log)
{
	idn_RetVal_t ret = IDN_OK;
	uint16_t addr = DATA_MEM_ADDR(0x61, abc);				// settings::configuration
	Setter_8Bits(addr, value);
	sprintf(log, "Set Enable Protections %d to 0x%02x", abc, value);
	return ret;
}

/**
  * @brief  BQCFG_Set_PinConifg (ALERT, 0x2A, NULL)
  *
  */
idn_RetVal_t BQCFG_Set_PinConifg (bq_pin_t pinx, uint8_t value, char* log)
{
	idn_RetVal_t ret = IDN_OK;
	uint16_t addr = DATA_MEM_ADDR(0xFA, pinx);				// settings::configuration
	Setter_8Bits(addr, value);
    sprintf(log, "Set Pin Config %d to 0x%02x", pinx, value);
	return ret;
}

//******************************************************************************
// 8-Bit Getter (TxRx) SubCommands
// It is recommended to first enter CONFIG_UPDATE mode
//******************************************************************************

/**
  * @brief  BQCFG_Get_EnableRegulator()
  * @param	regx 	regulator to enable
  * @param	result*
  * @param	log*
**/
idn_RetVal_t BQCFG_Get_EnableRegulator (regulator_t regx, uint8_t* result, char* log)
{
	idn_RetVal_t ret = IDN_OK;
	uint16_t addr = DATA_MEM_ADDR(0x36, regx);			// settings::configuration
	Getter(addr, 1);
	*result = Bq76952.buf[0];
	sprintf(log, "Get Enable Regulator %d : 0x%02x", regx, Bq76952.buf[0]);
	return ret;
}

/**
  * @brief 	BQCFG_Get_EnableProtection()
  * @param	abc 	protection to enable
  * @param	result* i.e. to verify that settings have COV and SCD protections enabled
  * @param	log*
**/
idn_RetVal_t BQCFG_Get_EnableProtection (type_protection_t abc, uint8_t* result, char* log)
{
	idn_RetVal_t ret = IDN_OK;
	uint16_t addr = DATA_MEM_ADDR(0x61, abc);			// settings::configuration
	Getter(addr, 1);
	*result = Bq76952.buf[0];
	sprintf(log, "Get Enable Protections %d : 0x%02x", abc, Bq76952.buf[0]);
	return ret;
}

/**
  * @brief  BQCFG_Get_PinConifg()
  * @param	pinx 	output pin to configure
  * @param	result*
  * @param	log*
**/
idn_RetVal_t BQCFG_Get_PinConifg(bq_pin_t pinx, uint8_t* result, char* log)
{
	idn_RetVal_t ret = IDN_OK;
	uint16_t addr = DATA_MEM_ADDR(0xFA, pinx);			// settings::configuration
	Getter(addr, 1);
	*result = Bq76952.buf[0];
	sprintf(log, "Get Pin Config %d : 0x%02x", pinx, Bq76952.buf[0]);
	return ret;
}

//******************************************************************************
// Read Only (TxRx) SubCommands
//******************************************************************************

idn_RetVal_t BQCFG_Get_ManufacturerStatus (uint16_t* status, char* log)
{
	idn_RetVal_t ret = IDN_OK;
	Getter(0x0057, 2);
	*status = Bq76952.buf[1] * 256 + Bq76952.buf[0];
	sprintf(log, "Get Manufacturer Status : 0x%04x", Bq76952.buf[1] * 256 + Bq76952.buf[0]);
	return ret;
}

idn_RetVal_t BQCFG_Get_DeviceNumber (uint16_t* device_number, char* log)
{
	idn_RetVal_t ret = IDN_OK;
	Getter(0x0001, 2);
	*device_number = Bq76952.buf[1] * 256 + Bq76952.buf[0];
	sprintf(log, "Get Device Number : 0x%04x", Bq76952.buf[1] * 256 + Bq76952.buf[0]);
	return ret;
}

//******************************************************************************
// Write Only (Tx) SubCommands
//******************************************************************************

/**
  * @brief  BQCFG_Set_FETEnable()
  *			Toggle FET_EN in Manufacturing Status (see below)
  *			FET_EN = 0 means FET Test Mode. FET_EN = 1 means Firmware FET Control
  * @param	log*
**/
idn_RetVal_t BQCFG_Set_FETEnable (char* log)
{
	idn_RetVal_t ret = IDN_OK;
	TX_2Byte[0] = BQ76952_REG_FET_ENABLE;
	TX_2Byte[1] = 0x00;
	BQ76952_WriteReg(BQ76952_SLAVE_ADDR, 0x3E, TX_2Byte, 2);
    I2C_WAIT(1);
	sprintf(log, "Toggle FET_EN in Manufacturing Status");
	return ret;
}

/**
  * @brief  BQCFG_Set_Reset()
  * @param	log*
**/
idn_RetVal_t BQCFG_Set_Reset (char* log)
{
	idn_RetVal_t ret = IDN_OK;
	TX_2Byte[0] = BQ76952_REG_RESET;
	TX_2Byte[1] = 0x00;
	BQ76952_WriteReg(BQ76952_SLAVE_ADDR, 0x3E, TX_2Byte, 2);
    I2C_WAIT(1);
	sprintf(log, "Returned to default settings");
	return ret;
}

/**
  * @brief  BQCFG_Set_ConfigUpdateMode()
  * @param	mode 	enter or exit {1,0}
  * @param	log*
**/
idn_RetVal_t BQCFG_Set_ConfigUpdateMode (uint8_t mode, char* log)
{
	idn_RetVal_t ret = IDN_OK;
	if (mode){
		TX_2Byte[0] = BQ76952_REG_SET_CFGUPDATE;
	} else {
		TX_2Byte[0] = BQ76952_REG_EXIT_CFGUPDATE;
	}
	TX_2Byte[1] = 0x00;
	BQ76952_WriteReg(BQ76952_SLAVE_ADDR, 0x3E, TX_2Byte, 2);
	I2C_WAIT(1);
	sprintf(log, "Set CONFIG_UPDATE mode");
	return ret;
}

//******************************************************************************
// 2-Byte Setter WR-RD Sub-commands
// It is recommended to first enter CONFIG_UPDATE mode
//******************************************************************************

/**
  * @brief  BQCFG_Set_VCellMode()
  *
  */
idn_RetVal_t BQCFG_Set_VCellMode (uint16_t mode, char* log)
{
	idn_RetVal_t ret = IDN_OK;
	uint16_t addr = 0x9304;			// settings::configuration
	Setter_2Bytes(addr, mode);
	sprintf(log, "Set Cell Mode Register to 0x%04x", mode);
	return ret;
}

idn_RetVal_t BQCFG_Get_VCellMode (uint16_t* mode, char* log)
{
	idn_RetVal_t ret = IDN_OK;
	return ret;
}

/**
  * @brief  BQCFG_Set_AlarmMask(0xE082, NULL)
  *
  */
idn_RetVal_t BQCFG_Set_AlarmMask (uint16_t mask, char* log)
{
	idn_RetVal_t ret = IDN_OK;
    uint16_t addr = 0x926D; 		// settings::configuration
    Setter_2Bytes(addr, mask);
	sprintf(log, "Set Default Alarm Mask to 0x%04x", mask);
	return ret;
}

idn_RetVal_t BQCFG_Get_AlarmMask (uint16_t* mask, char* log)
{
	idn_RetVal_t ret = IDN_OK;
	return ret;
}


