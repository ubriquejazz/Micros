#ifndef _UART__H__
#define _UART__H__

/*!\name      uart.h
 *
 * \brief     UART Classes
 *
 * \author    Juan Gago
 *
 */

#define TIMEOUT_UART_MS    100

typedef volatile unsigned char * volatile buffer_t;

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

/* Driver & Clients */

typedef struct {
    buffer_t            write, read;
    uint8_t             wlen, rlen;
    bool                isCommand, isResponse;
} UART_CLIENT;

typedef struct {
    uint8_t             index; 
    DRV_HANDLE          handle;   
    bool                isHalfDuplex;
    UART_DRV_STATE      state0;
    UART_CLIENT*        client;
} UART_DRIVER;

/* Application Initialization and State Machine Functions */

bool UART_Initialize (UART_DRIVER*, UART_CLIENT*, uint8_t);
bool UART_Tasks      (UART_DRIVER*, uint32_t*);

#endif
