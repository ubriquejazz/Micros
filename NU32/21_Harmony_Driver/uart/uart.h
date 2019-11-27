#ifndef _UART__H__
#define _UART__H__

/*!\name      uart.h
 *
 * \brief     UART Class
 *
 * \author    Juan Gago
 *
 */

#define TIMEOUT_WRITE_MS    100
#define TIMEOUT_READ_MS     100

typedef volatile unsigned char * volatile buffer_t;

typedef enum
{
    UART_DRV_INIT,
    UART_DRV_WORKING,                 
    UART_DRV_PRE_SILENCE,
    UART_DRV_POST_SILENCE,
    UART_DRV_DONE,

} UART_DRV_STATE;

typedef struct
{
    uint8_t         driver_index; 
  	uint32_t        error_count;    
    bool            isHalfDuplex;
    UART_DRV_STATE  state;
    /* Only one client per driver */
    buffer_t        write, read;
    uint8_t         wlen, rlen;
    bool            isCommand, isResponse;
    
} UART_DATA;

/* Application Initialization and State Machine Functions */

void UART_Initialize (UART_DATA*, uint8_t);
void UART_Tasks (UART_DATA*, uint32_t*);

#endif
