#include <stdio.h>   
#include <stdbool.h>

/* Type conversion */

uint16_t average(uint16_t a, uint16_t b, uint16_t c, uint16_t d)
{
    return (a+b+c+d)/4;
}

bool isHexadecimal(uint8_t c)
{
    bool isNumeric = (c>='0') && (c<='9');
    bool isAlfaNum = (c>='A') && (c<='F');           
    return isNumeric || isAlfaNum;
}

uint8_t ascii2decimal(uint8_t c)
{
    if (c>='0' && c<='9') { 
        return c - 48;
    }
    return 0; 
}

char ascii2status(char c)
{
    bool isAlfaNum = (c>='A') && (c<='F');
    bool isNumeric = (c>='0') && (c<='9');
    
    if (isAlfaNum) {
        return c - 'A' + 10;
    }
    else if (isNumeric) {
        return c - '0';
    }
    else {
        return 0;
    }
}

// Function to convert hexadecimal to decimal 
uint16_t hexa2decimal (uint8_t* hexVal) 
{    
    int hexa_base = 1;       // Initializing base value to 1, i.e 16^0 
    uint16_t dec_val = 0; 
    int i, len;
      
    len = strlen(hexVal); 
    for (i=len-1; i>=0; i--) 
    {    
        // if character lies in '0'-'9', converting  
        // it to integral 0-9 by subtracting 48 from 
        // ASCII value. 
        if (hexVal[i]>='0' && hexVal[i]<='9') 
        { 
            dec_val += (hexVal[i] - 48)*hexa_base; 
            hexa_base = hexa_base * 16; 
        } 
  
        // if character lies in 'A'-'F' , converting  
        // it to integral 10 - 15 by subtracting 55  
        // from ASCII value 
        else if (hexVal[i]>='A' && hexVal[i]<='F') 
        { 
            dec_val += (hexVal[i] - 55)*hexa_base; 
            hexa_base = hexa_base*16; 
        } 
    }       
    return dec_val; 
}

uint8_t hexa2ascii (uint8_t nibble)
{
    uint8_t retVal;
    uint8_t myValue = nibble & 0x0F;    
    if (myValue < 10) {
        retVal = myValue + '0';
    }
    else {
        retVal = myValue + ('A' - 10);
    }
    return retVal;
}

/* Command management */

bool validate_header (uint8_t *input_buffer, uint8_t minlen)
{
    bool retVal = false;
    uint8_t i = 1;     
    if (strlen(input_buffer) > minlen)
    {
        bool space1 = (input_buffer[i] == CONST_SP);
        bool space2 = (input_buffer[i+4] == CONST_SP);
        bool space3 = (input_buffer[i+8] == CONST_SP);    
        bool space4 = (input_buffer[i+12] == CONST_SP);            
        if (space1 && space2 && space3 && space4)
        {
            retVal = true;
        }    
    }
    return retVal;
}

bool populate_command (uint8_t *input_buffer, COMMAND_DATA* commandData)
{
    bool retVal = false;
    int argc = 0;
    uint8_t header[] = "! 123 USB MST";
    
    if (validate_header(input_buffer, 14))
    {
        // 1. header
        memcpy (header, input_buffer, sizeof(header));  
        char*  token = strtok(header, " ");
        volatile char isCommand = token;
        if (token != NULL)
        {
            token = strtok(NULL, " ");
            commandData->seq_no = token;
        }
        if (token != NULL)
        {
            token = strtok(NULL, " ");
            commandData->src_addr = token;
        }
        if (token != NULL)
        {
            token = strtok(NULL, " ");
            commandData->dst_addr = token;
            retVal = true;
        }      
        // 2. arguments
        token = strtok(input_buffer + sizeof(header), " ");
        commandData->argv[argc] = token;  
        while((token != NULL) && (argc++<MAX_NUM_ARG) ) 
        {
            token = strtok(NULL, " ");
            commandData->argv[argc] = token;
        }        
    }
    commandData->argc = argc;
    commandData->checksum = 0;
    return retVal;
}

bool crc_update (COMMAND_DATA* commandData)
{
    int argc = commandData->argc;
    uint8_t crc_inv[5];    
    if (strlen(commandData->argv[argc-1]) == 4)
    {
        crc_inv[3] = commandData->argv[argc-1][0];
        crc_inv[2] = commandData->argv[argc-1][1];
        crc_inv[1] = commandData->argv[argc-1][2];
        crc_inv[0] = commandData->argv[argc-1][3];
        crc_inv[4] = 0; // Decode CRC (****) into a decimal
        commandData->checksum = hexa2decimal(crc_inv);
        return true;
    }
    else
    {
        commandData->checksum = -1;
        return false;
    }
}