/*!\name      irda_dummy.c
 *
 * \brief     Dummy Pic - irda_pic.h should be included
 *
 * \author    Juan Gago
 */

#include <stdio.h>
#ifndef MOCK
    #include "callback.h"
    #include "irda_dummy.h"
    #include "irda_pic.h"
    #include "Blade.h"
    #include "Crc16.h"
    #include "Misc.h"
#endif

IRDA_STATUS_BYTE status;

bool DUMMY_UpdateStatus ()
{    
    bool retVal = false;
    status.irdaStatus = 0x00;
    status.bits.irdaStable = true;
    return retVal;
}

uint16_t DummyID[] = {0x1234, 0x1234, 0xFFFF, 0x1234, 0x1234};

/* update channel from command $BTx. deduce id and status */
bool __Preprocessor(char key, char* command, int i, uint8_t* addr, uint16_t* id)
{
    uint8_t len = strlen(command);
    if ( (len==4) && (command[i+0] == key) ) {
        *addr = command[i+2] - '0';
        uint8_t device = address2channel(*addr);        
        *id = DummyID[device-1];        
        DUMMY_UpdateStatus();
        return true;
    }
    return false;
}

bool BeaconProcessor_3argc (char* command, uint8_t* result)
{    
    uint8_t addr;
    uint16_t id;
    uint8_t i = 1;
    if (__Preprocessor(CONSOLE_IR_BEACON_REQ, command, i, &addr, &id))
    {
        if (command[i+1] == 'C') {
            sprintf (result, CONSOLE_IR_ACK_BC, addr, id, 0x0F & status.irdaStatus); 
            return true; 
        }
        if (command[i+1] == 'T') {
            sprintf (result, CONSOLE_IR_ACK_BT, addr, id, 0x0F & status.irdaStatus);  
            return true;
        }
    }
    // otherwise
    sprintf(result, "%sNACK", CONSOLE_IR_REPLY_START); 
    return false;
}

bool MessageProcessor_4argc (char* command, uint8_t* result)
{    
    bool retVal = true;
    uint8_t addr;
    uint16_t id;
    uint8_t i = 1;
    if (__Preprocessor(CONSOLE_IR_MESSAGE_REQ, command, i, &addr, &id))
    {

        if (command[i+1] == 'S') {
            sprintf (result, CONSOLE_IR_ACK_MS, addr, id, 0x0F & status.irdaStatus);  
        }
        else if (command[i+1] == 'R') {
            sprintf( result, CONSOLE_IR_ACK_MR, addr, id, 0x0F & status.irdaStatus, CONSOLE_EXAMPLE_ANSWER); 
        }
        else {
            sprintf(result, "%sNACK", CONSOLE_IR_REPLY_START); 
            retVal = false;
        }        
    }
    else {
        sprintf(result, "%sNACK", CONSOLE_IR_REPLY_START); 
        retVal = false;
    }
    return retVal;
}

void DUMMY_Receive_BC(COMMAND_DATA* commandData, uint8_t* result)
{
    int argc = commandData->argc;    
    if (argc == 3) {        
        BeaconProcessor_3argc(commandData->argv[0], result);
    }
    else {
        sprintf(result, "%sNACK", CONSOLE_IR_REPLY_START);
    }
    AppendCrc(result);
    strcat(result, CONSOLE_IR_REPLY_END);
}

void DUMMY_Receive_BT(COMMAND_DATA* commandData, uint8_t* result)
{
    int argc = commandData->argc;    
    if (argc == 2) {
        BeaconProcessor_3argc(commandData->argv[0], result); 
    }
    else  {
        sprintf(result, "%sNACK", CONSOLE_IR_REPLY_START);
    }
    AppendCrc(result);
    strcat(result, CONSOLE_IR_REPLY_END);
}

void DUMMY_Receive_MS(COMMAND_DATA* commandData, uint8_t* result)
{
    int argc = commandData->argc;    
    if (argc == 4) {        
        MessageProcessor_4argc(commandData->argv[0], result);
    }
    else {
        sprintf(result, "%sNACK", CONSOLE_IR_REPLY_START);
    }
    AppendCrc(result);
    strcat(result, CONSOLE_IR_REPLY_END);
}

void DUMMY_Receive_MR(COMMAND_DATA* commandData, uint8_t* result)
{
    int argc = commandData->argc;    
    if (argc == 3) {        
        MessageProcessor_4argc(commandData->argv[0], result);
    }
    else {
        sprintf(result, "%sNACK", CONSOLE_IR_REPLY_START);
    }
    AppendCrc(result);
    strcat(result, CONSOLE_IR_REPLY_END);
}

/* *****************************************************************************
 * End of file
 */