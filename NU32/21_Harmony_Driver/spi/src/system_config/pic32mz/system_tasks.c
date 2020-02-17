#include "system_config.h"
#include "system_definitions.h"
#include "sensor_app.h"
#include "fram_app.h"
 
static void _SYS_Tasks ( void );

void _USB_Tasks(void);
static void _SENSOR_APP_Tasks(void);
static void _FRAM_APP_Tasks(void);

/*******************************************************************************
  Function:
    void SYS_Tasks ( void )

  Remarks:
    See prototype in system/common/sys_module.h.
*/

void SYS_Tasks ( void )
{
    /* Create OS Thread for Sys Tasks. */
    xTaskCreate((TaskFunction_t) _SYS_Tasks,
                "Sys Tasks",
                1024, NULL, 1, NULL);
 
    /* Create task for gfx state machine*/
    /* Create OS Thread for USB Tasks. */
    xTaskCreate((TaskFunction_t) _USB_Tasks,
                "USB Tasks",
                1024, NULL, 1, NULL);

    /* Create OS Thread for SENSOR_APP Tasks. */
    xTaskCreate((TaskFunction_t) _SENSOR_APP_Tasks,
                "SENSOR_APP Tasks",
                1024, NULL, 1, NULL);

    /* Create OS Thread for FRAM_APP Tasks. */
    xTaskCreate((TaskFunction_t) _FRAM_APP_Tasks,
                "FRAM_APP Tasks",
                1024, NULL, 1, NULL);

    /**************
     * Start RTOS * 
     **************/
    vTaskStartScheduler(); /* This function never returns. */
}


/*******************************************************************************
  Function:
    void _SYS_Tasks ( void )

  Summary:
    Maintains state machines of system modules.
*/

static void _SYS_Tasks ( void)
{
    while(1)
    {
        /* Maintain system services */
    SYS_CONSOLE_Tasks(sysObj.sysConsole0);
    /* SYS_TMR Device layer tasks routine */ 
    SYS_TMR_Tasks(sysObj.sysTmr);

        /* Maintain Device Drivers */

        /* Maintain Middleware */

        /* Task Delay */
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}


void _USB_Tasks(void)
{
    while(1)
    {
        /* USBHS Driver Task Routine */ 
         DRV_USBHS_Tasks(sysObj.drvUSBObject);
         
        /* USB Device layer tasks routine */ 
        USB_DEVICE_Tasks(sysObj.usbDevObject0);
 

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
 }

/*******************************************************************************
  Function:
    void _SENSOR_APP_Tasks ( void )

  Summary:
    Maintains state machine of SENSOR_APP.
*/

static void _SENSOR_APP_Tasks(void)
{
    while(1)
    {
        SENSOR_APP_Tasks();
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}


/*******************************************************************************
  Function:
    void _FRAM_APP_Tasks ( void )

  Summary:
    Maintains state machine of FRAM_APP.
*/

static void _FRAM_APP_Tasks(void)
{
    while(1)
    {
        FRAM_APP_Tasks();
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}


/*******************************************************************************
 End of File
 */

