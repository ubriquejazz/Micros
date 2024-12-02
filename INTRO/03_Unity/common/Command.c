/*!\name      command.h
 *
 * \brief     Table of commands
 *
 * \author    Juan Gago
 *
 */
 
#include <stdio.h>
#include "command.h"
#include "miscellaneous.h"

const COMMAND commands[] = {
    {"PIC_RESET",           SoftReset},                 // 00P0bye0
    {"PIC_STATE",           Status},                    // 00P0lse0
    {"HELP",                ShowHelp},                   
    {"POWER_UP",            Power_Up},                  // 00P0pwr0    
    {"WR_GPIO",             Write_Digital},             // Write IO pin
    {"RD_GPIO",             Read_Digital},              // Read IO pin
    {"RD_ADC",              Read_Analog},               // Read AN channel 
#ifdef ONEW
    {"RD_ONEW",             Get_1w},                    // Read 1W
    {"WR_ONEW",             Set_1w},                    // Write string
#endif
};

const int N_COMMANDS = sizeof (commands)/ sizeof (COMMAND);

bool processCommand (COMMAND_DATA* commandData, uint8_t* buffer2)
{
    int idx;    
    
    if (commandData->argc>0)
    {
        for (idx = 0; idx < N_COMMANDS; idx++) 
        {
            int length = strlen(commands[idx].cmd);
            if (memcmp (commands[idx].cmd, commandData->argv[0], length) == 0) 
            {            
                commands[idx].func(commandData, buffer2);   
                return true;
            }
        } 
    }           
    return false;
}

void __attribute__((noreturn)) SoftReset(void)
{
    SYS_INT_Disable();
    /* perform a system unlock sequence ,starting critical sequence*/
    SYSKEY = 0x00000000; //write invalid key to force lock
    SYSKEY = 0xAA996655; //write key1 to SYSKEY
    SYSKEY = 0x556699AA; //write key2 to SYSKEY
    /* set SWRST bit to arm reset */
    RSWRSTSET = 1;
    /* read RSWRST register to trigger reset */
    unsigned int dummy;
    dummy = RSWRST;
    /* prevent any unwanted code execution until reset occurs*/
    while(1);
}

void ShowHelp (COMMAND_DATA* commandData, uint8_t* result) 
{ 
    sprintf (result, "Commands\r\n%s%s%s%s%s"
            , MSG_CMD_HELP_GENERIC 
#ifdef ONEW            
            , MSG_CMD_HELP_ONEW
#else 
            , ""
#endif
            );            
    // end sprintf
}

void Status        (COMMAND_DATA* commandData, uint8_t* result) { Cmd_Status        (commandData, result); }
void Power_Up      (COMMAND_DATA* commandData, uint8_t* result) { Cmd_Power_Up      (commandData, result); }
void Write_Digital (COMMAND_DATA* commandData, uint8_t* result) { Cmd_Write_Digital (commandData, result); }
void Read_Digital  (COMMAND_DATA* commandData, uint8_t* result) { Cmd_Read_Digital  (commandData, result); }
void Read_Analog   (COMMAND_DATA* commandData, uint8_t* result) { Cmd_Read_Analog   (commandData, result); }
#ifdef ONEW
void Get_1w (COMMAND_DATA* commandData, uint8_t* result) { Cmd_Get_1w (commandData, result); }
void Set_1w (COMMAND_DATA* commandData, uint8_t* result) { Cmd_Set_1w (commandData, result); }
#endif


/* *****************************************************************************
 End of File
 */
     