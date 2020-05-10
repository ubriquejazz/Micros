#include "drv_ow.h"
#include "ow.h"
#include "ow_search.h"
#include "buffer.h"

DRV_DATA driver; 

/*!\name      drv_ow.c
 *
 * \brief     OW Driver (intermediate level)
 *
 * \author    Juan Gago
 *
 */ 

int DRV_OW_Init(PIN_DEF pin) {
	driver.mutex = OW_init(pin);
	driver.state = DRV_STATE_INIT;
	return 0;
}

int DRV_OW_Tasks()
{
	static int cnt = 0;
	switch(driver.state)
	{
		case DRV_STATE_INIT:
			if (buffer_empty(0)){
				ptr = buffer_read(0);
				driver.ptr2req = (OW_REQUEST*) buffer_read(0);
			}
			break;

		CASE DRV_STATE_WRITE:
			if (cnt == driver.ptr2req->wlen) {

			}
			break;

		CASE DRV_STATE_READ:
			if (cnt == driver.ptr2req->rlen) {

			}
			break
	}
	return 0;
}

int DRV_OW_SendCommand(uint64_t addr, uint8_t command) {
	OW_match_rom(addr);
	OW_write_byte(command);
	return 0;
}

int	DRV_OW_BufferAdd(OW_REQUEST* req, uint64_t rom_no, callback_t callback) {

	req->base.self = (Base*) req;
	req->rom_no = rom_no;
	req->pfunc = callback;
	return buffer_write(0, (Base*)req);
}

int DRV_OW_GetDevice(uint64_t array[], uint8_t family_code) {
	return 0;
}

/*******************************************************************************
End of File
*/