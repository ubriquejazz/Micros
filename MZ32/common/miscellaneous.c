/*!\name      miscellaneous.c
 *
 * \brief     Generic functions 
 *
 * \author    Juan Gago
 *
 */

#include <stdio.h>
#include <string.h>

#ifdef MOCK
    const PORT_DEF hardwareSetup [] = {
        {AN23,  1, 2,  "AN23"},
    };

    const int N_PINS = sizeof (hardwareSetup) / sizeof (PORT_DEF);

    uint8_t get_N_PINS()
    {
        return N_PINS;
    }

    PORT_DEF get_port_by_index(int i)
    {
        return hardwareSetup[i];
    }

    uint8_t address2channel (uint8_t addr)
    {
        return addr + 1;        // Pic A
    }

    uint8_t channel2address (uint8_t channel)
    {
        return channel - 1;     // Pic A
    }

    void insert_by_index(NODE_T ** head, int n) 
    {
        NODE_T* new_node;
        NODE_T* prev_node = *head;
        new_node = (NODE_T*)malloc(sizeof(NODE_T));
        new_node->val = n;

        // Adding an item to the beginning of the list
        if ( *head == NULL || (prev_node->val >= n) )
        {
            new_node->next = *head;
            *head = new_node;
        }
        else
        {
            while ( prev_node->next != NULL && (prev_node->next->val < n) )
            {
                prev_node = prev_node->next;
            }
            // Founded the correct place
            new_node->next = prev_node->next;
            prev_node->next = new_node;
        }
    }

    int remove_by_index(NODE_T ** head, int n) 
    {
        int retval = -1;
        NODE_T * current = *head;
        NODE_T * prev_node = NULL;

        if (*head != NULL)
        {
            while ( current != NULL && (current->val < n) )
            {
                prev_node = current;
                current = current->next;
            }
            
            if ( current != NULL && (current->val == n) )
            {
                // Founded the correct place
                if (current == *head)
                {
                    *head = current->next;
                }
                else
                {
                    prev_node->next = current->next;
                }
                retval = 1;
                free(current);
            }
            else
            {
                // Nothing found
                retval = 0;
            }
        }
        return retval;
    }

    void print_list (NODE_T * head, uint8_t* result) 
    {
        NODE_T * current = head;
        while (current != NULL) 
        {
            sprintf(result, "%d %s\n", current->val);
            current = current->next;
        }
    }

    void bubble_sort (uint8_t* data)
    {
        uint8_t temp;
        int i, j;
        int size = strlen(data);
        
        for (i=0; i<size; i++)
        {
            for (j=0; j<(size-1); j++)
            {
                temp = data[j];
                data[j] = data[j+1];
                data[j+1] = temp;
            }
        }
    }
#else
    #include "miscellaneous.h"
#endif

/* Static table used for the table_driven implementation. */
static const uint16_t crc_table[16] =
{
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
    0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef
};

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

uint8_t hash(const char *str) 
{
    return str[0];
}

void copy_ports(PORT_DEF *source, PORT_DEF *target)
{
    target->channel = source->channel;
    target->bitPos = source->bitPos;
    strcpy(target->description, source->description);
}

bool pioMapping_by_index(PORT_DEF *portDefinition)
{
    int i;    
    PORT_DEF my_port;

    for (i = 0; i < get_N_PINS(); i++)
    {
        my_port = get_port_by_index(i);
        if (my_port.name == portDefinition->name)      
        {
            copy_ports(&my_port, portDefinition);
            return true;
        }
    }
    return false;
}

bool pioMapping_by_name (COMMAND_DATA* commandData, PORT_DEF *portDefinition)
{
    int i, my_size;
    PORT_DEF my_port;
    for (i = 0; i < get_N_PINS(); i++)
    {
        my_port = get_port_by_index(i);
        my_size = strlen(my_port.description);
        if (my_size > 0)
        {
            if (memcmp (my_port.description, commandData->argv[1], my_size) == 0)
            {
                copy_ports(&my_port, portDefinition);
                return true;
            }
        } // search only in the IO names with the same size in the command
    }
    return false;
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

/* Time Delays */

static uint32_t ReadCoreTimer()
{
#ifdef MOCK
    return 0;
#else
    volatile uint32_t timer;
    // get the count reg
    asm volatile("mfc0   %0, $9" : "=r"(timer));
    return(timer);
#endif
}

int64_t GetThisBeat()
{  
   return ReadCoreTimer();
}

int64_t GetBeatsPerSecond()
{  
   return 1000 * ms_SCALE;      
}

int64_t MillisecondsToBeats( int64_t milliseconds)
{
   return milliseconds * ms_SCALE;
}

int64_t MicrosecondsToBeats( int64_t microseconds)
{
   return microseconds * us_SCALE;
}

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
/***********************************************************
 *   Millisecond Delay function using the Count register
 *   in coprocessor 0 in the MIPS core.
 *   When running 200 MHz, CoreTimer frequency is 100 MHz
 *   CoreTimer increments every 2 SYS_CLK, CoreTimer period = 10ns
 *   1 ms = N x CoreTimer_period;
 *   To count 1ms, N = 100000 counts of CoreTimer
 *   1 ms = 10 ns * 100000 = 10e6 ns = 1 ms
 *   When running 80 MHz, CoreTimer frequency is 40 MHz 
 *   CoreTimer increments every 2 SYS_CLK, CoreTimer period = 25ns
 *   To count 1ms, N = 40000 counts of CoreTimer
 *   1ms = 25 ns * 40000 = 10e6 ns = 1 ms
 *   ms_SCALE = (GetSystemClock()/2000) @ 200 MHz = 200e6/2e3 = 100e3 = 100000
 *   ms_SCLAE = (GetSystemClock()/2000) @ = 80e6/2e3 = 40e3 = 40000 
 */
 
void DelayMs(unsigned long int msDelay )
{
      register unsigned int startCntms = ReadCoreTimer();
      register unsigned int waitCntms = msDelay * ms_SCALE;
 
      while( ReadCoreTimer() - startCntms < waitCntms );
}

/***********************************************************
 *   Microsecond Delay function using the Count register
 *   in coprocessor 0 in the MIPS core.
 *   When running 200 MHz, CoreTimer frequency is 100 MHz
 *   CoreTimer increments every 2 SYS_CLK, CoreTimer period = 10ns
 *   1 us = N x CoreTimer_period;
 *   To count 1us, N = 100 counts of CoreTimer
 *   1 us = 10 ns * 100 = 1000 ns  = 1us
 *   When running 80 MHz, CoreTimer frequency is 40 MHz 
 *   CoreTimer increments every 2 SYS_CLK, CoreTimer period = 25ns
 *   To count 1us, N = 40 counts of CoreTimer
 *   1us = 25 ns * 40 = 1000 ns = 1 us
 *   us_SCALE = (GetSystemClock()/2000) @ 200 MHz = 200e6/2e6 = 100 
 *   us_SCLAE = (GetSystemClock()/2000) @ 80 MHz = 80e6/2e6 = 40 
 */
 
void DelayUs(unsigned long int usDelay )
{
      register unsigned int startCnt = ReadCoreTimer();
      register unsigned int waitCnt = usDelay * us_SCALE;
 
      while( ReadCoreTimer() - startCnt < waitCnt );
}
 
void Delay10us (unsigned long int input )
{
    DelayUs(10*input);
}

/* *****************************************************************************
 * End of File
 */
