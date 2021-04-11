/*
 * bq34_standard.h
 *
 *  List of extended commands (table 2-8)
 *  Created on: 26 march 2021
 *      Author: IDJGB0
 */

#ifndef BQ34_STD_H_
#define BQ34_STD_H_

#include <stdio.h>
#include "cmsis_os.h"
#include "string.h"

#include "bq34z100g1.h"

uint16_t bq34ext_average_time_to_empty();
uint16_t bq34ext_average_time_to_full(); 
int16_t  bq34ext_passed_charge(); 
uint16_t bq34ext_do_d0_time(); 
uint16_t bq34ext_available_energy();
uint16_t bq34ext_average_power(); 
uint16_t bq34ext_serial_number(); 
uint16_t bq34ext_internal_temperature(); 
uint16_t bq34ext_cycle_count(); 
uint16_t bq34ext_state_of_health(); 
uint16_t bq34ext_charge_voltage(); 
uint16_t bq34ext_charge_current(); 
uint16_t bq34ext_pack_configuration();
uint16_t bq34ext_design_capacity();

/* bq34fl */

uint8_t  bq34ext_grid_number();
/**
 * This read-only command returns the learned status of the resistance table
 *
 * @param		status - table 2.9 - Learned status
 *
 * @return
 *  - 0: Everything went OK
 *  - !=0: Error
 */
uint8_t  bq34ext_learned_status();
/**
 * Depth of discharge (DOD) at the end of charge.
 * @return
 *  - 0: Everything went OK
 *  - !=0: Error
 */
uint16_t bq34ext_dod_at_eoc(); 
/**
 * Initial capacity QStart() calculated from IT simulation.
 * @return
 *  - 0: Everything went OK
 *  - !=0: Error
 */
uint16_t bq34ext_q_start(); 
/**
 * Remaining capacity from IT simulation without the effects of the smoothing function.
 * @return
 *  - 0: Everything went OK
 *  - !=0: Error
 */
 uint16_t bq34ext_true_rc(); 
/**
 * Full charge capacity from IT simulation without the effects of smoothing function.
 *
 * @return
 *  - 0: Everything went OK
 *  - !=0: Error
 */
uint16_t bq34ext_true_fcc(); 
/**
 * Returns the time past since last state change (DISCHARGE, CHARGE, REST).
 * @return
 *  - 0: Everything went OK
 *  - !=0: Error
 */
uint16_t bq34ext_state_time(); ;
/**
 * Returns the passed capacity since the last Qmax DOD update.
 * @return
 *  - 0: Everything went OK
 *  - !=0: Error
 */
uint16_t bq34ext_q_max_passed_q(); 
/**
 * Indicates the depth of discharge during the most recent OCV reading.
 * @return
 *  - 0: Everything went OK
 *  - !=0: Error
 */
uint16_t bq34ext_dod_0(); 
/**
 * Returns the DOD0 saved to be used for next QMax update of Cell 1.
 * The value is only valid when [VOK] = 1.
 * @return
 *  - 0: Everything went OK
 *  - !=0: Error
 */
uint16_t bq34ext_q_max_dod_0();
/**
 * Returns the time since the last Qmax DOD update.
 * @return
 *  - 0: Everything went OK
 *  - !=0: Error
 */
uint16_t bq34ext_q_max_time();

#endif /* BQ34_STD_H_ */