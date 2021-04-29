/*
 * bq_commands.h
 *
 *  Created on: 13 ene. 2021
 *      Author: IDJGB0
 */

#ifndef BQ_COMMANDS_H_
#define BQ_COMMANDS_H_

#define BQCMD_Cell_01_Voltage 	0x14
#define BQCMD_Cell_02_Voltage 	0x16
#define BQCMD_Cell_03_Voltage 	0x18
#define BQCMD_Cell_04_Voltage 	0x1A
#define BQCMD_Cell_05_Voltage 	0x1C
#define BQCMD_Cell_06_Voltage 	0x1E
#define BQCMD_Cell_07_Voltage 	0x20
#define BQCMD_Cell_08_Voltage 	0x22
#define BQCMD_Cell_09_Voltage 	0x24
#define BQCMD_Cell_10_Voltage 	0x26
#define BQCMD_Cell_11_Voltage 	0x28
#define BQCMD_Cell_12_Voltage 	0x2A
#define BQCMD_Cell_13_Voltage 	0x2C
#define BQCMD_Cell_14_Voltage 	0x2E
#define BQCMD_Cell_15_Voltage 	0x30
#define BQCMD_Cell_16_Voltage 	0x32
#define BQCMD_Stack_Voltage		0x34
#define BQCMD_Pack_Pin_Voltage 	0x36
#define BQCMD_Load_Pin_Voltage 	0x38
#define BQCMD_CC2_Current 		0x3A
#define BQCMD_TS1_Temperature 	0x70
#define BQCMD_TS2_Temperature 	0x72
#define BQCMD_TS3_Temperature 	0x74
#define BQCMD_HDQ_Temperature 	0x76
#define BQCMD_Int_Temperature 	0x68

/* Exported types ---------------------------------------------------- */

typedef enum {
	NORMAL,
	SLEEP,
	DEEPSLEEP,
	CFGUPDATE,
	NumOfModesBQ76952
} bq76952_mode_t;

typedef enum {
	NONE		= 0x00U,	/*!< State 0		*/
	FULLACCESS	= 0x01U,	/*!< State 0		*/
	UNSEALED 	= 0x02U,	/*!< State 0		*/
	SEALED		= 0x03U,	/*!< State 0		*/
	NumOfSecurityModesBQ76952
} bq76952_securitymode_t;

typedef enum {
	RELAX_MODE 	= 0x00U,	/*!< State 0		*/
	CHARGING 	= 0x01U,	/*!< State 0		*/
	DISCHARGING = 0x02U,	/*!< State 0		*/
	CHARGING_DISCHARGING,
	NumOfStatusTwoTransistorsBQ76952
} bq76952_fetstatus_t;

/* Command-Only Sub-commands (12.3) -----------------------------------------*/

idn_RetVal_t BQCMD_Set_CHGFET(char*);
idn_RetVal_t BQCMD_Set_DSGFET(char*);
idn_RetVal_t BQCMD_Set_PFEnable(char*);

inline idn_RetVal_t BQCMD_Set_Reset (char* log)
{
	idn_RetVal_t ret = IDN_OK;
	ret = BQ76952_Setter_Direct(0x3E, 0x0012);
    BQ76952_I2C_WAIT(15);
	sprintf(log, "Returned to default settings");
	return ret;
}

inline idn_RetVal_t BQCMD_Set_ConfigUpdateMode (idn_Bool_t mode, char* log)
{
	idn_RetVal_t ret = IDN_OK;
	if (mode)
	{
		ret = BQ76952_Setter_Direct(0x3E, 0x0090);
	} else {
		TX_2Byte[0] = 0x92;
		ret = BQ76952_Setter_Direct(0x3E, 0x0092);
	}
	BQ76952_I2C_WAIT(1);
	sprintf(log, "Set CONFIG_UPDATE mode : %d", mode);
	return ret;
}

/* Direct Commands (12.1) ----------------------------------------------------*/
idn_RetVal_t BQCMD_Get_CtrlStatus (ctrl_status_t*, char*);
idn_RetVal_t BQCMD_Get_BatteryStatus (batt_status_hgh_t*, batt_status_low_t*, char*);
idn_RetVal_t BQCMD_Get_FETStatus (fet_status_t*, char*);

/* Sub-commands with data (12.4) --------------------------------------------*/
idn_RetVal_t BQCMD_Get_ManufacturerStatus (uint16_t* status, char* );

inline idn_RetVal_t BQCMD_Get_DeviceNumber (uint16_t* device_number, char* log)
{
  idn_RetVal_t ret = IDN_BUSY;
  uint8_t* buf;
  ret = BQ76952_GetBuffer(buf, 2);
  if (ret == IDN_OK) {
    ret = BQ76952_Get_DirectCommand (0x05, 2);
    BQREG_Process_SB_Protection(WORD2B(buf[1], buf[0]), device_number);
    sprintf(log, "Get Device No. : 0x%04x", *device_number);
    mutex_unlock();
  }
  return ret;
}
#endif /* BQ_COMMANDS_H_ */
