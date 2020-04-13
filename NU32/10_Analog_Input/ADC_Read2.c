/*!\name      adc_read2.h
 *
 * \brief     Manual sampling and conversion.
 *            Read inputs AN14, AN15 every 500 ms
 *            Log the time that the conversion take (less than 5us)
 *            Tad = 6 * Tpb = 75ns; T = 250 ns (SAMPLE_TIME)
 *            
 * \author    Juan Gago
 *
 */

#include "NU32.h"          // constants, functions for startup and UART
#include "adc.h"

#define DELAY_TICKS       20000000  // 20M core ticks, between messages

int main(void) {
  unsigned int sample14 = 0, sample15 = 0, elapsed = 0;
  char msg[100] = {};

  NU32_Startup();         // cache on, interrupts on, LED/button init, UART init
  AD1PCFGbits.PCFG14 = 0;                 // AN14 is an adc pin
  AD1PCFGbits.PCFG15 = 0;                 // AN15 is an adc pin
  adc_init_manual();                      // turn on A/D converter
  while (1) {
    _CP0_SET_COUNT(0);                    // set the core timer count to zero
    sample14 = adc_sample_convert(14);    // sample and convert pin 14
    sample15 = adc_sample_convert(15);    // sample and convert pin 15
    elapsed = _CP0_GET_COUNT();           // how long it took to do two samples
                                          // send the results over serial
    sprintf(msg, "Time elapsed: %5u ns  AN14: %4u (%5.3f volts)"
                 "  AN15: %4u (%5.3f volts) \r\n",
                 elapsed * CORE_TICK_TIME, 
                 sample14, sample14 * VOLTS_PER_COUNT, 
                 sample15, sample15 * VOLTS_PER_COUNT);
    NU32_WriteUART3(msg);
    _CP0_SET_COUNT(0);                    
    while(_CP0_GET_COUNT() < DELAY_TICKS);  // delay to prevent a flood of messages
  }
  return 0;
}
