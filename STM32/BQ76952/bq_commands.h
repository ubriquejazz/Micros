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
	Bq76952.wr.buf[1] = 0x00
	Bq76952.wr.buf[0] = 0x12
	sprintf(log, "Returned to default settings");
	ret = BQ76952_SetterNoCRC(2);
	return ret;
}

inline idn_RetVal_t BQCMD_Set_ConfigUpdateMode (idn_Bool_t mode, char* log)
{
	idn_RetVal_t ret = IDN_OK;
	Bq76952.wr.buf[1] = 0x00
	Bq76952.wr.buf[0] = (mode) ? 0x90 : 0x92
	sprintf(log, "Set CONFIG_UPDATE mode : %d", mode);
	ret = BQ76952_SetterNoCRC(2);
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
	Bq76952.wr.buf[1] = 0x00
	Bq76952.wr.buf[0] = 0x01
	if(( xSemaphoreTake( Bq76952.wr.mutex, ( TickType_t ) 10 ) == pdTRUE ) &&
	   ( xSemaphoreTake( Bq76952.rd.mutex, ( TickType_t ) 10 ) == pdTRUE ))
	{
		ret = TICOMM_TIComm_ReadFlash (BQ76952_SLAVE_ADDR, NoCRC, 
							Bq76952.wr.buf, 2, Bq76952.rd.buf, 2);
		*device_number = WORD2B( Bq76952.rd.buf[1],  Bq76952.rd.buf[0]);
		sprintf(log, "Get Device No. : 0x%04x", *device_number);
		xSemaphoreGive(  Bq76952.wr.mutex );
		xSemaphoreGive(  Bq76952.rd.mutex );
	}
	else {
	sprintf(log, "Cannot access the shared resource safely");
	}	
	return ret;
}

#endif /* BQ_COMMANDS_H_ */
