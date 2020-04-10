#ifndef __pwm_functions_h    /* Guard against multiple inclusion */
#define __pwm_functions_h

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "system_config.h"
#include "system_definitions.h"
#include "enum.h"

/*!\name      functions.h
 *
 * \brief     Global definitions
 *
 * \author    Juan Gago
 *
 */

void        PWM_OC_Start		(void);
void        PWM_OC_PWSet		(int);
bool        IRDA_ReadAnalog		(void);
uint8_t     IRDA_GetPicPic		(char); 
void        IRDA_SetPicPic		(int, char);
uint8_t     FAN_SetPicPic		(int, char);

void  		Cmd_Set_Power     	(COMMAND_DATA*, uint8_t *);
void  		Cmd_Get_Power     	(COMMAND_DATA*, uint8_t *);
void  		Cmd_Ch_BeaconMR   	(COMMAND_DATA*, uint8_t *);
void  		Cmd_Ch_BeaconMS   	(COMMAND_DATA*, uint8_t *);
/* fan */	
void  		Cmd_Set_Speed     	(COMMAND_DATA*, uint8_t *);
void  		Cmd_Get_Speed     	(COMMAND_DATA*, uint8_t *);
void  		Cmd_Addr_BeaconMR 	(COMMAND_DATA*, uint8_t *);
void  		Cmd_Addr_BeaconMS 	(COMMAND_DATA*, uint8_t *);

#endif /* __pwm_functions_h */

/* *****************************************************************************
 End of File
 */
