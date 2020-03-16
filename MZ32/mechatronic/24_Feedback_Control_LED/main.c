#include "NU32.h"

#define DECIMATION 	10
#define NUMSAMPS	1000
#define PLOTPTS		200
#define EINTMAX		1

#define VOLTS_PER_COUNT (3.3/1024)
#define CORE_TICK_TIME 25    // nanoseconds between core ticks
#define SAMPLE_TIME 10       // 10 core timer ticks = 250 ns

static volatile int Waveform[NUMSAMPS];		// waveform
static volatile int ADCarray[PLOTPTS];		// measured values to plot
static volatile int REFarray[PLOTPTS];		// reference values to plot
static volatile int StoringData = 0;		// currently storing plot data
static volatile float Kp = 0, Ki = 0;		// control gains
static volatile int Eint = 0;				// error sum

int anti_windup (int x) {
	if (x > EINTMAX)
		return EINTMAX;
	else if (x < -EINTMAX)
		return -EINTMAX;
}

float h (float x) {
	if (x > 100.0)
		return 100.0;
	else if (x < 0.0)
		return 0.0;
}

float pi_controller (int error) {
	Eint = anti_windup (Eint + error); // error sum
	float u = Kp * error + Ki * Eint;
	return h(u + 50.0);
}

// sample & convert the value on the given adc pin 
// the pin should be configured as an analog input in AD1PCFG
unsigned int adc_sample_convert(int pin) 
{ 
    unsigned int elapsed = 0, finish_time = 0;
    AD1CHSbits.CH0SA = pin;                // connect chosen pin to MUXA for sampling
    AD1CON1bits.SAMP = 1;                  // start sampling

    elapsed = _CP0_GET_COUNT();
    finish_time = elapsed + SAMPLE_TIME;
    while (_CP0_GET_COUNT() < finish_time); // sample for more than 250 ns

    AD1CON1bits.SAMP = 0;                 // stop sampling and start converting
    while (!AD1CON1bits.DONE);            // wait for the conversion process to finish
    return ADC1BUF0;                      // read the buffer with the result
}

void __ISR(_TIMER_1_VECTOR, IPL5SOFT) Controller(void) {

  	static unsigned int counter = 0; 
	static unsigned int plotind = 0; 
	static unsigned int decctr = 0; 
	static int adcval = 0; 

	// manual sampling and automatic conversion ???
	adcval = adc_sample_convert(15);
	if (StoringData) {					// after DECIMATION control loops
		decctr++;						// store data in global arrays
		if (decctr == DECIMATION) {
			decctr = 0;
			ADCarray[plotind] = adcval;
			REFarray[plotind] = Waveform[counter];
			plotind++;					// increment plot data index
		}
		if (plotind == PLOTPTS) {
			plotind = 0;
			StoringData = 0;
		}
	}

	// PWM output
	int error = Waveform[counter] - adcval;
	float newu = pi_controller(error);
	OC1RS = (unsigned int) ((unew/100) * PR2);
	counter++;							// increment every time ISR is entered
	if (counter == NUMSAMPS) {
		counter = 0;					// roll the counter over
	}

	// insert line to clear interrupt flag
	IFS0bits.T1IF = 0; 
}

// square wave, fill in center value and amplitude
void makeWaveform(int center, int A) {
	int i;
	for (i=0; i<NUMSAMPS; i++) {
		if (i<NUMSAMPS/2) {
			Waveform[i] = center + A; // + amplitude
		}
		else {
			Waveform[i] = center - A; // + amplitude
		}
	}
	return;
}

void hardwareSetup()
{
	/* ADC */
	AD1PCFGbits.PCFG15 = 0;                 // AN15 is an adc pin
	AD1CON3bits.ADCS = 2;                   // Tad = 2*(ADCS+1)*Tpb = 2*3*12.5ns = 75ns
	AD1CON1bits.ADON = 1;                   // turn on A/D converter

	__builtin_disable_interrupts(); // INT step 2: disable interrupts at CPU
	PR1 = 62499;                    // INT step 3: setup period register
	TMR1 = 0;                       //             initialize count to 0
	T1CONbits.TCKPS = 3;            //             set prescaler to 256
	T1CONbits.TGATE = 0;            //             not gated input (the default)
	T1CONbits.TCS = 0;              //             PCBLK input (the default)
	T1CONbits.ON = 1;               //             turn on Timer1
	IPC1bits.T1IP = 5;              // INT step 4: priority
	IPC1bits.T1IS = 0;              //             subpriority
	IFS0bits.T1IF = 0;              // INT step 5: clear interrupt flag
	IEC0bits.T1IE = 1;              // INT step 6: enable interrupt
	__builtin_enable_interrupts();  // INT step 7: enable interrupts at CPU

	/* PWM */
	T2CONbits.TCKPS = 2;     // Timer2 prescaler N=4 (1:4)
	PR2 = 1999;              // period = (PR2+1) * N * 12.5 ns = 100 us, 10 kHz
	TMR2 = 0;                // initial TMR2 count is 0
	OC1CONbits.OCM = 0b110;  // PWM mode without fault pin; other OC1CON bits are defaults
	
	OC1RS = 500;             // duty cycle = OC1RS/(PR2+1) = 25%
	OC1R = 500;              // initialize before turning OC1 on; afterward it is read-only
	T2CONbits.ON = 1;        // turn on Timer2
	OC1CONbits.ON = 1;       // turn on OC1

	_CP0_SET_COUNT(0);       // delay 4 seconds to see the 25% duty cycle on a 'scope
	while(_CP0_GET_COUNT() < 4 * 40000000);
	OC1RS = 1000;            // set duty cycle to 50%
	return;
}

int main(void) 
{
	char message[100];				// message to and from Matlab
	float kptemp = 0, kitemp = 0;	// temporay local gains
	int i = 0;						// plot data loop counter

	NU32_Startup(); // cache on, min flash wait, interrupts on, LED/button init, UART init
	makeWaveform(500, 300);
	hardwareSetup();
	while(1) 
	{
		NU32_ReadUART3(message, sizeof(message));	// wait for a message from Matlab
		sscanf(message, "%f %f", &kptemp, &kitemp);
		__builtin_disable_interrupts();
		Kp = kptemp;								// copy local to globals used by ISR
		Ki = kitemp;								// avoid race conditions
		Eint = 0;
		__builtin_enable_interrupts();

		StoringData = 1;							// message to ISR to start storing data
		while(StoringData);							// wait until ISR says storing is done

		for (i=0; i<PLOTPTS; i++) {
			sprintf(message, "%d %d %d\r\n", PLOTPTS-i, ADCarray[i], REFarray[i]);
			NU32_WriteUART3(message);
		}	// Matlab knows we're done when the first field is '1'
	}
	return 0;
}