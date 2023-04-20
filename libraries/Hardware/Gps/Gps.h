/**
 ******************************************************************************
 * @file    Gps.h
 * @date    19 December 2022
 * @brief   Driver for Gps
 ******************************************************************************
 *
 * COPYRIGHT(c) 2021 EDGE Impulse
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

#ifndef GPS_H
#define GPS_H

#include <stdint.h>

/**
 * @brief  gps init
 * @param  none
 * @retval none
 */
void gps_init(void);

typedef enum {
    GPS_NO_DATA,
    GPS_NEW_DATA,
    GPS_ERROR
} gps_act_status_t;

/**
 * @brief  gps activity
 * @param  none
 * @retval statis from gps_act_status_t
 */
gps_act_status_t gps_act(void);

/**
 * @brief  get gps string data
 * @param  none
 * @retval pointer to gps data
 */
char *gps_get_data(void);

typedef struct {
    bool enable;
} gps_mode_t;

/**
 * @brief  get gps current mode
 * @param  [out] mode from gps_mode_t
 * @retval none
 */
void gps_get_mode(gps_mode_t* mode);

/**
 * @brief  set gps current mode
 * @param  [in] mode from gps_mode_t
 * @retval none
 */
void gps_set_mode(gps_mode_t* mode);

/**
 * @brief  enable gps
 * @param  None
 * @retval None
 */
void gps_enable(void);

/**
 * @brief  disable gps
 * @param  None
 * @retval None
 */
void gps_disable(void);



#endif // GPS_H
