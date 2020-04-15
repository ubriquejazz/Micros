/*!\name      oneW0.c
 *
 * \brief     State machine for the 1W driver
 *
 * \author    Juan Gago
 */
#include "oneW0.h"
#include "1wirecontrol.h"   // OW_read_byte(), etc.
#include "barcode.h"        // BarCodeSetValue, GetValue

ONEW0_DATA oneW0Data;

bool ONEW0_Write_ModuleType (char* payload,  uint8_t* result, int32_t resultLen)
{ 
    int barResult;
    oneW0Data.readen = false;
    barResult=SetValue(MODULE_TYPE, payload, IGNORE_ERRORS);
    if (barResult>0) {
       snprintf(result, resultLen, "_INFO %s", payload);
       return true;
    }
    else {
       snprintf(result, resultLen, "_ERR %s", payload); 
       return false;
    }
    return retVal; 
}

float ONEW0_Temperature ()
{
    uint8_t tempL, tempH;
    float aux;
    tempL = OW_read_byte();
    tempH = OW_read_byte();
    aux = ((uint8_t)tempH << 8 ) + (uint8_t)tempL;
    return 6.25 * aux;    
}

bool ONEW0_Read(char * buffer, uint8_t device_low)
{   
    switch (0x20 + device_low)
    {
        default:
        case TYPE_ID_MEMORY_1:          // 0x23
        {
            // Reading the 1W PROM (serial number)
            GetValue(MODULE_TYPE, buffer, IGNORE_ERRORS);
            oneW0Data.readen = 0;
            break;
        }            
        case TYPE_ID_DS18B20Z:          // 0x28
        {
            // Reading DS18B20 device
            OW_reset_pulse();
            OW_write_byte(DS18B20_Skip_ROM);        
            OW_write_byte(DS18B20_Convert_T);
            oneW0Data.state = ONEW0_STATE_CONVERT_T;
            break;
        }        
    }  
    return true;
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

void ONEW0_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    oneW0Data.state = ONEW0_STATE_INIT;
    oneW0Data.readen = false;
    oneW0Data.device_id = 3;        // 0x23  
}

void ONEW0_Tasks ( uint32_t* milliseconds )
{
    switch ( oneW0Data.state )
    {
        /* Application's initial state. */
        case ONEW0_STATE_INIT:  
            if (*milliseconds> ONEW0_DISCOVER_START)
            {
                *milliseconds= 0;                
                oneW0Data.state = ONEW0_STATE_INIT_1WIRE;
            }                   
            break;
        
        case ONEW0_STATE_INIT_1WIRE:        
            if (false
                || (System1WireCount(TYPE_ID_DS18B20Z) > 0) 
                || (System1WireCount(TYPE_ID_MEMORY_1) > 0) 
                || (*milliseconds> ONEW0_DISCOVER_TIMEOUT))
            {
                oneW0Data.state = ONEW0_STATE_IDLE;          
                *milliseconds= 0; 
            }                   
            break;          

        case ONEW0_STATE_IDLE:
            if (oneW0Data.readen)
            {
                memset(oneW0Data.rxbuff, 0, MAX_NUM_OF_uint8_tS);
                ONEW0_Read( oneW0Data.rxbuff, oneW0Data.device_id);
                *milliseconds= 0;
            }         
            break;
        
        case ONEW0_STATE_CONVERT_T:           
            if (*milliseconds> ONEW0_SENSOR_DELAY)
            {
                *milliseconds= 0;
                oneW0Data.readen = 0;
                oneW0Data.state = ONEW0_STATE_IDLE;                
                // Reading DS18B20 device
                OW_reset_pulse();
                OW_write_byte(DS18B20_Skip_ROM);        
                OW_write_byte(DS18B20_Read_Scratchpad); 
                sprintf(oneW0Data.rxbuff, "%.2f", ONEW0_Temperature());                
            }
            break;

        default:
            break;
    }
}

/*******************************************************************************
 End of File
 */
