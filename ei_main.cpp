/**
 ******************************************************************************
 * @file    ei_main.cpp
 * @date    10 November 2022
 * @brief   EDGE impulse main app
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
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

#include <stdint.h>
#include "ei_main.h"

#include "Appdefines.h"
#include "libraries/Hardware/Max7317/Max7317.h"
#include "libraries/Drivers/CXD5602/I2c/I2c.h"
#include "libraries/Hardware/Hts221/Hts221.h"
#include "libraries/Hardware/Lis2mdl/Lis2mdl.h"
#include "libraries/Hardware/Lps22hh/Lps22hh.h"
#include "libraries/Hardware/Lsm6dso32/Lsm6dso32.h"
#include "libraries/Hardware/Vl53l1x/VL53L1X_api.h"
#include "libraries/Hardware/Sgp4x/sensirion_common.h"
#include "libraries/Hardware/Sgp4x/sensirion_i2c.h"
#include "libraries/Hardware/Sgp4x/sensirion_i2c_hal.h"
#include "libraries/Hardware/Sgp4x/sgp40_i2c.h"
#include "libraries/Hardware/Sgp4x/sensirion_voc_algorithm.h"
#include "libraries/Hardware/Apds9250/Apds9250.h"
#include "libraries/Drivers/CXD5602/Pwm/Pwm.h"
#include "libraries/Hardware/Sdcard/Sdcard.h"
#include "libraries/ThirdParty/Fatfs/ff.h"
#include "libraries/Hardware/Led/Led.h"
#include "libraries/Hardware/Button/Button.h"
#include "libraries/Drivers/CXD5602/Gnss/Gnss.h"
#include "libraries/Drivers/CXD5602/Uart/Uart.h"
#include "libraries/Protocol/Protocol.h"
#include "libraries/Tools/Settings/Settings.h"
#include "libraries/Hardware/Bq27441/Bq27441.h"
#include "libraries/Hardware/LightSensor/LightSensor.h"
#include "libraries/Hardware/Gps/Gps.h"

VocAlgorithmParams voc_params = {0};
static uint8_t ret_user = 0;
static char user_path[4] = {"0:/"};
static FATFS user_fat_fs = {0};
static bool measure_run = false;
static bool sd_card_err = false;
extern bool sgp_40_first_run;

int usleep(useconds_t usec);
  
extern "C" void spresense_time_cb(uint32_t *sec, uint32_t *nano);

/**
 * @brief  Get currens system time [ns]
 * @param  None
 * @retval system time [ns]
 */
uint64_t get_current_time_ns(void) {
    uint32_t sec = 0;
    uint32_t nsec = 0;
    spresense_time_cb(&sec, &nsec);
    uint64_t result = ((uint64_t)sec * 1000000000ULL) + nsec;
    return result;
}

/**
 * @brief  Calculate passed time [ns]
 * @param  in_time begin time
 * @retval passed time [ns]
 */
uint64_t get_passed_time_ns(uint64_t in_time) {
    uint64_t time_now = get_current_time_ns();
    if (in_time <= time_now) {
        return time_now - in_time;
    }
    return (0xffffffffffffffff - in_time) + time_now;
}

/**
 * @brief  System init sensors and others
 * @param  None
 * @retval None
 */
static void system_init(void) {
    pwm_init();
    led_init();
    button_init();
    lightsensor_init();
    protocol_init();
    lsm6dso32_init();
    lis2mdl_init();
    lps22hh_init();
    hts221_init();
    sgp40_init();
    vl53l1x_init();
    gps_init();
    settings_init();
    bq27441_init();
    microphone_init();
    VocAlgorithm_defaultParam(&voc_params);
    VocAlgorithm_init(&voc_params);
    if (sdcard_init() != 0) {
        sd_card_err = true;
    }
    if( f_mount(&user_fat_fs,(TCHAR const*)user_path, 0) != FR_OK) {   
        sd_card_err = true;
    }
}

/**
 * @brief  System setup sensors and others
 * @param  None
 * @retval None
 */
static void system_setup(void) {
    lightsensor_set_mode(&settings.lightsensor_mode);
    lsm6dso32_set_mode(&settings.lsm6dso32_mode);
    lis2mdl_set_mode(&settings.lis2mdl_mode);
    lps22hh_set_mode(&settings.lps22hh_mode);
    hts221_set_mode(&settings.hts221_mode);
    sgp40_set_mode(&settings.sgp40_mode);
    vl53l1x_set_mode(&settings.vl53l1x_mode);
    gps_set_mode(&settings.gps_mode);
    microphone_set_mode(&settings.microphone_mode);
}

/**
 * @brief  System sleep sensors and others
 * @param  None
 * @retval None
 */
static void system_sleep(void) {
    Settings_t sleep_settings = {0};
    lightsensor_set_mode(&sleep_settings.lightsensor_mode);
    lsm6dso32_set_mode(&sleep_settings.lsm6dso32_mode);
    lis2mdl_set_mode(&sleep_settings.lis2mdl_mode);
    lps22hh_set_mode(&sleep_settings.lps22hh_mode);
    hts221_set_mode(&sleep_settings.hts221_mode);
    sgp40_set_mode(&sleep_settings.sgp40_mode);
    vl53l1x_set_mode(&sleep_settings.vl53l1x_mode);
    gps_set_mode(&sleep_settings.gps_mode);
}

/**
 * @brief  LEDs light activity
 * @param  [in] dev_mod device mode from device_mode_t
 * @param  [in] run if measure run
 * @retval None
 */
static void led_act (device_mode_t dev_mod, bool run) {
    static uint64_t time = 0;
    static bool color_setted = false;
    uint64_t passed = get_passed_time_ns(time);
    if (dev_mod == DEVICE_MODE_DEMO) {
        if (passed < (5 * 1000000000UL)) {
            led_green_off();
            led_red_off();
        } else if ((passed >= (5 * 1000000000UL)) && (passed < (55 * 100000000UL))) {
            led_red_on();
        } else {
            led_green_off();
            led_red_off();
            time = get_current_time_ns();
        }
        color_setted = false;
    } else if (dev_mod == DEVICE_MODE_MESURE) {
        if (run == false) {
            if (passed < (5 * 1000000000UL)) {
                led_green_off();
                led_red_off();
            } else if ((passed >= (5 * 1000000000UL)) && (passed < (55 * 100000000UL))) {
                led_green_on();
            } else {
                led_green_off();
                led_red_off();
                time = get_current_time_ns();
            }
            color_setted = false;
        } else {
            if (passed < (5 * 1000000000UL)) {
                led_green_off();
                led_red_off();
            } else if ((passed >= (5 * 1000000000UL)) && (passed < (55 * 100000000UL))) {
                static bool color = false;
                if (color_setted == false){
                    if (color == false) {
                        led_green_on();
                    } else {
                        led_red_on();
                    }
                    color = !color;
                    color_setted = true;
                }
            } else {
                led_green_off();
                led_red_off();
                time = get_current_time_ns();
                color_setted = false;
            }
        }
    }
}

void ei_main (void) {
    static bool run = false;
    static char dir [16] = {0};
    static char path[32] = {0};
    static char filepath [64] = {0};
    static uint64_t lightsensor_time = 0;
    static uint64_t lsm6dso32_acc_time = 0;
    static uint64_t lsm6dso32_gyro_time = 0;
    static uint64_t lis2mdl_time = 0;
    static uint64_t lps22hh_time = 0;
    static uint64_t hts221_time = 0;
    static uint64_t sgp40_time = 0;
    static uint64_t vl53l1x_time = 0;
    static uint64_t led_control_time_ns = 0;
    static float temperature;
    static float humidity;
    FIL file;
    system_init();
    led_red_off();
    led_green_off();
    printf("Boot start\r\n");
    usleep(250 * 1000);
    if (button_is_pressed() != false) {
        settings_load_default(&settings);
        settings_set_to_storage(&settings);
    }
#if 1
    settings_get_from_storage(&settings);
    if (settings_check(&settings) != 0) {
        settings_load_default(&settings);
        usleep(250 * 1000);
        settings_set_to_storage(&settings);
    }
#else
    settings_load_default(&settings);
#endif
    system_setup();
    if (sd_card_err == false) {
        led_green_on();
        pwm_run(500, 50, 500);
        while (pwm_act() == PWM_WORK);
        led_green_off();
    } else {
        led_red_on();
        pwm_run(500, 50, 250);
        while (pwm_act() == PWM_WORK);
        led_red_off();
        usleep(250 * 1000);
        led_red_on();
        pwm_run(500, 50, 250);
        while (pwm_act() == PWM_WORK);
        led_red_off();
        usleep(250 * 1000);
        led_red_on();
        pwm_run(500, 50, 250);
        while (pwm_act() == PWM_WORK);
        led_red_off();
        usleep(250 * 1000);
    }
    printf("Boot finish\r\n");
    while (1) {

        protocol_act();
        if (settings.pwm_mode_block != false) {
            while (pwm_act() == PWM_WORK);
        } else {
            pwm_act();
        }
        if (get_protocol_led_control_time_ns() != 0) {
            if (led_control_time_ns == 0){
                led_control_time_ns  = get_current_time_ns();
            }
            if (get_passed_time_ns(led_control_time_ns) >= get_protocol_led_control_time_ns()) {
                reset_protocol_led_control_time();
            }
        } else {
            led_act(settings.device_mode, run);
            led_control_time_ns = 0;
        }

        if (settings.device_mode == DEVICE_MODE_MESURE) {
            if (run != false) {
                if (settings.lightsensor_mode.enable != false) {
                    if (get_passed_time_ns(lightsensor_time) >= lightsensor_get_pheriod_ns(settings.lightsensor_mode.measure_rate)) {
                        if (settings.lightsensor_mode.sensor_mode == SENSOR_MODE_LIGHT) {
                            uint32_t light = lightsensor_get_light();
                            lightsensor_time = get_current_time_ns();
                            char data [64] = {0};
                            sprintf(data, "%016llX %08X\r\n", lightsensor_time, light);
                            sprintf(filepath, "%s/%s\0", path, "ligth.txt");
                            ret_user = f_open(&file, filepath, FA_WRITE | FA_OPEN_ALWAYS);
                            if (ret_user == FR_OK) {
                                f_lseek(&file, f_size(&file));
                                UINT bw;
                                f_write(&file, data, strlen(data), &bw);
                                f_close(&file);
                            }
                        } else {
                            uint32_t red = 0;
                            uint32_t green = 0;
                            uint32_t blue = 0;
                            uint32_t ir = 0;
                            lightsensor_get_all_colors(&red, &green, &blue, &ir);
                            lightsensor_time = get_current_time_ns();
                            char data [128] = {0};
                            sprintf(data, "%016llX %08X, %08X, %08X, %08X\r\n", lightsensor_time, red, green, blue, ir);
                            sprintf(filepath, "%s/%s\0", path, "colors.txt");
                            ret_user = f_open(&file, filepath, FA_WRITE | FA_OPEN_ALWAYS);
                            if (ret_user == FR_OK) {
                                f_lseek(&file, f_size(&file));
                                UINT bw;
                                f_write(&file, data, strlen(data), &bw);
                                f_close(&file);
                            }
                        }
                    }
                }
                if (settings.lsm6dso32_mode.enable != false) {
                    if (get_passed_time_ns(lsm6dso32_acc_time) >= lsm6dso32_get_pheriod_ns(settings.lsm6dso32_mode.acc_data_rate)) {
                        float axis_x = 0;
                        float axis_y = 0;
                        float axis_z = 0;
                        lsm6dso32_get_acceleration(&axis_x, &axis_y, &axis_z);
                        axis_x = acc_convert_to_m_s(axis_x);
                        axis_y = acc_convert_to_m_s(axis_y);
                        axis_z = acc_convert_to_m_s(axis_z);
                        lsm6dso32_acc_time = get_current_time_ns();
                        char data [128] = {0};
                        sprintf(data, "%016llX %.02f, %.02f, %.02f\r\n", lsm6dso32_acc_time, axis_x, axis_y, axis_z);
                        sprintf(filepath, "%s/%s\0", path, "acceler.txt");
                        ret_user = f_open(&file, filepath, FA_WRITE | FA_OPEN_ALWAYS);
                        if (ret_user == FR_OK) {
                            f_lseek(&file, f_size(&file));
                            UINT bw;
                            f_write(&file, data, strlen(data), &bw);
                            f_close(&file);
                        }
                    }
                    if (get_passed_time_ns(lsm6dso32_gyro_time) >= lsm6dso32_get_pheriod_ns(settings.lsm6dso32_mode.gyro_data_rate)) {
                        float axis_x = 0;
                        float axis_y = 0;
                        float axis_z = 0;
                        lsm6dso32_get_gyroscope(&axis_x, &axis_y, &axis_z);
                        axis_x = gyro_convert_to_rad_s(axis_x);
                        axis_y = gyro_convert_to_rad_s(axis_y);
                        axis_z = gyro_convert_to_rad_s(axis_z);
                        lsm6dso32_gyro_time = get_current_time_ns();
                        char data [128] = {0};
                        sprintf(data, "%016llX %.02f, %.02f, %.02f\r\n", lsm6dso32_gyro_time, axis_x, axis_y, axis_z);
                        sprintf(filepath, "%s/%s\0", path, "gyro.txt");
                        ret_user = f_open(&file, filepath, FA_WRITE | FA_OPEN_ALWAYS);
                        if (ret_user == FR_OK) {
                            f_lseek(&file, f_size(&file));
                            UINT bw;
                            f_write(&file, data, strlen(data), &bw);
                            f_close(&file);
                        }
                    }
                }
                if (settings.lis2mdl_mode.enable != false) {
                    if (get_passed_time_ns(lis2mdl_time) >= lis2mdl_get_pheriod_ns(settings.lis2mdl_mode.data_rate)) {
                        
                        float axis_x = 0;
                        float axis_y = 0;
                        float axis_z = 0;
                        lis2mdl_get_magnetic(&axis_x, &axis_y, &axis_z);
                        axis_x = magnetic_convert_to_ut(axis_x);
                        axis_y = magnetic_convert_to_ut(axis_y);
                        axis_z = magnetic_convert_to_ut(axis_z);
                        lis2mdl_time = get_current_time_ns();
                        char data [128] = {0};
                        sprintf(data, "%016llX %.02f, %.02f, %.02f\r\n", lis2mdl_time, axis_x, axis_y, axis_z);
                        sprintf(filepath, "%s/%s\0", path, "magnet.txt");
                        ret_user = f_open(&file, filepath, FA_WRITE | FA_OPEN_ALWAYS);
                        if (ret_user == FR_OK) {
                            f_lseek(&file, f_size(&file));
                            UINT bw;
                            f_write(&file, data, strlen(data), &bw);
                            f_close(&file);
                        }
                    
                    }
                }
                if (settings.lps22hh_mode.enable != false) {
                    if (get_passed_time_ns(lps22hh_time) >= lps22hh_get_pheriod_ns(settings.lps22hh_mode.data_rate)) {
                        float pressure = lps22hh_get_pressure_hpa();
                        lps22hh_time = get_current_time_ns();
                        char data [128] = {0};
                        sprintf(data, "%016llX %.02f\r\n", lps22hh_time, pressure);
                        sprintf(filepath, "%s/%s\0", path, "pressure.txt");
                        ret_user = f_open(&file, filepath, FA_WRITE | FA_OPEN_ALWAYS);
                        if (ret_user == FR_OK) {
                            f_lseek(&file, f_size(&file));
                            UINT bw;
                            f_write(&file, data, strlen(data), &bw);
                            f_close(&file);
                        }
                    }
                }
                if (settings.hts221_mode.enable != false) {
                    if (get_passed_time_ns(hts221_time) >= hts221_get_pheriod_ns(settings.hts221_mode.data_rate)) {
                        float hum;
                        float temp;
                        hts221_get_hym_temp(&hum, &temp);
                        hts221_time = get_current_time_ns();
                        char data [128] = {0};
                        sprintf(data, "%016llX %.02f, %.02f\r\n", hts221_time, hum, temp);
                        sprintf(filepath, "%s/%s\0", path, "humtemp.txt");
                        ret_user = f_open(&file, filepath, FA_WRITE | FA_OPEN_ALWAYS);
                        if (ret_user == FR_OK) {
                            f_lseek(&file, f_size(&file));
                            UINT bw;
                            f_write(&file, data, strlen(data), &bw);
                            f_close(&file);
                        }
                        temperature = temp;
                        humidity = hum;
                    }
                }
                if (settings.sgp40_mode.enable != false) {
                    if (get_passed_time_ns(sgp40_time) >= sgp40_get_pheriod_ns(settings.sgp40_mode.sgp40_period)) {
                        uint16_t hum_tics = 0;
                        uint16_t temp_tics = 0;
                        if (settings.sgp40_mode.sgp40_source == SGP40_SOURCE_DEFAULT) {
                            hum_tics = SGP40_DEFAULT_HUMIDITY;
                            temp_tics = SGP40_DEFAULT_TEMPERATURE;
                        } else {
                            if (settings.hts221_mode.enable != false) {
                                hum_tics = sgp40_get_rh_tick(humidity);
                                temp_tics = sgp40_get_t_tick(temperature);
                            } else {
                                hts221_mode_t mode;
                                float hum;
                                float temp;
                                hts221_get_mode(&mode);
                                hts221_mode_t mode_set;
                                mode_set.data_rate = HTS221_ONE_SHOT;
                                mode_set.enable = true;
                                mode_set.hts221_avgh = HTS221_H_AVG_ND;
                                mode_set.hts221_avgt = HTS221_T_AVG_ND;
                                hts221_set_mode(&mode_set);
                                hts221_get_hym_temp(&hum, &temp);
                                hts221_set_mode(&mode);
                                hum_tics = sgp40_get_rh_tick(hum);
                                temp_tics = sgp40_get_t_tick(temp);
                            }
                            
                        }
                        int32_t voc = 0;
                        uint16_t raw_voc = 0;
                        sgp40_measure_raw_signal(hum_tics, temp_tics, &raw_voc);     
                        if (sgp_40_first_run == false) {
                            sgp_40_first_run = true;
                            for (uint8_t i = 0; i < 200; i++) {
                                VocAlgorithm_process(&voc_params, raw_voc, &voc);
                            } 
                        } else {
                            VocAlgorithm_process(&voc_params, raw_voc, &voc);
                        }
                        sgp40_time = get_current_time_ns();
                        char data [128] = {0};
                        sprintf(data, "%016llX, %d, %d\r\n", sgp40_time, settings.sgp40_mode.sgp40_source, voc);
                        sprintf(filepath, "%s/%s\0", path, "voc.txt");
                        ret_user = f_open(&file, filepath, FA_WRITE | FA_OPEN_ALWAYS);
                        if (ret_user == FR_OK) {
                            f_lseek(&file, f_size(&file));
                            UINT bw;
                            f_write(&file, data, strlen(data), &bw);
                            f_close(&file);
                        }
                    }
                }
                if (settings.vl53l1x_mode.enable != false) {
                    if (get_passed_time_ns(vl53l1x_time) >= vl53l1x_get_pheriod_ns(settings.vl53l1x_mode.vl53l1x_period)) {

                        VL53L1X_Result_t results;
                        vl53l1x_start_ranging(0);
                        uint8_t tmp = 0;
                        while (tmp == 0)
                        {
                            vl53l1x_check_for_data_ready(0, &tmp);
                        }
                        vl53l1x_get_result(0, &results);
                        vl53l1x_clear_interrupt(0);
                        vl53l1x_stop_ranging(0);
                        vl53l1x_time = get_current_time_ns();
                        char data [128] = {0};
                        sprintf(data, "%016llX %d, %4X\r\n", vl53l1x_time, settings.vl53l1x_mode.vl53l1x_distance, results.Distance);
                        sprintf(filepath, "%s/%s\0", path, "proxim.txt");
                        ret_user = f_open(&file, filepath, FA_WRITE | FA_OPEN_ALWAYS);
                        if (ret_user == FR_OK) {
                            f_lseek(&file, f_size(&file));
                            UINT bw;
                            f_write(&file, data, strlen(data), &bw);
                            f_close(&file);
                        }
                    }
                }
                if (settings.gps_mode.enable != false) {
                    static bool gps_err = false;
                    gps_act_status_t gps_act_status = GPS_NO_DATA;
                    if (gps_err == false) {
                        gps_act_status = gps_act();
                        if (gps_act_status == GPS_NEW_DATA) {
                            sprintf(filepath, "%s/%s\0", path, "gps.txt");
                            ret_user = f_open(&file, filepath, FA_WRITE | FA_OPEN_ALWAYS);
                            if (ret_user == FR_OK) {
                                f_lseek(&file, f_size(&file));
                                UINT bw;
                                f_write(&file, gps_get_data(), strlen(gps_get_data()), &bw);
                                f_close(&file);
                            }
                        }
                    } else if (gps_act_status == GPS_ERROR) {
                        gps_err = true;
                        sprintf(filepath, "%s/%s\0", path, "gps.txt");
                        ret_user = f_open(&file, filepath, FA_WRITE | FA_OPEN_ALWAYS);
                        if (ret_user == FR_OK) {
                            f_lseek(&file, f_size(&file));
                            UINT bw;
                            f_write(&file, "GPS_ERROR\r\n", strlen("GPS_ERROR\r\n"), &bw);
                            f_close(&file);
                        }
                    }
                }
                if (settings.microphone_mode.enable != false) {
                    // TODO Add microphone logic
                    uint8_t *mic_data = nullptr;
                    uint16_t mic_data_size = 0;
                    bool result = microphone_is_data_ready(mic_data, &mic_data_size);
                    if (result != false) {
                        if ((mic_data != nullptr) && (mic_data_size != 0)) {
                            sprintf(filepath, "%s/%s\0", path, "mic.wav");
                            ret_user = f_open(&file, filepath, FA_WRITE | FA_OPEN_ALWAYS);
                            if (ret_user == FR_OK) {
                                microphone_write_to_wav(&file, mic_data, mic_data_size);
                                f_close(&file);
                            }
                        }
                    } 
                }
                if ((button_is_pressed() != false) || (protocol_stop() == true)) {
                    led_red_on();
                    pwm_run(500, 50, 1000);
                    while (pwm_act() == PWM_WORK);
                    led_red_off();
                    memset(dir, 0, sizeof(dir));
                    memset(path, 0, sizeof(path));
                    memset(filepath, 0, sizeof(filepath));
                    system_sleep();
                    reset_stop();
                    run = false;
                }
            } else {
                if (button_is_pressed() != false) {
                    if (sd_card_err != false) {
                        led_red_on();
                        pwm_run(500, 50, 250);
                        while (pwm_act() == PWM_WORK);
                        led_red_off();
                        usleep(250 * 1000);
                        led_red_on();
                        pwm_run(500, 50, 250);
                        while (pwm_act() == PWM_WORK);
                        led_red_off();
                        usleep(250 * 1000);
                        led_red_on();
                        pwm_run(500, 50, 250);
                        while (pwm_act() == PWM_WORK);
                        led_red_off();
                        usleep(250 * 1000);
                    } else {
                        led_green_on();
                        pwm_run(500, 50, 1000);
                        while (pwm_act() == PWM_WORK);
                        led_green_off();
                        sprintf(dir, "%08X\0", get_current_time_ns() / 1000000000UL);
                        sprintf(path, "%s%s\0", user_path, dir);
                        f_mkdir(dir);
                        system_setup();
                        reset_stop();
                        run = true;
                    }
                }
            }
        }
    }
}
