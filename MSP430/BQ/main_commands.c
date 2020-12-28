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

/* ReceiveBuffer: Buffer used to receive data in the ISR
 * RXByteCtr: Number of bytes left to receive
 * ReceiveIndex: The index of the next byte to be received in ReceiveBuffer
 * TransmitBuffer: Buffer used to transmit data in the ISR
 * TXByteCtr: Number of bytes left to transfer
 * TransmitIndex: The index of the next byte to be transmitted in TransmitBuffer
 * */
uint8_t ReceiveBuffer[MAX_BUFFER_SIZE] = {0};
uint8_t RXByteCtr = 0;
uint8_t ReceiveIndex = 0;
uint8_t TransmitBuffer[MAX_BUFFER_SIZE] = {0};
uint8_t TXByteCtr = 0;
uint8_t TransmitIndex = 0;

unsigned char CRC8(unsigned char *ptr, unsigned char len)
{
	unsigned char i;
	unsigned char crc=0;
	while(len--!=0)
	{
		for(i=0x80; i!=0; i/=2)
		{
			if((crc & 0x80) != 0)
			{
				crc *= 2;
				crc ^= 0x107;
			}
			else
				crc *= 2;

			if((*ptr & i)!=0)
				crc ^= 0x107;
		}
		ptr++;
	}
	return(crc);
}

void I2C_WriteReg(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint8_t count)
{
	#if CRC_Mode
	{
		uint8_t crc_count = 0;
		crc_count = count * 2;
		uint8_t crc1stByteBuffer [3] = {0x10, reg_addr, reg_data[0]};
		unsigned int j;
		unsigned int i;
		uint8_t temp_crc_buffer [3];
		
		TX_Buffer[0] = reg_data[0];
		TX_Buffer[1] = CRC8(crc1stByteBuffer,3);

		j = 2;
		for(i=1; i<count; i++)
		{
			TX_Buffer[j] = reg_data[i];
			j = j + 1;
			temp_crc_buffer[0] = reg_data[i];
			TX_Buffer[j] = CRC8(temp_crc_buffer,1);
			j = j + 1;
		}
		I2C_Master_WriteReg(dev_addr, reg_addr, TX_Buffer, crc_count);
	}
	#else
		I2C_Master_WriteReg(dev_addr, reg_addr, reg_data, count);
	#endif
}

void I2C_ReadReg(uint8_t dev_addr, uint8_t reg_addr, uint8_t count)
{
	#if CRC_Mode
	{
		uint8_t crc_count = 0;
		crc_count = count * 2;
		unsigned int j;
		unsigned int i;
		unsigned char CRC = 0;
		uint8_t temp_crc_buffer [3];
		RX_CRC_Check = 0;  // reset to 0
		
		I2C_Master_ReadReg(dev_addr, reg_addr, crc_count);
		uint8_t crc1stByteBuffer [4] = {0x10, reg_addr, 0x11, ReceiveBuffer[0]};
		CRC = CRC8(crc1stByteBuffer,4);
		if (CRC != ReceiveBuffer[1])
			RX_CRC_Check += 1;
		
		RX_Buffer[0] = ReceiveBuffer[0];
		
		j = 2; 
		for (i=1; i<count; i++)
		{
			RX_Buffer[i] = ReceiveBuffer[j];
			temp_crc_buffer[0] = ReceiveBuffer[j];
			j = j + 1;
			CRC = CRC8(temp_crc_buffer,1);
			if (CRC != ReceiveBuffer[j])
				RX_CRC_Check += 1;
			j = j + 1;
		}
		//CopyArray(ReceiveBuffer, RX_Buffer, crc_count);
	}
	#else
		I2C_Master_ReadReg(dev_addr, reg_addr, count);
		CopyArray(ReceiveBuffer, RX_Buffer, count);
	#endif
}

//******************************************************************************
// Main ************************************************************************
// Send and receive three messages containing the example commands *************
//******************************************************************************

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    initClockTo16MHz();
    initGPIO(); 
    initI2C();
    volatile i = 0;
    
    P1OUT = 0x01;   // Turn on red LED
    P6OUT = 0x00;	// Turn off green LED
	
    // Advance to next I2C command by pressing burron on P2.3 (SW2)
    while (P2IN & BIT3)
        wait(1); // wait ~1 second
    
	// ############# Direct Command Examples ###################
	
	// Write Alarm Enable to 0xF082
	TX_2Byte[0] = 0x82; TX_2Byte[1] = 0xF0;
    I2C_WriteReg(0x08, 0x66, TX_2Byte, 2);
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
	TX_2Byte[0] = 0x01; TX_2Byte[1] = 0x00;
    I2C_WriteReg(0x08, 0x3E, TX_2Byte, 2);
    wait(1);
    I2C_ReadReg(0x08, 0x40, 2);
    wait(2);
	while (P2IN & BIT3)
        wait(1); // wait ~1 second
		
	// Read Manufacturing Status
	TX_2Byte[0] = 0x57; TX_2Byte[1] = 0x00;
    I2C_WriteReg(0x08, 0x3E, TX_2Byte, 2);
    wait(1);
    I2C_ReadReg(0x08, 0x40, 2);
    wait(2);
	while (P2IN & BIT3)
        wait(1); // wait ~1 second
		
	// FET_ENABLE
	TX_2Byte[0] = 0x22; TX_2Byte[1] = 0x00;
    I2C_WriteReg(0x08, 0x3E, TX_2Byte, 2);
    wait(2);
	while (P2IN & BIT3)
        wait(1); // wait ~1 second
		
	// RESET - returns device to default settings
	TX_2Byte[0] = 0x12; TX_2Byte[1] = 0x00;
    I2C_WriteReg(0x08, 0x3E, TX_2Byte, 2);
    wait(2);
	while (P2IN & BIT3)
        wait(1); // wait ~1 second
		
	// ############# Reading and Writing to RAM Registers ############
	
	// Read 'Enabled Protections A' RAM register 0x9261
	TX_2Byte[0] = 0x61; TX_2Byte[1] = 0x92;
    I2C_WriteReg(0x08, 0x3E, TX_2Byte, 2);
    wait(1);
    I2C_ReadReg(0x08, 0x40, 1);
    wait(2);
	while (P2IN & BIT3)
        wait(1); // wait ~1 second
		
	// Set CONFIG_UPDATE Mode (RAM registers should be written while in
	// CONFIG_UPDATE mode and will take effect after exiting CONFIG_UPDATE mode
	TX_2Byte[0] = 0x90; TX_2Byte[1] = 0x00;
    I2C_WriteReg(0x08, 0x3E, TX_2Byte, 2);
    wait(2);
	while (P2IN & BIT3)
        wait(1); // wait ~1 second
	
	// Write to 'Enabled Protections A' RAM register to enable CUV protection
	TX_3Byte[0] = 0x61; TX_3Byte[1] = 0x92; TX_3Byte[2] = 0x8C;
    I2C_WriteReg(0x08, 0x3E, TX_3Byte, 3); 
    wait(1);
	TX_2Byte[0] = 0x80; TX_2Byte[1] = 0x05;
    I2C_WriteReg(0x08, 0x60, TX_2Byte, 2);	
    wait(2);
	while (P2IN & BIT3)
        wait(1); // wait ~1 second
	
	// Write to 'VCell Mode' RAM register to configure for a 9-cell battery
	TX_4Byte[0] = 0x04; TX_4Byte[1] = 0x93; TX_4Byte[2] = 0x03; TX_4Byte[3] = 0x7F;
    I2C_WriteReg(0x08, 0x3E, TX_4Byte, 4); 
    wait(1);
	TX_2Byte[0] = 0xE6; TX_2Byte[1] = 0x06;
    I2C_WriteReg(0x08, 0x60, TX_2Byte, 2);	
    wait(2);
	while (P2IN & BIT3)
        wait(1); // wait ~1 second
	
	// Exit CONFIG_UPDATE Mode
	TX_2Byte[0] = 0x92; TX_2Byte[1] = 0x00;
    I2C_WriteReg(0x08, 0x3E, TX_2Byte, 2);        
    wait(2);
	while (P2IN & BIT3)
        wait(1); // wait ~1 second
       
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



