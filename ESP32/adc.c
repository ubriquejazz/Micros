/*
 * adc.c
 *
 *  Created on: 24 Jun 2021
 *      Author: IDJGB0
 */


#include "adc.h"
#include "gatts_table_creat_demo.h"
#include <math.h>

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

int adc_GetRPM(int adc_reading, int numReadings)
{
    // numReadings = refresh_time * 100 / LOOPTIME
    // changing numReadings to a larger value will smooth the data collected
    static float filt = 0;          // keep value from previous iteration (oldf)
    float oldf = filt;

    filt = adc_reading - 2048;
//    printf("%d: %d\n",  xTaskGetTickCount(), (int)filt);

    static int cadence[32]; // keep value from previous iteration
    float cadence_avg = 0;
    int delta_ms;

    /* zero cross detection + hyst (5) */
    if( (filt>1 && oldf<-1) || (filt<-1 && oldf>1) )
    {
        delta_ms = (xTaskGetTickCount() - Record_Time) * portTICK_PERIOD_MS;
        Record_Time = xTaskGetTickCount();  // restart time counter

        cadence[0]= 30000 / delta_ms;   // calculate rpms
        printf("%d: %d rpm \t", Record_Time, cadence[0]);

        cadence_avg = 0;
        for (int i=numReadings; i>1; i--) {
            cadence[i-1] = cadence[i-2];                    // shift sample array
            cadence_avg += (float) cadence[i-1] / (numReadings-1);      // calculate RPM average
        }
        printf("avg: %f\n", cadence_avg);
    }

    return (cadence[0]);

//    static uint16_t aux = 0;
//    aux++;
//    return aux;
}

void adc_task(void *pvParameter)
{
    int adc_ch6, adc_ch7;
    floatIdn_t aux;
    while(1)
    {
        adc_ch6 = adc1_get_raw((adc1_channel_t)ADC_CHANNEL_6);
        adc_ch7 = adc1_get_raw((adc1_channel_t)ADC_CHANNEL_7);

        // algorithm to process the rpms
//      aux.f = adc_GetRPM(adc_ch6, Refresh_Time);
//      AppData.rpm = aux.i;
        AppData.rpm = adc_GetRPM(adc_ch6, Refresh_Time);

//       uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_ch6, adc_chars);
//       printf("%d: %d\tVoltage: %dmV\n",  xTaskGetTickCount(), adc_ch6, voltage);

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
