#ifndef __DELAY_H__
#define __DELAY_H__

// Timer 1 based delays

#define SYS_CLK 		(8000000L) //8 Mhz internal fast oscillator
#define PRESCALE 		8
#define REALCLK 		SYS_CLK/PRESCALE

#define COUNT1MS 		1000
#define COUNT100US 		10000
#define COUNT10US 		100000

void Delayms(unsigned int t);
void Delay100us(unsigned int t);
void Delay10us(unsigned int t);

#endif
