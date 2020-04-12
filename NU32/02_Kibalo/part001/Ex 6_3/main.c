/*
 * File:   main.c
 * Author: thk
 * chapter 5 "LCD Demo" excerise
 * Created on February 10, 2012, 6:46 PM
 */
#include "system.h"
#include "adc.h"
#include "lcd.h"
#include "delay.h"

const	char Message1[] ="ADC Demo";
const	char Message2[] ="count  volts";

char count[] = {'0','0','0','0',' ', ' ','0','.','0','V','d','c'};

void DemoADC(void) 
{
  int i=0;

  int adcvalue = readADC();    //select input channel
  binary_to_ascii(adcvalue);
  count[0]=bcd1000;   //adc count
  count[1]=bcd100;
  count[2]=bcdtens;
  count[3]=bcdunits;

  int voltage =0;
  for(i =0; i<adcvalue; i++) {  
    voltage += 3; // 3mV per count
  }
  binary_to_ascii(voltage/10);
  count[6]= bcd100;   //adc voltage
  count[8]= bcdtens;
  lcd_pos_cursor(32);
  lcd_write_array(count, sizeof count);	
  return;
}

int main() 
{

  ANSELA =0; //make sure analog is cleared
  ANSELB =0;
  DDPCON=0x00;

  lcd_initialize();			//LCD initialize
  lcd_clear_display();			//clear display
  initADC(26);          //make I/O pin 26 (AN9)an analog ADC input
  
  lcd_pos_cursor(0);			// position cursor to beginning of first row
  lcd_write_string((char*)Message1);	// output message "ADC Demo"
  lcd_pos_cursor(16);		// position to beginning of second row
  lcd_write_string((char*)Message2); // output message "count"
  
  while(1) { 						//start of continuous loop
    DemoADC();					//demo ADC show ADC Conversion in count and voltage
    Delayms(500);				//delay for 100 msec or so.
  };
  return 0;
}









