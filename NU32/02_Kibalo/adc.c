/*  Configure analog pins
 *  (AD1PCFG register doesn't exits on this PIC32 family)
 *   
 *   Set ANSELA or ANSELB, and AD1CHS based upon pin
 *   PIC32MX250F128B pinout ony accepts the following pin for analog:
 *
 *   pin   name        AH1CHS        ANSELB         ANSELA
 *   2     AN0/RA0     0             -              1
 *   3     AN1/RA1     0x10000       -              2
 *   4     AN2/RB0     0x20000       1
 *   5     AN3/RB1     0x30000       2
 *   6     AN4/RB2     0x40000       4
 *   7     AN5/RB3     0x50000       8
 *   24    AN11/RB13   0xb0000       0x2000
 *   25    AN10/RB14   0xa0000       0x4000
 *   26    AN9/RB15    0x90000       0x8000
 *
 * Note: AN9,AN10,AN11 can support analog ADC in channels are supported
 *
 */

#include "adc.h"

char bcd10000;
char bcd1000, bcd100;
char bcdtens, bcdunits;

void bcd_digits(char value) {
  if (value == 0) {
    bcd10000  = 0x00;
    bcd1000   = 0x00;
    bcd100    = 0x00;
    bcdtens   = 0x00;
    bcdunits  = 0x00; 
  }
  else {
    bcd10000  += value;
    bcd1000   += value;
    bcd100    += value;
    bcdtens   += value;
    bcdunits  += value; 
  }
}

void binary_to_ascii (int n) {
  bcd_digits(0);
  while (n>=10000) {
    n = n-10000;
    bcd10000++;
  }
  while (n>=1000) {
    n = n-1000;
    bcd1000++;
  }
  while (n>=100) {
    n = n-100;
    bcd100++;
  }
  while (n>=10) {
    n = n-10;
    bcdtens++;
  }
  bcdunits = n;
  bcd_digits(0x30);
}

//performs running average
int average_value (void) {
  int average =0;
  average = readADC();
  average = average +readADC();
  average = average +readADC();
  average = average +readADC();
  average = average/4;
  return average;
}

// Manual sampling, auto-conversion (SAR)
int initADC(int pin)
{
	AD1CON1bits.ADON = 0; // turn off the ADC
  switch (pin) {
      case (2): //AN0
        TRISAbits.TRISA0=1;
        AD1CHS =0x00000;
        ANSELA =1;
        ANSELB =0;
        break;
      case (3): //AN1
        TRISAbits.TRISA1=1;
        AD1CHS =0x10000;
        ANSELA =2;
        ANSELB =0;
        break;
      case (4): //AN2
        TRISBbits.TRISB0=1;
        AD1CHS =0x20000;
        ANSELA =0;
        ANSELB =1;
        break;
      case (5): //AN3
        TRISBbits.TRISB1=1;
        AD1CHS =0x30000;
        ANSELA =0;
        ANSELB =2;
        break;
      case (6): //AN4
        TRISBbits.TRISB2=1;
        AD1CHS =0x40000;
        ANSELA =0;
        ANSELB =4;
        break;
      case (7): //AN5
        TRISBbits.TRISB3=1;
        AD1CHS =0x50000;
        ANSELA =0;
        ANSELB =8;
        break;
      case (24): //AN11
        TRISBbits.TRISB13=1;
        AD1CHS =0xb0000;
        ANSELA =0;
        ANSELB =0x2000;
        break;
      case (25): //AN10
        TRISBbits.TRISB14=1;
        AD1CHS =0xa0000;
        ANSELA =0;
        ANSELB =0x4000;
        break;
      case (26): //AN9
        TRISBbits.TRISB15=1;
        AD1CHS =0x90000;
        ANSELA =0;
        ANSELB =0x8000;
        break;
      default: //UNKNOWN
        return (0); //return error
  }
  AD1CSSL = 0;                // no scanning required
  AD1CON2bits.VCFG = 0;       // AVss and AVdd are used as Vref+/-
  // AD1CON2 = 0;
  AD1CON1bits.ASAM = 0;       // Sampling begins when SAMP bit is set
  AD1CON1bits.SSRC = 0b111;   // Conversion starts when sampling ends
  // AD1CON1 = 0x00E0;        // Auto convert after end of sampling
  AD1CON3bits.ADRC = 0x1;     // Conversion Clock derived from FRC
  AD1CON3bits.SAMC = 0xF;     // Auto Sample Time = 31*Tad
  AD1CON3bits.ADCS = 0x02;    // ADC clock period is Tad = 2*(ADCS+1)*Tpb > 75ns
  // AD1CON3 = 0x1F02;        // Sample time = 31*Tad, Tad = 125ns 
  AD1CON1bits.ADON = 1;       // turn on the ADC
  return 1;
} 

// sample & convert the value on the given adc pin 
// the pin should be configured as an analog input 
int readADC(void)
{
    //AD1CHS.CH0SA = pin;       // select analog input channel based on pin
    AD1CON1bits.SAMP = 1;       // start sampling, automatic conversion will follow
    while (!AD1CON1bits.DONE);  // wait to complete the conversion
    return ADC1BUF0;            // read the conversion result
} 


