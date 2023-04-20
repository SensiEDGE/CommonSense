/**
 ******************************************************************************
 * @file    Microphone.h
 * @date    26 December 2022
 * @brief   Microphone driver
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

#ifndef MICROPHONE_H
#define MICROPHONE_H

#include <stdint.h>
#include "../../ThirdParty/Fatfs/ff.h"

typedef struct {
    bool enable;
} microphone_mode_t;

/**
 * @brief  microphone init
 * @param  None
 * @retval true if ok
 */
bool microphone_init(void);

/**
 * @brief  get microphone current mode
 * @param  [out] mode from microphone_mode_t
 * @retval none
 */
void microphone_get_mode(microphone_mode_t* mode);

/**
 * @brief  set microphone current mode
 * @param  [in] mode from microphone_mode_t
 * @retval none
 */
void microphone_set_mode(microphone_mode_t* mode);

/**
 * @brief  enable microphone
 * @param  None
 * @retval None
 */
void microphone_enable(void);

/**
 * @brief  disable microphone
 * @param  None
 * @retval None
 */
void microphone_disable(void);

/**
 * @brief  Check if microphone data ready
 * @param  [out] data pointer to data buffer
 * @param  [out] size size of data
 * @retval false if not ready 
 */
bool microphone_is_data_ready(uint8_t *data, uint16_t *size);

/**
 * @brief  Write microphone data to wav file
 * @param  [in] fp pointer file
 * @param  [in] data pointer to data
 * @param  [in] size size of data
 * @retval false if filed
 */
bool microphone_write_to_wav (FIL* fp, uint8_t *data, uint16_t size);

#if 0 // Microphone test
/**
 * @brief  Microphone just for test
 * @param  [in] argc nullptr
 * @param  [in] argv nullptr
 * @retval 0
 */
int mic_main(int argc, char *argv[]);
#endif // End Microphone test

#endif // MICROPHONE_H
