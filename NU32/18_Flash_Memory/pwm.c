/*!\name      pwm.h
 *
 * \brief     PIC32 peripheral
 *
 * \author    Juan Gago
 *
 */

#include "pwm.h"

void tmr2_setup(int prescaler, int period)
{
	switch (prescaler)
	{
		case 256:	T2CONbits.TCKPS = 7;     // prescaler 1:256
			break;
		case 64:	T2CONbits.TCKPS = 6;     // prescaler 1:64
			break;
		case 4:		T2CONbits.TCKPS = 2;     // prescaler 1:4 
			break;
		case 2:		T2CONbits.TCKPS = 1;     // prescaler 1:2
			break;
		default:
		case 1: 	T2CONbits.TCKPS = 0;     // prescaler 1:1
			break;
	}
	PR2 = period-1; // (PR2+1) * N * 12.5 ns
	return;
}

void oc1_duty_cycle (int duty_cycle)
{
	OC1CONbits.OCM = 0b110;  // PWM mode without fault pin; other OC1CON bits are defaults
	OC1RS = duty_cycle;      // duty cycle = OC1RS/(PR2+1) = 25%
	OC1R = duty_cycle;       // initialize before turning OC1 on; afterward it is read-only
	T2CONbits.ON = 1;        // turn on Timer2
	OC1CONbits.ON = 1;       // turn on OC1
	return;	
}
