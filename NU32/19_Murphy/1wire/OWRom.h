#ifndef _OWRom_H
#define _OWRom_H

/*!\name      OWRom.h
 *
 * \brief     Library to access the 64 bit ROM Commandss
 *			   
 *  		  read_rom_specs(family, ID, CRC)           
 *  		    Seperates different data from the rom   
 *  		  read_rom(data)                            
 *  		    Reads in the entire rom                 
 *  		  search_rom()                              
 *  		    Identify the connected device           
 *  		  match_rom(romdata)                        
 *  		    Device with correct 64-bit sequence will respond
 *  		  skip_rom()                                
 *  		    To save time on single drop bus system  
 *  		  result = check_crc8(data)                 
 *  		    checks CRC of 64 bit sequence  
 *            
 * \author    Juan Gago
 *
 */ 

// ROM commands
#define ROM_CMD_MATCH_ROM    0x55
#define ROM_CMD_SEARCH_ROM   0xF0
#define ROM_CMD_READ_ROM     0x33
#define ROM_CMD_SKIP_ROM     0xCC

/* This will take the 64-bit ROM and put it into seperate variables.
   The DS2493 will then be ready to excecute a memory function.
PARAMS: Pointers to the variables to put the various data into
NOTE: ID must be pointing to the beginning a 6 wide array
RETURNS: none
*/
void read_rom_specs(uint8_t* family, uint8_t ID[], uint8_t* CRC);

/* Only on a single device bus
   This will take the 64-bit ROM and put it into a variable. The DS2493
   will then be ready to excecute a memory function.
PARAM: A pointer to the variable to put the rom data into
NOTE: data must be pointing to the beginning of a 8-wide array.
NOTE: the LSB in the ROM (family) will start at index 0,
  the MSB in the rom (CRC) will be at index 7.
RETURNS: none
*/
void read_rom(uint8_t data[]);

/* This will send the match ROM command to the DS2493.
   If the rom data sent into the function equals the rom data on the
   DS2493, then the DS2493 will be ready for a memory function, otherwise
   the DS2493 will wait for a reset pulse to begin a rom function.
PARAM: A pointer to the beginning of the rom data to test
NOTE: romdata must be pointing to the beginning of a 8-wide array
RETURNS: none
*/
void match_rom(uint8_t romdata[]);

/* This will save time on single drop bus system by bypassing any rom function
   and heading straight to a memory function
PARAMS: none
RETURNS: none
*/
void skip_rom();

#endif