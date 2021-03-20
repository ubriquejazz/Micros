/*
 * bq_protection.h
 *
 *  Created on: 13 ene. 2021
 *      Author: IDJGB0
 */

#ifndef BQ_CBALANCING_H_
#define BQ_CBALANCING_H_

#include <stdio.h>
#include "cmsis_os.h"
#include "string.h"
#include "bq76952.h"

idn_RetVal_t BQCB_Set_MinCellTemp (int8_t, char*);                 
idn_RetVal_t BQCB_Set_MaxCellTemp (int8_t, char*);   
idn_RetVal_t BQCB_Set_Interval (uint8_t, char*);
idn_RetVal_t BQCB_Set_MaxCells (uint8_t, char*);
idn_RetVal_t BQCB_Set_MinCellV (int16_t, char*);
idn_RetVal_t BQCB_Set_MinDelta (uint8_t, char*);
idn_RetVal_t BQCB_Set_StopDelta (uint8_t, char*);

#endif /* BQ_CBALANCING_H_ */