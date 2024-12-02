/*!\name      Blade.c
 *
 * \brief     IrDA Pic - registers
 *
 * \author    Juan Gago, 2019
 */

#include <stdio.h>
#include "Global.h"

uint8_t  BladeCalibrated[] = {1, 0, 1, 0, 1};
uint8_t  BladeID_Counter[] = {3, 0, 3, 0, 3};

uint16_t BladePWR[]     = {10, 20, 10, 20, 10}; 
uint16_t BladeID[]      = {-1, -1, -1, -1, -1};
uint8_t  BladeState[]   = {-1, -1, -1, -1, -1}; // IRDA_STATUS_BYTE

uint8_t Scheduled_Message[5][MAX_NUM_OF_BYTES];
uint8_t Scheduled_Channel[] = {0,0,0,0,0};

/* simple api */

uint8_t BLADE_ReadCounter (uint8_t channel)
{
    uint8_t i = channel - 1;
    if (i <= 4)
    {
        return BladeID_Counter[i];
    }
    return 0;
}

uint8_t BLADE_ReadCalibrated (uint8_t channel)
{
    uint8_t i = channel - 1;
    if (i <= 4)
    {
        return BladeCalibrated[i];
    }
    return 0;
}

/* ID */

bool BLADE_WriteID (uint8_t channel, uint16_t id)
{
    uint8_t i = channel - 1;
    if (i <= 4)
    {
        BladeID[i] = id;
        BladeID_Counter[i] = BladeID_Counter[i] + 1;
    }
    return true;
}

uint16_t BLADE_ReadID (uint8_t channel)
{
    uint8_t i = channel - 1;
    if (i <= 4)
    {
        return (uint16_t)BladeID[i];
    }
    return 0;
}

/* IRDA_STATUS_BYTE */

bool BLADE_WriteState (uint8_t channel, IRDA_STATUS_BYTE state)
{
    uint8_t i = channel - 1;
    if (i <= 4)
    {
        BladeState[i] = state.irdaStatus;
    }
    return true;
}

IRDA_STATUS_BYTE BLADE_ReadState (uint8_t channel)
{
    IRDA_STATUS_BYTE local;
    local.irdaStatus = 0; 

    uint8_t i = channel - 1;
    if (i <= 4)
    {
        local.irdaStatus = BladeState[i];
    }
    return local;
}

/* */

bool BLADE_ResetAll()
{
    int i;
    for (i=0; i<5; i++)
    {
        BladeCalibrated[i] = 0;
        BladeID_Counter[i] = 0;
        BladePWR[i] = 0;
        BladeID[i] = 0x0;
        BladeState[i] = 0x0;
        Scheduled_Channel[i] =0;
    }
    return true;
}

bool BLADE_Calibrate (uint8_t channel)
{
    uint8_t i = channel - 1;
    if (i <= 4)
    {
        BladeCalibrated[i] = 1;
        // BladeID_Counter[i] = 0;
    }
    return true;
}

bool BLADE_Uncalibrate (uint8_t channel)
{
    uint8_t i = channel - 1;
    if (i <= 4)
    {
        BladeCalibrated[i] = 0;
        BladeID_Counter[i] = 0;
        BladePWR[i] = 0; 
        BladeID[i] = 0x0;
        BladeState[i] = 0x0;
        Scheduled_Channel[i] =0;
    }
    return true;
}

/* *****************************************************************************
 * End of File
 */