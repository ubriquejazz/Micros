/**
  ******************************************************************************
  * @file   bq_registers.c
  * @author  IDJGB0
  * @version V1.0.0
  * @date
  * @brief   selection of functions for configuration
  ******************************************************************************
  */

#include "bq76952.h"
#include "bq_registers.h"

idn_RetVal_t BQREG_Process_SA_Protection (uint8_t data, sa_protection_t* prot)
{
	prot->bits.SCD  = READBIT(LOW_BYTE(data), 7);	// Fault/Alert. Short Circuit in Discharge Protection
	prot->bits.OCD2 = READBIT(LOW_BYTE(data), 6);	// Fault/Alert. Overcurrent in Discharge (CC2 3ms)
	prot->bits.OCD1 = READBIT(LOW_BYTE(data), 5);	// Fault/Alert. Overcurrent in Discharge (CC1 250ms)
	prot->bits.OCC  = READBIT(LOW_BYTE(data), 4);	// Fault/Alert. Overcurrent in Charge Protection
	prot->bits.COV  = READBIT(LOW_BYTE(data), 3);	// Fault/Alert. Cell Overvoltage Protection
	prot->bits.CUV  = READBIT(LOW_BYTE(data), 2);	// Fault/Alert. Cell Undervoltage Protection
	return IDN_OK;
}

idn_RetVal_t BQREG_Process_SB_Protection (uint8_t data, sb_protection_t* temp)
{
	temp->bits.OTF 	 = READBIT(LOW_BYTE(data), 7);	// Fault/Alert. FET Overtemperature
	temp->bits.OTINT = READBIT(LOW_BYTE(data), 6);	// Fault/Alert. Internal Die Overtemperature
	temp->bits.OTD	 = READBIT(LOW_BYTE(data), 5);	// Fault/Alert. Overtemperature in Discharge
	temp->bits.OTC 	 = READBIT(LOW_BYTE(data), 4);	// Fault/Alert. Overtemperature in Charge
	temp->bits.UTINT = READBIT(LOW_BYTE(data), 2);	// Fault/Alert. Internal Undertemperature
	temp->bits.UTD 	 = READBIT(LOW_BYTE(data), 1);	// Fault/Alert. Undertemperature in Discharge
	temp->bits.UTC 	 = READBIT(LOW_BYTE(data), 0);	// Fault/Alert. Undertemperature in Charge
	return IDN_OK;
}

idn_RetVal_t BQREG_Process_SC_Protection (uint8_t data, sc_protection_t* prot)
{
	prot->bits.OCD3 = READBIT(LOW_BYTE(data), 7);	// Fault/Alert. Overcurrent in Discharge 3rd Tier Protection
	prot->bits.SCDL = READBIT(LOW_BYTE(data), 6);	// Fault/Alert. Short Circuit in Discharge Latch
	prot->bits.OCDL = READBIT(LOW_BYTE(data), 5);	// Fault/Alert. Overcurrent in Discharge Latch
	prot->bits.COVL = READBIT(LOW_BYTE(data), 4);	// Fault/Alert. Cell Overvoltage Latch
	prot->bits.HWDF = READBIT(LOW_BYTE(data), 1);	// Fault. Host Watchdog Fault
	return IDN_OK;
}

/* Secondary Protection PFs Register ----------------------------- */

idn_RetVal_t BQREG_Process_PFA_Safety (uint8_t data, pfa_safety_t* prot)
{
	prot->bits.CUDEP = READBIT(LOW_BYTE(data), 7);	// Fault/Alert. Copper Deposition	
	prot->bits.SOTF = READBIT(LOW_BYTE(data), 6);	// Fault/Alert. Safety Overtemperature FET
	prot->bits.SOT 	= READBIT(LOW_BYTE(data), 4);	// Fault/Alert. Safety Overtemperature
	prot->bits.SOCD = READBIT(LOW_BYTE(data), 3);	// Fault/Alert. Safety Overcurrent in Discharge 
	prot->bits.SOCC = READBIT(LOW_BYTE(data), 2);	// Fault/Alert. Safety Overcurrent in Charge
	prot->bits.SOV  = READBIT(LOW_BYTE(data), 1);	// Fault/Alert. Safety Cell Overvoltage	
	prot->bits.SUV  = READBIT(LOW_BYTE(data), 0);	// Fault/Alert. Safety Cell Undervoltage P
	return IDN_OK;
}

idn_RetVal_t BQREG_Process_PFB_Safety (uint8_t data, pfb_safety_t* prot)
{
	prot->bits.SCDL	  = READBIT(LOW_BYTE(data), 7);	// Fault-Alert. Short Circuit in Discharge Latch
	prot->bits.VIMA   = READBIT(LOW_BYTE(data), 4);	// Fault/Alert. Voltage Imbalance Active
	prot->bits.VIMR   = READBIT(LOW_BYTE(data), 3);	// Fault/Alert. Voltage Imbalance at Rest
	prot->bits._2LVL  = READBIT(LOW_BYTE(data), 2);	// Fault/Alert. Second Level Protector
	prot->bits.DFETF  = READBIT(LOW_BYTE(data), 1);	// Fault/Alert. Discharge FET
	prot->bits.CFETF  = READBIT(LOW_BYTE(data), 0);	// Fault/Alert. Charge FET
	return IDN_OK;
}

idn_RetVal_t BQREG_Process_PFC_Safety (uint8_t data, pfc_safety_t* prot)
{
	prot->bits.OTPF  = READBIT(LOW_BYTE(data), 0);	// OTP Memory Permanent Fail
	return IDN_OK;
}

idn_RetVal_t BQREG_Process_PFD_Safety (uint8_t data, pfd_safety_t* prot)
{
	prot->bits.TOSF  = READBIT(LOW_BYTE(data), 0);	// Top of Stack vs Cell Sum Permanent Fail
	return IDN_OK;
}

/* Configuration Register ----------------------------- */

idn_RetVal_t BQREG_Process_Ctrl_Status (uint16_t data, ctrl_status_t* l)
{
	l->bits.DEEPSLEEP  = READBIT(LOW_BYTE(data), 2);	// Device is in DEEPSLEEP mode.
	l->bits.LD_TIMEOUT = READBIT(LOW_BYTE(data), 1);	// LD function timed out and was deactivated.
	l->bits.LD_ON	   = READBIT(LOW_BYTE(data), 0);	// LD pullup was active during the previous LD pin measurement
	return IDN_OK;
}

idn_RetVal_t BQREG_Process_Mfg_Status_Init (uint16_t data, mfg_status_init_t* l)
{
	l->bits.OTPW_EN	= READBIT(LOW_BYTE(data), 7);	// Device may program OTP during operation.
	l->bits.PF_EN 	= READBIT(LOW_BYTE(data), 6);	// Permanent Failure checks are enabled.
	l->bits.FET_EN	= READBIT(LOW_BYTE(data), 4);	// Normal FET control is enabled. FET Test mode is disabled (ignore FET Test subcommands)
	return IDN_OK;
}

idn_RetVal_t BQREG_Process_Balance_Config (uint8_t data, balance_config_t* b)
{
	b->bits.CB_NO_CMD	= READBIT(LOW_BYTE(data), 4);	// Host-controlled balancing commands are ignored.
	b->bits.CB_NOSLEEP	= READBIT(LOW_BYTE(data), 3);	// SLEEP is prevented while cell balancing is active
	b->bits.CB_SLEEP	= READBIT(LOW_BYTE(data), 2);	// Cell balancing may be performed while in SLEEP mode
	b->bits.CB_RLX		= READBIT(LOW_BYTE(data), 1);	// Cell balancing is allowed in relax conditions.
	b->bits.CB_CHG		= READBIT(LOW_BYTE(data), 0);	// Cell balancing is allowed while charging
	return IDN_OK;
}

idn_RetVal_t BQREG_Process_Fet_Status (uint8_t data, fet_status_t* f)
{
	f->bits.ALRT_PIN	= READBIT(LOW_BYTE(data), 6);	// The ALERT pin is asserted.
	f->bits.DDSG_PIN	= READBIT(LOW_BYTE(data), 5);	// The DDSG pin is asserted.
	f->bits.DCHG_PIN	= READBIT(LOW_BYTE(data), 4);	// The DCHG pin is asserted.
	f->bits.PDSG_FET	= READBIT(LOW_BYTE(data), 3);	// The PDSG FET is on.
	f->bits.DSG_FET 	= READBIT(LOW_BYTE(data), 2);	// The DSG FET is on.
	f->bits.PCHG_FET	= READBIT(LOW_BYTE(data), 1);	// The PCHG FET is on.
	f->bits.CHG_FET 	= READBIT(LOW_BYTE(data), 0);	// The CHG FET is on.
	return IDN_OK;
}

idn_RetVal_t BQREG_Process_Fet_Options (uint8_t data, fet_options_t* f)
{
	f->bits.FET_INIT_OFF= READBIT(LOW_BYTE(data), 5);	// Default host FET control state forces FETs off
	f->bits.PDSG_EN		= READBIT(LOW_BYTE(data), 4);	// PDSG FET is turned on before DSG
	f->bits.FET_CTRL_EN	= READBIT(LOW_BYTE(data), 3);	// FETs are controlled by the device
	f->bits.HOST_FET_EN	= READBIT(LOW_BYTE(data), 2);	// Host FET control commands are allowed
	f->bits.SLEEPCHG	= READBIT(LOW_BYTE(data), 1);	// CHG FET may be enabled in SLEEP mode
	f->bits.SFET		= READBIT(LOW_BYTE(data), 0);	// Series FET mode: Body diode protection is enabled
	return IDN_OK;
}

idn_RetVal_t BQREG_Process_ChgPump_Control (uint8_t data, chg_pump_ctrl_t* f)
{
	f->bits.SFMODE_SLEEP = READBIT(LOW_BYTE(data), 2);	// Source-follower mode is enabled on the DSG FET driver while SLEEP
	f->bits.LVEN		= READBIT(LOW_BYTE(data), 1);	// Charge pump low overdrive level (5.5 V) is selected
	f->bits.CPEN		= READBIT(LOW_BYTE(data), 0);	// Charge pumps for FET drivers are enabled
	return IDN_OK;
}

/* Battery Status ---------------------------------------------------------- */

idn_RetVal_t BQREG_Process_Batt_Status (uint16_t data, batt_status_hgh_t* h, batt_status_low_t* l)
{
	h->bits.SLEEP    	= READBIT(HIGH_BYTE(data),7);	// Device is in SLEEP mode.
	h->bits.SD_CMD   	= READBIT(HIGH_BYTE(data),5);	// Shutdown due to command or pin is pending.
	h->bits.PF       	= READBIT(HIGH_BYTE(data),4);	// At least one Permanent Fail fault has triggered.
	h->bits.SS       	= READBIT(HIGH_BYTE(data),3);	// At least one enabled safety fault is triggered.
	h->bits.FUSE     	= READBIT(HIGH_BYTE(data),2);	// FUSE pin was asserted by device or secondary protector at last sample
	h->bits.SEC1     	= READBIT(HIGH_BYTE(data),1);	// Security mode
	h->bits.SEC0     	= READBIT(HIGH_BYTE(data),0);	// Security mode
	l->bits.OTPB     	= READBIT(LOW_BYTE(data), 7);	// Writes to OTP are blocked
	l->bits.OTPW   	 	= READBIT(LOW_BYTE(data), 6);	// Writes to OTP are pending.
	l->bits.COW_CHK  	= READBIT(LOW_BYTE(data), 5);	// Device is actively performing a cell open-wire check.
	l->bits.WD       	= READBIT(LOW_BYTE(data), 4);	// Previous reset was caused by the watchdog timer.
	l->bits.POR  	 	= READBIT(LOW_BYTE(data), 3);	// RST has occurred since .. and reconfig of any RAM settings is required.
	l->bits.SLEEP_EN 	= READBIT(LOW_BYTE(data), 2); 	// SLEEP mode is allowed when other SLEEP conditions are met.
	l->bits.PCHG_MODE	= READBIT(LOW_BYTE(data), 1); 	// Device is in PRECHARGE mode.
	l->bits.CFGUPDATE	= READBIT(LOW_BYTE(data), 0);  	// Device is in CONFIG_UPDATE mode.
	return IDN_OK;
}

/* Alarm Status, Enable, Raw ----------------------------------------------- */

idn_RetVal_t BQREG_Process_Alarm (uint16_t data, alarm_hgh_t* h, alarm_low_t* l)
{
	h->bits.SSBC     	= READBIT(HIGH_BYTE(data),7); 	// This bit is set when a bit is set in Safety Status B() or Safety Status C().
	h->bits.SSA      	= READBIT(HIGH_BYTE(data),6);	// This bit is set when a bit is set in Safety Status A().
	h->bits.PF 	     	= READBIT(HIGH_BYTE(data),5);	// This bit is set when an enabled Permanent Fail fault triggers.
	h->bits.MSK_SFALERT	= READBIT(HIGH_BYTE(data),4);	// This bit is set when a safety alert is triggered that is also enabled in ..
	h->bits.MSK_PFALERT	= READBIT(HIGH_BYTE(data),3);	// This bit is set when a PF alert is triggered that is also enabled in
	h->bits.INITSTART	= READBIT(HIGH_BYTE(data),2);	// Initialization started
	h->bits.INITCOMP	= READBIT(HIGH_BYTE(data),1);	// Initialization completed
	l->bits.FULLSCAN	= READBIT(LOW_BYTE(data), 7);	// Full Voltage Scan Complete.
	l->bits.XCHG     	= READBIT(LOW_BYTE(data), 6);	// This bit is set when the CHG FET is off.
	l->bits.XDSG     	= READBIT(LOW_BYTE(data), 5);	// This bit is set when the DSG FET is off.
	l->bits.SHUTV     	= READBIT(LOW_BYTE(data), 4);	// Stack voltage is below Power:Shutdown:Shutdown Stack Voltage.
	l->bits.FUSE     	= READBIT(LOW_BYTE(data), 3);	// FUSE Pin Driven.
	l->bits.CB     		= READBIT(LOW_BYTE(data), 2);	// This bit is set when cell balancing is active.
	l->bits.ADSCAN   	= READBIT(LOW_BYTE(data), 1);	// Voltage ADC Scan Complete (cell voltages)
	l->bits.WAKE		= READBIT(LOW_BYTE(data), 0);	// This bit is set when the device is wakened from SLEEP mode
	return IDN_OK;
}

/* Power Config ---------------------------------------------------------- */

idn_RetVal_t BQREG_Process_PowerConfig (uint16_t data, pwr_cfg_hgh_t* h, pwr_cfg_low_t* l)
{
	h->bits.DPSLP_OT 		= READBIT(HIGH_BYTE(data),5);	// On-chip OT is enabled in DEEPSLEEP, allowing transition to SHUTDOWN
	h->bits.SHUT_TS2 		= READBIT(HIGH_BYTE(data),4);	// SHUTDOWN mode replaced by low-power state waiting for rising edge on LD pin
	h->bits.DPSLP_PD 		= READBIT(HIGH_BYTE(data),3);	// A rising edge on the LD pin exits DEEPSLEEP
	h->bits.DPSLP_LDO		= READBIT(HIGH_BYTE(data),2);	// Leave REG1 and REG2 in present state when entering DEEPSLEEP
	h->bits.DPSLP_LFO 		= READBIT(HIGH_BYTE(data),1);	// Enable the Low Frequency Oscillator in DEEPSLEEP mode
	h->bits.SLEEP			= READBIT(HIGH_BYTE(data),0);	// Enable SLEEP mode by default
	l->bits.OTSD 			= READBIT(LOW_BYTE(data), 7);	// Enter SHUTDOWN mode when on-chip OT condition is detected
	l->bits.FASTADC 		= READBIT(LOW_BYTE(data), 6);	// This bit is set when the DSG FET is off
	l->bits.CB_LOOP_SLOW_1	= READBIT(LOW_BYTE(data), 5);	// Selects ADC scan loop speed while cell balancing is active
	l->bits.CB_LOOP_SLOW_0 	= READBIT(LOW_BYTE(data), 4);	// 0 = Full speed 1 = Half speed 2 = Quarter 3 = Eighth
	l->bits.LOOP_SLOW_1 	= READBIT(LOW_BYTE(data), 3);	// Selects normal ADC scan loop speed
	l->bits.LOOP_SLOW_0 	= READBIT(LOW_BYTE(data), 2);	// 0 = Full speed 1 = Half speed 2 = Quarter 3 = Eighth
	return IDN_OK;
}
