#ifndef _COMMAND_H    /* Guard against multiple inclusion */
#define _COMMAND_H

/*!\name      command.h
 *
 * \brief     Table of commands
 *
 * \author    Juan Gago
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "system_config.h"
#include "system_definitions.h"
#include "Global.h"

typedef struct {
    char    cmd[LENGTH_CMD];
    void   (*func) (COMMAND_DATA*, uint8_t *);
    
} COMMAND;

const char MSG_CMD_HELP_GENERIC [] =
    "PIC_RESET, PIC_STATE\r\n"
    "POWER_UP\r\n"   
    "WR_GPIO, RD_GPIO\r\n"
    "RD_ADC\r\n";

const char MSG_CMD_HELP_ONEW [] = 
    "RD_ONEW, WR_ONEW\r\n";
    
/* Interface Functions */

bool processCommand		(COMMAND_DATA*, uint8_t*);

void SoftReset();
void ShowHelp           (COMMAND_DATA*, uint8_t*);
void Status             (COMMAND_DATA*, uint8_t*);
void Write_Digital      (COMMAND_DATA*, uint8_t*);
void Read_Digital       (COMMAND_DATA*, uint8_t*);
void Read_Analog        (COMMAND_DATA*, uint8_t*);
void Power_Up           (COMMAND_DATA*, uint8_t*);

#ifdef ONEW
void Set_1w             (COMMAND_DATA*, uint8_t*);
void Get_1w             (COMMAND_DATA*, uint8_t*);
#endif

#endif /* _COMMAND_H */

/* *****************************************************************************
 End of File
 */
