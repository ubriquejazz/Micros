/*!\name      1wire.h
 *
 * \brief     1W functions
 *
 * \author    Juan Gago
 *
 */

#include "app1.h"
#include "1wire.h"

//! Drive 1-wire pin low
/*! Set pin to be an output and drive it low
*/ 
void drive_OW_low(void)
{   
    SYS_PORTS_PinDirectionSelect (PORTS_ID_0,
            SYS_PORTS_DIRECTION_OUTPUT,
            APP1_ONE_WIRE_CHANNEL, APP1_ONE_WIRE_BIT_POS);         
    
    SYS_PORTS_PinClear (PORTS_ID_0,
    	APP1_ONE_WIRE_CHANNEL, APP1_ONE_WIRE_BIT_POS);        
}

//! Drive 1-wire pin high
/*! Set pin to be an output and drive it high
*/
void drive_OW_high (void)
{
    SYS_PORTS_PinDirectionSelect (PORTS_ID_0,
            SYS_PORTS_DIRECTION_OUTPUT,
            APP1_ONE_WIRE_CHANNEL, APP1_ONE_WIRE_BIT_POS);
    
    SYS_PORTS_PinSet (PORTS_ID_0,
        APP1_ONE_WIRE_CHANNEL, APP1_ONE_WIRE_BIT_POS);
}

//! Read the 1-wire state
/*! Set pin to be an input and read the value
*/
unsigned char read_OW(void)
{
	unsigned char read_data=0;
    
	SYS_PORTS_PinDirectionSelect (PORTS_ID_0,
            SYS_PORTS_DIRECTION_INPUT,
            APP1_ONE_WIRE_CHANNEL, APP1_ONE_WIRE_BIT_POS);
        
    if (1 ==  SYS_PORTS_PinRead(PORTS_ID_0,        
            APP1_ONE_WIRE_CHANNEL, APP1_ONE_WIRE_BIT_POS) )
        read_data = 1;      
    else 	
        read_data = 0;      
    
    return read_data;
}

//! Place a reset pulse on the bus
/*! Reset all devices on the bus by sending a long low.
	 over the 1-wire bus followed by a presence pulse
*/ 
unsigned char OW_reset_pulse(void)
{
	unsigned char presence_detect;
	
  	drive_OW_low();                     // Reset Low Time (RSTL)
 	Delay10us(60);                      // tRSTL = 600us
 	drive_OW_high();                    // Presence-Detect Sample Time (MSP)
	Delay10us(7);                       // tMSP = 70us
	presence_detect = read_OW();        // Recovery (REC)
	Delay10us(35);                      // tREC = 350us
	return presence_detect;
}	

//! Send a single bit to the 1-wire bus
void OW_write_bit (unsigned char write_bit)
{
	if (write_bit)
	{
		drive_OW_low(); 				// Writing a bit '1'
		Delay10us(1);                   // tW1L = 10us
		drive_OW_high();  				// Time-Slot Duration
		Delay10us(7);                   // tSLOT = 70us
	}
	else
	{
		drive_OW_low(); 				// Writing a bit '0'
		Delay10us(4);      				// tW0L = 40us
		drive_OW_high();  				// Time-Slot Duration
		Delay10us(1);                   // tSLOT - tW0L = 10us
	}
	Delay10us(5);                       // tREC = 50us
}	

//! Read a single bit from the 1-wire
unsigned char OW_read_bit (void)
{
	unsigned char read_data; 

	//reading a bit 
	drive_OW_low();                     // Read Low Time (RL)
	Delay10us(1);                       // tRL = 10us
	drive_OW_high();                    // Read Sample Time (MSR)
	DelayUs(9);                         // tMSR = 9us
	read_data = read_OW();              // Recovery (REC)
	Delay10us(11);                      // tREC = 110us	
    return read_data;
}

//! Send a byte over the 1-wire
void OW_write_byte (unsigned char write_data)
{
	unsigned char loop;	
	for (loop = 0; loop < 8; loop++)
	{
		OW_write_bit(write_data & 0x01); 	//Sending LS-bit first
		write_data >>= 1;					// shift the data byte for the next bit to send
	}	
}	

//! Read a byte from the bus
unsigned char OW_read_byte (void)
{
	unsigned char loop, result=0;
	
	for (loop = 0; loop < 8; loop++)
	{		
		result >>= 1; 				// shift the result to get it ready for the next bit to receive
		if (OW_read_bit())
		result |= 0x80;				// if result is one, then set MS-bit
	}
	return result;					
}	

/*******************************************************************************
 End of File
 */
