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

//******************************************************************************
// Main ************************************************************************
//******************************************************************************

int main(void) 
{
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    initClockTo16MHz();
    initGPIO(); 
    initI2C();
    volatile int i = 0;
    
    P1OUT = 0x00;   // Turn off red LED
    P6OUT = 0x00;	// Turn off green LED

	// Wait for P2.3 Button to start running program
	while (P2IN & BIT3)
        wait(1); // wait ~1 second	
	
	// RESET - returns device to default settings
	BQ_Set_Reset(NULL);
	
	// Enter CONFIG_UPDATE  - Command 0x0090
	BQ_Set_ConfigUpdateMode(1, NULL);
	
	// Note: Block writing can be used to improve efficiency. In this example, write one 
	
	// 'VCell Mode' - Enable 7 cells - 0x9304 = 0x023F
	BQ76952_Set_VCellMode(0x23F, NULL);
	
	// Enable regulators, protections, temperatures
	BQ_EnableRegulators();

	// Enable all protections in 'Enabled Protections A', 'B' and 'C'
    BQ_EnableAllProtections(0xFC, 0xF7, 0xFE);
    BQ_SetTemperatures();
	
	// Set up Alert Pin - 0x92FC = 0x2A
	TX_3Byte[0] = 0xFC; TX_3Byte[1] = 0x92; TX_3Byte[2] = 0x2A;
    I2C_WriteReg(0x08, 0x3E, TX_3Byte, 3); 
    wait(1);
	TX_2Byte[0] = Checksum(TX_3Byte, 3); TX_2Byte[1] = 0x05;
    I2C_WriteReg(0x08, 0x60, TX_2Byte, 2);		
	
	// Default Alarm Mask 0x926D = 0xF882
    TX_4Byte[0] = 0x6D; TX_4Byte[1] = 0x92; TX_4Byte[2] = 0x82; TX_4Byte[3] = 0xE0;
    I2C_WriteReg(0x08, 0x3E, TX_4Byte, 4); 
	TX_2Byte[0] = Checksum(TX_4Byte, 4); TX_2Byte[1] = 0x06;  // Checksum and Length
    I2C_WriteReg(0x08, 0x60, TX_2Byte, 2);	
	
	// Exit CONFIG_UPDATE Mode - Command 0x0092
	BQ_Set_ConfigUpdateMode(0, NULL);    	
	
	// FET_ENABLE command to turn on CHG and DSG FETs
	BQ_Set_FETEnable(NULL);
    
    P1IFG &= ~BIT4; // P1.4 interrupt flag cleared
	
    while(1)
    {	
		__bis_SR_register(LPM3_bits + GIE);  // Enter Low Power Mode 3 w/ interrupt
		// Wait for Alert pin interrupt to indicate measurements available or fault present
		
		// Read Raw Alarm Status to figure out what to do
		I2C_ReadReg(0x08, 0x62, 2);		
		if (RX_Buffer[0] & 0x82) // If a new set of measurements is ready, go read them
		{
		    i += 1; //Counter to flash LEDs to show measurement activity
	    	TX_2Byte[0] = 0x82; TX_2Byte[1] = 0x00;
			I2C_WriteReg(0x08, 0x62, TX_2Byte, 2);  // Clear Scan Alert bits
		    BQ_PeriodicMeasurement();
		}
		
        // Toggle the green LED every 40 measurements
        if (i > 20)
			P6OUT = 0x40;
		else
			P6OUT = 0x00;
		if (i > 40)
            i = 0;

		// Read Raw Alarm Status to figure out what to do
		I2C_ReadReg(0x08, 0x64, 2);
		if (RX_Buffer[1] & 0xE0) // If there is a protection triggered or PF, light up red LED
		    P1OUT = 0x01;
		else
			P1OUT = 0x00;	

	   	TX_2Byte[0] = 0x00; TX_2Byte[1] = 0xF8;
	    I2C_WriteReg(0x08, 0x62, TX_2Byte, 2);  // Try to clear Alert bits
    }
    __bis_SR_register(LPM0_bits + GIE);
	return 0;
}

