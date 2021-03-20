/*
 * bq_commands.h
 *
 *  Created on: 13 ene. 2021
 *      Author: IDJGB0
 */

#ifndef BQ_COMMANDS_H_
#define BQ_COMMANDS_H_

#include <stdio.h>
#include "cmsis_os.h"
#include "string.h"

#include "bq76952.h"
#include "bq_registers.h"

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

typedef struct {
	uint8_t FET_EN;			// Normal FET control is enabled. FET Test mode is disabled (ignore FET Test subcommands)
	uint8_t	FET_INIT_OFF;	// Default host FET control state forces FETs off
	uint8_t	FET_CTRL_EN;	// FETs are controlled by the device
	uint8_t	HOST_FET_EN;	// Host FET control commands are allowed
	uint8_t	SLEEPCHG;		// CHG FET may be enabled in SLEEP mode
	uint8_t SFMODE_SLEEP; 	// Source-follower mode is enabled on the DSG FET driver while SLEEP
} bq76952_fetconfig_t;

/* Subcommand and direct-commands for CasandrAPP ----------------------------*/
idn_RetVal_t BQCMD_Get_SubCommand(uint16_t, uint8_t, void *variable);
idn_RetVal_t BQCMD_Get_DirectCommand(uint8_t, uint8_t, void *variable);

/* Command-Only Sub-commands (12.3) -----------------------------------------*/
idn_RetVal_t BQCMD_Set_Reset (char*);
idn_RetVal_t BQCMD_Set_ConfigUpdateMode (uint8_t mode, char*);
idn_RetVal_t BQCMD_Set_DeepSleep(uint8_t mode, char*);
idn_RetVal_t BQCMD_Set_Shutdown(char*);
idn_RetVal_t BQCMD_Set_Sealed(char*);
idn_RetVal_t BQCMD_Set_SleepEnable(uint8_t enable, char*);

/* Command-Only Sub-commands (12.3) -----------------------------------------*/
idn_RetVal_t BQCMD_Set_CHGFET(char*);
idn_RetVal_t BQCMD_Set_DSGFET(char*);
idn_RetVal_t BQCMD_Set_FETEnable (char*);
idn_RetVal_t BQCMD_Set_PFEnable(char*);
idn_RetVal_t BQCMD_Set_PFReset(char*);
idn_RetVal_t BQCMD_Set_AllFETs(uint8_t value, char*);
idn_RetVal_t BQCMD_Set_DSG_OFF(char*);
idn_RetVal_t BQCMD_Set_CHG_OFF(char*);

/* Command-Only Sub-commands (12.3) -----------------------------------------*/
idn_RetVal_t BQCMD_Set_LDRestart(char*);
idn_RetVal_t BQCMD_Set_ControlLD(uint8_t enable, char*);
idn_RetVal_t BQCMD_Set_ControlALERT(uint8_t value, char*);
idn_RetVal_t BQCMD_Set_ResetIntegratedCharge(char*);

/* Direct Commands (12.1) ----------------------------------------------------*/
idn_RetVal_t BQCMD_Get_CtrlStatus (ctrl_status_t*, char*);
idn_RetVal_t BQCMD_Get_BatteryStatus (batt_status_hgh_t*, batt_status_low_t*, char*);
idn_RetVal_t BQCMD_Get_FETStatus (fet_status_t*, char*);

/* Sub-commands with data (12.4) --------------------------------------------*/
idn_RetVal_t BQCMD_Get_ManufacturerStatus (uint16_t* status, char* );
idn_RetVal_t BQCMD_Get_DeviceNumber (uint16_t* device_number, char*);
idn_RetVal_t BQCMD_Get_IntegratedChargeInteger (uint32_t*, uint32_t*, char*);
idn_RetVal_t BQCMD_Get_IntegratedChargeFraction(uint32_t*, uint32_t*, char*);

/* User defined commands (BCMD2) --------------------------------------------*/
idn_RetVal_t BQCMD2_Get_Mode(bq76952_mode_t* mode);
idn_RetVal_t BQCMD2_Get_SecurityMode(bq76952_securitymode_t* mode);
idn_RetVal_t BQCMD2_Get_FetStatus(bq76952_fetstatus_t* fet);
idn_RetVal_t BQCMD2_Set_EnableBalancing(uint8_t balanceEnabled, char*);
idn_RetVal_t BQCMD2_Set_FETDriver (bq76952_fetconfig_t,  char*);
idn_RetVal_t BQCMD2_Get_InstantaneousCapacity(uint32_t*, char*);

/* Pin Configuration commands --------------------------------------------------------*/
idn_RetVal_t BQCMD2_Enable_ALERT_Pin(char*);		// ALERT interrupt pin for the MCU
idn_RetVal_t BQCMD2_Enable_FETS_Pin(char*);
idn_RetVal_t BQCMD2_Enable_STATUS_Pin(char*);
idn_RetVal_t BQCMD2_Disable_ALERT_Pin(char*);		// ALERT interrupt pin for the MCU
idn_RetVal_t BQCMD2_Disable_FETS_Pin(char*);
idn_RetVal_t BQCMD2_Disable_STATUS_Pin(char*);

/* Processing ALERT pin interrupt ----------------------------------------------------*/
idn_RetVal_t BQCMD2_Alert_SA_SB_SC(void (*callback)(Bq76952_AlarmState_t state));
idn_RetVal_t BQCMD2_Fault_SA(void (*callback)(Bq76952_AlarmState_t state));
idn_RetVal_t BQCMD2_Fault_SB_SC(void (*callback)(Bq76952_AlarmState_t state));
idn_RetVal_t BQCMD2_Alert_PFA_PFB_PFC_PFD (void (*callback)(Bq76952_AlarmState_t state));
idn_RetVal_t BQCMD2_Fault_PFA_PFB_PFC_PFD (void (*callback)(Bq76952_AlarmState_t state));

#endif /* BQ_COMMANDS_H_ */
