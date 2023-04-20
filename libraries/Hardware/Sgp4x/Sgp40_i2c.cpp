/**
 ******************************************************************************
 * @file    Sgp40_i2c.cpp
 * @date    20 Octpber 2022
 * @brief   SGP40 driver
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

#include <string.h>
#include "sgp40_i2c.h"
#include "sensirion_common.h"
#include "sensirion_i2c.h"
#include "sensirion_i2c_hal.h"
#include "../../Drivers/CXD5602/I2c/I2c.h"

#define SGP40_I2C_ADDRESS 0x59

sgp40_mode_t sgp40_mode = {0};

int16_t sgp40_measure_raw_signal(uint16_t relative_humidity, uint16_t temperature, uint16_t* sraw_voc) {
    int16_t error;
    uint8_t buffer[8];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(&buffer[0], offset, 0x260F);

    offset = sensirion_i2c_add_uint16_t_to_buffer(&buffer[0], offset,
                                                  relative_humidity);
    offset =
        sensirion_i2c_add_uint16_t_to_buffer(&buffer[0], offset, temperature);

    error = sensirion_i2c_write_data(SGP40_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }

    sensirion_i2c_hal_sleep_usec(30000);

    error = sensirion_i2c_read_data_inplace(SGP40_I2C_ADDRESS, &buffer[0], 2);
    if (error) {
        return error;
    }
    *sraw_voc = sensirion_common_bytes_to_uint16_t(&buffer[0]);
    return NO_ERROR;
}

int16_t sgp40_execute_self_test(uint16_t* test_result) {
    int16_t error;
    uint8_t buffer[3];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(&buffer[0], offset, 0x280E);

    error = sensirion_i2c_write_data(SGP40_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }

    sensirion_i2c_hal_sleep_usec(320000);

    error = sensirion_i2c_read_data_inplace(SGP40_I2C_ADDRESS, &buffer[0], 2);
    if (error) {
        return error;
    }
    *test_result = sensirion_common_bytes_to_uint16_t(&buffer[0]);
    return NO_ERROR;
}

int16_t sgp40_turn_heater_off(void) {
    int16_t error;
    uint8_t buffer[2];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(&buffer[0], offset, 0x3615);

    error = sensirion_i2c_write_data(SGP40_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }
    sensirion_i2c_hal_sleep_usec(1000);
    return NO_ERROR;
}

int16_t sgp40_get_serial_number(uint16_t* serial_number, uint8_t serial_number_size) {
    int16_t error;
    uint8_t buffer[9];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(&buffer[0], offset, 0x3682);

    error = sensirion_i2c_write_data(SGP40_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }

    sensirion_i2c_hal_sleep_usec(1000);

    error = sensirion_i2c_read_data_inplace(SGP40_I2C_ADDRESS, &buffer[0], 6);
    if (error) {
        return error;
    }

    serial_number[0] = sensirion_common_bytes_to_uint16_t(&buffer[0]);
    serial_number[1] = sensirion_common_bytes_to_uint16_t(&buffer[2]);
    serial_number[2] = sensirion_common_bytes_to_uint16_t(&buffer[4]);

    return NO_ERROR;
}

/**
 * @brief  Calculate humidity tics from humidity [%]
 * @param  [in] rh humidity from 0 to 100 [%]
 * @retval humidity in tics
 */
uint16_t sgp40_get_rh_tick(float rh) {
    float result = (rh * 65535.0) / 100.0;
    return (uint16_t) result;
}

/**
 * @brief  Calculate temerature tics from temerature [degC]
 * @param  [in] t temerature from [degC]
 * @retval temerature in tics
 */
uint16_t sgp40_get_t_tick(float t) {
    float result = ((t + 45.0) * 65535.0) / 175.0;
    return (uint16_t) result;
}

bool sgp40_init(void) {
    int16_t error = 0;
    uint16_t serial_number[3];
    uint8_t serial_number_size = 3;
    i2c_init();
    error = sgp40_get_serial_number(serial_number, serial_number_size);

    if (error) {
        return false;
    }
    return true;
}

/**
 * @brief  get mesure pheriod [ns]
 * @param  [in] rate measure rate from sgp40_period_t
 * @retval measure pheriod [ns], 0xffffffffffffffff in other caces
 */
uint64_t sgp40_get_pheriod_ns(sgp40_period_t rate) {
    switch (rate) {
        case SGP40_1S:
        return 1 * 1000000000;
        case SGP40_2S:
        return 2 * 1000000000;
        case SGP40_5S:
        return 5 * 1000000000;
        case SGP40_10S:
        return 10 * 1000000000;
        default:
        break;
    }
    return 0xffffffffffffffff;
}

/**
 * @brief  get sgp40 current mode
 * @param  [out] mode from sgp40_mode_t
 * @retval none
 */
void sgp40_get_mode(sgp40_mode_t* mode) {
    memcpy(mode, &sgp40_mode, sizeof(sgp40_mode_t));
}

/**
 * @brief  set sgp40 current mode
 * @param  [in] mode from sgp40_mode_t
 * @retval none
 */
void sgp40_set_mode(sgp40_mode_t* mode) {
    memcpy(&sgp40_mode, mode, sizeof(sgp40_mode_t));
    if (sgp40_mode.enable != false) {
        sgp40_enable();
    } else {
        sgp40_disable();
    }
}

/**
 * @brief  enable sgp40
 * @param  None
 * @retval None
 */
void sgp40_enable(void) {
    sgp40_mode.enable = true;
}

/**
 * @brief  disable sgp40
 * @param  None
 * @retval None
 */
void sgp40_disable(void) {
    sgp40_mode.enable = false;
}