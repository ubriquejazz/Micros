//********************************************************************
//
//  hh_consumer.c 
//  --------------------------------------------
//  Program to receive data from hh_generator process over link ports.
//  This code is designed to run on processor 2.
//
//********************************************************************
#include <def21160.h>
#include <21160.h>
#include <stdio.h>
#include <signal.h>
#include <macros.h>
#include <filter.h> 
#include "linkdma.h"
#include "extint0.h"

#define SIXTEEN_BIT          1                  // set flag to use 16-bit space
#define N_FFT 				 1024 
#define N_RFFT_OUT 			 ((N_FFT / 2) + 1) 
#define LINE_SIZE            1024               // 2 lines data = 1024 2-byte pixels 
#define LINK_PKT             LINE_SIZE/2        // each link sends 1/2 of line data (packet)
#define FRAME_SIZE           32					// 32 lines per frame for this test
#define SHIFT				 16
#define SLAVES				 2

//********************************************************************
// Globals
//********************************************************************

int rxbuf, lbuf = 0;
int rxpkts = 0;
int Frame_Size = FRAME_SIZE;
volatile int rx_link4 = 0;
volatile int rx_link5 = 0;

int rcvd_data[FRAME_SIZE/SLAVES][2*LINK_PKT];		// 16-deep rx and output data buffer
unsigned int *ptr;

volatile int time_temp, stat[FRAME_SIZE/SLAVES];	/* for benchmark routines */

	float pm twiddle_re[N_FFT/2]; 
	float dm twiddle_im[N_FFT/2]; 

	#pragma align 1024 
	float dm rfft1_re[N_FFT]; 
	float dm rfft1_im[N_FFT]; 

	#pragma align 1024 
	float pm rfft2_re[N_FFT]; 
	float pm rfft2_im[N_FFT]; 

	#pragma align 1024 
	float dm data_re[N_FFT]; 
	float pm data_im[N_FFT]; 

	float rspectrum_1[N_RFFT_OUT]; 
	float rspectrum_2[N_RFFT_OUT]; 


//********************************************************************
// Functions
//********************************************************************
void link5_handler(int signum)
{
    rx_link5 = 1;
}
void link4_handler(int signum)
{
    rx_link4 = 1;
}

//********************************************************************
//
//
//
//********************************************************************
void main(void)
{
    int i,j;
    
    // prepare processor for operation
    init_HH_manual(0xc, 0x01ce1b86, 2, 0x10);
	twidfftf(twiddle_re, twiddle_im, N_FFT); 
	for(i = 0; i < LINE_SIZE; i++)
	{
	    rfft1_im[i] = 0;
	    rfft2_im[i] = 0; 
	}

	// interrupt each time link 4 completes
    interrupt(SIG_LP4I, link4_handler); 
//  interrupt(SIG_LP5I, link5_handler); 
	
for (;;)
{
	rxbuf = 0;
	rxpkts = 0;
    
	// Receive data from HH_PROC1
	link_dma_noint_rcv((int *)&rcvd_data[rxbuf][0], // source buffer
		             LINK_PKT,         // buffer size
					 4,                 // link port
					 4,                 // link buffer
					 1,                 // link speed
					 8,                 // link bit width
					 32,                // data element bit width
					 1,                 // data stride
					 0,                 // wait for completion?
					 NULL);             // no abort function

	link_dma_noint_rcv((int *)&rcvd_data[rxbuf][LINK_PKT], // source buffer
		             LINK_PKT,         // buffer size
					 5,                 // link port
					 5,                 // link buffer
					 1,                 // link speed
					 8,                 // link bit width
					 32,                // data element bit width
					 1,                 // data stride
					 0,                 // wait for completion?
					 NULL);             // no abort function

	// wait for timer flag
	while (rx_link4 == 0)
	{
		idle;
	}
    rx_link4 = 0;
    rx_link5 = 0;
	rxpkts++;
    
	// update buffer managers
	lbuf = rxbuf;
	rxbuf++;
	rxbuf = rxbuf % (FRAME_SIZE/SLAVES);
    
	    do
		{
			// Receive data from HH_PROC1
	        link_dma_noint_rcv((int *)&rcvd_data[rxbuf][0], // source buffer
			             LINK_PKT,         // buffer size
						 4,                 // link port
						 4,                 // link buffer
						 1,                 // link speed
						 8,                 // link bit width
						 32,                // data element bit width
						 1,                 // data stride
						 0,                 // wait for completion?
						 NULL);             // no abort function

	 		link_dma_noint_rcv((int *)&rcvd_data[rxbuf][LINK_PKT], // source buffer
			             LINK_PKT,         // buffer size
						 5,                 // link port
						 5,                 // link buffer
						 1,                 // link speed
						 8,                 // link bit width
						 32,                // data element bit width
						 1,                 // data stride
						 0,                 // wait for completion?
						 NULL);             // no abort function

#if SIXTEEN_BIT

 	 		time_temp = count_start();		// Benchmark routine start 
			ptr = (unsigned int *)&rcvd_data[lbuf][0];

#pragma SIMD_for
			for(i=0; i < LINE_SIZE/2; i++)
			{			    
			    rfft1_re[i] 			  = (float)
					((ptr[i] & 0xFFFF0000)>>SHIFT );	// high word = tap #0
				rfft1_re[LINE_SIZE/2 + i] = (float)
   			    	(ptr[i] & 0x0000FFFF);				// low word  = tap #1
				
			    rfft2_re[i] 			  = (float)
				   	((ptr[LINE_SIZE/2 + i] & 0xFFFF0000)>>SHIFT );
			    rfft2_re[LINE_SIZE/2 + i] = (float) 
			    	(ptr[LINE_SIZE/2 + i] & 0x0000FFFF);
			}
			
			rfftf_2(rfft1_re, rfft1_im, 
					rfft2_re, rfft2_im, twiddle_re, twiddle_im, N_FFT); 

			fftf_magnitude(rfft1_re, rfft1_im, rspectrum_1, N_FFT, 2); 
			fftf_magnitude(rfft2_re, rfft2_im, rspectrum_2, N_FFT, 2);
	
#pragma SIMD_for
			for(i=0; i < LINE_SIZE/2; i++)
			{
				ptr[i] 				 = (int)rspectrum_1[i];
				ptr[LINE_SIZE/2 + i] = (int)rspectrum_2[i];
			}
	
			stat[rxpkts-1] = count_end(time_temp); 	// Benchmark routine end  
#endif    	

			// wait for timer flag
			while (rx_link4 == 0)
			{
				idle;
			}
		    rx_link4 = 0;
		    rx_link5 = 0;
			rxpkts++;

			// update buffer managers
			lbuf = rxbuf;
			rxbuf++;
			rxbuf = rxbuf % (FRAME_SIZE/SLAVES);

		}while(rxpkts < Frame_Size/SLAVES);
	
			ptr = (unsigned int *)&rcvd_data[lbuf][0];

			#pragma SIMD_for
			for(i=0; i < LINE_SIZE/2; i++)
			{			    
			    rfft1_re[i] 			  = (float)
					((ptr[i] & 0xFFFF0000)>>SHIFT );	// high word = tap #0
				rfft1_re[LINE_SIZE/2 + i] = (float)
   			    	(ptr[i] & 0x0000FFFF);				// low word  = tap #1
				
			    rfft2_re[i] 			  = (float)
				   	((ptr[LINE_SIZE/2 + i] & 0xFFFF0000)>>SHIFT );
			    rfft2_re[LINE_SIZE/2 + i] = (float) 
			    	(ptr[LINE_SIZE/2 + i] & 0x0000FFFF);
			}
			
			rfftf_2(rfft1_re, rfft1_im, 
					rfft2_re, rfft2_im, twiddle_re, twiddle_im, N_FFT); 

			fftf_magnitude(rfft1_re, rfft1_im, rspectrum_1, N_FFT, 2); 
			fftf_magnitude(rfft2_re, rfft2_im, rspectrum_2, N_FFT, 2);
	
			#pragma SIMD_for
			for(i=0; i < LINE_SIZE/2; i++)
			{
				ptr[i] 				 = (int)rspectrum_1[i];
				ptr[LINE_SIZE/2 + i] = (int)rspectrum_2[i];
			}
  	
	}
}
