#ifndef SYSTEM_DEFINITIONS_H
#define SYSTEM_DEFINITIONS_H

#include <stddef.h>
#include "system/common/sys_common.h"
#include "system/common/sys_module.h"
#include "system/devcon/sys_devcon.h"
#include "system/reset/sys_reset.h"
#include "system/clk/sys_clk.h"
#include "system/int/sys_int.h"
#include "system/tmr/sys_tmr.h"
#include "driver/tmr/drv_tmr.h"
#include "system/ports/sys_ports.h"
#include "system/command/sys_command.h"

#include "driver/usb/usbhs/drv_usbhs.h"
#include "usb/usb_device.h"
#include "usb/usb_device_cdc.h"

#include "tcpip/tcpip.h"
#include "driver/ethmac/drv_ethmac.h"
#include "driver/miim/drv_miim.h"

typedef struct
{
    SYS_MODULE_OBJ  sysTmr;
    SYS_MODULE_OBJ  drvTmr0;
    SYS_MODULE_OBJ  sysConsole0;

    SYS_MODULE_OBJ  drvUSBObject;
    SYS_MODULE_OBJ  usbDevObject0;

    SYS_MODULE_OBJ  tcpip;
    SYS_MODULE_OBJ  drvMiim;

} SYSTEM_OBJECTS;

extern SYSTEM_OBJECTS sysObj;

#endif 
