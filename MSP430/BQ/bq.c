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

idn_RetVal_t MillisecondsU2 (uint16_t ms, uint16_t* result) 
{
	idn_RetVal_t ret = IDN_OK;	
	float delay = (float)ms/3.3 - 2.0;
	if(delay < 1.0 || delay > 2047.0) {	
		*result = 74;
		ret = IDN_ERROR;
	}
	else {
		*result = (uint16_t) delay;
	}
	return ret;
}

idn_RetVal_t MillisecondsU1 (uint16_t ms, uint8_t offset, uint8_t* result) 
{
	idn_RetVal_t ret = IDN_OK;	
	float delay = (float)ms/3.3 - 2.0;
	if(delay < 1.0 || delay > 127.0) {	
		*result = offset;
		ret = IDN_ERROR;
	}
	else {
		*result = (uint8_t) delay;
	}
	return ret;
}

/**
  * @brief  Setter_8Bits (0xABCD, 0xFF)
  * @param 	addr 	16 bit address (subcommand)
  *	@param	value 	8 bits
**/
static inline void Setter_8Bits(uint16_t addr, uint8_t value) 
{
	TX_3Byte[0] = LOW_BYTE(addr);
	TX_3Byte[1] = HIGH_BYTE(addr);
	TX_3Byte[2] = value;
	I2C_WriteReg(0x08, 0x3E, TX_3Byte, 3);
	wait(1);
	TX_2Byte[0] = Checksum(TX_3Byte, 3);
	TX_2Byte[1] = 0x05; // Length
	I2C_WriteReg(0x08, 0x60, TX_2Byte, 2);	
}

/**
  * @brief  Setter_2Bytes (0xABCD, 0xFFFF)
  * @param 	addr 	16 bit address (subcommand)
  *	@param	value 	16 bits
**/
static inline void Setter_2Bytes(uint16_t addr, uint16_t value)
{
	TX_4Byte[0] = LOW_BYTE(addr);
	TX_4Byte[1] = HIGH_BYTE(addr);
	TX_4Byte[2] = HIGH_BYTE(value);
	TX_4Byte[3] = LOW_BYTE(value);
    I2C_WriteReg(0x08, 0x3E, TX_4Byte, 4); 
    wait(1);
	TX_2Byte[0] = Checksum(TX_4Byte, 4); 
	TX_2Byte[1] = 0x06;  // Length
    I2C_WriteReg(0x08, 0x60, TX_2Byte, 2);	
}

/**
  * @brief  Setter_Direct (0xAB, 0xFF)
  *	@param	addr 	8 bit address (direct command)
  *	@param	value 	16 bits
**/
static inline void Setter_Direct(uint8_t addr, uint16_t value) 
{
	TX_2Byte[0] = LOW_BYTE(value);
	TX_2Byte[1] = HIGH_BYTE(value);
    I2C_WriteReg(0x08, addr, TX_2Byte, 2);
}

/**
  * @brief  Getter (0xABCD, 1)
  *	@param	addr 	16 bit address (subcommand)
  *	@param	count 	number of bytes to get {1,2}
**/
static inline void Getter(uint16_t addr, uint8_t count) 
{
	TX_2Byte[0] = LOW_BYTE(addr);
	TX_2Byte[1] = HIGH_BYTE(addr);
    I2C_WriteReg(0x08, 0x3E, TX_2Byte, 2);
    wait(1);
    I2C_ReadReg(0x08, 0x40, count);
    wait(2);
}

//******************************************************************************
// Direct Commands 
//******************************************************************************

/**
  * @brief  BQ_Set_AlarmStatus()
  * @param 	status	bit description in Alarm Status Register
  * @param	log*	
**/
idn_RetVal_t BQ_Set_AlarmStatus(uint16_t status, char* log) 
{
	idn_RetVal_t ret = IDN_OK;
	Setter_Direct(0x62, status);
	sprintf(log, "Set Alarm Status to 0x%04x", TX_2Byte[1] * 256 + TX_2Byte[0]);
	return ret;
}

idn_RetVal_t BQ_Get_AlarmStatus (uint16_t* status, char* log) 
{
	idn_RetVal_t ret = IDN_OK;
	I2C_ReadReg(0x62, 2);
	*status = RX_2Byte[1] * 256 + RX_2Byte[0];
	sprintf(log, "Get Alarm Status : 0x%04x", RX_2Byte[1] * 256 + RX_2Byte[0]);
	return ret;
}

/**
  * @brief  BQ_Set_RawAlarmStatus()
  * @param 	raw_status	bit description in Alarm Raw Status Register
  * @param	log*	
**/
idn_RetVal_t BQ_Set_RawAlarmStatus(uint16_t raw_status, char* log) 
{
	idn_RetVal_t ret = IDN_OK;
	Setter_Direct(0x64, raw_status);
	sprintf(log, "Set Alarm Raw Status to 0x%04x", TX_2Byte[1] * 256 + TX_2Byte[0]);
	return ret;
}

idn_RetVal_t BQ_Get_RawAlarmStatus (uint16_t* raw_status, char* log) 
{
	idn_RetVal_t ret = IDN_OK;
	I2C_ReadReg(0x64, 2);
	*raw_status = RX_2Byte[1] * 256 + RX_2Byte[0];
	sprintf(log, "Get Alarm Raw Status : 0x%04x", RX_2Byte[1] * 256 + RX_2Byte[0]);
	return ret;
}

/**
  * @brief  BQ_Set_AlarmEnable(0xF082, NULL)
  * @param 	enable	bit description in Alarm Enable Register (default is 0xF800)
  * @param	log*	
**/
idn_RetVal_t BQ_Set_AlarmEnable (uint16_t enable, char* log) 
{
	idn_RetVal_t ret = IDN_OK;
	Setter_Direct(0x66, enable);
	sprintf(log, "Set Alarm Enable to 0x%04x", TX_2Byte[1] * 256 + TX_2Byte[0]);
	return ret;
}

idn_RetVal_t BQ_Get_AlarmEnable (uint16_t* enable, char* log) 
{
	idn_RetVal_t ret = IDN_OK;
	I2C_ReadReg(0x66, 2);
	*enable = RX_2Byte[1] * 256 + RX_2Byte[0];
	sprintf(log, "Get Alarm Enable : 0x%04x", RX_2Byte[1] * 256 + RX_2Byte[0]);
	return ret;
}

//******************************************************************************
// Write Only (Tx) SubCommands 
//******************************************************************************

/**
  * @brief  BQ_Set_FETEnable()
  *			Toggle FET_EN in Manufacturing Status (see below)
  *			FET_EN = 0 means FET Test Mode. FET_EN = 1 means Firmware FET Control
  * @param	log*	
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

/**
  * @brief  BQ_Set_Reset()
  * @param	log*	
**/
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

/**
  * @brief  BQ_Set_ConfigUpdateMode()
  * @param	mode 	enter or exit {1,0}
  * @param	log*	
**/
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

//******************************************************************************
// Read Only (TxRx) SubCommands 
//******************************************************************************

idn_RetVal_t BQ_Get_ManufacturerStatus (uint16_t* status, char* log) 
{
	idn_RetVal_t ret = IDN_OK;
	Getter(0x0057, 2);
	*status = RX_2Byte[1] * 256 + RX_2Byte[0];
	sprintf(log, "Get Manufacturer Status : 0x%04x", RX_2Byte[1] * 256 + RX_2Byte[0]);
	return ret;
}

idn_RetVal_t BQ_Get_DeviceNumber (uint16_t* device_number, char* log) 
{
	idn_RetVal_t ret = IDN_OK;
	Getter(0x0001, 2);
	*device_number = RX_2Byte[1] * 256 + RX_2Byte[0];
	sprintf(log, "Get Device Number : 0x%04x", RX_2Byte[1] * 256 + RX_2Byte[0]);
	return ret;
}

//******************************************************************************
// 8-Bit Getter (TxRx) SubCommands 
// It is recommended to first enter CONFIG_UPDATE mode
//******************************************************************************

/**
  * @brief  BQ_Get_EnableRegulator()
  * @param	regx 	regulator to enable
  * @param	result*	
  * @param	log*
**/
idn_RetVal_t BQ_Get_EnableRegulator (regulator_t regx, uint8_t* result, char* log) 
{
	idn_RetVal_t ret = IDN_OK;
	uint16_t addr = DATA_MEM_ADDR(0x36, regx);			// settings::configuration
	Getter(addr, 1);
	*result = RX_2Byte[0];
	sprintf(log, "Get Enable Regulator %d : 0x%02x", regx, RX_2Byte[0]);
	return ret;
}

/**
  * @brief 	BQ_Get_EnableProtection()
  * @param	abc 	protection to enable
  * @param	result* to verify that default settings have COV and SCD protectins enabled  
  * @param	log*
**/
idn_RetVal_t BQ_Get_EnableProtection (protection_t abc, uint8_t* result, char* log) 
{
	idn_RetVal_t ret = IDN_OK;
	uint16_t addr = DATA_MEM_ADDR(0x61, abc);			// settings::configuration
	Getter(addr, 1);
	*result = RX_2Byte[0];
	sprintf(log, "Get Enable Protections %d : 0x%02x", abc, RX_2Byte[0]);
	return ret;
}

/**
  * @brief  BQ_Get_ThermistorConfig()
  * @param	abc 	thermistor to configure
  * @param	result*	
  * @param	log*
**/
idn_RetVal_t BQ_Get_ThermistorConfig(thermistor_t tsx, uint8_t* result, char* log) 
{
	idn_RetVal_t ret = IDN_OK;
	uint16_t addr = DATA_MEM_ADDR(0xFD, tsx);			// settings::configuration
	Getter(addr, 1);
	*result = RX_2Byte[0];
	sprintf(log, "Get Enable Thermistor %d : 0x%02x", tsx, RX_2Byte[0]);
	return ret;
}

/**
  * @brief  BQ_Get_OutputPinConifg()
  * @param	abc 	output pin to configure
  * @param	result*	
  * @param	log* 	
**/
idn_RetVal_t BQ_Get_OutputPinConifg(output_pin_t pinx, uint8_t* result, char* log) 
{
	idn_RetVal_t ret = IDN_OK;
	uint16_t addr = DATA_MEM_ADDR(0xFA, pinx);			// settings::configuration
	Getter(addr, 1);
	*result = RX_2Byte[0];
	sprintf(log, "Get Enable Pin %d : 0x%02x", tsx, RX_2Byte[0]);
	return ret;
}

//******************************************************************************
// 8-Bit Setter (Tx) SubCommands 
// It is recommended to first enter CONFIG_UPDATE mode
//******************************************************************************

/**
  * @brief  BQ_Set_EnableRegulator(REG0, 0x01, NULL);
  * 		
  */
idn_RetVal_t BQ_Set_EnableRegulator (regulator_t regx, uint8_t value, char* log) 
{
	idn_RetVal_t ret = IDN_OK;	
	uint16_t addr = DATA_MEM_ADDR(0x36, regx);				// settings::configuration
	Setter_8Bits(addr, value);
	sprintf(log, "Set Regulator %d to 0x%02x", regx, value);
	return ret;
}

/**
  * @brief  BQ_Set_EnableProtection(A, 0x8C, NULL);
  *
**/
idn_RetVal_t BQ_Set_EnableProtection (protection_t abc, uint8_t value, char* log) 
{
	idn_RetVal_t ret = IDN_OK;
	uint16_t addr = DATA_MEM_ADDR(0x61, abc);				// settings::configuration
	Setter_8Bits(addr, value);
	sprintf(log, "Set Enable Protections %d to 0x%02x", abc, value);
	return ret;
}

/**
  * @brief  BQ_Set_ThermistorConfig(TS3, 0x0F, NULL);
  *
**/
idn_RetVal_t BQ_Set_ThermistorConfig (thermistor_t tsx, uint8_t value, char* log) 
{
	idn_RetVal_t ret = IDN_OK;
	uint16_t addr = DATA_MEM_ADDR(0xFD, tsx);				// settings::configuration
	Setter_8Bits(addr, value);
	sprintf(log, "Set Enable Thermistor %d to 0x%02x", tsx, value);
	return ret;
}

/**
  * @brief  BQ_Set_OutputPinConfig (ALERT, 0x2A, NULL)
  * 	
  */
idn_RetVal_t BQ_Set_OutputPinConfig (output_pin_t pinx, uint8_t value, char* log) 
{
	idn_RetVal_t ret = IDN_OK;
	uint16_t addr = DATA_MEM_ADDR(0xFA, pinx);				// settings::configuration
	Setter_8Bits(addr, value);
    sprintf(log, "Set Enable Pin %d to 0x%02x", pinx, value);
	return ret;
}

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
idn_RetVal_t BQ_Set_CellOpenWireTime (uint8_t sec, char* log) 
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

/**
  * @brief  BQ_Set_ChargingOverCurrent (50, 10, NULL)
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

/* OverCurrent ----------------------------------------------------------------*/

/**
  * @brief  BQ_Set_DischargingOverCurrent (70, 10, NULL)
  *			Set user-defined discharging current protection
  * @param	amps, ms
  * @param 	log*		
  */
idn_RetVal_t BQ_Set_DischargingOverCurrent (float amps, uint16_t ms, char* log) 
{
	idn_RetVal_t ret;
	uint8_t dly;
	ret = MillisecondsU1(ms, 1, &dly);

	uint8_t thresh = (uint8_t) (amps/2.0);
	if(thresh < 2 || thresh > 100) {
		thresh = 4;
		ret = IDN_ERROR;
	}
	// (ret == IDN_OK) 
	{
		Setter_8Bits(DATA_MEM_ADDR(0x82, 0), thresh);		// Protections:OCD1:Threshold
		Setter_8Bits(DATA_MEM_ADDR(0x82, 1), dly);			// Protections:OCD1:Delay
		sprintf(log, "[+] OCD1 => 0x%02x, 0x%04x", thresh, dly);
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

//******************************************************************************
// 2-Byte Setter SubCommands 
// It is recommended to first enter CONFIG_UPDATE mode
//******************************************************************************

/**
  * @brief  BQ76952_Set_VCellMode()
  * 		
  */
idn_RetVal_t BQ76952_Set_VCellMode (uint16_t mode, char* log) 
{
	idn_RetVal_t ret = IDN_OK;		
	uint16_t addr = 0x9304;			// settings::configuration
	Setter_2Bytes(addr, mode);
	sprintf(log, "Set Cell Mode Register to 0x%04x", mode);
	return ret;
}

idn_RetVal_t BQ76952_Get_VCellMode (uint16_t* mode, char* log) 
{
	idn_RetVal_t ret = IDN_OK;
	return ret;
}

/**
  * @brief  BQ76952_Set_AlarmMask(0xE082, NULL)
  * 		
  */
idn_RetVal_t BQ76952_Set_AlarmMask (uint16_t mask, char* log) 
{
	idn_RetVal_t ret = IDN_OK;			
    uint16_t addr = 0x926D; 		// settings::configuration
    Setter_2Bytes(addr, mask);
	sprintf(log, "Set Default Alarm Mask to 0x%04x", mask);
	return ret;
}

idn_RetVal_t BQ76952_Get_AlarmMask (uint16_t* mask, char* log) 
{
	idn_RetVal_t ret = IDN_OK;
	return ret;
}

//******************************************************************************
// System Commands 
//******************************************************************************

idn_RetVal_t BQ_PeriodicMeasurement(char* log) 
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
	sprintf(log, "New sample measured");
	return ret;
}