/*!\name      accel.h
 *
 * \brief     Interface with an LSM303D accelerometer/compass using spi
 *            Wire GND to GND, VDD to 3.3V, Vin is disconnected (on Pololu breakout board)
 *               SDI4 (F4) -> SDO     SDO4 (F5) -> SDI (labeled SDA on Pololu board),        
 *               SCK4 (B14) -> SCL    RB8 -> CS *
 *                    
 * \author    Juan Gago
 */

#include "accel.h"
#include "NU32.h"

#define CS LATBbits.LATB8        // use RB8 as CS

void spi_master_setup() {
  int ie = __builtin_disable_interrupts();
  SPI4CON = 0;              // turn off the SPI module and reset it
  SPI4BUF;                  // clear the rx buffer by reading from it
  SPI4BRG = 0x3;            // baud rate to 10MHz [SPI4BRG = (80000000/(2*desired))-1]
  SPI4STATbits.SPIROV = 0;  // clear the overflow bit
  SPI4CONbits.CKE = 1;      // data changes when clock goes from active to inactive 
                            //    (high to low since CKP is 0)
  SPI4CONbits.MSTEN = 1;    // master operation
  SPI4CONbits.ON = 1;       // turn on SPI 4

  if(ie & 1) {
    __builtin_enable_interrupts();
  }
}

// send a byte via spi and return the response
unsigned char spi_io(unsigned char o) {
  SPI4BUF = o;
  while(!SPI4STATbits.SPIRBF); // wait to receive the byte
  return SPI4BUF;
}

// read data from the accelerometer, given the starting register address.
// return the data in data
void acc_read_register(unsigned char reg, unsigned char data[], unsigned int len)
{
  unsigned int i;
  reg |= 0x80; // set the read bit (as per the accelerometer's protocol)
  if(len > 1) {
    reg |= 0x40; // set the address auto inc. bit (as per the accelerometer's protocol)
  }
  CS = 0;
  spi_io(reg);
  for(i = 0; i != len; ++i) {
    data[i] = spi_io(0); // read data from spi
  }
  CS = 1;
}

void acc_write_register(unsigned char reg, unsigned char data)
{
  CS = 0;               // bring CS low to activate SPI
  spi_io(reg);
  spi_io(data);
  CS = 1;               // complete the command
}

void acc_setup() {      // setup the accelerometer, using SPI 4
  TRISBbits.TRISB8 = 0;
  CS = 1;

  // Master - SPI4, pins are: SDI4(F4), SDO4(F5), SCK4(B14).  
  // we manually control SS4 as a digital output (B8)
  // since the PIC is just starting, we know that spi is off. We rely on defaults here
  
  spi_master_setup();              // setup SPI4
  acc_write_register(CTRL1, 0xAF); // set accelerometer data rate to 1600 Hz. 
                                   // Don't update until we read values
  acc_write_register(CTRL5, 0xF0); // 50 Hz magnetometer, high resolution, temp sensor on
  acc_write_register(CTRL7, 0x0);  // enable continuous reading of the magnetometer
}
