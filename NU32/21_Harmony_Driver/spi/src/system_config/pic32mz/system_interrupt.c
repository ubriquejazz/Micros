#include "system/common/sys_common.h"
#include "sensor_app.h"
#include "fram_app.h"
#include "system_definitions.h"

// *****************************************************************************
// *****************************************************************************
// Section: System Interrupt Vector Functions
// *****************************************************************************
// *****************************************************************************


void IntHandlerDrvTmrInstance0(void)
{
    DRV_TMR_Tasks(sysObj.drvTmr0);
}
 
void IntHandlerSPIRxInstance0(void)
{
    DRV_SPI_Tasks(sysObj.spiObjectIdx0);
}
void IntHandlerSPITxInstance0(void)
{
    DRV_SPI_Tasks(sysObj.spiObjectIdx0);
}
void IntHandlerSPIFaultInstance0(void)
{
    DRV_SPI_Tasks(sysObj.spiObjectIdx0);
}
void IntHandlerSPIRxInstance1(void)
{
    DRV_SPI_Tasks(sysObj.spiObjectIdx1);
}
void IntHandlerSPITxInstance1(void)
{
    DRV_SPI_Tasks(sysObj.spiObjectIdx1);
}
void IntHandlerSPIFaultInstance1(void)
{
    DRV_SPI_Tasks(sysObj.spiObjectIdx1);
}
	
	
	
void IntHandlerUSBInstance0(void)
{
    DRV_USBHS_Tasks_ISR(sysObj.drvUSBObject);
}

void IntHandlerUSBInstance0_USBDMA ( void )
{
    DRV_USBHS_Tasks_ISR_USBDMA(sysObj.drvUSBObject);
}


/*******************************************************************************
 End of File
*/
