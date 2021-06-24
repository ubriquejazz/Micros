/*
 * adc.c
 *
 *  Created on: 24 Jun 2021
 *      Author: IDJGB0
 */


#include "adc.h"

/* Globals */

static uint16_t Refresh_Time = 1;
TickType_t Record_Time = 0;

static esp_adc_cal_characteristics_t *adc_chars;
static const adc_bits_width_t width = ADC_WIDTH_BIT_12;
static const adc_atten_t atten = ADC_ATTEN_DB_11;

/* Private Functions */

static void print_char_val_type(esp_adc_cal_value_t val_type)
{
    if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
        printf("Characterized using Two Point Value\n");
    } else if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
    	printf("Characterized using eFuse Vref\n");
    } else {
    	printf("Characterized using Default Vref\n");
    }
}

void adc_task(void *pvParameter)
{
	uint32_t voltage = 0;

	int adc_ch6, adc_ch7;
	uint16_t millisecond = 0;
	while(1)
	{
        adc_ch6 = adc1_get_raw((adc1_channel_t)ADC_CHANNEL_6);
        adc_ch7 = adc1_get_raw((adc1_channel_t)ADC_CHANNEL_7);
        if (millisecond > Refresh_Time * 1000 / ADC_SAMPL_TIME)
        {
            //Convert adc_reading to voltage in mV
            voltage = esp_adc_cal_raw_to_voltage(adc_ch6, adc_chars);
            Record_Time = xTaskGetTickCount();  // restart time counter
            printf("%d: %d\tVoltage: %dmV\n", Record_Time, adc_ch6, voltage);

            millisecond = 0;
        }
        else
        {
        	 millisecond++;
        }
		vTaskDelay(ADC_SAMPL_TIME/portTICK_RATE_MS);
	}
}


void adc_init(uint16_t refresh_time)
{
	Refresh_Time = refresh_time;

    //Configure ADC
    adc1_config_width(width);
    adc1_config_channel_atten(ADC_CHANNEL_6, atten);
    adc1_config_channel_atten(ADC_CHANNEL_7, atten);

    //Characterize ADC
    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_1, atten, width, DEFAULT_VREF, adc_chars);
    print_char_val_type(val_type);

	xTaskCreate(&adc_task, "adc_task", 2048, NULL, 5, NULL );
}
