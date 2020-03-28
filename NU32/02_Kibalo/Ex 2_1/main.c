/* 
 * File:   main.c
 * Author: thk
 * chapter 2 "hello world' excerise
 * Created on February 4, 2012, 8:15 PM
 */
#include "system.h"
#define SYS_FREQ	(8000000L) //8 Mhz internal fast oscillator
#define iend 		400	//delay loop is iend times jend instruction cycles
#define jend 		1000
#define LED 		LATBbits.LATB5

//delay function
void delay(void) { // 501 msec
	int i,j;
	for (i=0; i<iend; i++)
		for (j=0;j<jend; j++);
}

int main() {
	
	ANSELA =0; //make sure analog is cleared
	ANSELB =0;
	LATBbits.LATB5 =0; //Clear bit 5 to ensure light is off.
	TRISBbits.TRISB5 =0; //Set bit 5 to ouput
	while (1) {
		delay();
		LED = ~LED;
    }
    return 0;
}

