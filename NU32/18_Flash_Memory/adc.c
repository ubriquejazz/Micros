/*!\name      adc.h
 *
 * \brief     PIC32 peripheral
 *
 * \author    Juan Gago
 *
 */

#include "adc.h"

static bool AdcSamplesReady;

void adc_init_manual(void) 
{
    AdcSamplesReady = false; 
    AD1CON3bits.ADCS = 2;   	// Tad = 2*(ADCS+1)*Tpb = 2*3*12.5ns = 75ns
    AD1CON1bits.ADON = 1;   	// turn on A/D converter
    return;
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

void adc_init_auto(void)
{
    AdcSamplesReady = false; 
    /* CON3: Clock */
    AD1CON3bits.SAMC = 2;    	// sample for 2 Tad 
    AD1CON3bits.ADCS = 2;    	// Tad = 6*Tpb
    /* CON2:Inputs */
    AD1CON2bits.VCFG = 3;    	// external Vref+ and Vref- for VREFH and VREFL
    AD1CON2bits.SMPI = 7;    	// interrupt after every 8th conversion (8 samples)
    AD1CON2bits.BUFM = 1;    	// adc buffer is two 8-word buffers
    /* CON1: S&C */
    AD1CON1bits.FORM = 0b100;	// unsigned 32 bit integer output
    AD1CON1bits.ASAM = 1;    	// autosampling begins after conversion
    AD1CON1bits.SSRC = 0b111;	// conversion starts when sampling ends
    AD1CON1bits.ON = 1;      	// turn on the ADC	
    return;
}

/**
* Initialises and switches on the on-chip ADC module.
*
* The ADC is setup to scan channels AN2 to AN15 and create an interrupt 
* when all channels have been sampled.
*
* To get a single sample will require acquisition + conversion time
* which is 15 * Tad + 12 * Tad = 5.4us
* To sequentially sample all channels will take 16 * 5.4us = 86.4us
* The intention is to poll every 1s, so 86.4us is more than enough.
*/
void adc_init_murphy(void)
{
    AdcSamplesReady = false; 
    /* CON1: S&C */
    AD1CON1bits.FORM = 0;   // Data output format as 16-bit integer
    AD1CON1bits.SSRC = 7;   // auto conversion
    AD1CON1bits.ASAM = 1; 	// autosampling begins after conversion
    AD1CON1bits.CLRASAM = 1;// after every interrupt ASAM needs to be asserted to initiate the next sampling cycle.
    /* CON2: Inputs */
    AD1CON2bits.VCFG = 3;   // external Vref+ and Vref- for VREFH and VREFL
    AD1CON2bits.SMPI = 0xD; // Interrupt after 0xD samples
    AD1CON2bits.CSCNA = 1;  // Scan inputs selected by CSSL
    /* CON3: Clock */
    AD1CON3bits.SAMC = 0xF; // Acquisition time  15 Tad
    AD1CON3bits.ADCS = 0x7; // Tad should be higher than 83.3ns according to the datasheet
                            // Tad is is set 200ns, no need to rush things        
                            // Tad = Tpb * 2 *(ADCS+1) = (1/80MHz) * 2 * (7 + 1)= 200ns
    	/* Others */
    AD1CHS = 0;				// pins to be sampled (CH0SA) compared to (CH0NA)
    AD1PCFG = 0;            // all Port B pins are configured as analog
    AD1CSSLSET = 0xFFFC;    // all inputs for input scan apart from AN0 & AN1

    AD1CON1bits.ON = 1;     // turn on the ADC	
    return;
}

/** 
* Re-enables ADC sampling. 
* 
* The function checks if the scan operation is complete by checking the status of the
* adcSamplesReady flag. If sampling is comlete it re-enanbles another scanning sequence
* by setting the ASAM bit.
*
* @return TRUE is the ADC scan operation is complete. 
*/
bool adc_poll_murphy (void)
{
    bool success;
    uint32_t loopIndex;

    if (adcSamplesReady == true)
    {
        adcSamplesReady = false;
        AD1CON1bits.ASAM = 1;
        success = true;
    }
    else
    {
        success = false;
    }
    return success;
}


