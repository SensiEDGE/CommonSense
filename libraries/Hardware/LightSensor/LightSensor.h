/**
 ******************************************************************************
 * @file    LightSensor.h
 * @date    16 November 2022
 * @brief   LightSensor.APIs
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

#ifndef LIGHTSENSOR_H
#define LIGHTSENSOR_H

#include <stdint.h>

typedef enum {
    SENSOR_MODE_LIGHT,
    SENSOR_MODE_COLOR,
    SENSOR_MODE_ENUM
} lightsensor_work_mode_t;

typedef enum {
    RES_20_BIT = 0,
    RES_19_BIT,
    RES_18_BIT,
    RES_17_BIT,
    RES_16_BIT,
    RES_13_BIT,
    RES_ENUM   
} lightsensor_resolution_t;

typedef enum {
    RATE_25_MS = 0,
    RATE_50_MS,
    RATE_100_MS,
    RATE_200_MS,
    RATE_500_MS,
    RATE_1000_MS,
    RATE_2000_MS,
    RATE_ENUM
} lightsensor_measure_rate_t;



typedef struct {
    bool enable;
    lightsensor_work_mode_t sensor_mode;
    lightsensor_resolution_t resolution;
    lightsensor_measure_rate_t measure_rate;
} lightsensor_mode_t;

/**
 * @brief  lightsensor init
 * @param  None
 * @retval true if ok
 */
bool lightsensor_init(void);

/**
 * @brief  lightsensor setup
 * @param  [in] mode from lightsensor_work_mode_t
 * @param  [in] resolution from lightsensor_resolution_t
 * @param  [in] measure rate ms from lightsensor_measure_rate_t
 * @retval none
 */
void lightsensor_set(lightsensor_work_mode_t mode, lightsensor_resolution_t resolution, lightsensor_measure_rate_t measure);

/**
 * @brief  get lightsensor current mode
 * @param  [out] mode from lightsensor_mode_t
 * @retval none
 */
void lightsensor_get_mode(lightsensor_mode_t* mode);

/**
 * @brief  set lightsensor current mode
 * @param  [in] mode from lightsensor_mode_t
 * @retval none
 */
void lightsensor_set_mode(lightsensor_mode_t* mode);

/**
 * @brief  lightsensor get light value
 * @param  None
 * @retval light value
 */
uint32_t lightsensor_get_light(void);

/**
 * @brief  lightsensor get all colors values
 * @param  [out] red red color value
 * @param  [out] green green color value
 * @param  [out] blue blue color value
 * @param  [out] ir ir color value
 * @retval true if Ok
 */
void lightsensor_get_all_colors (uint32_t* red, uint32_t* green, uint32_t* blue, uint32_t* ir);

/**
 * @brief  enable lightsensor
 * @param  None
 * @retval None
 */
void lightsensor_enable(void);

/**
 * @brief  disable lightsensor
 * @param  None
 * @retval None
 */
void lightsensor_disable(void);

/**
 * @brief  get mesure pheriod [ns]
 * @param  [in] rate measure rate from lightsensor_measure_rate_t
 * @retval measure pheriod [ns]
 */
uint64_t lightsensor_get_pheriod_ns(lightsensor_measure_rate_t rate);

#endif // LIGHTSENSOR_H
