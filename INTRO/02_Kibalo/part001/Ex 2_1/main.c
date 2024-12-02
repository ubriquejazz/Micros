/* 
 * File:   main.c
 * Author: thk
 * chapter 2 "hello world' excerise
 * Created on February 4, 2012, 8:15 PM
 */
#include "system.h"
#include "delay.h"

#define SYS_FREQ	(8000000L) //8 Mhz internal fast oscillator
#define LED 		LATBbits.LATB5

int main() {
	
	ANSELA =0; //make sure analog is cleared
	ANSELB =0;
	LATBbits.LATB5 =0; //Clear bit 5 to ensure light is off.
	TRISBbits.TRISB5 =0; //Set bit 5 to ouput
	while (1) {
		Delayms(10);
		LED = ~LED;
    }
    return 0;
}

