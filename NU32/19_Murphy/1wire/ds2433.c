///////////////////////////////////////////////////////////////////////////
////              Library for a DS1933 SRAM                            ////
////                                                                   ////
////   data = read_scratchpad(addr, ES)                                ////
////     Used to verify scratchpad data and target address             ////
////                                                                   ////
////   write_scratchpad(addr, data)                                    ////
////     Used to write data to scratchpad                              ////
////                                                                   ////
////   copy_scratchpad(addr, ES)                                       ////
////     Used to copy data from scratchpad to memory                   ////
////                                                                   ////
////   read_entire_memory(mem)                                         ////
////     used to read the entire 512 bytes of memory                   ////
////                                                                   ////
////   data = read_memory(addr)                                        ////
////     Used to read a place in memory                                ////
////                                                                   ////
////   block_fill(from, to, filler)                                    ////
////     Used to block fill a specified amount of memory               ////
////                                                                   ////
////   match_block_fill(from, to, filler)                              ////
////     Used to block fill a specified amount of memory               ////
////     from a specific ROM                                           ////
////                                                                   ////
////   Following functions are used to access the 64 bit lasered ROM   ////
////                                                                   ////
////   read_rom_specs(family, ID, CRC)                                 ////
////     Seperates different data from the rom                         ////
////                                                                   ////
////   read_rom(data)                                                  ////
////     Reads in the entire rom                                       ////
////                                                                   ////
////   search_rom()                                                    ////
////     Identify the connected device                                 ////
////                                                                   ////
////   match_rom(romdata)                                              ////
////     Device with correct 64-bit sequence will respond              ////
////                                                                   ////
////   skip_rom()                                                      ////
////     To save time on single drop bus system                        ////
////                                                                   ////
////   result = check_crc8(data)                                       ////
////     checks CRC of 64 bit sequence                                 ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////

#include "ds2433.h"
#include "1wire.h"      // OW_reset_pulse, OW_read_bit ...

/*
uint8_t read_scratchpad(int16 addr, uint8_t* ES)
This will read scratchpad data from a specified address
PARAM addr: The address to read from
NOTE: In order to copy data from the scratchpad, the ES byte must be known
RETURNS: The data in the address of scratchpad
*/
uint8_t read_scratchpad(uint8_t* ES)
{
  OW_write_byte(0xAA);    // read scratchpad command
  OW_read_byte ();
  OW_read_byte();
  *ES = OW_read_byte();
  return OW_read_byte();
}

/*
void write_scratchpad(int16 addr, uint8_t data)
This will write a byte of data to the scratchpad at a specified location
PARAM addr: The address to write to
PARAM data: The byte to write to the specified location
RETURNS: none
*/
void write_scratchpad(int16 addr, uint8_t data)
{
	OW_write_byte(0x0F);        // write scratchpad command
	OW_write_byte(addr);
	OW_write_byte(addr >> 8);
	OW_write_byte(data);  
}

/*
void copy_scratchpad(int16 addr, uint8_t ES)
This will copy the contents of the scratchpad to the specified location
PARAM addr: The address to copy to
PARAM ES: The E/S byte to use
NOTE: All parameters passed in must match the DS1993's 3 address registers
*/
void copy_scratchpad(int16 addr, uint8_t ES)
{
 	OW_write_byte(0x55);        // copy scratchpad command
	OW_write_byte(addr);
	OW_write_byte(addr >> 8);
	OW_write_byte(ES);
}

/*
uint8_t read_memory(int16 addr)
This will read one byte from memory at a specified address
PARAM: The address to read from
RETURNS: The data at the address
*/
uint8_t read_memory(int16 addr)
{
	OW_write_byte(0xF0);        // read memory command
	OW_write_byte(addr);
	OW_write_byte(addr >> 8);
	return OW_read_byte();
}

/*
void read_rom_specs(uint8_t* family, uint8_t ID[], uint8_t* CRC)
This will take the 64-bit lasered ROM and put it into seperate variables.
  The DS1993 will then be ready to excecute a memory function.
PARAMS: Pointers to the variables to put the various data into
NOTE: ID must be pointing to the beginning a 6 wide array
RETURNS: none
*/
void read_rom_specs(uint8_t* family, uint8_t ID[], uint8_t* CRC)
{
   uint8_t i;

  // give the command to read back the requested data
	OW_write_byte(0x33);

  // family is the first item in the array
	*family = OW_read_byte();

  // the ID is the next 6 bytes
  for(i = 0; i < 6; i++)
      ID[i] = OW_read_byte();

  // CRc is the last byte
	*CRC = OW_read_byte();
}

/* Only on a single device bus
void read_rom(uint8_t data[])
This will take the 64-bit lasered ROM and put it into a variable. The DS1993
  will then be ready to excecute a memory function.
PARAM: A pointer to the variable to put the rom data into
NOTE: data must be pointing to the beginning of a 8-wide array.
NOTE: the LSB in the ROM (family) will start at index 0,
  the MSB in the rom (CRC) will be at index 7.
RETURNS: none
*/
void read_rom(uint8_t data[])
{
   uint8_t i;

   // give the command to read back the requested data
	 OW_write_byte(0x33);

   // fill up data
   for(i = 0; i < 8; i++)
      data[i] = OW_read_byte();
}

/*
void match_rom(uint8_t romdata[])
This will send the match ROM command to the DS1993.
  If the rom data sent into the function equals the rom data on the
  DS1993, then the DS1993 will be ready for a memory function, otherwise
  the DS1993 will wait for a reset pulse to begin a rom function.
PARAM: A pointer to the beginning of the rom data to test
NOTE: romdata must be pointing to the beginning of a 8-wide array
RETURNS: none
*/
void match_rom(uint8_t romdata[])
{
   uint8_t romdatacopy, i;

   // give the command to match ROM
	 OW_write_byte(0x55);

    // Pass out the match data
    for (i = 0; i < 8; i++)
    {
        romdatacopy = romdata[i];    // Copy the value before sending it on
        OW_write_byte( romdatacopy );
    }
}


/*
void skip_rom()
This will save time on single drop bus system by bypassing any rom function
  and heading straight to a memory function
PARAMS: none
RETURNS: none
*/
void skip_rom()
{
   // give the command to skip ROM
	OW_write_byte(0xCC);
}

//constant data table used for checking CRC of 64 bit unique ROM codes
const char crc8_table[256]={
     0,  94, 188, 226,  97,  63, 221, 131, 194, 156, 126,  32, 163, 253,  31,  65,
   157, 195,  33, 127, 252, 162,  64,  30,  95,   1, 227, 189,  62,  96, 130, 220,
    35, 125, 159, 193,  66,  28, 254, 160, 225, 191,  93,   3, 128, 222,  60,  98,
   190, 224,   2,  92, 223, 129,  99,  61, 124,  34, 192, 158,  29,  67, 161, 255,
    70,  24, 250, 164,  39, 121, 155, 197, 132, 218,  56, 102, 229, 187,  89,   7,
   219, 133, 103,  57, 186, 228,   6,  88,  25,  71, 165, 251, 120,  38, 196, 154,
   101,  59, 217, 135,   4,  90, 184, 230, 167, 249,  27,  69, 198, 152, 122,  36,
   248, 166,  68,  26, 153, 199,  37, 123,  58, 100, 134, 216,  91,   5, 231, 185,
   140, 210,  48, 110, 237, 179,  81,  15,  78,  16, 242, 172,  47, 113, 147, 205,
    17,  79, 173, 243, 112,  46, 204, 146, 211, 141, 111,  49, 178, 236,  14,  80,
   175, 241,  19,  77, 206, 144, 114,  44, 109,  51, 209, 143,  12,  82, 176, 238,
    50, 108, 142, 208,  83,  13, 239, 177, 240, 174,  76,  18, 145, 207,  45, 115,
   202, 148, 118,  40, 171, 245,  23,  73,   8,  86, 180, 234, 105,  55, 213, 139,
    87,   9, 235, 181,  54, 104, 138, 212, 149, 203,  41, 119, 244, 170,  72,  22,
   233, 183,  85,  11, 136, 214,  52, 106,  43, 117, 151, 201,  74,  20, 246, 168,
   116,  42, 200, 150,  21,  75, 169, 247, 182, 232,  10,  84, 215, 137, 107,  53};

/*
int1 check_crc8(int8 *ptr)
This checks to see if the unique 64 bit ROM code was received correctly by 
checking it's CRC.
PARAMS: pointer to 64 bit ROM code.
RETURNS: TRUE if CRC matches, FALSE if CRC doesn't match
*/
BOOL check_crc8(int8 *ptr)
{
   int8 i;
   int8 result=0;
   
   for(i=0;i<8;i++)
   {
      result=crc8_table[result^ptr[i]];
   }
   
   if(result)
      return(FALSE);
   else
      return(TRUE);
}

/*******************************************************************************
 End of File
 */
