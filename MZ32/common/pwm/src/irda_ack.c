/*!\name      irda_ack.c
 *
 * \brief     IrDA Pic - 
 *
 * \author    Juan Gago
 */

#include <stdio.h>
#ifndef MOCK
    #include "callback.h"
    #include "irda_ack.h"
    #include "irda_pic.h"
    #include "Blade.h"
    #include "Crc16.h"
    #include "Misc.h"
#endif

extern uint8_t Scheduled_Message[5][MAX_NUM_OF_BYTES];
extern uint8_t HB_Beacons[], HB_Counter[], Scheduled_Channel[];
extern uint8_t BladeCalibrated[];

/* update channel from command ATx */ 
bool __AckPreprocessor (char key, char* command, uint8_t* channel)
{
    uint8_t len = strlen(command);
    if ( (len==3) && (command[0] == key)) {
        *channel = command[2] - '0'; 
        return true;
    }
    return false;
}  

bool __BeaconPrecheck ( uint8_t addr, uint8_t device, uint16_t id, uint16_t old_id, 
                        IRDA_STATUS_BYTE status, uint8_t* result)
{        
    if (id == 0xFFFF) {
    #ifdef VERBO
        sprintf (result, CONSOLE_ERR_ACK_ID_FFFF, addr, id);
    #endif 
        return false;
    }
    
    if (old_id != id) {
    #ifdef VERBO
        sprintf (result, CONSOLE_ERR_ACK_INVALID_ID, addr, id, old_id);
    #endif 
        BLADE_Uncalibrate(address2channel(addr));
        BLADE_Uncalibrate(device);
        return false;
    }  
    
    if (status.bits.irdaStable == false) {
    #ifdef VERBO
        sprintf (result, CONSOLE_ERR_ACK_INVALID_STATUS, addr, status.irdaStatus);
    #endif 
        BLADE_Uncalibrate(address2channel(addr));
        BLADE_Uncalibrate(device);
        return false;
    }    
    return true;
}

bool IRDA_UpdateTable (uint8_t addr, uint8_t device, uint16_t id, char status, uint8_t* result)
{
    bool retVal = false;
    
    IRDA_STATUS_BYTE old_status, new_status; 
    new_status.irdaStatus = status; 
#ifndef MOCK
    old_status = BLADE_ReadState(address2channel(addr));     // No compile in C99 -fms-extensions  
#endif
    uint16_t old_id = BLADE_ReadID(address2channel(addr));
    uint8_t counter = BLADE_ReadCounter(address2channel(addr));
    
    BLADE_WriteID (address2channel(addr), id);       
    BLADE_WriteState (address2channel(addr), new_status);

    if (old_id == 0x0) {
        sprintf (result, "\nTraining channel %d", addr);
        return true;
    }
    if (false == __BeaconPrecheck (addr, device, id, old_id, new_status, result)) {
        return false;
    }

    uint8_t result_verbo [MAX_NUM_OF_BYTES];
    if ( (old_id == id) && new_status.bits.irdaStable && (counter<3) ) {
        sprintf (result_verbo, "\nIncremented %d: %d", addr, counter+1);
        retVal = true;
    }
    else if ( (old_id == id) && new_status.bits.irdaStable ) { // && counter == 3
        sprintf (result_verbo, "\nUpdated register %d %04X", addr, id);
        BLADE_Calibrate(address2channel(addr));
        retVal = true;
    }

#ifdef VERBO
    sprintf(result, result_verbo);
#endif    
    return retVal;
}  

bool IRDA_ConfirmID (uint8_t addr, uint8_t device, uint16_t id, char status, uint8_t* result)
{
    bool retVal = false;

    IRDA_STATUS_BYTE old_status, new_status; 
    new_status.irdaStatus = status; 
#ifndef MOCK
    old_status = BLADE_ReadState(address2channel(addr));     // No compile in C99 -fms-extensions  
#endif
    uint16_t old_id = BLADE_ReadID(address2channel(addr));
    
    if (false == __BeaconPrecheck (addr, device, id, old_id, new_status, result)) {
        return false;
    }
    
    uint8_t result_verbo [MAX_NUM_OF_BYTES];
    if ( (id == old_id) && new_status.bits.irdaStable ) // Step 2: BC acknowledge (AC)
    {
        uint8_t i = address2channel(addr);
        if (new_status.bits.messageWaiting || Scheduled_Channel[i-1]) 
        {
            if (new_status.bits.messageWaiting) // Read or Send MR and MS 
            {
                Create_Message('R', i, Scheduled_Message[i-1]);
                sprintf (result_verbo, "\nAC%d: %s", addr, Scheduled_Message[i-1]); 
            }
            else // Scheduled_Channel[i-1]
            {
                Scheduled_Channel[i-1] = 0;
                Create_Message('S', i, Scheduled_Message[i-1]);                
                sprintf (result_verbo, "\nAC%d: %s", addr, Scheduled_Message[i-1]); 
            }

        #ifndef MOCK
            IRDA_QueueToSend(i, Scheduled_Message[i-1]); 
        #endif
        }
        else {
            sprintf (result_verbo, "\nAC%d: Calibrated (%04X)", addr, id);            
        }
        retVal = true;
    } 

#ifdef VERBO
    sprintf(result, result_verbo);
#endif
    return retVal;
}

bool AckProcessor_5argc (char* command, char* id, char* status, uint8_t device, uint8_t* result)
{    
    uint16_t new_id = hexa2decimal(id);
    uint8_t address;   
    uint8_t i = 1;
    if (__AckPreprocessor(CONSOLE_IR_BEACON_ACK, command, &address))
    {
        if (command[i] == 'T') {
            IRDA_UpdateTable (address, device, new_id, ascii2status(status[0]), result);  
            return true;
        }
        else if (command[i] == 'C') {
            IRDA_ConfirmID (address, device, new_id, ascii2status(status[0]), result); 
            return true;
        }
        else if (command[i] == 'S')  { // Step 4: Message ACK
            sprintf (result, "%s%s %s %s", CONSOLE_U0_REPLY_START, command, id, status);  
            return true;
        }              
    }
    // otherwise
    sprintf(result, "%s%c %d %s", CONSOLE_U0_REPLY_START, command[i], address, id);  
    return false;
}

bool AckProcessor_6argc (char* command, char* id, char* status, char* tail, uint8_t* result)
{    
    uint16_t new_id = hexa2decimal(id);
    uint8_t address; 

    if (__AckPreprocessor(CONSOLE_IR_BEACON_ACK, command, &address))
    {
        if (command[1] == 'R') { // Step 4: Message ACK
            sprintf (result, "%s%s %s %s %s", CONSOLE_U0_REPLY_START, command, id, status, tail);  
            return true;
        }              
    }
    // otherwise
    sprintf(result, "%s%c %d %s", CONSOLE_U0_REPLY_START, command[1], address, id);  
    return false;    
}

bool IRDA_Uart0Message (COMMAND_DATA* commandData, uint8_t device, uint8_t* result)
{
    bool retVal = false;

    if (commandData->argc > 5) {   
        retVal = AckProcessor_6argc (commandData->argv[1], commandData->argv[2], 
                                    commandData->argv[3], commandData->argv[4], result);    
    }  
    else if (commandData->argc == 5) {   
        retVal = AckProcessor_5argc (commandData->argv[1], commandData->argv[2], 
                                    commandData->argv[3], device, result);       
    }   
    else {
        sprintf(result, "%sNACK", CONSOLE_U0_REPLY_START);  
    }
    return retVal;
}

/* *****************************************************************************
 * End of File
 *
 */
