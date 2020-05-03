#include "OWRequest.h"
#include "OW.h"
#include "OWBus.h"
#include "buffer.h"

int OW_new_request(OW_Request* req, ...)
{
	int retVal = buffer_write((Base*)req);
	req->base.self = (Base*)req;
	// ...
	return retVal;
}

OW_Request* OW_request_read(void) 
{
	Base* ptr;
	buffer_read(ptr);			// 
	return (OW_Request*) ptr;	// casting
}

int OW_request_poll() 
{
	static OW_Request* req;
	static OW_REQ_STATES state = OW_REQ_STATE_INIT;
	int retVal = 0;

	switch(state)
	{
		case OW_STATE_INIT:
			if (!buffer_empty()) {
				OW_Request* req = OW_request_read();
				OW_reset_pulse();
				state = OW_STATE_MATCH;
			}
			break;

		case OW_STATE_MATCH:
			OWBus_match(req->rom_no);	// 64 bits
			i = 0;

		case OW_STATE_WRITE:
			if (i == req->wlen){
				i = 0;
				state = OW_STATE_READ;
			}
			else {
				OW_write_byte(req->to_write[i++]);
			}
			break;

		case OW_STATE_READ:
			if (i == req->rlen){
				i = 0;
				state = OW_STATE_CALLBACK;
			}
			else {
				req->to_read[i++] = OW_read_byte();
			}
			break;

		case OW_STATE_CALLBACK:
			uint16_t param[2];
			req->pfunc(param[1], param[0]);
			state = OW_STATE_INIT;
			break;

		default:
			break;
	}
	return retVal;
}

/*******************************************************************************
 End of File
 */
