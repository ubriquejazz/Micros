#include "OWRequest.h"
#include "buffer.h"

int OW_new_request(OW_Request* req, ...)
{
	req->base.self = (Base*)req;
	// ...
	return 0;
}	


int OW_request_write(OW_Request value) 
{
	return buffer_write(value.base.self)
}

int OW_request_read(OW_Request* value) 
{
	buffer_read(value->base.self);
	OW_Request* ptr = (OW_Request*) value->base.self;
	copy_request(value, ptr);
}

int OW_request_poll() 
{
	int retVal = 0;
	uint16_t param[2];

	if (!buffer_empty()) {
		OW_Request req;			// process request
		buffer_read(&req);		// return OK
				
		OW_reset_pulse();			// check if 0
		OWBus_match(req.rom_no);	// 64 bits

		for (i<req.wlen) {				// write loop
			OW_write_byte(to_write[i])	// 1st command
		}

		if (req.rlen>0) {			// rlen = 0 for a write request
			for (i<req.rlen)		// read loop
				to_read[i] = OW_read_byte();
		}
	}
	return retVal;
}


/*******************************************************************************
 End of File
 */
