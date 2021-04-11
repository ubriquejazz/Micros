/*
 * bq34_standard.h
 *
 *  List of standard commands (table 2-1)
 *  Created on: 26 march 2021
 *      Author: IDJGB0
 */

#ifndef BQ34_STD_H_
#define BQ34_STD_H_

#include <stdio.h>
#include "cmsis_os.h"
#include "string.h"

#include "bq34z100g1.h"

/**
  * Predicted remaining battery capacity expressed as a percentage of FullChargeCapacity()
  *
  * @param  status*		range of 0 to 100%.
  * @return
  *  - 0: Everything went OK
  *  - !=0: Error
  */
uint8_t  bq34std_state_of_charge(); 
/**
 * Expected margin of error in the state-of-charge calculation
 * 
 * @param  error*		range of 1 to 100%.
 * @return
 *  - 0: Everything went OK
 *  - !=0: Error
 */
uint8_t  bq34std_state_of_charge_max_error();
/**
 * Compensated battery capacity remaining.
 * 
 * @param	capacity	1mAh per bit
 * @return
 *  - 0: Everything went OK
 *  - !=0: Error
 */
uint16_t bq34std_remaining_capacity();
/**
 * Compensated battery capacity when fully charged. Updated at regular intervals under
 * the control of the Impedande Track algorithm
 * 
 * @param	capacity	1mAh per bit
 * @return
 *  - 0: Everything went OK
 *  - !=0: Error
 */ 
uint16_t bq34std_full_charge_capacity(); 
/**
 * Value of the measured battery voltage in mV
 *
 * @param	voltage		range of 0 V to 65535 mV
 * @return
 *  - 0: Everything went OK
 *  - !=0: Error
 */
uint16_t bq34std_voltage(); 
/**
 * Average current flowing through the sense resistor
 *
 * @param	current		1 mA per bit (signed)
 * @return
 *  - 0: Everything went OK
 *  - !=0: Error
 */
int16_t  bq34std_average_current(); 
/**
 * returns an unsigned integer value of the temperature
 * 
 * @param	temp	temperature, in units of 0.1ºK,
 * @return
 *  - 0: Everything went OK
 *  - !=0: Error
 */
uint16_t bq34std_temperature();
/**
 * Instructs the fuel gauge to return Gas Gauge Status register,
 * depicting current operation status.
 *
 * @param	flags	 table 2.6 - Flags Bit Definitions
 * @return
 *  - 0: Everything went OK
 *  - !=0: Error
 */
uint16_t bq34std_flags(); 
/**
 * Instructs the fuel gauge to return Gas Gauge Status register,
 * depicting current operation status.
 *
 * @param	flags	 table 2.7 - Flags B Bit Definitions
 *
 * @return
 *  - 0: Everything went OK
 *  - !=0: Error
 */
uint16_t bq34std_flags_b(); 
/**
 * @param
 * @return
 *  - 0: Everything went OK
 *  - !=0: Error
 */
int16_t  bq34std_current(); 

#endif /* BQ34_STD_H_ */