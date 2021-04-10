/*
 * bq34_standard.h
 *
 *  Created on: 26 march 2021
 *      Author: IDJGB0
 */

#ifndef BQ34_STD_H_
#define BQ34_STD_H_

#include <stdio.h>
#include "cmsis_os.h"
#include "string.h"

#include "bq34z100g1.h"

uint16_t bq34std_control_status(); 
uint16_t bq34std_device_type();
uint16_t bq34std_fw_version(); 
uint16_t bq34std_hw_version();
uint16_t bq34std_reset_data(); 
uint16_t bq34std_prev_macwrite(); 
uint16_t bq34std_chem_id(); 
uint16_t bq34std_board_offset(); 
uint16_t bq34std_cc_offset(); 
uint16_t bq34std_cc_offset_save(); 
uint16_t bq34std_df_version(); 
uint16_t bq34std_set_fullsleep(); 
uint16_t bq34std_static_chem_chksum(); 
uint16_t bq34std_sealed(); 
uint16_t bq34std_it_enable(); 
uint16_t bq34std_cal_enable(); 
uint16_t bq34std_reset(); 
uint16_t bq34std_exit_cal(); 
uint16_t bq34std_enter_cal(); 
uint16_t bq34std_offset_cal(); 
uint8_t  bq34std_state_of_charge(); 
uint8_t  bq34std_state_of_charge_max_error();
uint16_t bq34std_remaining_capacity(); 
uint16_t bq34std_full_charge_capacity(); 
uint16_t bq34std_voltage(); 
int16_t  bq34std_average_current(); 
uint16_t bq34std_temperature(); 
uint16_t bq34std_flags(); 
uint16_t bq34std_flags_b(); 
int16_t  bq34std_current(); 
uint16_t bq34std_average_time_to_empty();
uint16_t bq34std_average_time_to_full(); 
int16_t  bq34std_passed_charge(); 
uint16_t bq34std_do_d0_time(); 
uint16_t bq34std_available_energy();
uint16_t bq34std_average_power(); 
uint16_t bq34std_serial_number(); 
uint16_t bq34std_internal_temperature(); 
uint16_t bq34std_cycle_count(); 
uint16_t bq34std_state_of_health(); 
uint16_t bq34std_charge_voltage(); 
uint16_t bq34std_charge_current(); 
uint16_t bq34std_pack_configuration();
uint16_t bq34std_design_capacity();
uint8_t  bq34std_grid_number();
uint8_t  bq34std_learned_status();
uint16_t bq34std_dod_at_eoc(); 
uint16_t bq34std_q_start(); 
uint16_t bq34std_true_fcc(); 
uint16_t bq34std_state_time(); ;
uint16_t bq34std_q_max_passed_q(); ;
uint16_t bq34std_dod_0(); ;
uint16_t bq34std_q_max_dod_0();
uint16_t bq34std_q_max_time();

void bq34std_enter_calibration();
void bq34std_exit_calibration();
void bq34std_calibrate_cc_offset();
void bq34std_calibrate_board_offset();

static inline bool bq34_checksum(uint8_t delay)
{
	bq34_write_reg(0x60, bq34_flash_block_checksum(););
    OsDelay(delay);
    bq34std_reset();
    OsDelay(delay);
    return true;
}

#endif /* BQ34_STD_H_ */