/*!\name      main.c
 *
 * \brief	  LED dimmer:
 * 				- potentiometer between +3.3V and GND with 
 * 				- wiper tied to pin 2 of PIC32MX250F128B 
 *
 * \author    Juan Gago
 *
 */

#include "system.h"	/* generic header for PIC32MX family */
#include "adc.h"
#include "delay.h"

//note only valid pins are 2,3,4,5,6,7,24,25,26-> PIC32MX250F128B
//note 4,5 are used for ICSP, 2 is USer LED is J3 is used
int pin =2; //an0 pin 2
int adcvalue = 0;

int main ( void )
{ 
	ANSELA =0; //make sure analog is cleared
	ANSELB =0;
	PPSOutput(1, RPB7, OC1);
	TRISBbits.TRISB7 =0; 	//Set bit 7 to ouput (LED)
		
	OpenTimer2 (T2_ON |T2_SOURCE_INT | T2_PS_1_256 , 0x400);  //set period
	OpenOC1 (OC_ON | OC_TIMER_MODE16| OC_TIMER2_SRC | OC_PWM_FAULT_PIN_DISABLE, 0x400, 0x400);
	SetDCOC1PWM(0x200); 	//inital 50% duty cycle

	initADC(pin);
	while (1) 	/*  endless loop vary pot and capture reading*/
	{
		adcvalue = readADC ();	//force a conversaion
		SetDCOC1PWM(adcvalue);  //set PWM dutycycle to ADC value
		Delayms(1000);
	}
	return 0;
}