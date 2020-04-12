/*!\name      main.c
 *
 * \brief	 40MHz performance exercise
 * 			 You will be applying the 4 ‘C’s of performance optimization,
 * 			 Configuration, Clocks, Cache and Compiler to see how the system’s
 * 			 performance can be greatly improved.
 *
 * \author    Juan Gago
 *
 */
#include "system.h"
#include "db_utils\db_utils.h" // DBPRINTF

void led_disp(int val) {
	mPORTAWrite(val>>17);
}

int main() {
         
    volatile int i=0;
	int previous, temp;
	unsigned long ticks, cpu_ticks, time;

	ANSELA =0; //make sure analog is cleared
	ANSELB =0;
	LATA  = 0x0000;	// set latch levels for PORTB
	TRISA = 0x0000;	// set IO as outputs for PORTB

    #define SYSTEM_CLOCK  4000000 
	
	// Test #1 configure FLASH wait states
    // ***** ADD USER CODE HERE ***** 
    //SYSTEMConfigWaitStates(SYSTEM_CLOCK);

    // Test #2 enable the cache for Kseg0
	// ***** ADD USER CODE HERE ***** 
	//CheKseg0CacheOn();

	/* configure the cache for prefetch and 2 wait-state operation */
	// ***** ADD USER CODE HERE ***** 
	//mCheConfigure(CHE_CONF_WS2 | CHE_CONF_PF_C);

    // Test #3 comment out the functions/macros added above in 1 and 2
    // and replace with one of the SUPER macros
    // ***** ADD USER CODE HERE ***** 

	//Super function to configure critical paramters on system clock
	SYSTEMConfig(SYSTEM_CLOCK, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);
	PORTSetPinsDigitalOut(IOPORT_A,	BIT_0 | BIT_1 | BIT_2);
		
	// XC32 macro to clear Core Timer COUNT register
	_CP0_SET_COUNT(0);

	// create a delay and display on LEDS
	#define DELAY       350000  // DO NOT CHANGE THIS VALUE
	for(temp=0; temp < DELAY; temp++);

	for(temp=0; temp < DELAY; temp++)
		PORTAINV=7;

	for(i=0; i < DELAY; i++)
	{
		if (((i>>17)&7) != previous)
		{
			led_disp(i);
			previous = (i>>17)&7;
		}
	}
     
    // XC32 macro to read Core Timer COUNT register
	ticks = _CP0_GET_COUNT();
    cpu_ticks = ticks * 2;
    time = cpu_ticks/(SYSTEM_CLOCK/1000);

	DBPRINTF("Total CPU Clock cycles=%d\n", cpu_ticks);
	DBPRINTF("Total time = %d milliseconds\n", time);

	while (1);
	return 0;
}

	






