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

unsigned char Checksum(unsigned char *ptr, unsigned char len)
{
	unsigned char i;
	unsigned char checksum = 0;
	
	for(i=0; i<len; i++)
		checksum += ptr[i];
	checksum = 0xff & ~checksum;
	return(checksum);
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

int I2C_ReadReg(uint8_t dev_addr, uint8_t reg_addr, uint8_t count)
{
	unsigned int RX_CRC_Fail = 0;  // reset to 0. If in CRC Mode and CRC fails, this will be incremented.

	#if CRC_Mode
	{
		uint8_t crc_count = 0;
		crc_count = count * 2;
		unsigned int j;
		unsigned int i;
		unsigned char CRC = 0;
		uint8_t temp_crc_buffer [3];
		
		I2C_Master_ReadReg(dev_addr, reg_addr, crc_count);
		uint8_t crc1stByteBuffer [4] = {0x10, reg_addr, 0x11, ReceiveBuffer[0]};
		CRC = CRC8(crc1stByteBuffer,4);
		if (CRC != ReceiveBuffer[1])
			RX_CRC_Fail += 1;
		
		RX_Buffer[0] = ReceiveBuffer[0];
		
		j = 2; 
		for (i=1; i<count; i++)
		{
			RX_Buffer[i] = ReceiveBuffer[j];
			temp_crc_buffer[0] = ReceiveBuffer[j];
			j = j + 1;
			CRC = CRC8(temp_crc_buffer,1);
			if (CRC != ReceiveBuffer[j])
				RX_CRC_Fail += 1;
			j = j + 1;
		}
		//CopyArray(ReceiveBuffer, RX_Buffer, crc_count);
	}
	#else
		I2C_Master_ReadReg(dev_addr, reg_addr, count);
		CopyArray(ReceiveBuffer, RX_Buffer, count);
	#endif
	if (RX_CRC_Fail > 0)
		return -1;
	else
		return 0;
}

//******************************************************************************
// BQ Functions ***************************************************
//******************************************************************************

void BQ_EnableRegulators()
{
	// Enable REG0 - 0x9237 = 0x01
    TX_3Byte[0] = 0x37; TX_3Byte[1] = 0x92; TX_3Byte[2] = 0x01;
    I2C_WriteReg(0x08, 0x3E, TX_3Byte, 3); 
	TX_2Byte[0] = Checksum(TX_3Byte, 3); TX_2Byte[1] = 0x05;  // Checksum and Length
    I2C_WriteReg(0x08, 0x60, TX_2Byte, 2);	

	// Enable REG1 = 3.3V - 0x9236 = 0x0D
    TX_3Byte[0] = 0x36; TX_3Byte[1] = 0x92; TX_3Byte[2] = 0x0D;
    I2C_WriteReg(0x08, 0x3E, TX_3Byte, 3); 
	TX_2Byte[0] = Checksum(TX_3Byte, 3); TX_2Byte[1] = 0x05;  // Checksum and Length
    I2C_WriteReg(0x08, 0x60, TX_2Byte, 2);	
}

void BQ_ConfigureCells()
{
	// 'VCell Mode' - Enable 7 cells - 0x9304 = 0x023F
    TX_4Byte[0] = 0x04; TX_4Byte[1] = 0x93; TX_4Byte[2] = 0x3F; TX_4Byte[3] = 0x02;
    I2C_WriteReg(0x08, 0x3E, TX_4Byte, 4); 
	TX_2Byte[0] = Checksum(TX_4Byte, 4); TX_2Byte[1] = 0x06;  // Checksum and Length
    I2C_WriteReg(0x08, 0x60, TX_2Byte, 2);
}

void BQ_EnableAllProtections(uint8_t a, uint8_t b, uint8_t c)
{
	// Enable all protections in 'Enabled Protections A' 0x9241 = 0xFC
	TX_3Byte[0] = 0x61; TX_3Byte[1] = 0x92; TX_3Byte[2] = a;
    I2C_WriteReg(0x08, 0x3E, TX_3Byte, 3); 
    wait(1);
	TX_2Byte[0] = Checksum(TX_3Byte, 3); TX_2Byte[1] = 0x05;
    I2C_WriteReg(0x08, 0x60, TX_2Byte, 2);	

	// Enable all protections in 'Enabled Protections B' 0x9262 = 0xF7
	TX_3Byte[0] = 0x62; TX_3Byte[1] = 0x92; TX_3Byte[2] = b;
    I2C_WriteReg(0x08, 0x3E, TX_3Byte, 3); 
    wait(1);
	TX_2Byte[0] = Checksum(TX_3Byte, 3); TX_2Byte[1] = 0x05;
    I2C_WriteReg(0x08, 0x60, TX_2Byte, 2);	

	// Enable all protections in 'Enabled Protections C' 0x9263 = 0xFE
	TX_3Byte[0] = 0x63; TX_3Byte[1] = 0x92; TX_3Byte[2] = c;
    I2C_WriteReg(0x08, 0x3E, TX_3Byte, 3); 
    wait(1);
	TX_2Byte[0] = Checksum(TX_3Byte, 3); TX_2Byte[1] = 0x05;
    I2C_WriteReg(0x08, 0x60, TX_2Byte, 2);	
}

void BQ_SetTemperatures()
{
	// Set TS1 to measure Cell Temperature - 0x92FD = 0x07
	TX_3Byte[0] = 0xFD; TX_3Byte[1] = 0x92; TX_3Byte[2] = 0x07;
    I2C_WriteReg(0x08, 0x3E, TX_3Byte, 3); 
    wait(1);
	TX_2Byte[0] = Checksum(TX_3Byte, 3); TX_2Byte[1] = 0x05;
    I2C_WriteReg(0x08, 0x60, TX_2Byte, 2);	
	
	// Set TS3 to measure FET Temperature - 0x92FF = 0x0F
	TX_3Byte[0] = 0xFF; TX_3Byte[1] = 0x92; TX_3Byte[2] = 0x0F;
    I2C_WriteReg(0x08, 0x3E, TX_3Byte, 3); 
    wait(1);
	TX_2Byte[0] = Checksum(TX_3Byte, 3); TX_2Byte[1] = 0x05;
    I2C_WriteReg(0x08, 0x60, TX_2Byte, 2);	
}

void BQ_PeriodicMeasurement (void)
{
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
    volatile int i = 0;
    
    P1OUT = 0x00;   // Turn off red LED
    P6OUT = 0x00;	// Turn off green LED

	// Wait for P2.3 Button to start running program
	while (P2IN & BIT3)
        wait(1); // wait ~1 second	
	
	// RESET - returns device to default settings
	TX_2Byte[0] = 0x12; TX_2Byte[1] = 0x00;
    I2C_WriteReg(0x08, 0x3E, TX_2Byte, 2);
    wait(2);
	
	// Enter CONFIG_UPDATE  - Command 0x0090
	TX_2Byte[0] = 0x90; TX_2Byte[1] = 0x00;
    I2C_WriteReg(0x08, 0x3E, TX_2Byte, 2);
	
	// Note: Block writing can be used to improve efficiency. In this example, write one 
	
	// Enable regulators, protections, temperatures
	BQ_EnableRegulators();
	BQ_ConfigureCells();
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
	TX_2Byte[0] = 0x92; TX_2Byte[1] = 0x00;
    I2C_WriteReg(0x08, 0x3E, TX_2Byte, 2);     	
	
	// FET_ENABLE command to turn on CHG and DSG FETs
	TX_2Byte[0] = 0x22; TX_2Byte[1] = 0x00;
    I2C_WriteReg(0x08, 0x3E, TX_2Byte, 2);	
    
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

