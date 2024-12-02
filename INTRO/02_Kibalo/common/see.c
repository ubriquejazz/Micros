/*!\name      see.c
 *
 * \brief     Encapsulates 25LC256 Serial EEPROM 
 *            as a NVM storage device for PIC32MX250F128B
 *
 * \author    Juan Gago
 *
 */

#include "see.h"

// init the SPI2 peripheral 
void see_init( void) {
    CSEE = 1;                   // de-select the Serial EEPROM
    TCSEE = 0;                  // make SSEE pin output
    SPI2CON = SPI_CONF;         // enable the peripheral
    SPI2BRG = SPI_BAUD;         // select clock speed
}

// send one byte of data and receive one back at the same time
int writeSPI2(int i) {
    SPI2BUF = i;                  // write to buffer for TX
    while( !SPI2STATbits.SPIRBF); // wait for transfer complete
    return SPI2BUF;               // read the received value
}

// Check the Serial EEPROM status register
int see_read_status(void) {
    int i;	
    CSEE = 0;                   // select the Serial EEPROM
    writeSPI2( SEE_STAT);       // send a READ STATUS COMMAND
    i = writeSPI2( 0);          // send/receive
    CSEE = 1;                   // deselect terminate command
    return i;
} 

// 32-bit integer read and write functions
// NOTE: address must be an even value between 0x0000 and 0x3ffc
// (see page write restrictions on the device datasheet)
int see_read (int address)
{ 
    int i;
    // wait until any work in progress is completed
    while (see_read_status() & 0x1);// check WIP

    // perform a 16-bit read sequence (two byte sequential read)
    CSEE = 0;                   // select the Serial EEPROM
    writeSPI2( SEE_READ);       // read command
    writeSPI2( address >>8);    // address MSB first
    writeSPI2( address & 0xfc); // address LSB (word aligned)
    i = writeSPI2( 0);          // send dummy, read msb
    i = (i<<8)+ writeSPI2( 0);  // send dummy, read lsb
    i = (i<<8)+ writeSPI2( 0);  // send dummy, read lsb
    i = (i<<8)+ writeSPI2( 0);  // send dummy, read lsb
    CSEE = 1;
    return (i);
}

// send a Write Enable command
void write_enable(void) {
    CSEE = 0;               // select the Serial EEPROM
    writeSPI2( SEE_WEN);    // write enable command
    CSEE = 1;               // deselect to complete the command
}

void see_write( int address, int data)
{ 
    // wait until any work in progress is completed
    while ( readStatus() & 0x1);// check the WIP flag
    write_enable();             // set the Write Enable Latch

    // perform a 32-bit write sequence (4 byte page write)
    CSEE = 0;                   // select the Serial EEPROM
    writeSPI2( SEE_WRITE);      // write command
    writeSPI2( address>>8);     // address MSB first
    writeSPI2( address & 0xfc); // address LSB (word aligned)
    writeSPI2( data >>24);      // send msb
    writeSPI2( data >>16);      // send msb
    writeSPI2( data >>8);       // send msb
    writeSPI2( data);           // send lsb
    CSEE = 1;
}


