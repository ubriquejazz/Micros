/*
 * bq34_control.h
 *
 *  List of control subcommands (table 2-2)
 *  Created on: 26 march 2021
 *      Author: IDJGB0
 */

#ifndef BQ34_STD_H_
#define BQ34_STD_H_

#include <stdio.h>
#include "cmsis_os.h"
#include "string.h"

#include "bq34z100g1.h"


uint16_t bq34ctl_read_control(uint8_t address_lsb, uint8_t address_msb);

/**
  * Instructs the fuel gauge to return status information
  *
  * @param  status   table 2.3
  * @return
  *  - 0: Everything went OK
  *  - !=0: Error
  */
uint16_t bq34ctl_control_status(); 
uint16_t bq34ctl_device_type();
uint16_t bq34ctl_fw_version(); 
uint16_t bq34ctl_hw_version();
uint16_t bq34ctl_reset_data(); 
uint16_t bq34ctl_prev_macwrite(); 
uint16_t bq34ctl_chem_id(); 
uint16_t bq34ctl_board_offset(); 
uint16_t bq34ctl_cc_offset(); 
uint16_t bq34ctl_cc_offset_save(); 
uint16_t bq34ctl_df_version(); 
uint16_t bq34ctl_set_fullsleep(); 
uint16_t bq34ctl_static_chem_chksum(); 
uint16_t bq34ctl_sealed(); 
uint16_t bq34ctl_it_enable(); 
uint16_t bq34ctl_cal_enable(); 
uint16_t bq34ctl_reset(); 
uint16_t bq34ctl_exit_cal(); 
uint16_t bq34ctl_enter_cal(); 
uint16_t bq34ctl_offset_cal(); 

void bq34ctl_enter_calibration();
void bq34ctl_exit_calibration();
void bq34ctl_calibrate_cc_offset();
void bq34ctl_calibrate_board_offset();

#endif /* BQ34_STD_H_ */