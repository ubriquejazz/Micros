/*!\name      DIO_sample.c
 *
 * \brief     RB0, RB1 digital inputs with pull-ups
 *            RF3 digital input with pull-up resist
 *            RB2, RB3 digital inputs
 *            RB4, RB5 buffered digital ouptus
 *            RB6, RB7 open-drain digital outputs
 *            AN8-AN15 analog inputs
 *            
 * CN enabled on RB0 (CN2), RF4(CN17), RF5 (CN18)
 * ISR toggles one LED to indiate that a change has
 * been noticed on pin RB0, RF4 or RF5 
 *
 * \author    Juan Gago
 *
 */

#include "NU32.h"          // constants, funcs for startup and UART

volatile unsigned int oldB = 0, oldF = 0, newB = 0, newF = 0; // save port values

void __ISR(_CHANGE_NOTICE_VECTOR, IPL3SOFT) CNISR(void) 
{ 
  newB = PORTB;           // since pins on port B and F are being monitored 
  newF = PORTF;           // by CN, must read both to allow continued functioning
  // ... do something here with  
  // ... oldB, oldF, newB, newF
  oldB = newB;            // save the current values for future use
  oldF = newF;
  LATBINV = 0xF0;         // toggle buffered RB4, RB5 and open-drain RB6, RB7
  NU32_LED1 = !NU32_LED1; // toggle LED1
  IFS1bits.CNIF = 0;      // clear the interrupt flag
}

int main(void) 
{
  NU32_Startup(); // cache on, min flash wait, interrupts on, LED/button init, UART init

  AD1PCFG = 0x00FF;       // set B8-B15 as analog in, B0-B7 as digital pins
  TRISB = 0xFF0F;         // set B4-B7 as digital outputs, 0-3 as digital inputs
  ODCBSET = 0x00C0;       // set ODCB bits 6 and 7, so RB6, RB7 are open drain outputs
  CNPUEbits.CNPUE2 = 1;   // CN2/RB0 input has internal pull-up 
  CNPUEbits.CNPUE3 = 1;   // CN3/RB1 input has internal pull-up
  CNPUEbits.CNPUE17 = 1;  // CN17/RF4 input has internal pull-up
                          // due to errata internal pull-ups may not result in a logic 1
                          
  oldB = PORTB;           // bits 0-3 are relevant input
  oldF = PORTF;           // pins of port F are inputs, by default
  LATB = 0x0050;          // RB4 is buffered high, RB5 is buffered low,
                          // RB6 is floating open drain (could be pulled to 3.3 V by
                          // external pull-up resistor), RB7 is low

  __builtin_disable_interrupts(); // step 1: disable interrupts
  CNCONbits.ON = 1;               // step 2: configure peripheral: turn on CN
  CNENbits.CNEN2 = 1;             // Use CN2/RB0 as a change notification
  CNENbits.CNEN17 = 1;            // Use CN17/RF4 as a change notification
  CNENbits.CNEN18 = 1;            // Use CN18/RF5 as a change notification
  
  IPC6bits.CNIP = 3;              // step 3: set interrupt priority
  IPC6bits.CNIS = 2;              // step 4: set interrupt subpriority
  IFS1bits.CNIF = 0;              // step 5: clear the interrupt flag
  IEC1bits.CNIE = 1;              // step 6: enable the CN interrupt
  __builtin_enable_interrupts();  // step 7: CPU enabled for mvec interrupts

  while(1);
  return 0;
}
