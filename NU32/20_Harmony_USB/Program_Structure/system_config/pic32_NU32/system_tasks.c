/*!\name      system_tasks.c
 *
 * \brief     Code to run the system: SYS_Taks(void)
 *
 * \author    Juan Gago
 *
 */

#include "system_config.h"
#include "system_definitions.h"

// update the state machines
void SYS_Tasks(void) 
{
  SYS_DEVCON_Tasks(sysObj.sysDevcon);
  // timer tasks are interrupt driven, not polled.
  // for a polled application uncomment below 
  // DRV_TMR_Tasks(sysObj.drvTmr);
  APP_Tasks(); // application specific tasks
}
