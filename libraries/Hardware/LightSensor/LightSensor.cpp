/**
 ******************************************************************************
 * @file    LightSensor.cpp
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

#include <string.h>
#include "LightSensor.h"
#include "../Apds9250/Apds9250.h"
#include "../../Drivers/CXD5602/I2c/I2c.h"

Apds9250 myApds9250;

static lightsensor_mode_t lightsensor_mode = {0};

/**
 * @brief  lightsensor init
 * @param  None
 * @retval true if ok
 */
bool lightsensor_init(void) {
    static bool is_init = false;
    
    if (is_init != false) {
        return true;
    }
    if (myApds9250.begin()) {
        return false;
    }
    if (is_init == false) {
        is_init = true;
    } 
    myApds9250.setGain(gain1);
    return true;
}

/**
 * @brief  lightsensor setup
 * @param  [in] mode from lightsensor_work_mode_t
 * @param  [in] resolution from lightsensor_resolution_t
 * @param  [in] measure rate ms from lightsensor_measure_rate_t
 * @retval none
 */
void lightsensor_set(lightsensor_work_mode_t mode, lightsensor_resolution_t resolution, lightsensor_measure_rate_t measure) {
    myApds9250.setMode(modeColorSensor);
    myApds9250.setResolution(res18bit);
    myApds9250.setMeasurementRate(rate100ms);
    lightsensor_mode.enable = true;
    lightsensor_mode.sensor_mode = mode;
    lightsensor_mode.resolution = resolution;
    lightsensor_mode.measure_rate = measure;
}

/**
 * @brief  lightsensor get all colors values
 * @param  [out] red red color value
 * @param  [out] green green color value
 * @param  [out] blue blue color value
 * @param  [out] ir ir color value
 * @retval true if Ok
 */
void lightsensor_get_colors(uint32_t* red, uint32_t* green, uint32_t* blue, uint32_t* ir) {
    if ((red == nullptr) || (green == nullptr) || (blue == nullptr) || (ir == nullptr)) {
        return;
    }
    myApds9250.getAll(red, green, blue, ir);
}


/**
 * @brief  get lightsensor current mode
 * @param  [out] mode from lightsensor_mode_t
 * @retval none
 */
void lightsensor_get_mode(lightsensor_mode_t* mode) {
    if (mode == nullptr) {
        return;
    }
    memcpy(mode, &lightsensor_mode, sizeof(lightsensor_mode_t));
}

/**
 * @brief  set lightsensor current mode
 * @param  [in] mode from lightsensor_mode_t
 * @retval none
 */
void lightsensor_set_mode(lightsensor_mode_t* mode) {
    if (mode == nullptr) {
        return;
    }
    memcpy(&lightsensor_mode, mode, sizeof(lightsensor_mode_t));
    lightsensor_set(lightsensor_mode.sensor_mode, lightsensor_mode.resolution, lightsensor_mode.measure_rate);
    if (lightsensor_mode.enable != false) {
        lightsensor_enable();
    } else {
        lightsensor_disable();
    }
}

/**
 * @brief  lightsensor get light value
 * @param  None
 * @retval light value
 */
uint32_t lightsensor_get_light(void) { 

#if 0
    return myApds9250.getAmbientLight(); 
#else
    uint32_t red = 0;
    uint32_t green = 0;
    uint32_t blue = 0;
    uint32_t ir = 0;
    myApds9250.getAll(&red, &green, &blue, &ir);
    return myApds9250.ConvertLightData(green, ir);
#endif
}

/**
 * @brief  lightsensor get all colors values
 * @param  [out] red red color value
 * @param  [out] green green color value
 * @param  [out] blue blue color value
 * @param  [out] ir ir color value
 * @retval true if Ok
 */
void lightsensor_get_all_colors (uint32_t* red, uint32_t* green, uint32_t* blue, uint32_t* ir) {
    if ((red == nullptr) || (green == nullptr) || (blue == nullptr) || (ir == nullptr)) {
        return;
    }
    myApds9250.getAll(red, green, blue, ir);
}

/**
 * @brief  enable lightsensor
 * @param  None
 * @retval None
 */
void lightsensor_enable(void) {
    myApds9250.enable();
    lightsensor_mode.enable = true;
}

/**
 * @brief  disable lightsensor
 * @param  None
 * @retval None
 */
void lightsensor_disable(void) {
    myApds9250.disable();
    lightsensor_mode.enable = false;
}

/**
 * @brief  get mesure pheriod [ns]
 * @param  [in] rate measure rate from lightsensor_measure_rate_t
 * @retval measure pheriod [ns] 0xffffffffffffffff in other caces
 */
uint64_t lightsensor_get_pheriod_ns(lightsensor_measure_rate_t rate) {
    switch (rate) {
        case RATE_25_MS:
        return 25*1000000;
        case RATE_50_MS:
        return 50*1000000;
        case RATE_100_MS:
        return 100*1000000;
        case RATE_200_MS:
        return 200*1000000;
        case RATE_500_MS:
        return 500*1000000;
        case RATE_1000_MS:
        return 1000*1000000;
        case RATE_2000_MS:
        return 2000*1000000;
        default:
        break;
    }
    return 0xffffffffffffffff;
}
