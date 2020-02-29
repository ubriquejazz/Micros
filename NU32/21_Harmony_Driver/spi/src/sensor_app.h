#ifndef _SENSOR_APP_H
#define _SENSOR_APP_H

/*!\name      sensor_app.h
 *
 * \brief     Sensor Application
 *
 * \author    Juan Gago
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "system_config.h"
#include "system_definitions.h"
#include "b_hanlde.h"

#define TEMPERATURE_READ_PERIOD                                     500

#define WR(addr)                                                    (addr & 0x7F)
#define RD(addr)                                                    (addr | 0x80)
#define BME280_RST_REG                                              0xE0  /*Softreset Register */
#define BME280_TEMPERATURE_CALIB_DIG_T1_LSB_REG                     0x88  /*Temperature Calibration Start Register*/
#define BME280_CTRL_MEAS_REG                                        0xF4  /*Ctrl Measure Register */
#define BME280_SOFT_RESET                                           0xB6  /*Soft Reset Value*/
#define BME280_NORMAL_MODE                                          0x03  /*Normal Mode Value*/
#define BME280_OVERSAMP_1X                                          0x20  /*Temperature Oversampling Value*/
#define BME280_TEMP_REG_ADDR                                        0xFA  /*Temperature Register*/
#define BME280_CHIP_ID_REG_ADDR                                     0xD0

typedef enum
{
  /* Application's state machine's initial state. */
  SENSOR_APP_STATE_INIT=0,
  SENSOR_APP_STATE_SENSOR_INIT,
  SENSOR_APP_STATE_WAIT_INIT_COMPLETE,
  SENSOR_APP_STATE_WAIT_MEAS_REQ,
  SENSOR_APP_STATE_WAIT_MEAS_COMPLETE,
  SENSOR_APP_STATE_ERROR,
} SENSOR_APP_STATES;

typedef struct
{
  uint8_t                             wrBuffer[10];
  uint8_t                             nBytes;
  BUFFER_STATUS	                      bufferStatus;
  DRV_SPI_BUFFER_HANDLE               bufferHandle;
} SENSOR_APP_SPI_WR_REQ;

typedef struct
{
  uint8_t                             wrBuffer[10];
  uint8_t                             nWrBytes;
  uint8_t                             rdBuffer[10];
  uint8_t                             nRdBytes;
  BUFFER_STATUS	                      bufferStatus;
  DRV_SPI_BUFFER_HANDLE               bufferHandle;
} SENSOR_APP_SPI_WR_RD_REQ;

typedef struct
{
  /* The application's current state */
  SENSOR_APP_STATES           state;
  DRV_HANDLE                  handle;
  uint8_t                     sensorCalibValues[6];
  volatile bool               readTemperatureReq;
  float                       temperature;

  /* control data: write oversampling */
  SENSOR_APP_SPI_WR_REQ       sensorControlData;

  /* read calibration data */
  SENSOR_APP_SPI_WR_RD_REQ    calibrationData;

  /* temperature meas. */
  SENSOR_APP_SPI_WR_RD_REQ    temperatureData;

	/* Un-comment below line of code for Lab 3 */
  // SemaphoreHandle_t 	xSemaphore;
  
} SENSOR_APP_DATA;


// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Routines
// *****************************************************************************
// *****************************************************************************

float SENSOR_APP_GetTemperature(void);

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

void SENSOR_APP_Initialize ( void );

void SENSOR_APP_Tasks( void );

#endif /* _SENSOR_APP_H */

/*******************************************************************************
 End of File
 */

