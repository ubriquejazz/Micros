/*!\name      main.h
 *
 * \brief     TCPIP example.
 *
 * \author    Juan Gago
 *
 */

#include "system_config.h"
#include "system_definitions.h"
#include <sys/attribs.h>

#include "tcp.h"
#include "cdc.h"
#include "ledcontrol.h"
#include "buttoncontrol.h"

/* Structure to hold the object handles for the modules in the system. */
SYSTEM_OBJECTS sysObj;

SYS_CMD_INIT sysCmdInit =
{
    .moduleInit = {0},
    .consoleCmdIOParam = SYS_CMD_FULL_COMMAND_READ_CONSOLE_IO_PARAM,
};

SYS_MODULE_OBJ sysConsoleObjects[] = { SYS_MODULE_OBJ_INVALID };

/* Declared in console device implementation (sys_console_appio.c) */
extern SYS_CONSOLE_DEV_DESC consAppIODevDesc;

SYS_CONSOLE_INIT consUsbInit0 =
{
    .moduleInit = {0},
    .consDevDesc = &consAppIODevDesc,
};

const DRV_TMR_INIT drvTmr0InitData =
{
    .moduleInit.sys.powerState = DRV_TMR_POWER_STATE_IDX0,
    .tmrId = DRV_TMR_PERIPHERAL_ID_IDX0,
    .clockSource = DRV_TMR_CLOCK_SOURCE_IDX0,
    .prescale = DRV_TMR_PRESCALE_IDX0,
    .mode = DRV_TMR_OPERATION_MODE_16_BIT,
    .interruptSource = DRV_TMR_INTERRUPT_SOURCE_IDX0,
    .asyncWriteEnable = false,
};

const SYS_TMR_INIT sysTmrInitData =
{
    .moduleInit = {SYS_MODULE_POWER_RUN_FULL},
    .drvIndex = DRV_TMR_INDEX_0,
    .tmrFreq = 1000, 
};

/*** ISR ***/
void __ISR(_TIMER_1_VECTOR, ipl1AUTO) IntHandlerDrvTmrInstance0(void)
{
    DRV_TMR_Tasks(sysObj.drvTmr0);
}

void SYS_Initialize (void) 
{
    /* Core Processor Initialization */
    SYS_CLK_Initialize( NULL );
    SYS_DEVCON_Initialize(SYS_DEVCON_INDEX_0, (SYS_MODULE_INIT*)NULL);
    SYS_DEVCON_PerformanceConfig(SYS_CLK_SystemFrequencyGet());       

    /*** TMR Service Initialization Code ***/
    sysObj.drvTmr0 = DRV_TMR_Initialize(DRV_TMR_INDEX_0, (const SYS_MODULE_INIT * const) &drvTmr0InitData);
    sysObj.sysTmr  = SYS_TMR_Initialize(SYS_TMR_INDEX_0, (const SYS_MODULE_INIT  * const) &sysTmrInitData);

    /*** Command Service Initialization Code ***/
    SYS_CMD_Initialize((SYS_MODULE_INIT*)&sysCmdInit);
    sysObj.sysConsole0 = SYS_CONSOLE_Initialize(SYS_CONSOLE_INDEX_0, (SYS_MODULE_INIT *) &consUsbInit0);

    /*** Random Service Initialization Code ***/
    SYS_RANDOM_Initialize(0, 0);

    /* Initialize BSP and Ports */
    SYS_PORTS_Initialize();
    BSP_Initialize(); 

    /* Set priority of Timer interrupt source */
    SYS_INT_VectorPrioritySet(INT_VECTOR_T1, INT_PRIORITY_LEVEL1);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_T1, INT_SUBPRIORITY_LEVEL0);

    /*** Interrupt Service Initialization Code ***/
    SYS_INT_Initialize();
}

int main() {

    SYS_Initialize();
    cdc_setup();
    tcp_setup();
    SYS_INT_Enable();

    LEDCONTROL_Initialize();
    BUTTONCONTROL_Initialize();

    while (1) {

        /* SYS_COMMAND layer tasks routine */ 
        SYS_CMD_Tasks();
        SYS_CONSOLE_Tasks(sysObj.sysConsole0);

        /* Maintain system services */
        SYS_TMR_Tasks(sysObj.sysTmr);
         
        /* Maintain Middleware */
        cdc_tasks();
        tcp_tasks();

        /* Maintain the application's state machine. */
        LEDCONTROL_Tasks();
        BUTTONCONTROL_Tasks();
  }
  return 0;
}
