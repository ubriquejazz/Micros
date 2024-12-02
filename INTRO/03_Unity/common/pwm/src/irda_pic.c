/*!\name      irda_pic.c
 *
 * \brief     IrDA Pic
 *
 * \author    Juan Gago
 */

#include <stdio.h>
#ifndef MOCK
    #include "irda_pic.h"
    #include "blade.h"
    #include "pwm.h"
    #include "Blade.h"
    #include "Crc16.h"
    #include "Misc.h"

    #include "irda_2channel.h"
    #include "irda_3channel.h"

    extern APP_DATA     appData;
    extern UART_DATA    uart1Data, uart2Data;
    extern UART_DATA    uart3Data, uart4Data, uart5Data;

    bool IRDA_MessageSent(uint8_t device, uint32_t time, uint32_t timeout)
    {        
        bool retVal = false;
        switch(device)
        {
            case 1:    
                retVal = (uart1Data.state == UART_STATE_DONE);
                break;            
            case 2:    
                retVal = (uart2Data.state == UART_STATE_DONE);
                break;            
            case 3:    
                retVal = (uart3Data.state == UART_STATE_DONE);
                break;            
            case 4:    
                retVal = (uart4Data.state == UART_STATE_DONE);
                break;            
            case 5:    
                retVal = (uart5Data.state == UART_STATE_DONE);
                break;            
            default: 
                Nop();
                break;
        }        
        if (retVal  || (time > timeout)) {
            if (time == 1 + timeout) {
                appData.error_count++;
            }
            retVal = true;        
        }     
        return retVal;
    }

    bool IRDA_Add_Message (uint8_t device, char* message)
    {
        bool retVal = true;            
        switch(device)
        {
            case 1:    
                UART_Object_ResendBuffer(&uart1Data, message);
                break;
            case 2:
                UART_Object_ResendBuffer(&uart2Data, message);
                break;
            case 3:
                UART_Object_ResendBuffer(&uart3Data, message);
                break;
            case 4:
                UART_Object_ResendBuffer(&uart4Data, message);
                break;
            case 5:    
                UART_Object_ResendBuffer(&uart5Data, message);
                break;            
            default: 
                retVal = false;
                break;
        } 
        return retVal;
    }

    /* State Machines */

    void IRDA_2channel_Setup(bool ledOn)
    {
        (ledOn)? DBGL_3On() : DBGL_3Off();
        PLIB_USART_ReceiverDisable(IRDA_ADDR_3);
        PLIB_USART_ReceiverDisable(IRDA_ADDR_4);
        PLIB_USART_ReceiverDisable(IRDA_ADDR_5);        
    }

    void IRDA_3channel_Setup(bool ledOn)
    {
        (ledOn)? DBGL_3On() : DBGL_3Off();
        PLIB_USART_ReceiverDisable(IRDA_ADDR_1);
        PLIB_USART_ReceiverDisable(IRDA_ADDR_2);        
    }

#endif

extern uint8_t Scheduled_Message[5][MAX_NUM_OF_BYTES];
extern uint8_t Scheduled_Channel[];

uint8_t infrared_silence[] = {CONST_BLANK, CONST_BLANK};

void Create_Beacon(uint8_t device, uint8_t* beacon)
{
    uint8_t addr = channel2address(device);
    sprintf(beacon, CONSOLE_IR_REQ_BC, addr, BLADE_ReadID(device));
    AppendCrc(beacon);
}

void Create_Message(char key, uint8_t device, uint8_t* message)
{
    uint8_t addr = channel2address(device);
    if (key  == 'R')  {        
        sprintf(message, CONSOLE_IR_REQ_MR, addr, BLADE_ReadID(device)); 
    }
    else if (key == 'S') {
        sprintf(message, CONSOLE_IR_REQ_MS, addr, BLADE_ReadID(device), CONSOLE_EXAMPLE_COMMAND);
    }
    AppendCrc(message);    
}

bool IRDA_Add_BeaconAndMessage (char key, uint8_t i, uint8_t* result)
{
    bool retVal = false;
    uint8_t result_int [MAX_NUM_OF_BYTES];  
    uint8_t beacon[MAX_NUM_OF_BYTES];
    uint8_t addr = channel2address(i);           // device == i
    if (BLADE_ReadCalibrated(i)) {   
        Create_Beacon(i, beacon);
        Create_Message(key, i, Scheduled_Message[i-1]);
        Scheduled_Channel[i-1] = 1;
        sprintf(result_int, "%s\r%s", beacon, infrared_silence);
        retVal = true;
    }
    else { // BT
        sprintf(result_int, CONSOLE_IR_REQ_BT, addr); 
        AppendCrc(result_int); 
        retVal = true;
    }
#ifdef MOCK
    sprintf(result, "%s", result_int);
#else
    IRDA_Add_Message(i, result_int);    // Step 1: Beacon
#endif
    return retVal;
}

bool IRDA_Add_Beacon (uint8_t i, uint8_t j, char pic_address, uint8_t* result)
{
    bool retVal = false;
    uint8_t result_int[MAX_NUM_OF_BYTES];    
    uint8_t addr = channel2address(i);           // device == i
    if (BLADE_ReadCalibrated(i)) {   
        sprintf(result_int, CONSOLE_IR_REQ_BC, addr, BLADE_ReadID(i)); // pic_address
        AppendCrc(result_int);
        retVal = true;
    }
    else { // BT
        sprintf(result_int, CONSOLE_IR_REQ_BT, addr); 
        AppendCrc(result_int);
        retVal = true;
#ifdef MOCK
    }
    sprintf(result, "%s", result_int);
#else
    }
    IRDA_Add_Message (i, result_int);
#endif
    return retVal;
}

/* *****************************************************************************
 * End of File
 */
