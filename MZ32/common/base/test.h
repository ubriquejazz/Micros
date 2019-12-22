#ifndef _ENUM_H    /* Guard against multiple inclusion */
#define _ENUM_H

#include <stdint.h>
#include <stdbool.h>

/*!\brief     Macros. */

#define MASK(x)                 ((uint8_t)(1<<x))
#define F8(x)                   (uint8_t)x
#define F16(x)                  (uint16_t)x
#define F32(x)                  (uint32_t)x
#define SQUARE(x)               (x*x)

/*!\brief     ASCII Codes. */

#define CONST_LF                10      // (new) Line Feed
#define CONST_CR                13      // Carriage Return
#define CONST_SP                32      // Space Character
#define CONST_DOLLAR            36      // Dollar
#define CONST_EXCLAMATION       33      // Exclamation Mark
#define CONST_STAR              42      // Start Symbol
#define CONST_QUESTION          63      // Question Mark
#define CONST_BLANK             128     // 0x80


/*!\brief     Global Constants. */

#define MAX_NUM_OF_BYTES        512
#define LENGTH_ARG              64
#define LENGTH_CMD              32
#define COMMAND_SIZE            103     // 3(cmd) + 1(seq) + 99 (payload)
#define HEADER_SIZE             4       // 2(size) + 2(addresses) 
#define MAX_NUM_ARG             5      // ! + cmd + arg + ! + packet + crc16 (+5)
#define HB_NUM_RETRIES          5
#define GetSystemClock()        (SYS_CLK_FREQ)
#define us_SCALE                (GetSystemClock()/2000000)
#define ms_SCALE                (GetSystemClock()/2000)

/*!\brief     Command */

typedef struct {  

    char*       seq_no;             // unique id string
    char*       src_addr;           // Source of message. Used to route reply back
    char*       dst_addr;           // Destination of message: the PSU, IrDA, FAN            
    uint8_t     argc;               // 
    char*       argv[MAX_NUM_ARG];  // argc = 12 
    uint16_t    checksum;           // 
    bool        ready;              //      

} COMMAND_DATA;

/*!\brief     Status register */

typedef union{
    uint8_t         irdaStatus;
    
    struct  {
        uint8_t     A       : 2;
        uint8_t     B       : 2;
        uint8_t     PS      : 3;
        uint8_t     MSB     : 1;         // Bit 7
    } MergedBits;    
    struct 
    {
        bool irdaStable     : 1;
        bool messageWaiting : 1;
        bool thirdBit       : 1;
        bool fourthBit      : 1;
        char topFourBits    : 4;
    } bits;
        
} IRDA_STATUS_BYTE;

#endif // _ENUM_H 