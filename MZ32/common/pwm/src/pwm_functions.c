/*!\name      functions.c
 *
 * \brief     Local functions
 *
 * \author    Juan Gago
 */

#include <stdio.h>

#ifndef MOCK
    #include "pwm_functions.h"
    #include "pwm.h"
    #include "ir_core.h"

    extern APP_DATA appData;

    #ifdef DEV
        #include "uart.h"
        #include "command.h"
        extern UART_DATA    uart0Data;
        extern COMMAND_DATA commandData;
    #endif

    void PWM_OC_Start(void)
    {
        DRV_OC0_Start();
        DRV_OC1_Start();
        DRV_OC2_Start();
        DRV_OC3_Start();
        DRV_OC4_Start(); 
    }

    void PWM_OC_PWSet(int value)
    {
        DRV_OC0_PulseWidthSet(value);
        DRV_OC1_PulseWidthSet(value);
        DRV_OC2_PulseWidthSet(value);
        DRV_OC3_PulseWidthSet(value);
        DRV_OC4_PulseWidthSet(value); 
    }
    
    uint8_t IRDA_GetPicPic(char pic_address)
    {
        uint8_t led_a, led_b;   // Inputs with Pull-Up Resistor
        if (pic_address == 'A')
        {
            led_a = SYS_PORTS_PinRead(PORTS_ID_0, PIC_PIC_IO_5_PORT, PIC_PIC_IO_5_PIN); 
            return led_a;                 
        }
        if (pic_address == 'B')
        {
            led_b = SYS_PORTS_PinRead(PORTS_ID_0, PIC_PIC_IO_4_PORT, PIC_PIC_IO_4_PIN); 
            return led_b;
        }   
        return -1;
    }

    void IRDA_SetPicPic(int input, char pic_address)
    {
        // Open Drain Outputs
        if ('A' == pic_address)
        {
            // (1 == input)? PIC_PIC_IO_6On() : PIC_PIC_IO_6Off();
            SYS_PORTS_PinDirectionSelect (PORTS_ID_0, SYS_PORTS_DIRECTION_OUTPUT,
                PIC_PIC_IO_6_PORT, PIC_PIC_IO_6_PIN);
            (1 == input)?
                SYS_PORTS_PinSet(PORTS_ID_0, PIC_PIC_IO_6_PORT, PIC_PIC_IO_6_PIN):
                SYS_PORTS_PinClear(PORTS_ID_0, PIC_PIC_IO_6_PORT, PIC_PIC_IO_6_PIN); 
        }

        if ('B' == pic_address)
        {
            // (1 == input)? PIC_PIC_IO_7On() : PIC_PIC_IO_7Off();
            SYS_PORTS_PinDirectionSelect (PORTS_ID_0, SYS_PORTS_DIRECTION_OUTPUT,
                PIC_PIC_IO_7_PORT, PIC_PIC_IO_7_PIN);
            (1 == input)?
                SYS_PORTS_PinSet(PORTS_ID_0, PIC_PIC_IO_7_PORT, PIC_PIC_IO_7_PIN):
                SYS_PORTS_PinClear(PORTS_ID_0, PIC_PIC_IO_7_PORT, PIC_PIC_IO_7_PIN);   
        }
    }

    uint8_t FAN_SetPicPic(int input, char pic_address)
    {
        if (pic_address == 'A')
        {
            // (1 == input)? PIC_PIC_IO_5On() : PIC_PIC_IO_5Off();
            (1 == input)?
                SYS_PORTS_PinSet(PORTS_ID_0, PIC_PIC_IO_5_PORT, PIC_PIC_IO_5_PIN):
                SYS_PORTS_PinClear(PORTS_ID_0, PIC_PIC_IO_5_PORT, PIC_PIC_IO_5_PIN);
        }
        if (pic_address == 'B')
        {
            // (1 == input)? PIC_PIC_IO_4On() : PIC_PIC_IO_4Off();
            (1 == input)?
                SYS_PORTS_PinSet(PORTS_ID_0, PIC_PIC_IO_4_PORT, PIC_PIC_IO_4_PIN):
                SYS_PORTS_PinClear(PORTS_ID_0, PIC_PIC_IO_4_PORT, PIC_PIC_IO_4_PIN);
        }
        return 0;
    } 
#endif

extern uint8_t Scheduled_Message[5][MAX_NUM_OF_BYTES];
extern uint8_t Scheduled_Channel[];
    
/* ***************** **** ***************** */
/* ***************** IRDA ***************** */
/* ***************** **** ***************** */

bool IRDA_ReadAnalog()
{ 
#ifdef MOCK    
    DPRINTF("NULL\n\r");
    return false;
#else
    bool success = false;

    if(true == DRV_ADC_SamplesAvailable(40))
    {
        appData.adc_buffer[0] = DRV_ADC_SamplesRead(40);
        success = true;
    }
    
    if(true == DRV_ADC_SamplesAvailable(41))
    {
        appData.adc_buffer[1] = DRV_ADC_SamplesRead(41);
        success = true;
    }
    
    if(true == DRV_ADC_SamplesAvailable(42))
    {
        appData.adc_buffer[2] = DRV_ADC_SamplesRead(42);
        success = true;
    }
    
    if(true == DRV_ADC_SamplesAvailable(7))
    {
        appData.adc_buffer[3] = DRV_ADC_SamplesRead(7);
        success = true;
    }
    
    if(true == DRV_ADC_SamplesAvailable(8))
    {
        appData.adc_buffer[4] = DRV_ADC_SamplesRead(8);
        success = true;
    }

    return success;
#endif
}

uint8_t IRDA_Get_Analog_Channels(uint8_t *result)
{
#ifdef MOCK    
    DPRINTF("NULL\n\r");
    return 0;
#else
    sprintf (result, "%d %d %d %d %d \r\n", 
              appData.adc_buffer[0], appData.adc_buffer[1], 
              appData.adc_buffer[2], appData.adc_buffer[3], 
              appData.adc_buffer[4]);
    return 0;
#endif
}

void IRDA_SetTxLevel_Simple(uint8_t channel, int width)
{
#ifdef MOCK    
    DPRINTF("NULL\n\r");
#else
    switch (channel)
    {
        case 1:                            
            DRV_OC0_PulseWidthSet(width);
            break;
        case 2:                            
            DRV_OC1_PulseWidthSet(width);
            break;
        case 3:                            
            DRV_OC2_PulseWidthSet(width);
            break;
        case 4:                            
            DRV_OC3_PulseWidthSet(width);
            break;
        case 5:                            
            DRV_OC4_PulseWidthSet(width);
            break;
        case 0:                            
            DRV_OC0_PulseWidthSet(width);
            DRV_OC1_PulseWidthSet(width);
            DRV_OC2_PulseWidthSet(width);
            DRV_OC3_PulseWidthSet(width);
            DRV_OC4_PulseWidthSet(width);
            break;          
        default:
            break;            
    } // end switch
#endif
}

void IRDA_SetTxLevel(uint8_t channel, int width, uint8_t* result)
{
#ifdef MOCK    
    sprintf(result, "All the transmitters set to %d", width);
#else
    IRDA_SetTxLevel_Simple(channel, width);
    switch (channel)
    {
        case 1:                            
            sprintf(result, "IR transmitter A set to %d", width);
            break;
        case 2:                            
            sprintf(result, "IR transmitter B set to %d", width);
            break;
        case 3:                            
            sprintf(result, "IR transmitter C set to %d", width);
            break;
        case 4:                            
            sprintf(result, "IR transmitter D set to %d", width);
            break;
        case 5:                            
            sprintf(result, "IR transmitter E set to %d", width);
            break;
        case 0:                            
            sprintf(result, "All the transmitters set to %d", width);
            break;          
        default:
            break;            
    } // end switch
#endif
}

bool IRDA_Send_Message (char* command, uint8_t channel, uint8_t* result)
{    
    uint8_t result_verbo[MAX_NUM_OF_BYTES];
    bool retVal = true;
    int i = 4; // command[] = "LCL_MR"
    
    if (command[i] == 'M') 
    {
        if (command[i+1] == 'R') 
        {
            #ifdef MOCK    
                #ifdef PROD0 
                sprintf(result_verbo, "Sent: ! MR%d 1234", channel); 
                #else
                sprintf(result_verbo, "Sent: $MR%d 1234", channel); 
                #endif
            #else 
                if ( (appData.edge_count != 2*channel) && APP_isUptime() )
                {
                    appData.irMessage = 'R';
                    appData.irChannel = channel;                    
                    sprintf(result_verbo, "Sent: ! MR%d <id>", channel);
                }
                else
                {
                    sprintf(result, "Channel %d is busy", channel);
                    return true;
                }                
            #endif
        }
        else if (command[i+1] == 'S') 
        {
            #ifdef MOCK  
                #ifdef PROD0  
                sprintf(result_verbo, "Sent: ! MS%d 1234 %s", channel, CONSOLE_EXAMPLE_COMMAND);
                #else
                sprintf(result_verbo, "Sent: $MS%d 1234 %s", channel, CONSOLE_EXAMPLE_COMMAND);
                #endif
            #else                 
                if ( (appData.edge_count != 2*channel) && APP_isUptime() )
                {
                    appData.irMessage = 'S';
                    appData.irChannel = channel;
                    sprintf(result_verbo, "Sent: ! MS%d <id> %s", channel, CONSOLE_EXAMPLE_COMMAND);
                }
                else
                {
                    sprintf(result, "Channel %d is busy", channel); 
                    return true;
                }
            #endif
        }
        else
        {
            retVal = false;
        }
    }
    else
    {
        retVal = false;
    }

    
#ifdef VERBO
    sprintf(result, result_verbo);
#else    
    sprintf(result, "Sent!");
#endif
    return retVal;
}

bool FAN_Send_Message (char* command, uint8_t address, uint8_t* result)
{ 
    uint8_t result_verbo[MAX_NUM_OF_BYTES];
    bool retVal = true;
    int i = 4; // command[] = "RMT_MR"

    sprintf(result, "Address %d is busy", address); 
    return retVal;
}

/* ***************** *** ***************** */
/* ***************** FAN ***************** */
/* ***************** *** ***************** */

void FAN_GetSpeed(uint8_t bank, uint8_t* result)
{
#ifdef MOCK    
     sprintf (result, "[None] 0 0 0 0 0");
#else
    switch (bank)
    {
        case 1:
            sprintf (result, "[B1] %d %d %d %d", 
                    appData.fan_rpm[0], appData.fan_rpm[1],
                    appData.fan_rpm[2], appData.fan_rpm[3]);
            break;
        
        case 2:  
            sprintf (result, "[B2] %d %d %d %d", 
                    appData.fan_rpm[4], appData.fan_rpm[5],
                    appData.fan_rpm[6], appData.fan_rpm[7]);
            break;
        
        case 3:
            sprintf (result, "[B3] %d %d %d %d", 
                    appData.fan_rpm[8], appData.fan_rpm[9],
                    appData.fan_rpm[10], appData.fan_rpm[11]);
            break;
        
        case 4:
            sprintf (result, "[B4] %d %d %d %d", 
                    appData.fan_rpm[12], appData.fan_rpm[13],
                    appData.fan_rpm[14], appData.fan_rpm[15]);
            break;
        
        case 5:  
            sprintf (result, "[B5] %d %d %d %d", 
                    appData.fan_rpm[16], appData.fan_rpm[17],
                    appData.fan_rpm[18], appData.fan_rpm[19]);
            break;
        
        default:
            sprintf (result, "[None] 0 0 0 0 0");
            break;
        
    } // end switch 
#endif
}

void FAN_SetSpeed(uint8_t bank, int width, uint8_t* result)
{
#ifdef MOCK    
    sprintf(result, "All the banks set to %d", width);
#else
    switch (bank)
    {
        case 1:                            
            DRV_OC0_PulseWidthSet(width);
            sprintf(result, "Bank A set to %d", width);
            break;

        case 2:                            
            DRV_OC2_PulseWidthSet(width);
            sprintf(result, "Bank B set to %d", width);
            break;

        case 3:                            
            DRV_OC1_PulseWidthSet(width);
            sprintf(result, "Bank C set to %d", width);
            break;
              
        case 4:                            
            DRV_OC3_PulseWidthSet(width);
            sprintf(result, "Bank D set to %d", width);
            break;

        case 5:                            
            DRV_OC4_PulseWidthSet(width);
            sprintf(result, "Bank E set to %d", width);
            break;
              
        case 0:                            
            DRV_OC0_PulseWidthSet(width);
            DRV_OC1_PulseWidthSet(width);
            DRV_OC2_PulseWidthSet(width);
            DRV_OC3_PulseWidthSet(width);
            DRV_OC4_PulseWidthSet(width);
            sprintf(result, "All the banks set to %d", width);
            break;

        default:
            break;
    }  // end switch
#endif
}

/*** IRDA Channel 1:5 ****/

void __Cmd_Channel_Helper (COMMAND_DATA* commandData, uint8_t* result)
{
    int argc = commandData->argc;
    if (argc < 2)
    {
       sprintf(result, "%s <irda_ch>", commandData->argv[0]);
    }
    else
    {
        int channel = atoi(commandData->argv[1]);
        if (channel >= 1 && channel <= 5)
            IRDA_Send_Message(commandData->argv[0], channel, result);
        else
            sprintf(result, "<irda_ch> = %s", commandData->argv[1]);
    }
    strcat(result, "\r\n");      
}

/*** FAN Addresses 0:9 ****/

void __Cmd_Address_Helper (COMMAND_DATA* commandData, uint8_t* result)
{
    int argc = commandData->argc;
    if (argc < 2)
    {
       sprintf(result, "%s <irda_addr>", commandData->argv[0]);
    }
    else
    {
        int addr = atoi(commandData->argv[1]);
        if (addr >= 0 && addr <= 9)
            FAN_Send_Message(commandData->argv[0], addr, result);
        else
            sprintf(result, "<irda_addr> = %s", commandData->argv[1]);
    }
    strcat(result, "\r\n");         
}

/* IRDA */

void Cmd_Ch_BeaconMR (COMMAND_DATA* commandData, uint8_t* result)
{
    __Cmd_Channel_Helper (commandData, result);
}

void Cmd_Ch_BeaconMS (COMMAND_DATA* commandData, uint8_t* result)
{
    __Cmd_Channel_Helper (commandData, result);
}

/* FAN */

void Cmd_Addr_BeaconMR (COMMAND_DATA* commandData, uint8_t* result)
{
    __Cmd_Address_Helper (commandData, result);
}

void Cmd_Addr_BeaconMS (COMMAND_DATA* commandData, uint8_t* result)
{
    __Cmd_Address_Helper (commandData, result);
}

/** Setter Functions (DEV) **/

void Cmd_Set_Power(COMMAND_DATA* commandData, uint8_t* result)
{
    int  pulse_width;     // min 500 max 1
    int argc = commandData->argc;  
    if (argc < 3)
    {
       sprintf(result, "%s <irda_ch> <pwm>", commandData->argv[0]);
    }
    else
    {    
        pulse_width = atoi(commandData->argv[2]);  
        if (pulse_width >= 0 && pulse_width <= 500)
        {
            IRDA_SetTxLevel(atoi(commandData->argv[1]), pulse_width, result);
        }
    }
    strcat(result, "\r\n");
}

void Cmd_Set_Speed(COMMAND_DATA* commandData, uint8_t* result)
{
    int  pulse_width;     // min 500 max 1
    int argc = commandData->argc;    
    if (argc < 3)
    {
       sprintf(result, "%s <bank> <pwm>", commandData->argv[0]);
    }
    else
    {    
        pulse_width = atoi(commandData->argv[2]);  
        if (pulse_width >= 0 && pulse_width <= 500)
        {
            FAN_SetSpeed(atoi(commandData->argv[1]), pulse_width, result);
        }
    }
    strcat(result, "\r\n");
}

/*** Getter Functions (DEV) ***/

void Cmd_Get_Power(COMMAND_DATA* commandData, uint8_t* result)
{    
    int argc = commandData->argc;   
    if (argc < 2)
    {
        sprintf(result, "%s <irda_ch>\r\n", commandData->argv[0]);
    }
    else if ( IRDA_ReadAnalog() )
    {
       IRDA_Get_Analog_Channels(result);
    }
    else
    {
      sprintf(result, "[ERROR] ReadAnalog command\r\n");
    }
}

void Cmd_Get_Speed(COMMAND_DATA* commandData, uint8_t* result)
{   
    int argc = commandData->argc;       
    if (argc < 2)
    {
       sprintf(result, "%s <bank>", commandData->argv[0]);
    }
    else if ( 1 != strlen(commandData->argv[1]) )
    {
      sprintf(result, "%s - bank should be 0:4", commandData->argv[1]);   
    }
    else
    {
        FAN_GetSpeed( atoi(commandData->argv[1]), result);        
    }
    strcat(result, "\r\n");      
}