#include "OW.h"
#include "timing.h"
#include "mutex.h"
#include "buffer.h"

// Private variables
static PIN_DEF 	PinOne;
static int		MutexOneWire;
static OW_REQ	RequestBuffer[BUFFER_LEN];

int OW_init(PIN_DEF pin) 
{
	static bool firstTime = true;
	if (firstTime) {
        MutexOneWire = mutex_new();
        firstTime = false;
    }
	PinOne = pin;
	return MutexOneWire;
}

int OW_poll(uint64_t address)
{
	OW_REQ req;
	int retVal = 0;
	if (!buffer_empty())
	{
		buffer_read(&req);
		for (i<req.wlen) {
			OW_write_byte(to_write[i])
		}
		if (req.rlen>0) {
			for (i<req.rlen)
				to_read[i] = OW_read_byte();
		}
		retVal = (req.pfunc)(12,12);
	}
	return retVal;
}

/*! \fn         OW_write_byte (write_data)
 * 
 *  \brief      Send a byte over the 1-wire
 *
 *  \param		write_data - byte of data to write to the bus
 */	
void OW_write_byte (uint8_t write_data)
{
	uint8_t loop;	
	for (loop = 0; loop < 8; loop++) {
		OW_write_bit(write_data & 0x01);    // sending LS-bit first
		write_data >>= 1; // shift the data byte for the next bit to send
	}	
}	

/*! \fn         OW_read_byte ()
 * 
 *  \brief      Read a byte from the bus
 * 
 *  \return     byte read back from the 1-wire
 */	
uint8_t OW_read_byte (void)
{
	uint8_t loop, result=0;
	for (loop = 0; loop < 8; loop++) {		
		result >>= 1; // shift result to get it ready for next bit to receive
		if (OW_read_bit())
            result |= 0x80;    	// if result is one, then set MS-bit
	}
	return result;					
}

/*******************************************************************************
 End of File
 */
