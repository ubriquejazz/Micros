// Timer 1 based delays

#include "delay.h"

void initTMR1() 
{
    T1CONbits.TCKPS = 0;   // set prescaler to 8
    T1CONbits.TGATE = 0;   // not gated input (the default)
    T1CONbits.TCS = 0;     // PCBLK input (the default)
    T1CONbits.ON = 1;      // turn on Timer1 
    // T1CON = 0x8000;     // enable tmr1, Tcy, 1:1
}

//timer1 hardware delays
void Delayms(unsigned int t)
{
    initTMR1();
    while (t--) {
        TMR1 = 0;
        while(TMR1 < REALCLK/COUNT1MS);
    }
}

void Delay100us(unsigned int t)
{
    initTMR1();
    while (t--) {
        TMR1 = 0;
        while(TMR1 < REALCLK/COUNT100US);
    }
} 

void Delay10us(unsigned int t)
{
    initTMR1();
    while (t--) {
        TMR1 = 0;
        while(TMR1 < REALCLK/COUNT10US);
    }
} 
