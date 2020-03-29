/*!\name      main.c
 *
 * \brief     An interrupt is triggered every 4 million cycles (8 MHz)
 *            4,000,000 = (PR2+1) * 256
 *
 * \author    Juan Gago
 *
 */

#include "system.h"
#define SYS_FREQ            (8000000L) //8 MHz FRC mode

// Let's compile time pre-processor calculate the PR1 (period)
#define PRESCALE            256
#define T2_TICK             15624

// Timer 2 interrupt handler
void __ISR(_TIMER_2_VECTOR, ipl2) Timer2Handler(void)
{
    PORTToggleBits(IOPORT_B, BIT_5);    // toggle RB5
    IFS0bits.T2IF = 0;                  // clear interrupt flag
}

int main() 
{	
    ANSELA =0; //make sure analog is cleared
    ANSELB =0;
    INTEnableSystemMultiVectoredInt();

    mPORTBClearBits(BIT_5);		    //Clear bits to ensure light is off.
    mPORTBSetPinsDigitalOut(BIT_5);	//Set port as output

    __builtin_disable_interrupts(); // INT step 2: disable interrupts at CPU
    PR2 = T2_TICK;                  // INT step 3: setup period register
    TMR2 = 0;                       //             initialize count to 0
    T2CONbits.TCKPS = 3;            //             set prescaler to 256
    T2CONbits.TGATE = 0;            //             not gated input (the default)
    T2CONbits.TCS = 0;              //             PCBLK input (the default)
    T2CONbits.ON = 1;               //             turn on Timer1
    IPC1bits.T2IP = 2;              // INT step 4: priority
    IPC1bits.T2IS = 0;              //             subpriority
    IFS0bits.T2IF = 0;              // INT step 5: clear interrupt flag
    IEC0bits.T2IE = 1;              // INT step 6: enable interrupt
    __builtin_enable_interrupts();  // INT step 7: enable interrupts at CPUs

    while (1);
    return 0;
}



