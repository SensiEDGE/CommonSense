/**
 ******************************************************************************
 * @file    Pca9538.cpp
 * @date    26 August 2022
 * @brief   PCA9538 driver
 ******************************************************************************
 *
 * COPYRIGHT(c) 2022 Droid-Technologies LLC
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of Droid-Technologies LLC nor the names of its contributors may
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

#ifndef PCA9538_H
#define PCA9538_H

#include <stdint.h>



#define PCA9538_MAX_PIN    8

/**
 * @brief  PCA9538 init
 * @param  None
 * @retval None
 */
void pca9538_init(void);

/**
 * @brief  PCA9538 set GPO pin
 * @param  [in] pinNum set GPO Pin pin number
 * @retval None
 */
void pca9538_set_pin(uint8_t pin_num);

/**
 * @brief  PCA9538 reset GPO pin
 * @param  [in] pinNum reset GPO pin number
 * @retval None
 */
void pca9538_reset_pin(uint8_t pin_num);

/**
 * @brief  PCA9538 Set pins
 * @param  [in] pinNum set GPO Pins
 * @retval None
 */
void pca9538_set_pins(uint8_t pins);

/**
 * @brief  PCA9538 get GPI pins
 * @param  None
 * @retval Pin get GPI status
 */
uint8_t pca9538_get_pins(void);

/**
 * @brief  PCA9538 get GPIO pins mode
 * @param  None
 * @retval GPIO Pins mask, 0 - output, 1 - input
 */
uint8_t pca9538_get_pins_mode(void);

/**
 * @brief  PCA9538 Set GPIO pins mode
 * @param  [in] pins set GPIO Pins mask, 0 - output, 1 - input
 * @retval None
 */
void pca9538_set_pins_mode(uint8_t pins);

#endif // PCA9538_H