#ifndef _TMR__H__
#define _TMR__H__

/*!\name      tmr.h
 *
 * \brief     Timer Class
 *
 * \author    Juan Gago
 *
 */


typedef enum
{
    TMR_DRV_INIT,
    TMR_DRV_DONE,

} TMR_DRV_STATE;

typedef struct
{
    uint8_t         driver_index; 
  	uint32_t        error_count;  
     /* Only one client per driver */
    uint32_t        alarm, now;  
    TMR_DRV_STATE   state;

} TMR_DATA;

/* Application Initialization and State Machine Functions */

void TMR_Initialize (TMR_DATA*, uint8_t);
void TMR_Tasks (TMR_DATA*, uint16_t);

#endif
