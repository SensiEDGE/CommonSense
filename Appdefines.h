/**
 ******************************************************************************
 * @file    Appdefines.h
 * @date    23 March 2022
 * @brief   App defines
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

#ifndef APPDEFINES_H
#define APPDEFINES_H

#if 0 // Port expander is MAX7317
#define PORT_EXPANDER_MAX7317
#else // Port expander is PCA9538
#define PORT_EXPANDER_PCA9538
#endif


#if defined (PORT_EXPANDER_MAX7317)

#define LED_GREEN_MAX7317_PIN 7
#define LED_RED_MAX7317_PIN 6

#define BUTTON_MAX7317_PIN 2

#define VL53L1_MAX7317_PIN 5

#elif defined (PORT_EXPANDER_PCA9538)

#define LED_GREEN_PCA9538_PIN 4
#define LED_RED_PCA9538_PIN 5

#define BUTTON_PCA9538_PIN 0

#define VL53L1_PCA9538_PIN 1

#else

#error "Port expander doesn't selected"

#endif 

#define MAX7317_CS_PIN PIN_EMMC_DATA3
#define LIS2MDL_INT_PIN PIN_EMMC_DATA3
#define HTS221_DRDY_PIN PIN_I2S0_DATA_OUT

#endif // APPDEFINES_H
