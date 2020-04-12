/*
** SPI2
** 
*/
#include "see.h"

// configuration bit settings, Fcy=72MHz, Fpb=9MHz
#pragma config POSCMOD=XT, FNOSC=PRIPLL 
#pragma config FPLLIDIV=DIV_2, FPLLMUL=MUL_18, FPLLODIV=DIV_1
#pragma config FPBDIV=DIV_8, FWDTEN=OFF, CP=OFF, BWP=OFF

// send one byte of data and receive one back at the same time
int writeSPI2( int i)
{
    SPI2BUF = i;                  // write to buffer for TX
    while( !SPI2STATbits.SPIRBF); // wait for transfer complete
    return SPI2BUF;               // read the received value
}//writeSPI2

main()
{
    int i;

    // 1. init the SPI peripheral 
    TCSEE = 0;                  // make SSEE pin output
    CSEE = 1;                   // de-select the Serial EEPROM
    SPI2CON = SPI_CONF;         // select mode and enable SPI2
    SPI2BRG = SPI_BAUD;         // select clock speed

    // 2. main loop
    while( 1)
    {
    	// 2.1 send a Write Enable command
        CSEE = 0;               // select the Serial EEPROM
        writeSPI2( SEE_WEN);    // write enable command
        CSEE = 1;               // deselect to complete command

        // 2.2 Check the Serial EEPROM status 
        CSEE = 0;               // select the Serial EEPROM
        writeSPI2( SEE_STAT);   // send a READ STATUS COMMAND
        i = writeSPI2( 0);      // send/receive
        CSEE = 1;               // deselect terminate command
    } // main loop 
} // main
