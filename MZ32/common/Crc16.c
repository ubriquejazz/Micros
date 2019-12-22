/*!\name      Crc16.c
 *
 * \brief     Calculates CRC for the given data and len
 *
 * \author    Juan Gago, 2019
 */

#include <stdio.h>

/* Static table used for the table_driven implementation. */
static const uint16_t crc_table[16] =
{
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
    0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef
};


/********************************************************************
* Function:     CalculateCrc()
*
* Precondition:
*
* Input:        Data pointer and data length
*
* Output:       CRC.
*
* Side Effects: None.
*
* Overview:     Calculates CRC for the given data and len
*
*
* Note:         None.
********************************************************************/
uint32_t CalculateCrc(uint8_t *data, uint32_t len)
{
    uint32_t i;
    uint16_t crc = 0;
    
    while(len--)
    {
        i = (crc >> 12) ^ (*data >> 4);
        crc = crc_table[i & 0x0F] ^ (crc << 4);
        i = (crc >> 12) ^ (*data >> 0);
        crc = crc_table[i & 0x0F] ^ (crc << 4);
        data++;
    }
    return (crc & 0xFFFF);
}

uint16_t CalculateCrc_String(uint8_t* in_buffer, uint8_t* out_buffer)
{
   uint16_t crc = CalculateCrc(in_buffer, strlen(in_buffer));
   sprintf(out_buffer, "%04X", crc);
   return crc;
}

uint16_t AppendCrc(uint8_t* buffer)
{
    uint8_t crc_string[5];
    uint8_t crc_inv[5];
    strcat (buffer, " "); // Add an extra space
    uint16_t crc = CalculateCrc(buffer, strlen(buffer));
    sprintf(crc_string, "%04X", crc);
    crc_inv[0] = crc_string[3];
    crc_inv[1] = crc_string[2];
    crc_inv[2] = crc_string[1];
    crc_inv[3] = crc_string[0];
    crc_inv[4] = 0;
    strcat(buffer, crc_inv);    
    return crc;
}

/* *****************************************************************************
 * End of File
 */