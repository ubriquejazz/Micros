/*
 * bq_config.h
 *
 */

#ifndef BQ_CONFIG_H_
#define BQ_CONFIG_H_

#include <stdio.h>
#include "cmsis_os.h"
#include "string.h"
#include "bq76952.h"
#include "IdnDefinitions.h"

#define BQ76952_REG_SET_CFGUPDATE 0x90  // W. Enters CONFIG_UPDATE mode
#define BQ76952_REG_EXIT_CFGUPDATE  0x92  // W. Also clears the Battery Status() [POR] and Battery Status()[WD] bits.
#define BQ76952_REG_FET_ENABLE    0x22  // W. FET_EN = 0 means Test Mode. FET_EN = 1 means FW Control
#define BQ76952_REG_RESET         0x12  // W. Reset the device

// Bits in Safety Fault and Alert Registers (SA, SB)
#define BIT_SA_SC_DCHG            7
#define BIT_SA_OC2_DCHG           6
#define BIT_SA_OC1_DCHG           5
#define BIT_SA_OC_CHG             4
#define BIT_SA_CELL_OV            3
#define BIT_SA_CELL_UV            2
#define BIT_SB_OTF                7
#define BIT_SB_OTINT              6
#define BIT_SB_OTD                5
#define BIT_SB_OTC                4
#define BIT_SB_UTINT              2
#define BIT_SB_UTD                1
#define BIT_SB_UTC                0

/* Exported types ---------------------------------------------------- */

typedef union {
  struct {
    uint8_t SC_DCHG            :1;
    uint8_t OC2_DCHG           :1;
    uint8_t OC1_DCHG           :1;
    uint8_t OC_CHG             :1;
    uint8_t CELL_OV            :1;
    uint8_t CELL_UV            :1;
  } bits;
  uint8_t value;
} sa_protection_t;

typedef union {
  struct {
    uint8_t OVERTEMP_FET      :1;
    uint8_t OVERTEMP_INTERNAL :1;
    uint8_t OVERTEMP_DCHG     :1;
    uint8_t OVERTEMP_CHG      :1;
    uint8_t UNDERTEMP_INTERNAL  :1;
    uint8_t UNDERTEMP_DCHG    :1;
    uint8_t UNDERTEMP_CHG     :1;
  } bits;
  uint8_t value;
} sb_protection_t;

typedef enum {
  PROTECTION_A,     // SA Register: SCD, OCD2, OCD1, OCC, COV, CUV
  PROTECTION_B,   // SB Register: OTF, OTINT, OTD, OTC, UTINT, UTD, UTC
    PROTECTION_C,   // SC Register: OCD3, SCDL, OCDL, COVL, PTO, HWDF
    NumOfProtections
} type_protection_t;

typedef enum {
  REG12,             // 7:4 REG2 ; 3:0 REG1
  REG0,            // REG0_EN only (bit 0)
  NumOfRegulators
} regulator_t;

typedef enum {
  CFETOFF,           // BAT_CFETOFF (in)
  DFETOFF,           // BAT_DFETOFF (in)
  ALERT,           // BAT_ALERT (output)
  TS1,           // Thermistor NTC1 (analog)
  TS2,               // Thermistor NTC2 (analog)
  TS3,           // Thermistor NTC3 (analog)
  HDQ,               // Thermistor for the FET temp. control
  DCHG,              // BAT_DDSG (output)
  DDSG,              // BAT_DCHG (output)
  NumOfPins
} bq_pin_t;

/* Direct Commands ----------------------------------------------------------- */

idn_RetVal_t BQCFG_Get_PrimaryProtection (sa_protection_t*, char*);
idn_RetVal_t BQCFG_Get_TemperatureProtection (sb_protection_t*, char*);

idn_RetVal_t BQCFG_Set_AlarmStatus (uint16_t status, char*);
idn_RetVal_t BQCFG_Get_AlarmStatus (uint16_t* status, char*);

idn_RetVal_t BQCFG_Set_RawAlarmStatus (uint16_t raw_status, char*);
idn_RetVal_t BQCFG_Get_RawAlarmStatus (uint16_t* raw_status, char*);

idn_RetVal_t BQCFG_Set_AlarmEnable (uint16_t enable, char*);
idn_RetVal_t BQCFG_Get_AlarmEnable (uint16_t* enable, char*);

/* Sub-commands --------------------------------------------------------------*/

idn_RetVal_t BQCFG_Get_EnableRegulator (regulator_t, uint8_t* result, char*);
idn_RetVal_t BQCFG_Set_EnableRegulator (regulator_t, uint8_t value, char*);

idn_RetVal_t BQCFG_Get_EnableProtection (type_protection_t, uint8_t* result, char*);
idn_RetVal_t BQCFG_Set_EnableProtection (type_protection_t, uint8_t value, char*);

idn_RetVal_t BQCFG_Get_PinConifg (bq_pin_t, uint8_t* result, char*);
idn_RetVal_t BQCFG_Set_PinConifg (bq_pin_t, uint8_t value, char*);

idn_RetVal_t BQCFG_Get_ManufacturerStatus (uint16_t* status, char* );
idn_RetVal_t BQCFG_Get_DeviceNumber (uint16_t* device_number, char*);

idn_RetVal_t BQCFG_Set_FETEnable (char*);
idn_RetVal_t BQCFG_Set_Reset (char*);
idn_RetVal_t BQCFG_Set_ConfigUpdateMode (uint8_t mode, char*);

/* WR-RD Sub-commands ---------------------------------------------------------*/

idn_RetVal_t BQCFG_Set_VCellMode (uint16_t mode, char*);
idn_RetVal_t BQCFG_Get_VCellMode (uint16_t* mode, char*);

idn_RetVal_t BQCFG_Set_AlarmMask (uint16_t mask, char*);
idn_RetVal_t BQCFG_Get_AlarmMask (uint16_t* mask, char*);

#endif /* BQ_CONFIG_H_ */
