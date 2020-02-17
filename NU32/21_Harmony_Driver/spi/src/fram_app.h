#ifndef _FRAM_APP_H
#define _FRAM_APP_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "system_config.h"
#include "system_definitions.h"

/* FRAM Command definitions */
#define FRAM_CMD_WREN               0x06
#define FRAM_CMD_WRITE              0x02
#define FRAM_CMD_READ               0x03

/* FRAM address to write temperature value to */
#define TEMPERATURE_LOG_START_ADDR         0x0000
#define MAX_TEMPERATURE_LOGS               10

typedef enum
{
  /* Application's state machine's initial state. */
  FRAM_APP_STATE_INIT=0,
  FRAM_APP_STATE_CHECK_WRITE_REQ,
  FRAM_APP_STATE_CHECK_READ_REQ,
  FRAM_APP_STATE_CHECK_WRITE_REQ_STATUS,
  FRAM_APP_STATE_CHECK_READ_REQ_STATUS,
  FRAM_APP_STATE_ERROR,
  } FRAM_APP_STATES;

typedef enum
{
  FRAM_APP_SPI_BUFFER_STATUS_COMPLETE = 0,
  FRAM_APP_SPI_BUFFER_STATUS_ERROR,
  FRAM_APP_SPI_BUFFER_STATUS_INVALID,
}FRAM_APP_SPI_BUFFER_STATUS;  

typedef struct
{
  uint8_t                             wrBuffer[50];
  uint8_t                             nBytes;
  FRAM_APP_SPI_BUFFER_STATUS          bufferStatus;
  DRV_SPI_BUFFER_HANDLE               bufferHandle;
} FRAM_APP_SPI_WR_REQ;

typedef struct
{
  uint8_t                             wrBuffer[10];
  uint8_t                             nWrBytes;
  uint8_t                             rdBuffer[50];
  uint8_t                             nRdBytes;
  FRAM_APP_SPI_BUFFER_STATUS          bufferStatus;
  DRV_SPI_BUFFER_HANDLE               bufferHandle;
} FRAM_APP_SPI_WR_RD_REQ;

typedef struct
{
    /* The application's current state */
    FRAM_APP_STATES         state;
    DRV_HANDLE              handle;
    /* Data structure to hold SPI request to enable FRAM writes */
    FRAM_APP_SPI_WR_REQ     framWriteEnableData;
    /* Data structure to hold SPI request to write to FRAM */
    FRAM_APP_SPI_WR_REQ     framWriteData;
    /* Data structure to hold SPI request to read from FRAM */
    FRAM_APP_SPI_WR_RD_REQ  framReadData;
    uint8_t                 keyValue;
    bool                    isWriteReq;
    uint8_t                 wrIndex;
} FRAM_APP_DATA;


// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Routines
// *****************************************************************************
// *****************************************************************************

void FRAM_APP_SetWriteReq(void);

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

void FRAM_APP_Initialize ( void );

void FRAM_APP_Tasks( void );

#endif /* _FRAM_APP_H */

/*******************************************************************************
 End of File
 */

