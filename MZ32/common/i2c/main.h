#ifndef MAIN_H
#define	MAIN_H

/*!\name      main.h
 *
 * \brief     Contains prototype for main
 *
 * \author    Juan Gago
 *
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "i2c_bitbang_isr.h"

#define SYS_CLOCK_MHZ               200000000ul
#define PBCLK2_MHZ                  100000000ul
#define I2C_BAUD_RATE               100000ul
#define I2C_BAUD_TIME               PBCLK2_MHZ/I2C_BAUD_RATE

#define EEPROM_address              0xA0
#define RTCC_address                0xDE
#define Start_I2C_Transaction()     I2C_SW_ISR_TRIGGER = 1;

#define SLAVE_ADDRESS               0x6F
#define SLAVE_ADDRESS_READ          ((SLAVE_ADDRESS << 1) | 1)
#define SLAVE_ADDRESS_WRITE         ((SLAVE_ADDRESS << 1) | 0)

#define GetSystemClock()            (SYS_CLOCK_MHZ)/* Fcy = 200MHz */
#define us_SCALE                    (GetSystemClock()/2000000)
#define ms_SCALE                    (GetSystemClock()/2000)

#if defined(__DEBUG)
    #define SYS_DEBUG_BreakPoint()  __asm__ volatile (" sdbbp 0")
#else
    #define SYS_DEBUG_BreakPoint()  while(true)
#endif

typedef enum
{
    LED_H0_D3_TOGGLE,
    LED_H0_D3_IDLE,
    LED_H2_D4_TOGGLE,
    LED_H2_D4_IDLE,
    LED_H1_D5_TOGGLE,
    LED_H1_D5_IDLE
} LED_TOGGLE_STATES;

typedef enum
{
    I2C_TRANSACTION_SUCCESS,
    I2C_TRANSACTION_IN_PROGRESS,
    I2C_ADDRESS_BYTE_NACK,
    I2C_DATA_BYTE_NACK
} I2C_TRANSACTION_STATUS;


typedef struct
{
    uint16_t                address;    
    uint8_t*                wrBuf;
    uint8_t*                rdBuf;
    uint16_t                wrlen;
    uint16_t                rdlen;
    
    I2C_OPERATION_STATUS    i2coperationstatus;
    uint16_t                wrcnt;
    uint16_t                rdcnt;
    uint16_t                client;
} BUFFER_TRANSFER_QUEUE;

uint8_t QueueLength;

void PERFORMANCE_VARS_INITIALIZE(void);

void APP_LED_Initialize(void);

void APP_LED_STATE(void);

void APP_INITIALIZE(void);

void APP_I2C_STATE(void);

void APP_I2C_Initialize(void);

void APP_I2C_BUFFER_TRANSFER_QUEUE  (BUFFER_TRANSFER_QUEUE* MessageQueue);

void APP_I2C_POPULATE_QUEUE_DATA    (BUFFER_TRANSFER_QUEUE* BTQ3, uint16_t slavedeviceaddress, uint8_t* i2ctxdata, uint8_t* i2crxdata, uint8_t writelength, uint8_t readlength);


#endif	/* MAIN_H */


