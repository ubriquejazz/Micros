#ifndef  __DS2433_H
#define  __DS2433_H

#include <stdio.h>

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

/*
uint8_t read_scratchpad(int16 addr, uint8_t* ES)
This will read scratchpad data from a specified address
PARAM addr: The address to read from
NOTE: In order to copy data from the scratchpad, the ES byte must be known
RETURNS: The data in the address of scratchpad
*/
uint8_t read_scratchpad(uint8_t* ES);

/*
void write_scratchpad(int16 addr, uint8_t data)
This will write a byte of data to the scratchpad at a specified location
PARAM addr: The address to write to
PARAM data: The byte to write to the specified location
RETURNS: none
*/
void write_scratchpad(int16 addr, uint8_t data);

/*
void copy_scratchpad(int16 addr, uint8_t ES)
This will copy the contents of the scratchpad to the specified location
PARAM addr: The address to copy to
PARAM ES: The E/S byte to use
NOTE: All parameters passed in must match the DS1993's 3 address registers
*/
void copy_scratchpad(int16 addr, uint8_t ES);

/*
void read_entire_memory(uint8_t mem[])
This will read all 512 bytes of a DS1993 into a 512 byte array on the PIC
PARAM: A pointer a variable to put the data into
NOTE: mem must be pointing to the beginning of a 512 wide array
RETURNS: none
*/
void read_entire_memory(uint8_t mem[]);

/*
uint8_t read_memory(int16 addr)
This will read one byte from memory at a specified address
PARAM: The address to read from
RETURNS: The data at the address
*/
uint8_t read_memory(int16 addr);

/*
void block_fill(int16 from, int16 to, uint8_t filler)
This will block fill a chunk of memory
PARAM: Where to start the block fill
PARAM: Where to stop the block fill, including the address
PARAM: The byte to fill the block with
RETURNS: none
*/
void block_fill(int16 from, int16 to, uint8_t filler);

/*
void match_block_fill(uint8_t rom[], int16 from, int16 to, uint8_t filler)
This will block fill a chunk of memory at a specific DS1993
PARAM: A pointer to the beginning of the rom data to test
PARAM: Where to start the block fill
PARAM: Where to stop the block fill, including the address
PARAM: The byte to fill the block with
NOTE: rom must be pointing to the beginning of a 8-wide array
RETURNS: none
*/
void match_block_fill(uint8_t address[], int16 from, int16 to, uint8_t filler);

/*
void read_rom_specs(uint8_t* family, uint8_t ID[], uint8_t* CRC)
This will take the 64-bit lasered ROM and put it into seperate variables.
  The DS1993 will then be ready to excecute a memory function.
PARAMS: Pointers to the variables to put the various data into
NOTE: ID must be pointing to the beginning a 6 wide array
RETURNS: none
*/
void read_rom_specs(uint8_t* family, uint8_t ID[], uint8_t* CRC);

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
void read_rom(uint8_t data[]);

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
void match_rom(uint8_t romdata[]);

/*
void skip_rom()
This will save time on single drop bus system by bypassing any rom function
  and heading straight to a memory function
PARAMS: none
RETURNS: none
*/
void skip_rom();

#endif

