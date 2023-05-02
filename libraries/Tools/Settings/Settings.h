/**
 ******************************************************************************
 * @file    Settings.h
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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdint.h>
#include "../../../../Appdefines.h"
#include "../../Hardware/Max7317/Max7317.h"
#include "../../Drivers/CXD5602/I2c/I2c.h"
#include "../../Hardware/Hts221/Hts221.h"
#include "../../Hardware/Lis2mdl/Lis2mdl.h"
#include "../../Hardware/Lps22hh/Lps22hh.h"
#include "../../Hardware/Lsm6dso32/Lsm6dso32.h"
#include "../../Hardware/Vl53l1x/VL53L1X_api.h"
#include "../../Hardware/Sgp4x/sensirion_common.h"
#include "../../Hardware/Sgp4x/sensirion_i2c.h"
#include "../../Hardware/Sgp4x/sensirion_i2c_hal.h"
#include "../../Hardware/Sgp4x/sgp41_i2c.h"
#include "../../Hardware/Sgp4x/sgp40_i2c.h"
#include "../../Hardware/Apds9250/Apds9250.h"
#include "../../Drivers/CXD5602/Pwm/Pwm.h"
#include "../../Hardware/Sdcard/Sdcard.h"
#include "../../ThirdParty/Fatfs/ff.h"
#include "../../Hardware/Led/Led.h"
#include "../../Hardware/Button/Button.h"
#include "../../Drivers/CXD5602/Gnss/Gnss.h"
#include "../../Drivers/CXD5602/Uart/Uart.h"
#include "../../Protocol/Protocol.h"
#include "../../Tools/Settings/Settings.h"
#include "../../Hardware/Bq27441/Bq27441.h"
#include "../../Hardware/LightSensor/LightSensor.h"
#include "../../Hardware/Gps/Gps.h"
#include "../../Hardware/Microphone/Microphone.h"

typedef enum {
    DEVICE_MODE_DEMO = 0,
    DEVICE_MODE_MESURE,
    DEVICE_MODE_ENUM
} device_mode_t;

typedef struct {
    char device_name[DEVICE_NAME_LEN];
    uint8_t device_version;
    uint8_t device_subversion;
    device_mode_t device_mode;
    lightsensor_mode_t lightsensor_mode;
    lsm6dso32_mode_t lsm6dso32_mode;
    lis2mdl_mode_t lis2mdl_mode;
    lps22hh_mode_t lps22hh_mode;
    hts221_mode_t hts221_mode;
    sgp40_mode_t sgp40_mode;
    vl53l1x_mode_t vl53l1x_mode;
    gps_mode_t gps_mode;
    microphone_mode_t microphone_mode;
    bool pwm_mode_block;
} Settings_t;

/**
 * @brief  Settings init
 * @param  none
 * @retval none
 */
void settings_init (void);

/**
 * @brief  Get settings data fron storage
 * @param  [out] settings struct
 * @retval false if false
 */
bool settings_get_from_storage (Settings_t *settings);

/**
 * @brief  Write settings data fron storage
 * @param  [out] settings struct
 * @retval false if false
 */
bool settings_set_to_storage (Settings_t *settings);

/**
 * @brief  Check if settings ok
 * @param  [out] settings struct
 * @retval 0 if OK
 */
uint16_t settings_check (Settings_t *settings);

/**
 * @brief  Set defoult settings 
 * @param  [out] settings struct
 * @retval false if false
 */
bool settings_load_default (Settings_t *settings);

/**
 * @brief  Get settings 
 * @param  none
 * @retval pointer to settings struct
 */
Settings_t *get_settings(void);

/**
 * @brief  Printing settings
 * @param  [in] settings struct
 * @retval none
 */
void settings_print(Settings_t *settings);

extern Settings_t settings;

#endif // SETTINGS_H
