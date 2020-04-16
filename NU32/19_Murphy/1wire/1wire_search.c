/*! \file 1wire_search.c

    \brief Control and locate an actual device
	
	 Drive an actual device on the 1-Wire bus by addressing
	 it directly. Search for all devices on the bus. 
*/

#include "1wire_search.h"
#include "1wire.h"      // OW_reset_pulse, OW_read_bit ...

unsigned char   ROM_NO[8];
int             LastDiscrepancy;
int             LastFamilyDiscrepancy;
int             LastDeviceFlag;
unsigned char   crc8;

//! Check if a device is on the bus
/*! \return 1 - Device on bus 0 - No Device
*/
unsigned char Detect_Slave_Device(void)
{
    if (!OW_reset_pulse()) {
        return 1;
    }
    else {
        return 0;
    }
}  

//! Get a pointer to the location of the found address
/*! \param void Nothing passed in
    \return uint8_t pointer to the EEPROM address
*/
uint8_t* GetAddresLoc(void)
{
    return &ROM_NO[0];
}


//! Find the first device on the 1-Wire bus
/*! \return TRUE - ROM number in \a ROM_NO  FALSE - no device present
*/
int OWFirst(void)
{
    LastDiscrepancy = 0;    // reset the search state
    LastDeviceFlag = FALSE;
    LastFamilyDiscrepancy = 0;
    return OWSearch();
}

//! Find the next device on the 1-wire bus
/*! \return TRUE - ROM number in \a ROM_NO  FALSE - no device present
*/
int OWNext(void)
{
    // leave the search state alone
    return OWSearch();
}

/*! \brief Perform the 1-Wire Search Algorithm on the 1-Wire bus */
/*! \return TRUE - ... FALSE - no device present
*/
int OWSearchHelper(int* id_bit_number, int* last_zero)
{
    int retVal, rom_byte_number;
    unsigned char rom_byte_mask;
    unsigned char search_direction;

    rom_byte_number = 0; // loop through all ROM bytes 0-7
    do
    {
        // read a bit and its complement
        int id_bit      = OW_read_bit();
        int cmp_id_bit  = OW_read_bit();

        // check for no devices on 1-wire bus
        if ((id_bit == 1) && (cmp_id_bit == 1)) {
            retVal = false;
            break;
        }
        else
        {
            // all devices coupled have 0 or 1
            if (id_bit != cmp_id_bit) {
                search_direction = id_bit; // bit write value for search
            }
            else
            {
                int bit_position = *id_bit_number; // read value
                // if this discrepancy is from a previous iteration 
                if (bit_position < LastDiscrepancy) {
                    search_direction = ((ROM_NO[rom_byte_number] & rom_byte_mask) > 0);
                }
                else { // if equal to last pick 1, if not then pick 0
                    search_direction = (bit_position == LastDiscrepancy);
                }

                // if 0 was picked then record its position in LastZero
                if (search_direction == 0) {
                    *last_zero = bit_position;
                    if (bit_position < 9) {
                        LastFamilyDiscrepancy = bit_position; // check for Last discrepancy in family
                    }
                }
            }

            // set or clear the bit in the ROM byte rom_byte_number with mask rom_byte_mask
            if (search_direction == 1) {
                ROM_NO[rom_byte_number] |= rom_byte_mask;
            }
            else {
                ROM_NO[rom_byte_number] &= ~rom_byte_mask;
            }
            OW_write_bit(search_direction); // serial number search direction write bit
            (*id_bit_number)++;             // increment the byte counter id_bit_number
            rom_byte_mask <<= 1;            // and shift the mask rom_byte_mask
        
            // if the mask is 0 then go to new SerialNum byte rom_byte_number and reset mask
            if (rom_byte_mask == 0)
            {
                docrc8(ROM_NO[rom_byte_number]); // accumulate the CRC
                rom_byte_number++;
                rom_byte_mask = 1;
            }
        }
    } 
    while(rom_byte_number < 8);
    return retVal;
}

/*! \brief Perform the 1-Wire Search Algorithm on the 1-Wire bus using the existing
	search state.
*/
/*! \return TRUE - device found, ROM number in \a ROM_NO buffer  FALSE - no device present
*/
int OWSearch(void)
{
    int id_bit_number;      // bit position; from 0 to 63
    int last_zero = 0;      // conflict position resolved with 0
    int search_result;      // return value

    crc8 = 0;               // initialize values
    id_bit_number = 1;      
    if (!LastDeviceFlag)    // if the last call was not the last one
    {
        // 1-Wire reset
        if (!Detect_Slave_Device())
        {
            // nothing detected, reset the search
            LastDiscrepancy = 0;
            LastDeviceFlag = FALSE;
            LastFamilyDiscrepancy = 0;
            return FALSE;
        }

        // issue the search command and process
        OW_write_byte(SEARCH_ROM_CMD);
        OWSearchHelper(&id_bit_number, &last_zero);

        // if the search was successful then set LastDiscrepancy, LastDeviceFlag, search_result
        if ( ((id_bit_number < 65) || (crc8 != 0)) == false )
        {
            LastDiscrepancy = last_zero;
            if (LastDiscrepancy == 0) {
                LastDeviceFlag = TRUE;      // last device
            }
            search_result = TRUE;
        }
    }

    // if no device found then reset counters so next 'search' will be like a first
    if (!search_result || !ROM_NO[0])
    {
        LastDiscrepancy = 0;
        LastDeviceFlag = FALSE;
        LastFamilyDiscrepancy = 0;
        search_result = FALSE;
    }
    return search_result;
}

const unsigned char dscrc_table[] = {
    0, 94,188,226, 97, 63,221,131,194,156,126, 32,163,253, 31, 65,
    157,195, 33,127,252,162, 64, 30, 95, 1,227,189, 62, 96,130,220,
    35,125,159,193, 66, 28,254,160,225,191, 93, 3,128,222, 60, 98,
    190,224, 2, 92,223,129, 99, 61,124, 34,192,158, 29, 67,161,255,
    70, 24,250,164, 39,121,155,197,132,218, 56,102,229,187, 89, 7,
    219,133,103, 57,186,228, 6, 88, 25, 71,165,251,120, 38,196,154,
    101, 59,217,135, 4, 90,184,230,167,249, 27, 69,198,152,122, 36,
    248,166, 68, 26,153,199, 37,123, 58,100,134,216, 91, 5,231,185,
    140,210, 48,110,237,179, 81, 15, 78, 16,242,172, 47,113,147,205,
    17, 79,173,243,112, 46,204,146,211,141,111, 49,178,236, 14, 80,
    175,241, 19, 77,206,144,114, 44,109, 51,209,143, 12, 82,176,238,
    50,108,142,208, 83, 13,239,177,240,174, 76, 18,145,207, 45,115,
    202,148,118, 40,171,245, 23, 73, 8, 86,180,234,105, 55,213,139,
    87, 9,235,181, 54,104,138,212,149,203, 41,119,244,170, 72, 22,
    233,183, 85, 11,136,214, 52,106, 43,117,151,201, 74, 20,246,168,
    116, 42,200,150, 21, 75,169,247,182,232, 10, 84,215,137,107, 53
};
//------------------------------------------------------------------
// Calculate the CRC8 of the byte value provided with the current
// global 'crc8' value.
// Returns current global crc8 value

//! 1 byte CRC check	
/*! @param value Value to add to the byte crc
    \return last calculated crc
*/
unsigned char docrc8(unsigned char value)
{
    crc8 = dscrc_table[crc8 ^ value];
    return crc8;
}

/*******************************************************************************
 End of File
 */
