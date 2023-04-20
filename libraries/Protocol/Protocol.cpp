/**
 ******************************************************************************
 * @file    Protocol.h
 * @date    17 Octpber 2022
 * @brief   Protocol routine
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
 * DAMAGES(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, data, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */


#include <string.h>
#include <stdio.h>
#include <sys/boardctl.h>
#include "Protocol.h"
#include "../Drivers/CXD5602/Uart/Uart.h"
#include <stdio.h>
#include "../Appdefines.h"
#include "../Hardware/Battery/Battery.h"
#include "../Hardware/Lps22hh/Lps22hh.h"
#include "../Hardware/Hts221/Hts221.h"
#include "../Hardware/Lsm6dso32/Lsm6dso32.h"
#include "../Hardware/Lis2mdl/Lis2mdl.h"
#include "../Hardware/Led/Led.h"
#include "../Hardware/Button/Button.h"
#include "../Hardware/LightSensor/LightSensor.h"
#include "../Settings/Settings.h"
#include "cxd56_clock.h"
#include "../../spresense-exported-sdk/nuttx/include/unistd.h"
#include "../Hardware/Gps/Gps.h"
#include "../Hardware/Microphone/Microphone.h"
#include "../Hardware/Sgp4x/sensirion_voc_algorithm.h"

extern VocAlgorithmParams voc_params;
static char* rx_pnt = nullptr;
static bool new_command = false;
static uint16_t protocol_led_control_time_s = 0;
static bool stop = false;
bool sgp_40_first_run = false;
static bool gyro_enabled = false;
static lsm6dso32_mode_t gyro_saved_mode = {0};
static uint64_t gyro_timeout = 0;

uint16_t s_crc16Table[256] = {
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
    0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
    0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
    0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
    0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
    0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
    0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
    0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
    0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
    0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
    0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
    0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
    0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
    0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
    0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
    0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
    0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
    0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
    0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
    0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
    0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
    0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
    0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
    0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
    0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
    0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
    0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
    0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
    0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
    0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
    0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
};

static void protocol_send_ok(void);

uint64_t get_current_time_ns(void);

uint64_t get_passed_time_ns(uint64_t in_time);

uint16_t culate_crc16(char *pc_block, uint16_t len)
{
    uint16_t l_crc = 0xFFFF;

    while(len--)
        l_crc =(l_crc << 8) ^ s_crc16Table[(l_crc >> 8) ^ *pc_block++];

    return l_crc;
}

/**
 * @brief  Get protocol LED control time
 * @param  none
 * @retval LED control time ns
 */
uint64_t get_protocol_led_control_time_ns(void) {
    return (uint64_t) protocol_led_control_time_s * 1000000000;
}

/**
 * @brief  Reset protocol LED control time
 * @param  none
 * @retval none
 */
void reset_protocol_led_control_time(void) {
    protocol_led_control_time_s = 0;
}

/**
 * @brief  Converting uint16 to HEX string
 * @param  [in] data uint16_t data
 * @param  [out] str_start poitner sting for result
 * @retval true if data ok
 */
static bool conv_uint16_to_hex(uint16_t data, char* str_start) {    
    if (str_start == NULL || strlen(str_start) >= 5) {
        return false;
    }
    sprintf(str_start,",%02X", data);
    return true;
}

/**
 * @brief  Converting HEX string to uint16 
 * @param  [in] start start of HEX string
 * @retval true if data ok
 */
uint16_t conv_hex_to_uint16(char* start) {
    uint8_t b;
    uint16_t Out = 0;

    for(uint8_t a = 0; a < 4; a++) {
        switch(start[a]) {
            case '0': 
            b = 0; 
            break;
            case '1': 
            b = 1; 
            break;
            case '2': 
            b = 2; 
            break;
            case '3': 
            b = 3; 
            break;
            case '4': 
            b = 4; 
            break;
            case '5': 
            b = 5; 
            break;
            case '6': 
            b = 6; 
            break;
            case '7': 
            b = 7; 
            break;
            case '8': 
            b = 8; 
            break;
            case '9': 
            b = 9; 
            break;
            case 'a': 
            b = 10; 
            break;
            case 'b': 
            b = 11; 
            break;
            case 'c': 
            b = 12; 
            break;
            case 'd': 
            b = 13; 
            break;
            case 'e': 
            b = 14; 
            break;
            case 'f': 
            b = 15; 
            break;
            case 'A': 
            b = 10; 
            break;
            case 'B': 
            b = 11; 
            break;
            case 'C': 
            b = 12; 
            break;
            case 'D': 
            b = 13; 
            break;
            case 'E': 
            b = 14; 
            break;
            case 'F': 
            b = 15; 
            break;
            default: 
            return 0xffff;
        }
        switch(a) {
            case 0: 
            Out |=((uint32_t)b<<12); 
            break;
            case 1: 
            Out |=((uint32_t)b<<8); 
            break;
            case 2: 
            Out |=((uint32_t)b<<4); 
            break;
            case 3: 
            Out |=((uint32_t)b<<0); 
            break;
        }
    }
    return Out;
}

/**
 * @brief  Protocol check if received data ok
 * @param  [in] ptr poitner to data
 * @retval true if data ok
 */
static bool protocol_check_data(char* ptr) {
    char* crc_ptr = 0;
    uint16_t data_size;
    uint16_t data_crc = 0;
    uint16_t calc_crc = 0;

    if(ptr == nullptr) {
        return false;
    }
    crc_ptr = strstr(ptr, ",");
    if(crc_ptr == nullptr) {
        return false;
    }
    data_size = crc_ptr - ptr;
    crc_ptr++;
    data_crc = conv_hex_to_uint16(crc_ptr);
    calc_crc = culate_crc16(ptr, data_size);

    if(data_crc != calc_crc) {
#ifndef PROTOCOL_CRC_TEST
        return false;
#else
        if(data_crc != 0xffff) {
            return false;
        }
#endif // PROTOCOL_CRC_TEST
    }

    return true;
}

void new_command_callback(char* rx) {
    rx_pnt = rx;
    new_command = true;
}

void protocol_init(void) {
    uart_init();
    uart_attach_symbol_callback('\n', new_command_callback);
}

/**
 * @brief  Protocol Send response on INFO=0 command
 * @param  None
 * @retval None
 */
static void protocol_send_info_0(void) {
    char data [64] = {0};
    sprintf(data, "DEV=%s;VERS=%d.%d\0", DEVICE_NAME, DEVICE_VER, DEVICE_SUBVER);
    protocol_send_data(data, 0);
}

/**
 * @brief  Protocol Send response on INFO=1 command
 * @param  None
 * @retval None
 */
static void protocol_send_info_1(void) {
    char data [64] = {0};
    uint8_t charge = 0;
    charge = battery_get_charge();
    sprintf(data, "BAT=%d\0", charge);
    protocol_send_data(data, 0);
}

/**
 * @brief  Protocol Send response on INFO=3 command
 * @param  None
 * @retval None
 */
static void protocol_send_info_3(void) {
    char data [64] = {0};
    float pressure = 0;
    float humidity = 0;
    float temperature = 0;
    static float old_pressure = 0;
    static float old_humidity = 0;
    static float old_temperature = 0;
    lps22hh_mode_t lps22hh_mode;
    lps22hh_get_mode(&lps22hh_mode);
    lps22hh_mode_t lps22hh_mesure_mode = {0};
    lps22hh_mesure_mode.enable = true;
    lps22hh_mesure_mode.data_rate = LPS22HH_ONE_SHOOT;
    pressure = lps22hh_get_pressure_hpa();
    lps22hh_set_mode(&lps22hh_mode);
    hts221_mode_t hts221_mode;
    hts221_get_mode(&hts221_mode);
    
    hts221_mode_t hts221_mode_set = {
        true,
        HTS221_ONE_SHOT,
        HTS221_H_AVG_4,
        HTS221_T_AVG_4
    };
    hts221_set_mode(&hts221_mode_set);
    hts221_get_hym_temp(&humidity, &temperature);
    hts221_set_mode(&hts221_mode);
    if(pressure != 0) {
        old_pressure = pressure;
    }
    if(humidity != 0) {
        old_humidity = humidity;
    }
    if(temperature != 0) {
        old_temperature = temperature;
    }
    sprintf(data, "PRESS=%.02f;HUM=%.02f;TEMP=%.02f", old_pressure, old_humidity, old_temperature);
    protocol_send_data(data, 0);
}

/**
 * @brief  Protocol Send response on INFO=4 command
 * @param  None
 * @retval None
 */
static void protocol_send_info_4(void) {
    char data [64] = {0};
    float axis_x = 0;
    float axis_y = 0;
    float axis_z = 0;
    static float old_axis_x = 0;
    static float old_axis_y = 0;
    static float old_axis_z = 0;
    lsm6dso32_mode_t saved_mode;
    lsm6dso32_mode_t mode = {0};

    lsm6dso32_get_mode(&saved_mode);
    mode.enable = true;
    mode.acc_scale = LSM6DSO32_4g;
    mode.acc_data_rate = LSM6DSO32_6667Hz;
    lsm6dso32_init();
    lsm6dso32_get_acceleration(&axis_x, &axis_y, &axis_z);

    lsm6dso32_set_mode(&saved_mode);

    if(axis_x != 0) {
        old_axis_x = acc_convert_to_m_s(axis_x);
    }
    if(axis_y != 0) {
        old_axis_y = acc_convert_to_m_s(axis_y);
    }
    if(axis_z != 0) {
        old_axis_z = acc_convert_to_m_s(axis_z);
    }

    sprintf(data, "ACC=%.02f;%.02f;%.02f", old_axis_x, old_axis_y, old_axis_z);
    protocol_send_data(data, 0);
}

/**
 * @brief  Protocol Send response on INFO=5 command
 * @param  None
 * @retval None
 */
static void protocol_send_info_5(void) {
    char data [64] = {0};
    float axis_x = 0;
    float axis_y = 0;
    float axis_z = 0;
    static float old_axis_x = 0;
    static float old_axis_y = 0;
    static float old_axis_z = 0;
    lsm6dso32_mode_t mode = {0};
    
    if (gyro_enabled == false) {
        lsm6dso32_get_mode(&gyro_saved_mode);
        gyro_enabled = true;
        mode.enable = true;
        mode.gyro_scale = LSM6DSO32_2000dps;
        mode.gyro_data_rate = LSM6DSO32_6667Hz;
        lsm6dso32_set_mode(&mode);
    }
    lsm6dso32_get_gyroscope(&axis_x, &axis_y, &axis_z);

    if(axis_x != 0) {
        old_axis_x = gyro_convert_to_rad_s(axis_x);
    }
    if(axis_y != 0) {
        old_axis_y = gyro_convert_to_rad_s(axis_y);
    }
    if(axis_z != 0) {
        old_axis_z = gyro_convert_to_rad_s(axis_z);
    }
    
    sprintf(data, "GYRO=%.02f;%.02f;%.02f", old_axis_x, old_axis_y, old_axis_z);
    protocol_send_data(data, 0);
    gyro_timeout = get_current_time_ns();
}

/**
 * @brief  Protocol Send response on INFO=6 command
 * @param  None
 * @retval None
 */
static void protocol_send_info_6(void) {
    char data [64] = {0};
    float axis_x = 0;
    float axis_y = 0;
    float axis_z = 0;
    static float old_axis_x = 0;
    static float old_axis_y = 0;
    static float old_axis_z = 0;
    lis2mdl_get_magnetic(&axis_x, &axis_y, &axis_z);

    if(axis_x != 0) {
        old_axis_x = magnetic_convert_to_ut(axis_x);
    }
    if(axis_y != 0) {
        old_axis_y = magnetic_convert_to_ut(axis_y);
    }
    if(axis_z != 0) {
        old_axis_z = magnetic_convert_to_ut(axis_z);
    }

    sprintf(data, "MAG=%.02f;%.02f;%.02f", old_axis_x, old_axis_y, old_axis_z);
    protocol_send_data(data, 0);
}

/**
 * @brief  Protocol Send response on INFO=7 command
 * @param  None
 * @retval None
 */
static void protocol_send_info_7(void) {
    char data [64] = {0};
    sprintf(data, "LED=%d", led_get_status());
    protocol_send_data(data, 0);
}

/**
 * @brief  Protocol Send response on INFO=8 command
 * @param  None
 * @retval None
 */
static void protocol_send_info_8(void) {
    char data [64] = {0};
    lightsensor_mode_t lightsensor_mode;
    lightsensor_get_mode(&lightsensor_mode);
    lightsensor_enable();
    lightsensor_set(SENSOR_MODE_LIGHT, RES_20_BIT, RATE_25_MS);
    uint32_t light = lightsensor_get_light();
    lightsensor_set_mode(&lightsensor_mode);
    sprintf(data, "LIGHT=%d", light);
    protocol_send_data(data, 0);
}

/**
 * @brief  Protocol Send response on INFO=9 command
 * @param  None
 * @retval None
 */
static void protocol_send_info_9(void) {
    char data [64] = {0};
    sprintf(data, "EN=%d;FS=%d;ODR=%d", settings.lsm6dso32_mode.enable, settings.lsm6dso32_mode.acc_scale, settings.lsm6dso32_mode.acc_data_rate);
    protocol_send_data(data, 0);
}

/**
 * @brief  Protocol Send response on INFO=10 command
 * @param  None
 * @retval None
 */
static void protocol_send_info_10(void) {
    char data [64] = {0};
    sprintf(data, "EN=%d;FS=%d;ODR=%d", settings.lsm6dso32_mode.enable, settings.lsm6dso32_mode.gyro_scale, settings.lsm6dso32_mode.gyro_data_rate);
    protocol_send_data(data, 0);
}

/**
 * @brief  Protocol Send response on INFO=11 command
 * @param  None
 * @retval None
 */
static void protocol_send_info_11(void) {
    char data [64] = {0};
    sprintf(data, "EN=%d;ODR=%d", settings.lis2mdl_mode.enable, settings.lis2mdl_mode.data_rate);
    protocol_send_data(data, 0);
}

/**
 * @brief  Protocol Send response on INFO=12 command
 * @param  None
 * @retval None
 */
static void protocol_send_info_12(void) {
    char data [64] = {0};
    sprintf(data, "EN=%d;ODR=%d", settings.lps22hh_mode.enable, settings.lps22hh_mode.data_rate);
    protocol_send_data(data, 0);
}

/**
 * @brief  Protocol Send response on INFO=13 command
 * @param  None
 * @retval None
 */
static void protocol_send_info_13(void) {
    char data [64] = {0};
    sprintf(data, "EN=%d;AVG=%d;ODR=%d", settings.hts221_mode.enable, settings.hts221_mode.hts221_avgh, settings.hts221_mode.data_rate);
    protocol_send_data(data, 0);
}

/**
 * @brief  Protocol Send response on INFO=14 command
 * @param  None
 * @retval None
 */
static void protocol_send_info_14(void) {
    char data [64] = {0};
    sprintf(data, "EN=%d;AVG=%d", settings.hts221_mode.enable, settings.hts221_mode.hts221_avgt);
    protocol_send_data(data, 0);
}

/**
 * @brief  Protocol Send response on INFO=15 command
 * @param  None
 * @retval None
 */
static void protocol_send_info_15(void) {
    char data [64] = {0};
    sprintf(data, "EN=%d;BIT=%d;MS=%d", settings.lightsensor_mode.enable, settings.lightsensor_mode.resolution, settings.lightsensor_mode.measure_rate);
    protocol_send_data(data, 0);
}

/**
 * @brief  Protocol Send response on INFO=18 command
 * @param  None
 * @retval None
 */
static void protocol_send_info_18(void) {
    char data [64] = {0};

    bool btn = false;
    btn = button_is_pressed();
    sprintf(data, "BTN=%d", btn);
    protocol_send_data(data, 0);
}

/**
 * @brief  Protocol Send response on INFO=19 command
 * @param  None
 * @retval None
 */
static void protocol_send_info_19(void) {
    char data [64] = {0};
    uint16_t raw_voc = 0;
    int32_t voc = 0;

    sgp40_measure_raw_signal(SGP40_DEFAULT_HUMIDITY, SGP40_DEFAULT_TEMPERATURE, &raw_voc);

    if (sgp_40_first_run == false) {
        sgp_40_first_run = true;
        for (uint8_t i = 0; i < 200; i++) {
            VocAlgorithm_process(&voc_params, raw_voc, &voc);
        } 
    } else {
        VocAlgorithm_process(&voc_params, raw_voc, &voc);
    }
    
    sprintf(data, "VOC=%d", voc);
    protocol_send_data(data, 0);
}

/**
 * @brief  Protocol Send response on INFO=20 command
 * @param  None
 * @retval None
 */
static void protocol_send_info_20(void) {
    char data [64] = {0};
    uint16_t raw_voc = 0;
    int32_t voc = 0;
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

    sgp40_measure_raw_signal(sgp40_get_rh_tick(hum), sgp40_get_t_tick(temp), &raw_voc); 
    
    if (sgp_40_first_run == false) {
        sgp_40_first_run = true;
        for (uint8_t i = 0; i < 200; i++) {
            VocAlgorithm_process(&voc_params, raw_voc, &voc);
        } 
    } else {
        VocAlgorithm_process(&voc_params, raw_voc, &voc);
    }
    
    sprintf(data, "VOC=%d", voc);
    protocol_send_data(data, 0);
}

/**
 * @brief  Protocol Send response on INFO=21 command
 * @param  None
 * @retval None
 */
static void protocol_send_info_21(void) {
    char data [64] = {0};
    sprintf(data, "EN=%d;SRC=%d;MS=%d", settings.sgp40_mode.enable, settings.sgp40_mode.sgp40_source, settings.sgp40_mode.sgp40_period);
    protocol_send_data(data, 0);
}

/**
 * @brief  Protocol Send response on INFO=22 command
 * @param  None
 * @retval None
 */
static void protocol_send_info_22(void) {
    char data [64] = {0};
    vl53l1x_mode_t mode;
    VL53L1X_Result_t results;

    vl53l1x_get_mode(&mode);
    vl53l1x_mode_t mode_set;
    mode_set.enable = true;
    mode_set.vl53l1x_distance = VL53_DISTANCE_SHORT;
    mode_set.vl53l1x_period = VL53_PERIOD_10S;
    vl53l1x_set_mode(&mode_set);
    vl53l1x_start_ranging(0);
    uint8_t tmp = 0;
    while (tmp == 0)
    {
        vl53l1x_check_for_data_ready(0, &tmp);
    }
    
    vl53l1x_get_result(0, &results);
    vl53l1x_clear_interrupt(0);
    vl53l1x_stop_ranging(0);
    vl53l1x_set_mode(&mode);

    sprintf(data, "DIST=%d", results.Distance);
    protocol_send_data(data, 0);
}

/**
 * @brief  Protocol Send response on INFO=23 command
 * @param  None
 * @retval None
 */
static void protocol_send_info_23(void) {
    char data [64] = {0};
    vl53l1x_mode_t mode;
    VL53L1X_Result_t results;

    vl53l1x_get_mode(&mode);
    vl53l1x_mode_t mode_set;
    mode_set.enable = true;
    mode_set.vl53l1x_distance = VL53_DISTANCE_LONG;
    mode_set.vl53l1x_period = VL53_PERIOD_10S;
    vl53l1x_set_mode(&mode_set);
    vl53l1x_start_ranging(0);
    uint8_t tmp = 0;
    while (tmp == 0)
    {
        vl53l1x_check_for_data_ready(0, &tmp);
    }
    
    vl53l1x_get_result(0, &results);
    vl53l1x_clear_interrupt(0);
    vl53l1x_stop_ranging(0);
    vl53l1x_set_mode(&mode);

    sprintf(data, "DIST=%d", results.Distance);
    protocol_send_data(data, 0);
}

/**
 * @brief  Protocol Send response on INFO=24 command
 * @param  None
 * @retval None
 */
static void protocol_send_info_24(void) {
    char data [64] = {0};
    sprintf(data, "EN=%d;DIST=%d;MS=%d", settings.vl53l1x_mode.enable, settings.vl53l1x_mode.vl53l1x_distance, settings.vl53l1x_mode.vl53l1x_period);
    protocol_send_data(data, 0);
}

/**
 * @brief  Protocol Send response on INFO=25 command
 * @param  None
 * @retval None
 */
static void protocol_send_info_25(void) {
    char data [64] = {0};
    lightsensor_mode_t lightsensor_mode;
    lightsensor_get_mode(&lightsensor_mode);
    lightsensor_enable();
    lightsensor_set(SENSOR_MODE_COLOR, RES_20_BIT, RATE_25_MS);
    uint32_t red = 0;
    uint32_t green = 0;
    uint32_t blue = 0;
    uint32_t ir = 0;
    lightsensor_get_all_colors(&red, &green, &blue, &ir);
    lightsensor_set_mode(&lightsensor_mode);
    sprintf(data, "RED=%d;GREEN=%d;BLUE=%d;IR=%d", red, green, blue, ir);
    protocol_send_data(data, 0);
}

/**
 * @brief  Protocol Send response on INFO=26 command
 * @param  None
 * @retval None
 */
static void protocol_send_info_26(void) {
    char data [64] = {0};
    sprintf(data, "DEVMOD=%d", get_settings()->device_mode);
    protocol_send_data(data, 0);
}

/**
 * @brief  Protocol Send response on INFO=27 command
 * @param  None
 * @retval None
 */
static void protocol_send_info_27(void) {
    char data [64] = {0};
    sprintf(data, "GPS=%d", get_settings()->gps_mode.enable);
    protocol_send_data(data, 0);
}

/**
 * @brief  Protocol Send response on INFO=28 command
 * @param  None
 * @retval None
 */
static void protocol_send_info_28(void) {
    char data [64] = {0};
    sprintf(data, "MIC=%d", get_settings()->microphone_mode.enable);
    protocol_send_data(data, 0);
}

/**
 * @brief  Protocol Send response on INFO=29 command
 * @param  None
 * @retval None
 */
static void protocol_send_info_29(void) {
    char data [64] = {0};
    sprintf(data, "PWM_MODE=%d", get_settings()->pwm_mode_block);
    protocol_send_data(data, 0);
}

/**
 * @brief  Protocol set on SETT:ID=0 command
 * @param  None
 * @retval None
 */
static void protocol_set_0(char *pnt) {
    Settings_t sett;
    memcpy (&sett, &settings, sizeof(Settings_t));
    sscanf(pnt,"EN=%hhu;FS=%hhu;ODR=%hhu", &sett.lsm6dso32_mode.enable, &sett.lsm6dso32_mode.acc_scale, &sett.lsm6dso32_mode.acc_data_rate);
    if (settings_check(&sett) == 0) {
        memcpy(&settings, &sett, sizeof(Settings_t));
        lsm6dso32_set_mode(&settings.lsm6dso32_mode);
        settings_set_to_storage(&settings);
        protocol_send_ok();
    }
}

/**
 * @brief  Protocol set on SETT:ID=1 command
 * @param  None
 * @retval None
 */
static void protocol_set_1(char *pnt) {
    Settings_t sett;
    memcpy (&sett, &settings, sizeof(Settings_t));
    sscanf(pnt,"EN=%hhd;FS=%hhd;ODR=%hhd", &sett.lsm6dso32_mode.enable, &sett.lsm6dso32_mode.gyro_scale, &sett.lsm6dso32_mode.gyro_data_rate);
    if (settings_check(&sett) == 0) {
        memcpy(&settings, &sett, sizeof(Settings_t));
        lsm6dso32_set_mode(&settings.lsm6dso32_mode);
        settings_set_to_storage(&settings);
        protocol_send_ok();
    }
}

/**
 * @brief  Protocol set on SETT:ID=2 command
 * @param  None
 * @retval None
 */
static void protocol_set_2(char *pnt) {
    Settings_t sett;
    memcpy (&sett, &settings, sizeof(Settings_t));
    sscanf(pnt,"EN=%hhu;ODR=%hhu", &sett.lis2mdl_mode.enable, &sett.lis2mdl_mode.data_rate);
    if (settings_check(&sett) == 0) {
        memcpy(&settings, &sett, sizeof(Settings_t));
        lis2mdl_set_mode(&settings.lis2mdl_mode);
        settings_set_to_storage(&settings);
        protocol_send_ok();
    }
}

/**
 * @brief  Protocol set on SETT:ID=3 command
 * @param  None
 * @retval None
 */
static void protocol_set_3(char *pnt) {
    Settings_t sett;
    memcpy (&sett, &settings, sizeof(Settings_t));
    sscanf(pnt,"EN=%hhu;ODR=%hhu", &sett.lps22hh_mode.enable, &sett.lps22hh_mode.data_rate);
    if (settings_check(&sett) == 0) {
        memcpy(&settings, &sett, sizeof(Settings_t));
        lps22hh_set_mode(&settings.lps22hh_mode);
        settings_set_to_storage(&settings);
        protocol_send_ok();
    }
}

/**
 * @brief  Protocol set on SETT:ID=4 command
 * @param  None
 * @retval None
 */
static void protocol_set_4(char *pnt) {
    Settings_t sett;
    memcpy (&sett, &settings, sizeof(Settings_t));
    sscanf(pnt,"EN=%hhu;ODR=%hhu;AVG=%hhu", &sett.hts221_mode.enable, &sett.hts221_mode.data_rate, &sett.hts221_mode.hts221_avgh);
    if (settings_check(&sett) == 0) {
        memcpy(&settings, &sett, sizeof(Settings_t));
        hts221_set_mode(&settings.hts221_mode);
        settings_set_to_storage(&settings);
        protocol_send_ok();
    }
}

/**
 * @brief  Protocol set on SETT:ID=5 command
 * @param  None
 * @retval None
 */
static void protocol_set_5(char *pnt) {
    Settings_t sett;
    memcpy (&sett, &settings, sizeof(Settings_t));
    sscanf(pnt,"EN=%hhu;AVG=%hhu", &sett.hts221_mode.enable, &sett.hts221_mode.hts221_avgt);
    if (settings_check(&sett) == 0) {
        memcpy(&settings, &sett, sizeof(Settings_t));
        hts221_set_mode(&settings.hts221_mode);
        settings_set_to_storage(&settings);
        protocol_send_ok();
    }
}

/**
 * @brief  Protocol set on SETT:ID=6 command
 * @param  None
 * @retval None
 */
static void protocol_set_6(char *pnt) {
    Settings_t sett;
    memcpy (&sett, &settings, sizeof(Settings_t));
    sscanf(pnt,"EN=%hhu;BIT=%hhu;MS=%hhu", &sett.lightsensor_mode.enable, &sett.lightsensor_mode.resolution, &sett.lightsensor_mode.measure_rate);
    if (settings_check(&sett) == 0) {
        memcpy(&settings, &sett, sizeof(Settings_t));
        lightsensor_set_mode(&settings.lightsensor_mode);
        settings_set_to_storage(&settings);
        protocol_send_ok();
    }
}

/**
 * @brief  Protocol set on SETT:ID=10 command
 * @param  None
 * @retval None
 */
static void protocol_set_10(char *pnt) {
    Settings_t sett;
    memcpy (&sett, &settings, sizeof(Settings_t));
    sscanf(pnt,"EN=%hhu;SRC=%hhu;MS=%hhu", &sett.sgp40_mode.enable, &sett.sgp40_mode.sgp40_source, &sett.sgp40_mode.sgp40_period);
    if (settings_check(&sett) == 0) {
        memcpy(&settings, &sett, sizeof(Settings_t));
        sgp40_set_mode(&settings.sgp40_mode);
        settings_set_to_storage(&settings);
        protocol_send_ok();
    }
}

/**
 * @brief  Protocol set on SETT:ID=11 command
 * @param  None
 * @retval None
 */
static void protocol_set_11(char *pnt) {
    Settings_t sett;
    memcpy (&sett, &settings, sizeof(Settings_t));
    sscanf(pnt,"EN=%hhu;DIST=%hhu;MS=%hhu", &sett.vl53l1x_mode.enable, &sett.vl53l1x_mode.vl53l1x_distance, &sett.vl53l1x_mode.vl53l1x_period);
    if (settings_check(&sett) == 0) {
        memcpy(&settings, &sett, sizeof(Settings_t));
        vl53l1x_set_mode(&settings.vl53l1x_mode);
        settings_set_to_storage(&settings);
        protocol_send_ok();
    }
}

/**
 * @brief  Protocol set on SETT:ID=12 command
 * @param  None
 * @retval None
 */
static void protocol_set_12(char *pnt) {
    Settings_t sett;
    memcpy (&sett, &settings, sizeof(Settings_t));
    sscanf(pnt, "DEVMOD=%hhd", &sett.device_mode);
    if (settings_check(&sett) == 0) {
        memcpy(&settings, &sett, sizeof(Settings_t));
        settings_set_to_storage(&settings);
        protocol_send_ok();
    }
}

/**
 * @brief  Protocol set on SETT:ID=13 command
 * @param  None
 * @retval None
 */
static void protocol_set_13(char *pnt) {
    Settings_t sett;
    memcpy (&sett, &settings, sizeof(Settings_t));
    sscanf(pnt, "GPS=%hhd", &sett.gps_mode.enable);
    if (settings_check(&sett) == 0) {
        memcpy(&settings, &sett, sizeof(Settings_t));
        settings_set_to_storage(&settings);
        protocol_send_ok();
    }
}

/**
 * @brief  Protocol set on SETT:ID=14 command
 * @param  None
 * @retval None
 */
static void protocol_set_14(char *pnt) {
    Settings_t sett;
    memcpy (&sett, &settings, sizeof(Settings_t));
    sscanf(pnt, "MIC=%hhd", &sett.microphone_mode.enable);
    if (settings_check(&sett) == 0) {
        memcpy(&settings, &sett, sizeof(Settings_t));
        settings_set_to_storage(&settings);
        protocol_send_ok();
    }
}

/**
 * @brief  Protocol set on SETT:ID=15 command
 * @param  None
 * @retval None
 */
static void protocol_set_15(char *pnt) { // PWM_MODE
    Settings_t sett;
    memcpy (&sett, &settings, sizeof(Settings_t));
    sscanf(pnt, "PWM_MODE=%hhd", &sett.pwm_mode_block);
    if (settings_check(&sett) == 0) {
        memcpy(&settings, &sett, sizeof(Settings_t));
        settings_set_to_storage(&settings);
        protocol_send_ok();
    }
}

/**
 * @brief  Protocol set on SETT:ID=16 command
 * @param  None
 * @retval None
 */
static void protocol_set_16(char *pnt) {
    uint16_t freq_hz = 0;
    uint8_t duty = 0;
    uint16_t time_ms = 0;
    sscanf(pnt, "FREQ=%hd;DUTY=%hhd;TIME=%hd", &freq_hz, &duty, &time_ms);
    pwm_run(freq_hz, duty, time_ms);
    protocol_send_ok();
}

/**
 * @brief  Protocol Send error response
 * @param  None
 * @retval None
 */
static void protocol_send_error(void) {
    protocol_send_data("ERROR", 0);
}

/**
 * @brief  Protocol Send ok response
 * @param  None
 * @retval None
 */
static void protocol_send_ok(void) {
    protocol_send_data("OK", 0);
}

void protocol_act(void) {
    uart_act_callback();
    if(new_command == true) {
        if(rx_pnt != nullptr) {
            if(protocol_check_data(rx_pnt) != false) {
                if(strstr(rx_pnt, "INFO=0,") != 0){
                    protocol_send_info_0();
                } else if(strstr(rx_pnt, "INFO=1,") != 0) {
                    protocol_send_info_1();
                } else if(strstr(rx_pnt, "INFO=3,") != 0) {
                    protocol_send_info_3();
                } else if(strstr(rx_pnt, "INFO=4,") != 0) {
                    protocol_send_info_4();
                } else if(strstr(rx_pnt, "INFO=5,") != 0) {
                    protocol_send_info_5();
                } else if(strstr(rx_pnt, "INFO=6,") != 0) {
                    protocol_send_info_6();
                } else if(strstr(rx_pnt, "INFO=7,") != 0) {
                    protocol_send_info_7();
                } else if(strstr(rx_pnt, "INFO=8,") != 0) {
                    protocol_send_info_8();
                } else if(strstr(rx_pnt, "INFO=9,") != 0) {
                    protocol_send_info_9();
                } else if(strstr(rx_pnt, "INFO=10,") != 0) {
                    protocol_send_info_10();
                } else if(strstr(rx_pnt, "INFO=11,") != 0) {
                    protocol_send_info_11();
                } else if(strstr(rx_pnt, "INFO=12,") != 0) {
                    protocol_send_info_12();
                } else if(strstr(rx_pnt, "INFO=13,") != 0) {
                    protocol_send_info_13();
                } else if(strstr(rx_pnt, "INFO=14,") != 0) {
                    protocol_send_info_14();
                } else if(strstr(rx_pnt, "INFO=15,") != 0) {
                    protocol_send_info_15();
                } else if(strstr(rx_pnt, "INFO=18,") != 0) {
                    protocol_send_info_18();
                } else if(strstr(rx_pnt, "INFO=19,") != 0) {
                    protocol_send_info_19();
                } else if(strstr(rx_pnt, "INFO=20,") != 0) {
                    protocol_send_info_20();
                } else if(strstr(rx_pnt, "INFO=21,") != 0) {
                    protocol_send_info_21();
                } else if(strstr(rx_pnt, "INFO=22,") != 0) {
                    protocol_send_info_22();
                } else if(strstr(rx_pnt, "INFO=23,") != 0) {
                    protocol_send_info_23();
                } else if(strstr(rx_pnt, "INFO=24,") != 0) {
                    protocol_send_info_24();
                } else if(strstr(rx_pnt, "INFO=25,") != 0) {
                    protocol_send_info_25();
                } else if(strstr(rx_pnt, "INFO=26,") != 0) {
                    protocol_send_info_26();
                } else if(strstr(rx_pnt, "INFO=27,") != 0) {
                    protocol_send_info_27();
                } else if(strstr(rx_pnt, "INFO=28,") != 0) {
                    protocol_send_info_28();
                } else if(strstr(rx_pnt, "INFO=29,") != 0) {
                    protocol_send_info_29();
                } else if(strstr(rx_pnt, "LED=0;") != 0) {
                    sscanf(rx_pnt, "LED=0;TIME=%hd,", &protocol_led_control_time_s);
                    led_green_off();
                    led_red_off();
                    protocol_send_ok();
                } else if(strstr(rx_pnt, "LED=1;") != 0) {
                    sscanf(rx_pnt, "LED=1;TIME=%hd,", &protocol_led_control_time_s);
                    led_green_on();
                    led_red_off();
                    protocol_send_ok();
                } else if(strstr(rx_pnt, "LED=2;") != 0) {
                    sscanf(rx_pnt, "LED=2;TIME=%hd,", &protocol_led_control_time_s);
                    led_green_off();
                    led_red_on();
                    protocol_send_ok();
                } else if(strstr(rx_pnt, "LED=3;") != 0) {
                    sscanf(rx_pnt, "LED=3;TIME=%hd,", &protocol_led_control_time_s);
                    led_green_on();
                    led_red_on();
                    protocol_send_ok();
                } else if(strstr(rx_pnt, "SETT:ID=0.") != 0) {
                    char *pnt = strstr(rx_pnt, ".");
                    pnt++;
                    protocol_set_0(pnt);
                } else if(strstr(rx_pnt, "SETT:ID=1.") != 0) {
                    char *pnt = strstr(rx_pnt, ".");
                    pnt++;
                    protocol_set_1(pnt);
                } else if(strstr(rx_pnt, "SETT:ID=2.") != 0) {
                    char *pnt = strstr(rx_pnt, ".");
                    pnt++;
                    protocol_set_2(pnt);
                } else if(strstr(rx_pnt, "SETT:ID=3.") != 0) {
                    char *pnt = strstr(rx_pnt, ".");
                    pnt++;
                    protocol_set_3(pnt);
                } else if(strstr(rx_pnt, "SETT:ID=4.") != 0) {
                    char *pnt = strstr(rx_pnt, ".");
                    pnt++;
                    protocol_set_4(pnt);
                } else if(strstr(rx_pnt, "SETT:ID=5.") != 0) {
                    char *pnt = strstr(rx_pnt, ".");
                    pnt++;
                    protocol_set_5(pnt);
                } else if(strstr(rx_pnt, "SETT:ID=6.") != 0) {
                    char *pnt = strstr(rx_pnt, ".");
                    pnt++;
                    protocol_set_6(pnt);
                } else if(strstr(rx_pnt, "SETT:ID=10.") != 0) {
                    char *pnt = strstr(rx_pnt, ".");
                    pnt++;
                    protocol_set_10(pnt);
                } else if(strstr(rx_pnt, "SETT:ID=11.") != 0) {
                    char *pnt = strstr(rx_pnt, ".");
                    pnt++;
                    protocol_set_11(pnt);
                } else if(strstr(rx_pnt, "SETT:ID=12.") != 0) {
                    char *pnt = strstr(rx_pnt, ".");
                    pnt++;
                    protocol_set_12(pnt);
                } else if(strstr(rx_pnt, "SETT:ID=13.") != 0) {
                    char *pnt = strstr(rx_pnt, ".");
                    pnt++;
                    protocol_set_13(pnt);
                } else if(strstr(rx_pnt, "SETT:ID=14.") != 0) {
                    char *pnt = strstr(rx_pnt, ".");
                    pnt++;
                    protocol_set_14(pnt);
                } else if(strstr(rx_pnt, "SETT:ID=15.") != 0) {
                    char *pnt = strstr(rx_pnt, ".");
                    pnt++;
                    protocol_set_15(pnt);
                } else if(strstr(rx_pnt, "SETT:ID=16.") != 0) {
                    char *pnt = strstr(rx_pnt, ".");
                    pnt++;
                    protocol_set_16(pnt);
                } else if(strstr(rx_pnt, "STOP,") != 0) {
                    stop = true;
                    protocol_send_ok();
                } else if(strstr(rx_pnt, "DEFAULT,") != 0) {
                    settings_load_default(&settings);
                    settings_set_to_storage(&settings);
                    protocol_send_ok();
                } else if(strstr(rx_pnt, "SETTINGS,") != 0) {
                    settings_print(&settings);
                } else if(strstr(rx_pnt, "RESET,") != 0) {
#if 1
                    boardctl(BOARDIOC_RESET, 0);
#else
                    cxd56_cpu_reset(0);
#endif
                } else {
                    protocol_send_error();
                    new_command = false;
                    uart_clr_rx_buf();
                    return;
                }
            }
        }
        new_command = false;
        uart_clr_rx_buf();
    }

    if (get_passed_time_ns(gyro_timeout) > (1000 * 1000000)) {
        if (gyro_enabled != false) {
            gyro_enabled = false;
            lsm6dso32_set_mode(&gyro_saved_mode);
        }
    }
}

void protocol_send_data(char* data, uint16_t size) {
    char crc16 [5] = {0};
    if(data == nullptr) {
        return;
    }
    if(size == 0) {
        size = strlen(data);
    }
    uint16_t crc = culate_crc16(data, size);
    conv_uint16_to_hex(crc, crc16);
    uart_send_data(data, size);
    uart_send_data(crc16, 0);
    uart_send_data("\r\n", 0);
}

/**
 * @brief  Protocol check if data record should be stoped
 * @param  None
 * @retval Stop status
 */
bool protocol_stop(void) {
    return stop;
}

/**
 * @brief  Protocol reset stop status
 * @param  None
 * @retval None
 */
void reset_stop(void) {
    stop = false;
}
