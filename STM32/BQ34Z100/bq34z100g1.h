/*
 * bq34z100g1.h
 *
 *  Created on: 26 march 2021
 *      Author: IDJGB0
 */

#ifndef BQ34Z100G1_H_
#define BQ34Z100G1_H_

#include <stdio.h>
#include "cmsis_os.h"
#include "string.h"

#define BQ34Z100_G1_ADDRESS 0xAA

void 	 bq34_unsealed ();
uint16_t bq34_read_register(uint8_t address, uint8_t length);
void 	 bq34_write_reg(uint8_t addr, uint8_t val);
double 	 bq34_xemics_to_double(uint32_t value);
uint32_t bq34_double_to_xemics(double value);
    
#endif /* BQ34Z100G1_H_ */