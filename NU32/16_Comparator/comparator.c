/*!\name      comparator.h
 *
 * \brief     Uses comparator 2 to interrupt on a low voltage condition.
 *              The + comparator terminal is connected to C2IN+ (B3).
 *              The - comparator terminal is connected to the internal voltage IVref (1.2 V).
 *            Both NU32 LEDs illuminate if the + input is > 1.2 V; otherwise, off.
 *            The comparator output can be viewed on C2OUT (B9) with a voltmeter or measured 
 *            by ADC.
 *
 * \author    Juan Gago
 *
 */

#include "NU32.h"  // constants, funcs for startup and UART

void __ISR(_COMPARATOR_2_VECTOR, IPL6SOFT) Cmp2ISR(void) 
{ 
  NU32_LED1 = 0;                  // LED1 and LED2 on
  NU32_LED2 = 0;
  _CP0_SET_COUNT(0);
  while(_CP0_GET_COUNT() < 10000000); // delay for 10 M core ticks, 0.25 s

  NU32_LED1 = 1;                  // LED1 and LED2 off
  NU32_LED2 = 1;
  IFS1bits.CMP2IF = 0;            // clear interrupt flag IFS0<3>
}

int main(void) {

  NU32_Startup();        // cache on, interrupts on, LED/button init, UART init

  __builtin_disable_interrupts(); // INT step 2: disable interrupts

                                  // INT step 3: configure ADC generating interrupts
  CM2CONbits.COE = 1;    // comparator output is on the C2OUT pin, so you can measure it 
  CM2CONbits.CCH = 0x3;  // connect - input to IVref; by default + connected to C2IN+
  TRISBbits.TRISB9 = 0;  // configure B9 as an output, which must be set to use C2OUT
  CM2CONbits.ON = 1;

  IPC6bits.CMP2IP = 6;             // INT step 4: IPL6, to use shadow register set
  IFS1bits.CMP2IF = 0;             // INT step 5: clear ADC interrupt flag
  IEC1bits.CMP2IE = 1;             // INT step 6: enable ADC interrupt
  __builtin_enable_interrupts();   // INT step 7: enable interrupts at CPU

  while(1) {
    // test the comparator output
    // if(CMSTATbits.C2OUT) { // if output is high then the input signal > 1.2 V
    //   NU32_LED1 = 0;
    //   NU32_LED2 = 0;
    // } else {
    //   NU32_LED1 = 1;
    //   NU32_LED2 = 1;
    // }
  }
  return 0;
}
