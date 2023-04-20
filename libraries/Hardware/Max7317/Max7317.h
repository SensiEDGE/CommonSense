/**
 ******************************************************************************
 * @file    Max7317.h
 * @date    24 March 2022
 * @brief   Driver for MAX7317
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

#ifndef MAX7317_H
#define MAX7317_H

#include <stdint.h>

#define MAX7317_MAX_PIN    9

/**
 * @brief  MAX7317 init
 * @param  None
 * @retval None
 */
void max7317_init(void);

/**
 * @brief  MAX7317 set GPO pin
 * @param  [in] pinNum set GPO Pin pin number
 * @retval None
 */
void max7317_set_pin(uint8_t pin_num);

/**
 * @brief  MAX7317 reset GPO pin
 * @param  [in] pinNum reset GPO pin number
 * @retval None
 */
void max7317_reset_pin(uint8_t pin_num);

/**
 * @brief  MAX7317 Set pins
 * @param  [in] pinNum set GPO Pins
 * @retval None
 */
void max7317_set_pins(uint16_t pins);

/**
 * @brief  MAX7317 get GPI pins
 * @param  None
 * @retval Pin get GPI status
 */
uint16_t max7317_get_pins(void);

#endif // MAX7317_H
