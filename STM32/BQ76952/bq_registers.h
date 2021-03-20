/*
 * bq_registers.h
 *
 *  Created on: 13 ene. 2021
 *      Author: IDJGB0
 */

#ifndef BQ_REGISTERS_H_
#define BQ_REGISTERS_H_

/* Pimary Protection - Safety x Register ----------------------------------- */

typedef union {
  struct {
    uint8_t SCD     :1;
    uint8_t OCD2    :1;
    uint8_t OCD1    :1;
    uint8_t OCC     :1;
    uint8_t COV     :1;
    uint8_t CUV     :1;
    uint8_t RSVD_1  :1;
    uint8_t RSVD_0  :1;
  } bits;
  uint8_t value;
} sa_protection_t;

typedef union {
  struct {
    uint8_t OTF     :1;
    uint8_t OTINT   :1;
    uint8_t OTD     :1;
    uint8_t OTC     :1;
    uint8_t RSVD_3  :1;
    uint8_t UTINT   :1;
    uint8_t UTD     :1;
    uint8_t UTC     :1;
  } bits;
  uint8_t value;
} sb_protection_t;

typedef union {
  struct {
    uint8_t OCD3    :1;   
    uint8_t SCDL    :1;
    uint8_t OCDL    :1;
    uint8_t COVL    :1;
    uint8_t HWDF    :1;
    uint8_t RSVD_2  :1;
    uint8_t RSVD_1  :1;
    uint8_t RSVD_0  :1;
  } bits;
  uint8_t value;
} sc_protection_t;

idn_RetVal_t BQREG_Process_SA_Protection (uint8_t data, sa_protection_t* prot);
idn_RetVal_t BQREG_Process_SB_Protection (uint8_t data, sb_protection_t* temp);
idn_RetVal_t BQREG_Process_SC_Protection (uint8_t data, sc_protection_t* prot);

/* Secondary Protection PFs Register --------------------------------------- */

typedef union {
  struct {
    uint8_t CUDEP           :1;
    uint8_t SOTF            :1;
    uint8_t RSVD_5          :1;
    uint8_t SOT             :1;
    uint8_t SOCD            :1;
    uint8_t SOCC            :1;
    uint8_t SOV             :1;
    uint8_t SUV             :1;
  } bits;
  uint8_t value;
} pfa_safety_t;

typedef union {
  struct {
    uint8_t SCDL            :1;
    uint8_t RSVD_6          :1;
    uint8_t RSVD_5          :1;
    uint8_t VIMA            :1;
    uint8_t VIMR            :1;
    uint8_t _2LVL           :1;
    uint8_t DFETF           :1;
    uint8_t CFETF           :1;
  } bits;
  uint8_t value;
} pfb_safety_t;

typedef union {
  struct {
    uint8_t RSVD_7          :1;
    uint8_t RSVD_6          :1;
    uint8_t RSVD_5          :1;
    uint8_t RSVD_4          :1;
    uint8_t RSVD_3          :1;
    uint8_t RSVD_2          :1;
    uint8_t RSVD_1          :1;            
    uint8_t OTPF            :1;
  } bits;
  uint8_t value;
} pfc_safety_t;

typedef union {
  struct {
    uint8_t RSVD_7          :1;
    uint8_t RSVD_6          :1;
    uint8_t RSVD_5          :1;
    uint8_t RSVD_4          :1;
    uint8_t RSVD_3          :1;
    uint8_t RSVD_2          :1;
    uint8_t RSVD_1          :1;   
    uint8_t TOSF            :1;
  } bits;
  uint8_t value;
} pfd_safety_t;

idn_RetVal_t BQREG_Process_PFA_Safety (uint8_t data, pfa_safety_t* prot);
idn_RetVal_t BQREG_Process_PFB_Safety (uint8_t data, pfb_safety_t* prot);
idn_RetVal_t BQREG_Process_PFC_Safety (uint8_t data, pfc_safety_t* prot);
idn_RetVal_t BQREG_Process_PFD_Safety (uint8_t data, pfd_safety_t* prot);

/* Configuration Register -------------------------------------------------- */

typedef union {
  struct {
    uint8_t RSVD_7        :1;
    uint8_t RSVD_6        :1;
    uint8_t RSVD_5        :1;
    uint8_t CB_NO_CMD     :1;
    uint8_t CB_NOSLEEP    :1;
    uint8_t CB_SLEEP      :1;
    uint8_t CB_RLX        :1;
    uint8_t CB_CHG        :1;
  } bits;
  uint8_t value;
} balance_config_t;

typedef union {
  struct {
    uint8_t RSVD_7        :1;
    uint8_t ALRT_PIN      :1;
    uint8_t DDSG_PIN      :1;
    uint8_t DCHG_PIN      :1;
    uint8_t PDSG_FET      :1;
    uint8_t DSG_FET       :1;
    uint8_t PCHG_FET      :1;
    uint8_t CHG_FET       :1;
  } bits;
  uint8_t value;
} fet_status_t;

typedef union {
  struct {
    uint8_t RSVD_7        :1;
    uint8_t RSVD_6        :1;
    uint8_t FET_INIT_OFF  :1;
    uint8_t PDSG_EN       :1;
    uint8_t FET_CTRL_EN   :1;
    uint8_t HOST_FET_EN   :1;
    uint8_t SLEEPCHG      :1;
    uint8_t SFET          :1;
  } bits;
  uint8_t value;
} fet_options_t;

typedef union {
  struct {
    uint8_t RSVD_7        :1;
    uint8_t RSVD_6        :1;
    uint8_t RSVD_5        :1;
    uint8_t RSVD_4        :1;
    uint8_t RSVD_3        :1;
    uint8_t SFMODE_SLEEP  :1;
    uint8_t LVEN          :1;
    uint8_t CPEN          :1;
  } bits;
  uint8_t value;
} chg_pump_ctrl_t;

typedef union {
  struct {
    uint8_t RSVD_7        :1;
    uint8_t RSVD_6        :1;
    uint8_t RSVD_5        :1;
    uint8_t RSVD_4        :1;
    uint8_t RSVD_3        :1;
    uint8_t DEEPSLEEP     :1;
    uint8_t LD_TIMEOUT    :1;
    uint8_t LD_ON         :1;
  } bits;
  uint8_t value;
} ctrl_status_t;

typedef union {
  struct {
    uint8_t OTPW_EN       :1;
    uint8_t PF_EN         :1;
    uint8_t RSVD_4        :1;
    uint8_t FET_EN        :1;
    uint8_t RSVD_3        :1;
    uint8_t RSVD_2        :1;
    uint8_t RSVD_1        :1;
    uint8_t RSVD_0        :1;
  } bits;
  uint8_t value;
} mfg_status_init_t;

typedef union {
  struct {
    uint8_t RSVD_7          :1;
    uint8_t RSVD_6          :1;
    uint8_t DPSLP_OT        :1;
    uint8_t SHUT_TS2        :1;
    uint8_t DPSLP_PD        :1;
    uint8_t DPSLP_LDO       :1;
    uint8_t DPSLP_LFO       :1;
    uint8_t SLEEP           :1;
  } bits;
  uint8_t value;
} pwr_cfg_hgh_t;

typedef union {
  struct {
    uint8_t OTSD            :1;
    uint8_t FASTADC         :1;
    uint8_t CB_LOOP_SLOW_1  :1;
    uint8_t CB_LOOP_SLOW_0  :1;
    uint8_t LOOP_SLOW_1     :1;
    uint8_t LOOP_SLOW_0     :1;
    uint8_t RSVD_1          :1;     
    uint8_t RSVD_0          :1; 
  } bits;
  uint8_t value;
} pwr_cfg_low_t;

idn_RetVal_t BQREG_Process_PowerConfig (uint16_t data, pwr_cfg_hgh_t* h, pwr_cfg_low_t* l);
idn_RetVal_t BQREG_Process_Balance_Config (uint8_t data, balance_config_t* b);
idn_RetVal_t BQREG_Process_Fet_Status (uint8_t data, fet_status_t* f);
idn_RetVal_t BQREG_Process_Fet_Options (uint8_t data, fet_options_t* f);
idn_RetVal_t BQREG_Process_ChgPump_Control (uint8_t data, chg_pump_ctrl_t* f);

idn_RetVal_t BQREG_Process_Ctrl_Status (uint16_t data, ctrl_status_t* l);
idn_RetVal_t BQREG_Process_Mfg_Status_Init (uint16_t data, mfg_status_init_t* l);

/* Battery Status ---------------------------------------------------------- */

typedef union {
  struct {
    uint8_t SLEEP     :1;
    uint8_t RSVD_6    :1;
    uint8_t SD_CMD   	:1;
    uint8_t PF     		:1;
    uint8_t SS     		:1;
    uint8_t FUSE   		:1;
    uint8_t SEC1     	:1;
    uint8_t SEC0     	:1;
  } bits;
  uint8_t value;
} batt_status_hgh_t;

typedef union {
  struct {
    uint8_t OTPB     	:1;
    uint8_t OTPW   		:1;
    uint8_t COW_CHK   :1;
    uint8_t WD     		:1;
    uint8_t POR     	:1;
    uint8_t SLEEP_EN  :1;
    uint8_t PCHG_MODE :1;
    uint8_t CFGUPDATE :1;
  } bits;
  uint8_t value;
} batt_status_low_t;

idn_RetVal_t BQREG_Process_Batt_Status (uint16_t data, batt_status_hgh_t* h, batt_status_low_t* l);

/* Alarm Status, Enable, Raw ----------------------------------------------- */

typedef union {
  struct {
    uint8_t SSBC        :1;
    uint8_t SSA         :1;
    uint8_t PF          :1;
    uint8_t RSVD_4      :1;
    uint8_t MSK_SFALERT :1;
    uint8_t MSK_PFALERT :1;
    uint8_t INITSTART   :1;
    uint8_t INITCOMP    :1;
  } bits;
  uint8_t value;
} alarm_hgh_t;

typedef union {
  struct {
    uint8_t FULLSCAN    :1;
    uint8_t XCHG        :1;
    uint8_t XDSG        :1;
    uint8_t SHUTV       :1;
    uint8_t FUSE        :1;
    uint8_t CB          :1;
    uint8_t ADSCAN      :1;
    uint8_t WAKE        :1;
  } bits;
  uint8_t value;
} alarm_low_t;  

idn_RetVal_t BQREG_Process_Alarm (uint16_t data, alarm_hgh_t* h, alarm_low_t* l);

#endif /* BQ_REGISTERS_H_ */
