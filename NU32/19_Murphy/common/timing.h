#ifndef __TIMING_H
#define __TIMING_H

// Core Timer CP0
#define MAX_CORE_TIMER      0xFFFFFFFF
#define us_SCALE            (SYS_CLK_FREQ/2000000)
#define ms_SCALE            (SYS_CLK_FREQ/2000)
#define MS100_CORE_TICKS    50 * ms_SCALE

void DelayMs(uint32_t);
void DelayUs(uint32_t);
int Elapsed_Time_100ms (void);
int Elapsed_Time (uint32_t);

#endif // __TIMING_H