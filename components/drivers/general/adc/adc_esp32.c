/**
 * ESP-Drone Firmware
 *
 * Copyright 2019-2020  Espressif Systems (Shanghai)
 * Copyright (C) 2011-2012 Bitcraze AB
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, in version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * adc.c - Analog Digital Conversion
 *
 *
 */

#include "esp_idf_version.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "adc_esp32.h"
#include "config.h"
#include "pm_esplane.h"
#include "stm32_legacy.h"
#define DEBUG_MODULE "ADC"
#include "debug_cf.h"

static bool isInit;

static adc_oneshot_unit_handle_t adc_handle;
static adc_cali_handle_t adc_cali_handle;
static bool adc_calibrated;
#ifdef CONFIG_IDF_TARGET_ESP32
static const adc_channel_t channel = ADC_CHANNEL_6; //GPIO34 if ADC1
#elif defined(CONFIG_IDF_TARGET_ESP32S2) || defined(CONFIG_IDF_TARGET_ESP32S3)
static const adc_channel_t channel = ADC_CHANNEL_6;     // GPIO7 if ADC1
#endif

static const adc_bitwidth_t width = ADC_BITWIDTH_DEFAULT;
static const adc_atten_t atten = ADC_ATTEN_DB_12;   //12dB attenuation gives full-scale voltage ~3.1V
static const adc_unit_t unit = ADC_UNIT_1;
#define NO_OF_SAMPLES   30          //Multisampling

static bool adc_calibration_init(void)
{
    esp_err_t ret = ESP_ERR_NOT_SUPPORTED;
#if ADC_CALI_SCHEME_CURVE_FITTING_SUPPORTED
    adc_cali_curve_fitting_config_t cali_config = {
        .unit_id = unit,
        .chan = channel,
        .atten = atten,
        .bitwidth = width,
    };
    ret = adc_cali_create_scheme_curve_fitting(&cali_config, &adc_cali_handle);
    if (ret == ESP_OK) {
        printf("ADC calibrated using curve fitting\n");
    }
#elif ADC_CALI_SCHEME_LINE_FITTING_SUPPORTED
    adc_cali_line_fitting_config_t cali_config = {
        .unit_id = unit,
        .atten = atten,
        .bitwidth = width,
    };
    ret = adc_cali_create_scheme_line_fitting(&cali_config, &adc_cali_handle);
    if (ret == ESP_OK) {
        printf("ADC calibrated using line fitting\n");
    }
#endif
    if (ret != ESP_OK) {
        printf("ADC calibration not available, using raw estimate\n");
    }
    return ret == ESP_OK;
}

float analogReadVoltage(uint32_t pin)
{
    uint32_t adc_reading = 0;
    for (int i = 0; i < NO_OF_SAMPLES; i++) {
        int raw = 0;
        adc_oneshot_read(adc_handle, channel, &raw);
        adc_reading += raw;
    }
    adc_reading /= NO_OF_SAMPLES;
    //Convert adc_reading to voltage in mV
    int voltage = 0;
    if (adc_calibrated) {
        adc_cali_raw_to_voltage(adc_cali_handle, adc_reading, &voltage);
    } else {
        voltage = adc_reading * 3100 / 4095;
    }
    return voltage / 1000.0;
}

void adcInit(void)
{
    if (isInit) {
        return;
    }

    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = unit,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config, &adc_handle));

    adc_oneshot_chan_cfg_t chan_config = {
        .bitwidth = width,
        .atten = atten,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc_handle, channel, &chan_config));

    //Characterize ADC
    adc_calibrated = adc_calibration_init();

    isInit = true;
}

bool adcTest(void)
{
    return isInit;
}
