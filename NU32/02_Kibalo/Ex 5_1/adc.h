#ifndef __ADCDRV_H__
#define __ADCDRV_H__

extern char bcd10000;
extern char bcd1000, bcd100;
extern char bcdtens, bcdunits;

int readADC(void);   //parameter I/O expansion pin number this function does conversion
int initADC(int);	 //initialize ADC operaion for pin

void binary_to_ascii(int n);
int average_value (void);

#endif