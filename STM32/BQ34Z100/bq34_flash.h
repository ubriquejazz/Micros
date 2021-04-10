/*
 * bq34_flash.h
 *
 *  Created on: 26 march 2021
 *      Author: IDJGB0
 */

#ifndef BQ34_FLASH_H_
#define BQ34_FLASH_H_

#include <stdio.h>
#include "cmsis_os.h"
#include "string.h"

typedef struct
{
    int8_t  set;
    int8_t  clear;
} voltage_t;

typedef struct 
{
    int16_t     current;
    int16_t     min_capacity;
    int16_t     cell_voltage;
    uint8_t     window;
    voltage_t   tca;
    voltage_t   fc;
} taper_t;

bool bq34fl_update_design_capacity(int16_t capacity);
bool bq34fl_update_q_max(int16_t capacity)
bool bq34fl_update_design_energy(int16_t energy); 
bool bq34fl_update_cell_charge_voltage_range(uint16_t t1_t2, uint16_t t2_t3, uint16_t t3_t4);
bool bq34fl_update_number_of_series_cells(uint8_t cells);
bool bq34fl_update_pack_configuration(uint16_t config);
bool bq34fl_update_charge_termination_parameters(taper_t);

void bq34fl_calibrate_cc_offset() 
void bq34fl_calibrate_board_offset()
void bq34fl_calibrate_voltage_divider(uint16_t applied_voltage, uint8_t cells_count);
void bq34fl_calibrate_sense_resistor(int16_t applied_current);

#endif /* BQ34_FLASH_H_ */