/**
 ******************************************************************************
 * @file    Settings.cpp
 * @date    20 Octpber 2022
 * @brief   Settings routine
 ******************************************************************************
 *
 * COPYRIGHT(c) 2022 EDGE Impulse
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of EDGE Impulse nor the names of its contributors may
 *      be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, data, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */


#include "Settings.h"
#include <string.h>
#include "../../Hardware/M24c32/M24c32.h"
#include "../Appdefines.h"

Settings_t settings = {0};

/**
 * @brief  Settings init
 * @param  none
 * @retval none
 */
void settings_init (void) {
    m24c32_init();
}

/**
 * @brief  Get settings data fron storage
 * @param  [out] settings struct
 * @retval false if false
 */
bool settings_get_from_storage (Settings_t *settings) {
    uint8_t* pnt = (uint8_t*) settings;
    for (uint16_t i = 0; i < sizeof (Settings_t); i++){
        pnt[i] = m24c32_read_byte(STORAGE_SETTINGS_START_ADDR + i);
    }
    return true;
}

/**
 * @brief  Write settings data fron storage
 * @param  [out] settings struct
 * @retval false if false
 */
bool settings_set_to_storage (Settings_t *settings) {
    uint8_t* pnt = (uint8_t*) settings;
    for (uint16_t i = 0; i < sizeof (Settings_t); i++){
        m24c32_write_byte(STORAGE_SETTINGS_START_ADDR + i, pnt[i]);
    }
    return true;
}

/**
 * @brief  Check if settings ok
 * @param  [out] settings struct
 * @retval 0 if OK
 */
uint16_t settings_check (Settings_t *settings) {

    volatile uint16_t a = 0;
    if (memcmp(settings->device_name, DEVICE_NAME, DEVICE_NAME_LEN) != 0) {
        a++;
    }
    if (settings->device_version != DEVICE_VER) {
        a++;
    }
    if (settings->device_subversion != DEVICE_SUBVER) {
        a++;
    }
    if (settings->device_mode >= DEVICE_MODE_ENUM) {
        a++;
    }
    if (settings->lightsensor_mode.measure_rate >= RATE_ENUM) {
        a++;
    }
    if (settings->lightsensor_mode.resolution >= RES_ENUM) {
        a++;
    }
    if (settings->lightsensor_mode.sensor_mode >= SENSOR_MODE_ENUM) {
        a++;
    }
    if (settings->lsm6dso32_mode.acc_data_rate >= LSM6DSO32_RATE_ENUM){
        a++;
    }
    if (settings->lsm6dso32_mode.gyro_data_rate >= LSM6DSO32_RATE_ENUM){
        a++;
    }
    if (settings->lsm6dso32_mode.acc_scale >= LSM6DSO32_XL_ODR_ENUM){
        a++;
    }
    if (settings->lsm6dso32_mode.gyro_scale >= LSM6DSO32_GY_ODR_ENUM){
        a++;
    }
    if (settings->lis2mdl_mode.data_rate >= LIS2MDL_ODR_ENUM) {
        a++;
    }
    if (settings->lps22hh_mode.data_rate >= LPS22HH_ODR_ENUM) {
        a++;
    }
    if (settings->hts221_mode.data_rate >= HTS221_ENUM) {
        a++;
    }
    if (settings->hts221_mode.hts221_avgh >= HTS221_H_AVG_ENUM) {
        a++;
    }
    if (settings->hts221_mode.hts221_avgt >= HTS221_T_AVG_ENUM) {
        a++;
    }
    if (settings->sgp40_mode.sgp40_period >= SGP40_ENUM) {
        a++;
    }
    if (settings->sgp40_mode.sgp40_source >= SGP40_SOURCE_ENUM) {
        a++;
    }
    if (settings->vl53l1x_mode.vl53l1x_distance >= VL53_DISTANCE_ENUM) {
        a++;
    }
    if (settings->vl53l1x_mode.vl53l1x_period >= VL53_PERIOD_ENUM) {
        a++;
    }
    return a;
}

/**
 * @brief  Set defoult settings 
 * @param  [out] settings struct
 * @retval false if false
 */
bool settings_load_default (Settings_t *settings) {
    memset(settings, 0, sizeof(Settings_t));
    memcpy(settings->device_name, DEVICE_NAME, DEVICE_NAME_LEN);
    settings->device_version = DEVICE_VER;
    settings->device_subversion = DEVICE_SUBVER;
    settings->device_mode=DEVICE_MODE_MESURE;

    settings->lsm6dso32_mode.enable = true;
    settings->lsm6dso32_mode.acc_data_rate = LSM6DSO32_12Hz5;
    settings->lsm6dso32_mode.gyro_data_rate = LSM6DSO32_12Hz5;

    settings->gps_mode.enable = true;

    settings->hts221_mode.enable = true;
    settings->hts221_mode.data_rate = HTS221_ODR_1Hz;

    settings->lightsensor_mode.enable = true;
    settings->lightsensor_mode.measure_rate = RATE_2000_MS;

    settings->lis2mdl_mode.enable = true;
    settings->lis2mdl_mode.data_rate = LIS2MDL_ODR_10Hz;

    settings->lps22hh_mode.enable = true;
    settings->lps22hh_mode.data_rate = LPS22HH_1_Hz;

    settings->sgp40_mode.enable = true;
    settings->sgp40_mode.sgp40_period = SGP40_5S;

    settings->vl53l1x_mode.enable = true;
    settings->vl53l1x_mode.vl53l1x_period = VL53_PERIOD_10S;
    settings->pwm_mode_block = true;
    return true;
}

/**
 * @brief  Get settings 
 * @param  none
 * @retval pointer to settings struct
 */
Settings_t *get_settings(void) {
    return &settings;
}

/**
 * @brief  Printing settings
 * @param  [in] settings struct
 * @retval none
 */
void settings_print(Settings_t *settings) {
    printf ("lsm6dso32_mode.enable = %d\r\n", settings->lsm6dso32_mode.enable);
    printf ("lsm6dso32_mode.acc_data_rate = %d\r\n", settings->lsm6dso32_mode.acc_data_rate);
    printf ("lsm6dso32_mode.gyro_data_rate = %d\r\n", settings->lsm6dso32_mode.gyro_data_rate = LSM6DSO32_12Hz5);

    printf ("gps_mode.enable = %d\r\n", settings->gps_mode.enable);

    printf ("microphone_mode.enable = %d\r\n", settings->microphone_mode.enable);

    printf ("hts221_mode.enable = %d\r\n", settings->hts221_mode.enable);
    printf ("hts221_mode.data_rate = %d\r\n", settings->hts221_mode.data_rate);

    printf ("lightsensor_mode.enable = %d\r\n", settings->lightsensor_mode.enable);
    printf ("measure_rate = %d\r\n", settings->lightsensor_mode.measure_rate);

    printf ("lis2mdl_mode.enable = %d\r\n", settings->lis2mdl_mode.enable);
    printf ("lis2mdl_mode.data_rate = %d\r\n", settings->lis2mdl_mode.data_rate);

    printf ("lps22hh_mode.enable = %d\r\n", settings->lps22hh_mode.enable);
    printf ("lps22hh_mode.data_rate = %d\r\n", settings->lps22hh_mode.data_rate);

    printf ("sgp40_mode.enable = %d\r\n", settings->sgp40_mode.enable);
    printf ("sgp40_mode.sgp40_period = %d\r\n", settings->sgp40_mode.sgp40_period);

    printf ("vl53l1x_mode.enable = %d\r\n", settings->vl53l1x_mode.enable);
    printf ("vl53l1x_mode.vl53l1x_period = %d\r\n", settings->vl53l1x_mode.vl53l1x_period);

    printf ("pwm_mode_block = %d\r\n", settings->pwm_mode_block);
}
