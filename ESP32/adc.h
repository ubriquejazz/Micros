/*
 * adc.h
 *
 *  Created on: 24 Jun 2021
 *      Author: IDJGB0
 */

#ifndef MAIN_ADC_H_
#define MAIN_ADC_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "sdkconfig.h"

#define DEFAULT_VREF        1100    // use adc2_vref_to_gpio() to obtain a better estimate
#define SAMPLES     		64
#define ADC_SAMPL_TIME 		20

void adc_init(uint16_t refresh_time);

#endif /* MAIN_ADC_H_ */
