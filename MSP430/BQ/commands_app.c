//******************************************************************************
//   MSP430FR2355 example code for BQ769x2 
//
// 	 This main program goes through the command examples illustrated in SLUAA11
// 	 to show examples for the different BQ769x2 command types. The program pauses 
// 	 after each command and waits for the user to press the P2.3 button to execute
// 	 the next command. This allows for easy bus capture on a logic analyzer.
//   
//   Revision History:
//   Rev 00 - 2/12/2020 Initial version. Cycles through examples from SLUAA11 app note
//   Rev 01 - 2/18/2020 Added CRC_Mode. Now I2C and I2C+CRC mode can be selected by changing CRC_Mode
//						Checks CRC from BQ769x2 on I2C Reads to make sure it is valid.
//	 Rev 02 - 7/23/2020 Updated for final production version of the device (register address and checksum updates)
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
//                |                 |
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
unsigned int RX_CRC_Check = 0;

//******************************************************************************
// Main ************************************************************************
//******************************************************************************

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    initClockTo16MHz();
    initGPIO(); 
    initI2C();
    volatile uint16_t i = 0;
    
    P1OUT = 0x01;   // Turn on red LED
    P6OUT = 0x00;	// Turn off green LED
	
    // Advance to next I2C command by pressing burron on P2.3 (SW2)
    while (P2IN & BIT3)
        wait(1); // wait ~1 second

	// RESET - returns device to default settings
	BQ_Set_Reset(NULL);
    
	// ############# Direct Command Examples ###################
	
	// Write Alarm Enable to 0xF082
    BQ_Set_AlarmEnable(0xF082, NULL);
    wait(2);
    while (P2IN & BIT3)
        wait(1); // wait ~1 second
    
	// Read Voltage on Cell #1
    I2C_ReadReg(0x08, 0x14, 2);
    wait(2);
	while (P2IN & BIT3)
        wait(1); // wait ~1 second
	
	// Read Internal Temperature
    I2C_ReadReg(0x08, 0x68, 2);
    wait(2);
	while (P2IN & BIT3)
        wait(1); // wait ~1 second
		
	// Read CC2 Current Measurement
    I2C_ReadReg(0x08, 0x3A, 2);
    wait(2);
	while (P2IN & BIT3)
        wait(1); // wait ~1 second
		
	// ############# Subcommand Examples ###################
	
	// Read Device Number
	BQ_Get_DeviceNumber(&i, NULL);
	while (P2IN & BIT3)
        wait(1); // wait ~1 second
		
	// Read Manufacturing Status
	BQ_Get_ManufacturerStatus(&i, NULL);
	while (P2IN & BIT3)
        wait(1); // wait ~1 second
		
	// ############# Reading and Writing to RAM Registers ############
	
	// Read 'Enabled Protections A' RAM register 0x9261
	BQ_Get_EnableProtection(&i, NULL);
	while (P2IN & BIT3)
        wait(1); // wait ~1 second
		
	// Set CONFIG_UPDATE Mode (RAM registers should be written while in
	// CONFIG_UPDATE mode and will take effect after exiting CONFIG_UPDATE mode
	BQ_Set_ConfigUpdateMode(1, NULL);
    wait(2);
	while (P2IN & BIT3)
        wait(1); // wait ~1 second
	
	// Write to 'Enabled Protections A' RAM register to enable CUV protection
    BQ_Set_EnableProtection(PROTECTION_A, 0x8C, NULL);
    wait(2);
	while (P2IN & BIT3)
        wait(1); // wait ~1 second
	
	// Write to 'VCell Mode' RAM register to configure for a 9-cell battery
    BQ76952_Set_VCellMode(0x37F, NULL);
    wait(2);
	while (P2IN & BIT3)
        wait(1); // wait ~1 second
	
	// Exit CONFIG_UPDATE Mode
	BQ_Set_ConfigUpdateMode(0, NULL);        
    wait(2);
	while (P2IN & BIT3)
        wait(1); // wait ~1 second

	// FET_ENABLE
	BQ_Set_FETEnable(NULL);
       
    while(1)
    {
        // LEDs toggle red/green when P2.3 button is pushed
		// This just shows user program is done executing.
        if (P2IN & BIT3)
		{
			P1OUT = 0x00;
			P6OUT = 0x40;
		}
		else
		{
            P1OUT = 0x01;
            P6OUT = 0x00;
		}
    }
    __bis_SR_register(LPM0_bits + GIE);
	return 0;
}



