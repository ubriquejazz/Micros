#ifndef __SEE_H__
#define __SEE_H__

/*!\name      see.h
 *
 * \brief	  Encapsulates 25LC256 Serial EEPROM 
 * 			  as a NVM storage device for PIC32MX250F128B
 *
 * \author    Juan Gago
 *
 */

// I/O definitions
#define CSEE    	_RB11       // select line for Serial EEPROM
#define TCSEE   	_TRISB11    // tris control for CSEE pin

// peripheral configurations
#define SPI_CONF    0x8120  // SPI on, 8-bit master,CKE=1,CKP=0
#define SPI_BAUD    15      // clock divider Fpb/(2 * (15+1)) 250KHZ

// 25LC256 Serial EEPROM commands
#define SEE_WRSR    1       // write status register	
#define SEE_WRITE   2       // write command
#define SEE_READ    3       // read command
#define SEE_WDI     4       // write disable
#define SEE_STAT    5       // read status register
#define SEE_WEN     6       // write enable

void see_init (void);		// initialize access to memory device
int  see_read_status(void); // Check the Serial EEPROM status register
int  see_read (int);		// read a 32-bit value starting at an even address
void see_write (int, int);	// write a 32-bit value starting at an even address

#endif