/* 
 * File:   main.c
 * Author: thk
 * chapter 2 "hello world' excerise
 * Created on February 4, 2012, 8:15 PM
 */

#include "system.h"		

#define LED     LATBbits.LATB5
#define SWITCH  PORTBbits.RB3
#define iend    25	  //delay loop is iend times jend instruction cycles
#define jend    1000

int binarycount =0; //actual binary count
int debounce =0 ; //debounce counter

void delay(void) { // 501 msec
  int i,j;
  for (i=0; i<iend; i++)
    for (j=0;j<jend; j++); 	
}

int main ( void )
{
  ANSELA =0; //make sure analog is cleared
  ANSELB =0; //set all pins that share analog and make them digital
  LATB = 0; 
  TRISB=0;	            // set all PORTB for output initially
  TRISBbits.TRISB3=1;  
  CNPUBbits.CNPUB3=1; 	// set RB3 as input with pull-up in PORTB

  while (1) 
  {
    if (SWITCH == 0) {   //test if low-switch depressed
      delay();
      debounce++; //increment count after delat
      if (debounce>4) {  //button down for 3 counts?
        binarycount = binarycount+0x7f; //yes, then increment count
        if (binarycount>4096) 
          binarycount =0;
        LATB =binarycount;
        debounce =0;          
      }
    }
  }
  return 0;
}

