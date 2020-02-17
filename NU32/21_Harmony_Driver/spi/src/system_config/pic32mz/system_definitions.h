#ifndef _SYS_DEFINITIONS_H
#define _SYS_DEFINITIONS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "system/common/sys_common.h"
#include "system/common/sys_module.h"
#include "system/devcon/sys_devcon.h"
#include "system/clk/sys_clk.h"
#include "system/int/sys_int.h"
#include "system/console/sys_console.h"
#include "system/tmr/sys_tmr.h"
#include "driver/tmr/drv_tmr.h"
#include "system/ports/sys_ports.h"
#include "driver/spi/drv_spi.h"
#include "system/debug/sys_debug.h"
#include "driver/usb/usbhs/drv_usbhs.h"
#include "usb/usb_device.h"
#include "usb/usb_device_cdc.h"
#include "FreeRTOS.h"
#include "task.h"
#include "sensor_app.h"
#include "fram_app.h"

typedef struct
{
    SYS_MODULE_OBJ  sysTmr;
    SYS_MODULE_OBJ  drvTmr0;
    SYS_MODULE_OBJ  sysDebug;
    SYS_MODULE_OBJ  sysConsole0;

    /*** SPI Object for Index 0 ***/
    SYS_MODULE_OBJ				spiObjectIdx0;
    
    /*** SPI Object for Index 1 ***/
    SYS_MODULE_OBJ				spiObjectIdx1;

    /* USB */
    SYS_MODULE_OBJ  drvUSBObject;    
    SYS_MODULE_OBJ  usbDevObject0;

} SYSTEM_OBJECTS;

extern SYSTEM_OBJECTS sysObj;

#endif /* _SYS_DEFINITIONS_H */
/*******************************************************************************
 End of File
*/

