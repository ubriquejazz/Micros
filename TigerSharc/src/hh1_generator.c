//*********************************************************
//
//  hh_generator.c 
//  --------------------------------------------
//  Program to collect data from local SDRAM using proc 1, 
//  transfer via link to proc 2
//  This program also gives examples of timing diagnostics and
//  simulated input pacing using interrupts
//
//  LINE_SIZE = 1024 4-byte pixels
//  FRAME_SIZE = 32 lines
//  TOTAL_PIXELS = 32 * 4K = 128K (16MB)
//  
//*********************************************************
#include <def21160.h>
#include <21160.h>
#include <stdio.h>
#include <signal.h>
#include <macros.h>
#include "linkdma.h"
#include "extint0.h"
asm("#include <def21160.h>");

#define INIT_SDRAM           0                     // set flag to init SDRAM with incrementing pattern
#define DIAGNOSTICS          0                     // set flag for diagnostics

#define LINE_SIZE            1024                   // 2 lines data = 1024 2-byte pixels 
#define LINK_PKT             LINE_SIZE/2           	// each link sends 1/4 of line data (packet)
#define FRAME_SIZE           32                		// 32 lines per frame for this test
#define TOTAL_PIXELS         LINE_SIZE*FRAME_SIZE  	// total number of pixels (32-bit words)
#define FILE_SDRAM_ADDR      0x800000              	// addr of beginning of SDRAM
#define FREQ				 1250				 	// Hz line rate
#define SLAVES				 2

#if DIAGNOSTICS
//*********************************************************
// defines for diagnostic metrics
//*********************************************************
typedef struct
{
    volatile int timer_intr;
	volatile int chan10_count;
	volatile int link1_count;
	volatile int link5_count;
	volatile int link2_count;
	volatile int link3_count;
}status_t;

status_t stat[25];
volatile int pkt_cnt = 0;

void DMAC10_complete(int sig)
{
    stat[pkt_cnt].chan10_count = timer_on();
}

void link1_complete(int sig)
{
    stat[pkt_cnt].link1_count = timer_on();
}

void link5_complete(int sig)
{
    stat[pkt_cnt].link5_count = timer_on();
}

void link2_complete(int sig)
{
    stat[pkt_cnt].link2_count = timer_on();
}

void link3_complete(int sig)
{
    stat[pkt_cnt].link3_count = timer_on();
}
#endif

//*********************************************************
// Globals
//*********************************************************
int Line_Freq 		= FREQ;
int Frame_Size 		= FRAME_SIZE;
int Total_Pixels	= TOTAL_PIXELS;

int done = 0;			// host pc flag
int input = 0;			// host pc semaphore
int output = 0;			// master dsp semaphore
int frame_cnt = 0;		// host pc flag

int offset = 0;
int sbuf = 0;
int lbuf = 0;
int ticks;
int *sdram = (int *)FILE_SDRAM_ADDR;    		   // pointer to SDRAM raw input
unsigned int *ptr;                                 // pointer used for ...
volatile int tx_data = 0;                          // timer interrupt flag
int in_data4[2][LINE_SIZE];                         // ping-pong input data buffer
int in_data2[2][LINE_SIZE];                         // ping-pong input data buffer

//*********************************************************
// Functions
//*********************************************************
int calc_timer_ticks(int hz)
{
    return(80000000/(hz));
}

void timer_interrupt(int sig)
{

    asm("BIT TGL flags FLG2;");		// Toggle Line-Trigger
#if DIAGNOSTICS
	if(pkt_cnt < 25) pkt_cnt++;
	stat[pkt_cnt].timer_intr = timer_on();
#endif
	tx_data = 1;

}

//*********************************************************
//
//
//*********************************************************
void main(void)
{
    int i;

    init_Hammerhead();
	Total_Pixels = LINE_SIZE * Frame_Size;

#if INIT_SDRAM
    ptr = (unsigned int *)sdram;
    for(i = 0; i < Total_Pixels ; i++)
	{
	    *ptr++ = 0x00010001;
	}
#endif
	// Init the flags as outputs
	asm("bit set mode2 FLG2O;");
	asm("bit set mode2 FLG3O;");
    // Set up a timer interrupt to pace simulated input
	timer_off();
	ticks = calc_timer_ticks(Line_Freq); // calc cycles needed to simulate a FREQ kHz line rate
	timer_set(ticks, ticks);
	interrupt(SIG_TMZ0, timer_interrupt);

#if DIAGNOSTICS
	interrupt(SIG_LP1I, link1_complete);
	interrupt(SIG_LP5I, link5_complete);
	interrupt(SIG_LP2I, link2_complete);
	interrupt(SIG_LP3I, link3_complete);
	interrupt(SIG_EP0I, DMAC10_complete);
#endif

//for (frame_cnt = 0; frame_cnt < Num_Frames; frame_cnt++ )
	do
	{
		offset = 0;		
		sbuf = 0;		// next index in in_data (last index in fft_data)
		lbuf = 0;		// actual index of in_data and fft_data
		while (!input)
		{
			idle;
		}
		input = 0;				// semaphore
		timer_on();
		
		
	// Transfer data from external to internal memory.
	extmem_to_intmem((int *)sdram + offset,    // Source buffer
          	         (int *)in_data2[sbuf],                // Destination buffer
	       	         LINE_SIZE,                           // internal word count
	       	         LINE_SIZE,                           // external word count
					 1,                                   // External Increment (EM)
        	  		 10+lbuf,                             // DMA Channel
                     DMAC_DEN | DMAC_MASTER | DMAC_BURST, // flags for this xfer
             		 0,            	                      // Wait for DMA to complete?
					 0);                                  // Use timer?

	// Transfer data from external to internal memory.
	extmem_to_intmem((int *)sdram + Total_Pixels/SLAVES + offset, // Source buffer
          	         (int *)in_data4[sbuf],               // Destination buffer
	       	         LINE_SIZE,                           // internal word count
	       	         LINE_SIZE,                           // external word count
					 1,                                   // External Increment (EM)
        	  		 12+lbuf,                             // DMA Channel
                     DMAC_DEN | DMAC_MASTER | DMAC_BURST, // flags for this xfer
             		 0,            	                      // Wait for DMA to complete?
					 0);                                  // Use timer?

    sbuf = 1;
	offset += LINE_SIZE;      // word offset into SDRAM
	
	// wait for timer flag
	while(tx_data == 0);
    tx_data = 0;

	//*********************************************************
    // SDRAM is already loaded with test data from file; start main loop
	//*********************************************************
	do
	{
		// Transfer data from external to internal memory.
		extmem_to_intmem((int *)sdram + offset,    // Source pointer
          	             (int *)in_data2[sbuf],                // Destination buffer
	       	             LINE_SIZE,                           // internal word count
	       	             LINE_SIZE,                           // external word count
						 1,                                   // External Increment (EM)
            	  		 10+lbuf,                             // DMA Channel
                         DMAC_DEN | DMAC_MASTER | DMAC_BURST, // flags for this xfer
                 		 0,            	                      // Wait for DMA to complete?
						 0);                                  // Use timer?
		// Transfer data from external to internal memory.
		extmem_to_intmem((int *)sdram + Total_Pixels/SLAVES + offset, // Source buffer
    	      	         (int *)in_data4[sbuf],               // Destination buffer
		       	         LINE_SIZE,                           // internal word count
	       	        	 LINE_SIZE,                           // external word count
						 1,                                   // External Increment (EM)
        		  		 12+lbuf,                             // DMA Channel
        	             DMAC_DEN | DMAC_MASTER | DMAC_BURST, // flags for this xfer
    	         		 0,            	                      // Wait for DMA to complete?
						 0);                                  // Use timer?

		// Send 1/2 raw data to HH_PROC2
        link_dma_noint_xmt((int *)&in_data2[lbuf][0], // source buffer
		             LINK_PKT,          // buffer size
					 2,                 // link port
					 2,                 // link buffer
					 1,                 // link speed
					 8,                 // link bit width
					 32,                // data element bit width
					 1,                 // data stride
					 0,                 // wait for completion?
					 NULL);             // no abort function

 		link_dma_noint_xmt((int *)&in_data2[lbuf][LINK_PKT], // source buffer
		             LINK_PKT,          // buffer size
					 3,                 // link port
					 3,                 // link buffer
					 1,                 // link speed
					 8,                 // link bit width
					 32,                // data element bit width
					 1,                 // data stride
					 0,                 // wait for completion?
					 NULL);             // no abort function

		// Send 1/2 raw data to HH_PROC4
        link_dma_noint_xmt((int *)&in_data4[lbuf][0], // source buffer
		             LINK_PKT,          // buffer size
					 1,                 // link port
					 1,                 // link buffer
					 1,                 // link speed
					 8,                 // link bit width
					 32,                // data element bit width
					 1,                 // data stride
					 0,                 // wait for completion?
					 NULL);             // no abort function

 		link_dma_noint_xmt((int *)&in_data4[lbuf][LINK_PKT], // source buffer
		             LINK_PKT,          // buffer size
					 5,                 // link port
					 5,                 // link buffer
					 1,                 // link speed
					 8,                 // link bit width
					 32,                // data element bit width
					 1,                 // data stride
					 0,                 // wait for completion?
					 NULL);             // no abort function

		// update buffer managers
		lbuf = sbuf;
		sbuf = (sbuf) ? 0 : 1;	
	    offset += LINE_SIZE;      // word offset into SDRAM

		// wait for timer flag
		while(tx_data == 0);
        tx_data = 0;

	}while(offset < Total_Pixels/SLAVES);


	//*********************************************************
	// Ship out last buffer of data
	//*********************************************************
    link_dma_noint_xmt((int *)&in_data2[lbuf][0], // source buffer
	             LINK_PKT,          // buffer size
				 2,                 // link port
				 2,                 // link buffer
				 1,                 // link speed
				 8,                 // link bit width
				 32,                // data element bit width
				 1,                 // data stride
				 0,                 // wait for completion?
				 NULL);             // no abort function

	link_dma_noint_xmt((int *)&in_data2[lbuf][LINK_PKT], // source buffer
	             LINK_PKT,          // buffer size
				 3,                 // link port
				 3,                 // link buffer
				 1,                 // link speed
				 8,                 // link bit width
				 32,                // data element bit width
				 1,                 // data stride
				 0,                 // wait for completion?
				 NULL);             // no abort function
		
	// Send 1/2 raw data to HH_PROC4
    link_dma_noint_xmt((int *)&in_data4[lbuf][0], // source buffer
	             LINK_PKT,          // buffer size
				 1,                 // link port
				 1,                 // link buffer
				 1,                 // link speed
				 8,                 // link bit width
				 32,                // data element bit width
				 1,                 // data stride
				 0,                 // wait for completion?
				 NULL);             // no abort function

 	link_dma_noint_xmt((int *)&in_data4[lbuf][LINK_PKT], // source buffer
	             LINK_PKT,          // buffer size
				 5,                 // link port
				 5,                 // link buffer
				 1,                 // link speed
				 8,                 // link bit width
				 32,                // data element bit width
				 1,                 // data stride
				 0,                 // wait for completion?
				 NULL);             // no abort function

	asm("BIT TGL flags FLG3;");		// Toggle Buffer-Trigger
	frame_cnt++;
	output = 1;						// semaphore

	}while (!done);
	timer_off();
}
