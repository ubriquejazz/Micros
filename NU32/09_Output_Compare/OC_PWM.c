#include "NU32.h"          // constants, functions for startup and UART
#include "pwm.h"

int main(void) 
{
  NU32_Startup();       // cache on, interrupts on, LED/button init, UART init
  
  tmr2_setup(4, 2000);	// 2000 * 4 * 12.5 ns = 2000 * 50ns = 100us
  oc1_set_duty(500);	// turn on OC1 and TMR2 (25%)

  _CP0_SET_COUNT(0);       // delay 4 seconds to see the 25% duty cycle on a 'scope
  while(_CP0_GET_COUNT() < 4 * 40000000);
  OC1RS = 1000;            // set duty cycle to 50%

  while(1);
  return 0;
}

