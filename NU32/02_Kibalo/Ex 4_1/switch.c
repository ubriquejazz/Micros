/**********************************************************************
* External INT0 on Switch to toggle single light
* *modified by THK for chapter3.5 execerises 11/21/2012
*********************************************************************/
#include "system.h"

#define LED1    LATBbits.LATB5
#define LED2    LATBbits.LATB8
#define SW      PORTBbits.RB7    //INT0 pin 16
#define iend    25	//delay loop is iend times jend instruction cycles
#define jend    1000

void __ISR(_EXTERNAL_0_VECTOR , ipl2) INT0_Handler(void)
{
    PORTToggleBits(IOPORT_B, LED1);
    IFS0bits.INT0IF = 0;                // clear interrupt flag IFS0<3>
}

void delay(void) { // 501 msec
    int i,j;
    for (i=0; i<iend; i++)
        for (j=0;j<jend; j++);
}

//***************************************
// Main
//***************************************
int main ( void )
{
    ANSELA =0; //make sure analog is cleared
    ANSELB =0; //set all pins that share analog and make them digital

    //  enable multi-vector interrupts
    INTEnableSystemMultiVectoredInt();

    LATB = 0;
    TRISB = 0;          // set all PORTB for output 
    TRISBbits.TRISB7=1; // enable for switch input
    CNPUBbits.CNPUB7=1; //change notice pull=up

    __builtin_disable_interrupts(); // step 2: disable interrupts
    INTCONbits.INT0EP = 0;          // step 3: INT0 triggers on falling edge
    IPC0bits.INT0IP = 2;            // step 4: interrupt priority 2
    IPC0bits.INT0IS = 1;            // step 4: interrupt priority 1
    IFS0bits.INT0IF = 0;            // step 5: clear the int flag
    IEC0bits.INT0IE = 1;            // step 6: enable INT0 by setting IEC0<3>
    __builtin_enable_interrupts();  // step 7: enable interrupts
 
    while (1) {
        PORTToggleBits(IOPORT_B, LED2);
        delay();
    } 
    return 0;   
}