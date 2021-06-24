/*
 * functions.c
 *
 *  Created on: 23 Jun 2021
 *      Author: IDJGB0
 */

#include "dac.h"

/* Private Functions */

void dac_task(void *pvParameter)
{
    uint8_t count=0;
    uint8_t positive = 1;
    for (;;)
    {
    	if (positive) {
    		count++;
    		if (count == 255) {
    			positive = 0;
    	        gpio_set_level(BLINK_GPIO, 0);
    		}
    	}
    	else {
    		count--;
    		if (count == 0) {
     			positive = 1;
     	        gpio_set_level(BLINK_GPIO, 1);
    		}
    	}
    	dac_output_voltage( DAC_EXAMPLE_CHANNEL, count );
        vTaskDelay(DAC_SAMPL_TIME / portTICK_RATE_MS);
    }
}

void dac_init(void)
{
    gpio_num_t dac_gpio_num;
    dac_pad_get_io_num( DAC_EXAMPLE_CHANNEL, &dac_gpio_num );
    printf("DAC channel %d @ GPIO %d.\n", DAC_EXAMPLE_CHANNEL + 1, dac_gpio_num );
    dac_output_enable( DAC_EXAMPLE_CHANNEL );

    gpio_pad_select_gpio(BLINK_GPIO);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

    xTaskCreate(&dac_task, "dac_task", 2048, NULL, 5, NULL );
}

