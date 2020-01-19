#ifndef _UART__H__
#define _UART__H__

/*!\name      uart.h
 *
 * \brief     UART Classes
 *
 * \author    Juan Gago
 *
 */

#define TIMEOUT_WRITE_MS    100
#define TIMEOUT_READ_MS     100

typedef volatile unsigned char * volatile buffer_t;

typedef uint32_t object_t;
typedef struct NODE node_t;

struct NODE {
    node_t*     self;
    node_t*     owner;
    object_t    data;
};

/*  States */

typedef enum
{
    UART_DRV_IDLE=0, 
    UART_DRV_INIT,
    UART_DRV_WORKING,                 
    UART_DRV_PRE_SILENCE,
    UART_DRV_POST_SILENCE,
    UART_DRV_DONE,

} UART_DRV_STATE;

typedef enum
{
    UART_CLIENT_NONE=0, 
    UART_CLIENT_REQ, 
    UART_CLIENT_COMPLETE,    // Complete
    UART_CLIENT_USED,        // Processed   
    UART_CLIENT_ERROR,       // Error
} UART_CLIENT_STATE;

/* Driver & Clients */

typedef struct {
    node_t              base;
    UART_CLIENT_STATE   state;
    buffer_t            write, read;
    uint8_t             wlen, rlen;
    bool                isCommand, isResponse;
} UART_CLIENT;

typedef struct {
    node_t              base;
    UART_DRV_STATE      state;
    uint8_t             index; 
  	uint32_t            error_count; 
    DRV_HANDLE          handle;   
    bool                isHalfDuplex;
} UART_DRIVER;

typedef struct {
    UART_CLIENT         user; /* Only one client per driver */ 
    UART_DRIVER         drv;
} UART_OBJECT;

/* Application Initialization and State Machine Functions */

void    UART_Initialize (UART_OBJECT*, uint8_t);

int     UART_Tasks (UART_CLIENT*, uint32_t*);

#endif
