/*!\name      adc_max_rate.h
 *
 * \brief     This program reads from a single analog input, AN2, at the maximum speed
 *            that fits the PIC32 Electrical Characteristics and the 80 MHz PBCLK
 *            (Tpb = 12.5 ns).  The input to AN2 is a 5 kHz 25% duty cycle PWM from
 *            OC1.  The results of 1000 analog input reads is sent to the user's 
 *            terminal.  An LED on the NU32 also toggles every 8 million samples.
 *            
 *            RB1/VREF- must be connected to ground and RB0/VREF+ connected to 3.3 V.
 *            
 * \author    Juan Gago
 *
 */

#include "NU32.h"                    // constants, functions for startup and UART
#include "adc.h"
#include "pwm.h"

#define NUM_ISRS    125             // the number of 8-sample ISR results to be printed
#define NUM_SAMPS   (NUM_ISRS*8)    // the number of samples stored 1000
#define LED_TOGGLE  1000000         // toggle the LED every 1M ISRs (8M samples)
                                            
// these variables are static because they are not needed outside this C file
// volatile because they are written to by ISR, read in main
static volatile int storing = 1;   // if 1, currently storing data to print; if 0, done

static volatile unsigned int trace[NUM_SAMPS];   // array of stored analog inputs
static volatile unsigned int isr_time[NUM_ISRS]; // time of ISRs from Timer45

void __ISR(_ADC_VECTOR, IPL6SRS) ADCHandler(void) 
{
  // these variable maintains its value in between function (ISR) calls (static)
  static unsigned int sample_num = 0;  // current analog input sample number
  static unsigned int isr_counter = 0; // the number of times the isr has been called

  if (isr_counter <= NUM_ISRS) {
    isr_time[isr_counter] = TMR4;      // keep track of Timer45 time the ISR is entered
  }

  if (AD1CON2bits.BUFS) {              // 1=ADC filling BUF8-BUFF, 0=filling BUF0-BUF7
    trace[sample_num++] = ADC1BUF0;    // all ADC samples must be read in, even
    trace[sample_num++] = ADC1BUF1;    // if we don't want to store them, so that
    trace[sample_num++] = ADC1BUF2;    // the interrupt can be cleared
    trace[sample_num++] = ADC1BUF3;
    trace[sample_num++] = ADC1BUF4;
    trace[sample_num++] = ADC1BUF5;
    trace[sample_num++] = ADC1BUF6;
    trace[sample_num++] = ADC1BUF7;
  } else {
    trace[sample_num++] = ADC1BUF8;
    trace[sample_num++] = ADC1BUF9;
    trace[sample_num++] = ADC1BUFA;
    trace[sample_num++] = ADC1BUFB;
    trace[sample_num++] = ADC1BUFC;
    trace[sample_num++] = ADC1BUFD;
    trace[sample_num++] = ADC1BUFE;
    trace[sample_num++] = ADC1BUFF;
  }
  if (sample_num >= NUM_SAMPS) {     
    storing = 0;                       // done storing data
    sample_num = 0;                    // reset sample number
  }
  ++isr_counter;                       // increment ISR count
  if (isr_counter == LED_TOGGLE) {     // toggle LED every 1M ISRs (8M samples)
    LATFINV = 0x02;
    isr_counter = 0;                   // reset ISR counter
  }
  IFS1bits.AD1IF = 0;                  // clear interrupt flag
}

int main(void) 
{
  char msg[100] ={};              // buffer for writing messages to uart

  NU32_Startup(); // cache on, min flash wait, interrupts on, LED/button init, UART init

  __builtin_disable_interrupts(); // INT step 2: disable interrupts

  tmr2_setup (1, 16000);          // period * 1 * 12.5 ns = 200 us, 5 kHz
  oc1_duty_cycle (4000);          // turn on OC1, TMR2 (25%)

                                  // set up Timer45 to count every pbclk cycle
  T4CONbits.T32 = 1;              // configure 32-bit mode
  PR4 = 0xFFFFFFFF;               // rollover at the maximum possible period, the default
  T4CONbits.TON = 1;              // turn on Timer45
                                 
                                  // INT step 3: configure ADC generating interrupts
  AD1PCFGbits.PCFG2 = 0;          //        make RB2/AN2 an analog input (the default)
  AD1CHSbits.CH0SA = 2;           //        AN2 is the positive input to the sampler
  adc_init_auto();                //        turn on the ADC
  IPC6bits.AD1IP = 6;             // INT step 4: IPL6, to use shadow register set
  IFS1bits.AD1IF = 0;             // INT step 5: clear ADC interrupt flag
  IEC1bits.AD1IE = 1;             // INT step 6: enable ADC interrupt
  __builtin_enable_interrupts();  // INT step 7: enable interrupts at CPU

  TMR4 = 0;                       // start timer 4 from zero
  while(storing);                 // wait until first NUM_SAMPS samples taken
  IEC1bits.AD1IE = 0;             // disable ADC interrupt

  sprintf(msg,"Values of %d analog reads\r\n",NUM_SAMPS);
  NU32_WriteUART3(msg);
  NU32_WriteUART3("Sample #   Value   Voltage   Time");

  float tot_time = 0.0;           // time between 8 samples
  unsigned int prev_time = 0;     // used for calculating time differences
  int i, j, ind;
  for (i = 0; i < NUM_ISRS; ++i) // write out NUM_SAMPS analog samples 
  {
    for (j = 0; j < 8; ++j) {
      ind = i * 8 + j;            // compute the index of the current sample
      sprintf(msg,"\r\n%5d %10d %9.3f ", ind, trace[ind], trace[ind]*3.3/1024);
      NU32_WriteUART3(msg);
    }
    tot_time = (isr_time[i] - prev_time) * 0.0125; // total time elapsed, in microseconds
    sprintf(msg,"%9.4f us; %d timer counts; %6.4f us/read for last 8 reads",
        tot_time, isr_time[i]-prev_time,tot_time/8.0);
    NU32_WriteUART3(msg);
    prev_time = isr_time[i];
  }

  NU32_WriteUART3("\r\n");
  IEC1bits.AD1IE = 1;         // enable ADC interrupt. won't print the information again,
                              // but you can see the light blinking
  while(1);
  return 0;
}
