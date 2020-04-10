/*!\name      callback.c
 *
 * \brief     Callback functions
 *
 * \author    Juan Gago
 */

#include <stdio.h>
#include "callback.h"
#include "pwm.h"
#include "uart.h"
#include "command.h"
#include "irda_ack.h"

extern APP_DATA     appData;
extern UART_DATA    uart0Data;
COMMAND_DATA        commandDataIR;

/* Callback Helpers */
bool DisableUsartModule(uint32_t base_address)
{
    bool retVal = true;

    PLIB_USART_ReceiverParityErrorHasOccurred(base_address);   // Reading the parity error will clear it
    PLIB_USART_ReceiverFramingErrorHasOccurred(base_address);  // Reading the framing error will clear it
    PLIB_USART_ReceiverOverrunErrorClear(base_address);        // Clear Overrun error    

    switch(base_address)
    {
        case IRDA_ADDR_1:    
            DRV_USART1_Close();
            DRV_USART1_Deinitialize();    
            DRV_USART1_Initialize();    
            break;  
        case IRDA_ADDR_2:    
            DRV_USART2_Close();
            DRV_USART2_Deinitialize();    
            DRV_USART2_Initialize();    
            break; 
        case IRDA_ADDR_3:    
            DRV_USART3_Close();
            DRV_USART3_Deinitialize();    
            DRV_USART3_Initialize();    
            break; 
        case IRDA_ADDR_4:    
            DRV_USART4_Close();
            DRV_USART4_Deinitialize();    
            DRV_USART4_Initialize();    
            break; 
        case IRDA_ADDR_5:    
            DRV_USART5_Close();
            DRV_USART5_Deinitialize();    
            DRV_USART5_Initialize();    
            break;             
        default: 
            retVal = false;
            break;
    }   
    return retVal;
}

#ifdef DUMMY 

bool DUMMY_Message_Debugger(char* buffer, uint32_t len, uint8_t device)
{
    uint8_t debuginfo[MAX_NUM_OF_BYTES];         
    
    if (buffer[0] != CONST_DOLLAR)
    {
        sprintf(debuginfo, "[%d ms] %s\r\n", appData.milliseconds, buffer);            
        UART_Object_ResendBuffer(&uart0Data, debuginfo);             
    }       
    else if (buffer[1] == 'M')
    {
        sprintf(debuginfo, "[%d] M%c%d %d\r\n", appData.milliseconds, buffer[2], device, len);
        UART_Object_ResendBuffer(&uart0Data, debuginfo);                         
    }   
    return true;
}

bool DUMMY_Message_Processor (UART_DATA* ptr, uint16_t crc, uint8_t* message)
{
    bool retVal = true;
    if (commandDataIR.checksum == 0)
    {                    
        UART_Object_CmdExecute(ptr, &commandDataIR);
    }
    else if (commandDataIR.checksum != crc)
    {
        sprintf (message, CONSOLE_ERR_FAILING_CRC_CHECK, commandDataIR.checksum, crc);
        UART_Object_ResendBuffer(ptr, message);            
        retVal = false;
    }
    else
    {
        UART_Object_CmdExecute(ptr, &commandDataIR);
    }       
    return retVal;
}

void DUMMY_Callback_Helper (UART_DATA* ptr, uint8_t c)
{    
    if ( (c < 0x80) && (c != 0x00) )
    { 
        ptr->error_count = 0;        
        if (c == CONST_DOLLAR)
        {
            appData.milliseconds = 0;
            memset(ptr->rx_buf, 0, COMMAND_SIZE);
            ptr->rx_buf[0] = CONST_DOLLAR;
            ptr->rx_count = 1;
        } 
        else if(c != CONST_CR) 
        {
            ptr->rx_buf [ptr->rx_count++] = c;    
        }                      
        else 
        {   
            uint8_t message[MAX_NUM_OF_BYTES]; 
            memset(message, 0, MAX_NUM_OF_BYTES);
            if (ptr->rx_count > 4)
            {
                uint32_t crc_start = ptr->rx_count-4;
                uint16_t crc = CalculateCrc(ptr->rx_buf, crc_start);            
                DUMMY_Message_Debugger (ptr->rx_buf, ptr->rx_count, ptr->driverIndex);                               
                // 1. CmdUpdate() - rx_buf corrupted
                if (UART_Object_CmdUpdate(ptr, &commandDataIR))
                {
                    // 2. CrcUpdate() - true if the last argument is a 4 digit CRC
                    if ( UART_Object_CrcUpdate(ptr, &commandDataIR) )            
                    {
                        DUMMY_Message_Processor (ptr, crc, message);
                    }
                    else
                    {
                        sprintf (message, CONSOLE_ERR_INVALID_CRC_FORMAT, commandDataIR.argv[0]);    
                        UART_Object_ResendBuffer(ptr, message);            
                    }   
                } 
            }
            else // 0. End rx_count < 4
            {
                sprintf (message, CONSOLE_ERR_SIMPLE_MESSAGE, ptr->rx_buf);    
                UART_Object_ResendBuffer(ptr, message);
            }             
            ptr->state = UART_STATE_INIT;
            memset(ptr->rx_buf, 0, COMMAND_SIZE);
            ptr->rx_count = 0;            
        }   
    }
    else
    {
        ptr->error_count++;
    }   
}

#else

bool IRDA_Message_Processor (uint8_t device, uint16_t crc, uint8_t* message)
{
    bool retVal = true;
    if (commandDataIR.checksum == 0)
    {                    
        IRDA_Uart0Message(&commandDataIR, device, message);
    }
    else if (commandDataIR.checksum != crc)
    {
        //  (message, CONSOLE_ERR_FAILING_CRC_CHECK, commandDataIR.checksum, crc);
        retVal = false;
    }
    else
    {
        IRDA_Uart0Message(&commandDataIR, device, message);                        
    } 
    return retVal;
}

bool IRDA_Message_Debugger(char* buffer)
{
    uint8_t debuginfo[MAX_NUM_OF_BYTES];     
    bool isNotAT = (buffer[3] != 'T');
    bool isNotAC = (buffer[3] != 'C');
    
    if (isNotAT && isNotAC) 
    {
        sprintf(debuginfo, "[%d ms] %s\r\n", 1, buffer);            
        UART_Object_ResendBuffer(&uart0Data, debuginfo);     
    }    
    return false;    
}

void IRDA_Callback_Helper (UART_DATA* ptr, uint8_t c)
{        
    if ( (c < 0x80) && (c != 0x00) )
    { 
        ptr->error_count = 0;
        if(c != CONST_CR) 
        {
            ptr->rx_buf [ptr->rx_count++] = c;
        }
        else 
        {   
            uint8_t message[MAX_NUM_OF_BYTES];
            memset(message, 0, MAX_NUM_OF_BYTES);
            if (ptr->rx_count > 4)    
            {
                uint32_t crc_start = ptr->rx_count-4;
                uint16_t crc = CalculateCrc(ptr->rx_buf, crc_start);
                uint8_t device = ptr->driverIndex;                
                IRDA_Message_Debugger (ptr->rx_buf);

                // 1. CmdUpdate() - rx_buf corrupted 
                if (UART_Object_CmdUpdate(ptr, &commandDataIR))
                {
                    // 2. CrcUpdate() - true if the last argument is a 4 digit CRC
                    if ( UART_Object_CrcUpdate(ptr, &commandDataIR) ) 
                    {
                        HB_NewCallback (device);
                        IRDA_Message_Processor (device, crc, message);
                    }
                }
            }            
            // UART_Object_ResendBuffer(&uart0Data, message); 
            // ptr->state = UART_STATE_INIT;
            memset(ptr->rx_buf, 0, COMMAND_SIZE);
            ptr->rx_count = 0;
        } 
    }
    else
    {
        ptr->error_count++;
    }       
}
#endif

/* *****************************************************************************
 * End of File
 */
     