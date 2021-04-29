/*
 * bq_config.h
 *
 */
#ifndef BQ_CONFIG_H_
#define BQ_CONFIG_H_

/* Exported types ---------------------------------------------------- */

typedef enum {
  PROTECTION_A,     // Table 13-18: SCD, OCD2, OCD1, OCC, COV, CUV
  PROTECTION_B,   // Table 13-19: OTF, OTINT, OTD, OTC, UTINT, UTD, UTC
  PROTECTION_C,   // Table 13-20: OCD3, SCDL, OCDL, COVL, PTO, HWDF
  NumOfProtections
} type_protection_t;

typedef enum {
  PFAIL_A,    // Table 13-18: SCD, OCD2, OCD1, OCC, COV, CUV
  PFAIL_B,    // Table 13-19: OTF, OTINT, OTD, OTC, UTINT, UTD, UTC
  PFAIL_C,    // Table 13-20: OCD3, SCDL, OCDL, COVL, PTO, HWDF
  PFAIL_D,
  NumOfPFails
} type_pfail_t;

typedef enum {
  REG12,    // 7:4 REG2 ; 3:0 REG1
  REG0,     // REG0_EN only (bit 0)
  NumOfRegulators
} regulator_t;

/* Regulator Sub-commands ---------------------------------------------------------*/

inline idn_RetVal_t BQCFG_Get_EnableRegulator (regulator_t regx, uint8_t* result, char* log)
{
  idn_RetVal_t ret = IDN_OK;
  uint16_t addr = DATA_MEM_ADDR(0x36 + regx);     // settings::configuration
  ret = BQ76952_Getter(addr, 1);
  *result = Bq76952.buf[0];
  sprintf(log, "Get Enable Regulator %d : 0x%02x", regx, Bq76952.buf[0]);
  return ret;
}

inline idn_RetVal_t BQCFG_Set_EnableRegulator (regulator_t regx, uint8_t value, char* log)
{
  idn_RetVal_t ret = IDN_OK;
  uint16_t addr = DATA_MEM_ADDR(0x36 + regx);       // settings::configuration
  ret = BQ76952_Setter_8Bits(addr, value);
  sprintf(log, "Set Regulator %d to 0x%02x", regx, value);
  return ret;
}

/* Protection Sub-commands --------------------------------------------------------*/
idn_RetVal_t BQCFG2_Get_EnableProtection (type_protection_t, uint8_t* result, char*);
idn_RetVal_t BQCFG2_Set_EnableProtection (type_protection_t, uint8_t value, char*);
idn_RetVal_t BQCFG2_Get_CFET_Protection (type_protection_t, uint8_t* result, char*);
idn_RetVal_t BQCFG2_Set_CFET_Protection (type_protection_t, uint8_t value, char*);
idn_RetVal_t BQCFG2_Get_DFET_Protection (type_protection_t, uint8_t* result, char*);
idn_RetVal_t BQCFG2_Set_DFET_Protection (type_protection_t, uint8_t value, char*);

/* PFs Sub-commands ---------------------------------------------------------------*/
idn_RetVal_t BQCFG2_Get_EnablePFail (type_pfail_t, uint8_t* result, char*);
idn_RetVal_t BQCFG2_Set_EnablePFail (type_pfail_t, uint8_t value, char*);

#endif /* BQ_CONFIG_H_ */
