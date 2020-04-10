/*!\name      main.c
 *
 * \brief	  Simple demo program for ADC operation
 * 				- potentiometer between +3.3V and GND with 
 * 				- wiper tied to pin 2 of PIC32MX250F128B 
 *
 * \author    Juan Gago
 *
 */

#include "system.h"	/* generic header for PIC32MX family */
#include "adc.h"
#include "delay.h"

// only valid pins are 2,3,4,5,6,7,24,25,26-> PIC32MX250F128B
// 4,5 are used for ICSP, and 2 is USER LED if J3 is used
int pin = 2; //an0 pin 2
int adcvalue = 0;

int main ( void )
{
	char count[] = {'0','0','0','0', '0'};
	initADC(pin);

	while (1) 	/*  endless loop vary pot and capture reading*/
	{ 
		adcvalue = readADC();		// force a conversion 
		binary_to_ascii(adcvalue); 	// convert binary to ASCII
		
		count[0] = bcd10000; //store ASCII result in array
		count[1] = bcd1000;
		count[2] = bcd100;
		count[3] = bcdtens;
		count[4] = bcdunits;
		delay();	  
	}
	return 0;	
}


