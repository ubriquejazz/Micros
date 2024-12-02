/**********************************************************************
* Change Notice Detection on Switch to toggle single light
*modified by THK for chapter3.5 execerises 11/19/2012
*********************************************************************/
#include "system.h"

#define LED1    LATBbits.LATB5
#define LED2    LATBbits.LATB8
#define SW      PORTBbits.RB7    //INT0 pin 16
#define iend    25  //delay loop is iend times jend instruction cycles
#define jend    1000

volatile unsigned int temp;

void __ISR(_CHANGE_NOTICE_VECTOR, ipl2) ChangeNotice_Handler(void)
{
  if ((CNSTATBbits.CNSTATB3 ==1) && (PORTBbits.RB3==1)) {
    PORTToggleBits(IOPORT_B, LED1); // clear the mismatch condition
    temp = mPORTBRead();
  }
  IFS1bits.CNBIF = 0;      // clear the interrupt flag
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
    TRISB =0; // set all PORTB for output 
    LATB = 0;
    INTEnableSystemMultiVectoredInt();  //  enable multi-vector interrupts

    // Read port(s) to clear mismatch on change notice pins
    temp = mPORTBRead();

    __builtin_disable_interrupts(); // step 1: disable interrupts
    CNCONBbits.ON = 1;              // step 2: configure peripheral: turn on CN
    TRISBbits.TRISB3=1;             // enable for switch input
    CNPUBbits.CNPUB3=1;             // change notice pull=up
    CNENBbits.CNIEB3 =1;            // change notice enable

    IPC8bits.CNIP = 2;              // step 3: set interrupt priority
    IPC8bits.CNIS = 2;              // step 4: set interrupt subpriority
    IFS1bits.CNBIF = 0;             // step 5: clear the interrupt flag
    IEC1bits.CNIE = 1;              // step 6: enable the CN interrupt
    __builtin_enable_interrupts();  // step 7: CPU enabled for mvec interrupts

    while (1) {
        PORTToggleBits(IOPORT_B, LED2);
        delay();
    }  
    return 0;  
}
