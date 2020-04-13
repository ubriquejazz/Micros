#ifndef _ADC_H
#define _ADC_H

#define VOLTS_PER_COUNT   (3.3/1024)
#define CORE_TICK_TIME    25        // nanoseconds between core ticks
#define SAMPLE_TIME       10        // 10 core timer ticks = 250 ns

void adc_init_manual(void);
void adc_init_auto(void);
void adc_init_murphy(void);

unsigned int adc_sample_convert(int);

#endif //ADC_H