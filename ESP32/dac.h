/*
 * functions.h
 *
 *  Created on: 23 Jun 2021
 *      Author: IDJGB0
 */

#ifndef MAIN_DAC_H_
#define MAIN_DAC_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/dac.h"
#include "sdkconfig.h"

#define DAC_EXAMPLE_CHANNEL 0
#define BLINK_GPIO 			23
#define DAC_SAMPL_TIME 		20

void dac_init();

#endif /* MAIN_DAC_H_ */
