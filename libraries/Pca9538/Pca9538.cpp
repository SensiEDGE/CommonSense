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

#include "Pca9538.h"
#include "Appdefines.h"
#include "../I2c/I2c.h"

#define PCA9538_I2C_ADDR    0xE0 >> 1

#define PCA9538_INPUT_PORT_REG  0
#define PCA9538_OUTPUT_PORT_REG  1
#define PCA9538_POLARITY_INVERSION_REG  2
#define PCA9538_CONFIG_REG  3

void pca9538_init(void) {
    i2c_init();
}

/**
 * @brief  Read PCA9538 dara from reg via I2C 
 * @param  [in] reg register addres
 * @retval register data
 */
static uint8_t pca9538_read_reg(uint8_t reg) {
    uint8_t data = 0;
    i2c_read_reg(PCA9538_I2C_ADDR, reg, &data);
    return data;
}

/**
 * @brief  Write PCA9538 reg via I2C
 * @param  [in] reg register addres
 * @param  [in] data for write
 * @retval None
 */
void pca9538_write_reg(uint8_t reg, uint8_t data) {
    i2c_write_reg(PCA9538_I2C_ADDR, reg, data);
}

void pca9538_set_pin(uint8_t pin_num) {
    if (pin_num > PCA9538_MAX_PIN) {
        return;
    }
    uint8_t outputs = pca9538_read_reg(PCA9538_OUTPUT_PORT_REG);
    pca9538_write_reg(PCA9538_OUTPUT_PORT_REG, outputs | pin_num);
}

void pca9538_reset_pin(uint8_t pin_num) {
    if (pin_num > PCA9538_MAX_PIN) {
        return;
    }
    
    uint8_t outputs = pca9538_read_reg(PCA9538_OUTPUT_PORT_REG);
    pca9538_write_reg(PCA9538_OUTPUT_PORT_REG, outputs & (~pin_num));
}

void pca9538_set_pins(uint8_t pins) {
    for (uint8_t i = 0; i <= PCA9538_MAX_PIN; i++) {
        if ((pins & (1 << i)) != 0){
            pca9538_set_pin(i);
        } else {
            pca9538_reset_pin(i);
        }
    }
}

uint8_t pca9538_get_pins(void) {
    return pca9538_read_reg(PCA9538_INPUT_PORT_REG);
}

uint8_t pca9538_get_pins_mode(void) {
    return pca9538_read_reg(PCA9538_CONFIG_REG);
}

void pca9538_set_pins_mode(uint8_t pins) {
    return pca9538_write_reg(PCA9538_CONFIG_REG, pins);
}
