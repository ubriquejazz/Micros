//******************************************************************************
//   MSP430FR2355 example code for BQ769x2 
//
// 	 This program builds on the basic example commands by illustrating a basic system example.
// 	 This example is using the BQ76942 for a 7-cell battery. The ALERT pin of the BQ76942 will
// 	 operate as an interrupt for the MCU to notify that measurements are ready or faults are 
// 	 present. 
//
//	 Revision History:
//   Rev 00 - 2/12/2020 Initial version. 
//   Rev 00 - 2/25/2020 Builds on Basic Command Examples, but sets up basic example code 
//						for a real battery system
//	 Rev 01 - 7/23/2020 Updated for final production devices (register address updates)
//
//   Description: I2C master communicates to BQ769x2 sending and receiving
//   different command types. I2C master will enter LPM0 mode
//   while waiting for the messages to be sent/receiving using I2C interrupt.
//
//   ACLK = NA, MCLK = SMCLK = DCO 16MHz.
//
//                                     /|\ /|\
//                   MSP430FR2355     4.7k |
//                 -----------------    |  4.7k
//            /|\ |             P1.3|---+---|-- I2C Clock (UCB0SCL)
//             |  |                 |       |
//             ---|RST          P1.2|-------+-- I2C Data (UCB0SDA)
//                |                 |
//                |             P1.4|---- BQ76942 Alert Pin (interrupt)
//                |                 |
//          SW2---|P2.3         P6.6|--- Green LED
//                |                 |
//          SW1---|P4.1         P1.0|--- Red LED
//                |                 |
//
//******************************************************************************

#include <msp430.h> 
#include <stdint.h>
#include <stdbool.h>

#include "bq.h"
#include "init.h"

extern uint8_t RX_Buffer [MAX_BUFFER_SIZE];

/* Exported struct ---------------------------------------------------- */

typedef struct {
  uint16_t      COV_Delay;        // Cell OverVoltage Protection
  float			COV_Thresh;       //
  uint16_t      CUV_Delay;        // Cell UnderVoltage Protection
  float	      	CUV_Thresh;       //
  uint16_t      OCC_Delay;        // OverCurrent Charge
  float       	OCC_Thresh;       // 
  uint16_t      OCD_Delay;        // OverCurrent Discharge
  float       	OCD_Thresh;       // 
  uint16_t      SCD_Delay;        // ShortCircuit Discharge
  scd_thresh_t  SCD_Thresh;       //
  uint8_t       OTC_Delay;        // OverTemperature Charge
  int16_t       OTC_Thresh;       // signed 
  uint8_t       OTD_Delay;        // OverTemperature Discharge
  int16_t       OTD_Thresh;       // signed   
} tagBMSInitConfig;

/* Exported struct ---------------------------------------------------- */


int BMS_Init(tagBMSInitConfig init, char* log) 
{
	// COV and CUV
	BQ_Set_CellOverVoltage (init.COV_Thresh, init.COV_Delay, NULL);
	BQ_Set_CellUnderVoltage (init.CUV_Thresh, init.CUV_Delay, NULL);

	// OCC, OCD, SCD_40
	BQ_Set_ChargingOverCurrent (init.OCC_Thresh, init.OCC_Delay, NULL);
	BQ_Set_DischargingOverCurrent (init.OCD_Thresh, init.OCD_Delay, NULL);
	BQ_Set_DischargingShortCircuit (init.SCD_Thresh, init.SCD_Delay, NULL);

	// OTD, OTC
	BQ_Set_ChargingOverTemperature(init.OTC_Thresh, init.OTC_Delay, NULL);
	BQ_Set_DischargingOverTemperature(init.OTD_Thresh, init.OTD_Delay, NULL);

	// Protections
    BQ_Set_EnableProtection(PROTECTION_A, 0xFC, NULL);
    BQ_Set_EnableProtection(PROTECTION_B, 0xF7, NULL);	
	BQ_Set_EnableProtection(PROTECTION_C, 0xFE, NULL);	
	
	sprintf(log, "BMS Configured");
	return 0;
}

int Setup ()
{
	// RESET - returns device to default settings
	BQ_Set_Reset(NULL);
	
	// Enter CONFIG_UPDATE  - Command 0x0090
	BQ_Set_ConfigUpdateMode(1, NULL);
	
	// 'VCell Mode' - Enable 7 cells - 0x9304 = 0x023F
	BQ76952_Set_VCellMode(0x23F, NULL);
	
	// Enable REG0 - 0x9237 = 0x01
	BQ_Set_EnableRegulator(REG0, 0x01, NULL);

	// Enable REG1 = 3.3V - 0x9236 = 0x0D
    BQ_Set_EnableRegulator(REG12, 0x0D, NULL);

	// Enable all protections in 'Enabled Protections A', 'B' and 'C'
    BQ_Set_EnableProtection(PROTECTION_A, 0xFC, NULL);
    BQ_Set_EnableProtection(PROTECTION_B, 0xF7, NULL);	
	BQ_Set_EnableProtection(PROTECTION_C, 0xFE, NULL);	

	// Set TS1 to measure Cell Temperature - 0x92FD = 0x07
	BQ_Set_ThermistorConfig(TS1, 0x07, NULL);

	// Set TS3 to measure FET Temperature - 0x92FF = 0x0F
    BQ_Set_ThermistorConfig(TS3, 0x0F, NULL);
	
	// Set up Alert Pin - 0x92FC = 0x2A
    BQ_Set_OutputPinConifg(ALERT, 0x2A, NULL);

	// Default Alarm Mask 0x926D = 0xF882
	BQ76952_Set_AlarmMask(0xE082, NULL);
	
	// Exit CONFIG_UPDATE Mode - Command 0x0092
	BQ_Set_ConfigUpdateMode(0, NULL);    	
	
	// FET_ENABLE command to turn on CHG and DSG FETs
	BQ_Set_FETEnable(NULL);
	return 0;
}

/**
  * @brief  Loop() called in a while loop. Wait for Alert pin interrupt to indicate 
  *			measurements available or fault present. LED's control 
  *	
  * @retval 	0
**/
int Loop ()
{
    static int i = 0;

    // Alarm Status to figure out if a new set of measurements is ready
	I2C_ReadReg(0x08, 0x62, 2);		
	if (RX_Buffer[0] & 0x82){	// ... go read them
	    i += 1; // counter to flash LEDs to show measurement activity
		BQ_Set_AlarmStatus(0x0082, NULL); // Clear Scan Alert bits
	    BQ_PeriodicMeasurement(NULL);
	}
		
    // Toggle the green LED every 40 measurements
    if (i > 20)
		P6OUT = 0x40;
	else
		P6OUT = 0x00;
	if (i > 40)
        i = 0;

	// Raw Alarm Status to figure out if there is a PF
	I2C_ReadReg(0x08, 0x64, 2);
	if (RX_Buffer[1] & 0xE0) 	// ... light up red LED
	    P1OUT = 0x01;
	else
		P1OUT = 0x00;
	    
	BQ_Set_AlarmStatus(0xF800, NULL);// Try to clear Alert bits
	return 0;
}

//******************************************************************************
// Main ************************************************************************
//******************************************************************************

int main(void) 
{
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    initClockTo16MHz();
    initGPIO(); 
    initI2C();
    
    P1OUT = 0x00;   // Turn off red LED
    P6OUT = 0x00;	// Turn off green LED
 
	// Wait for P2.3 Button to start running program
	while (P2IN & BIT3)
        wait(1); // wait ~1 second	
	
    Setup();
    P1IFG &= ~BIT4; // P1.4 interrupt flag cleared
	
	while(1) {	
		__bis_SR_register(LPM3_bits + GIE);  // Enter Low Power Mode 3 w/ interrupt		
		Loop();
    }
    __bis_SR_register(LPM0_bits + GIE);
	return 0;
}

