/*!\name      main.h
 *
 * \brief     CDC example.
 *
 * \author    Juan Gago
 *
 */
#include "cdc.h"
#include "system_config.h"
#include "system_definitions.h"
#include <sys/attribs.h>

/* Structure to hold the object handles for the modules in the system. */
SYSTEM_OBJECTS sysObj;

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

void SYS_Initialize ( void )
{
    /* Core Processor Initialization */
    SYS_CLK_Initialize( NULL );
    SYS_DEVCON_Initialize(SYS_DEVCON_INDEX_0, (SYS_MODULE_INIT*)NULL);
    SYS_DEVCON_PerformanceConfig(SYS_CLK_SystemFrequencyGet());

    /* Initialize Drivers */
    sysObj.drvTmr0 = DRV_TMR_Initialize(DRV_TMR_INDEX_0, (const SYS_MODULE_INIT * const) &drvTmr0InitData);
    sysObj.sysTmr  = SYS_TMR_Initialize(SYS_TMR_INDEX_0, (const SYS_MODULE_INIT  * const) &sysTmrInitData);

    /* Initialize System Services */
    SYS_PORTS_Initialize();

    /*** Interrupt Service Initialization Code ***/
    SYS_INT_VectorPrioritySet(INT_VECTOR_T1, INT_PRIORITY_LEVEL1);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_T1, INT_SUBPRIORITY_LEVEL0); 

    /*** Interrupt Service Initialization Code ***/
    SYS_INT_Initialize();
}

int main () {

    SYS_Initialize();
    cdc_setup();
    SYS_INT_Enable();

    while(1) {

        /* Maintain system services */
        SYS_TMR_Tasks(sysObj.sysTmr);

        /* Maintain Middleware */
        cdc_update();
    }
    return 0;
}
