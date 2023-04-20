/**
 ******************************************************************************
 * @file    Max7317.cpp
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

#include "Max7317.h"
#include "arch/board/board.h"
#include "../../Drivers/CXD5602/Spi/Spi.h"
#include "cxd56_gpio.h"
#include "cxd56_pinconfig.h"
#include "../../../Appdefines.h"
#include "../../../spresense-exported-sdk/nuttx/include/unistd.h"

#define MAX7371_SPI_MODE SPI_MODE0

/**
 * @brief  MAX7317 SPI init
 * @param  None
 * @retval None
 */
static void max7317_spi_init(void) {
    spi_init();
    set_spi_mode(MAX7371_SPI_MODE);
}

/**
 * @brief  Read MAX7317 dara from reg via SPI 
 * @param  [in] reg register addres
 * @retval register data
 */
static uint16_t max7317_spi_read_reg(uint8_t reg) {
    set_spi_mode(MAX7371_SPI_MODE);
    cxd56_gpio_write(MAX7317_CS_PIN, false);
    reg |= 0b10000000;
    uint16_t addr_data = reg << 8;
    spi_send_16(addr_data);
    cxd56_gpio_write(MAX7317_CS_PIN, true);
    usleep(10);
    cxd56_gpio_write(MAX7317_CS_PIN, false);
    uint16_t data = spi_send_16(0);
    cxd56_gpio_write(MAX7317_CS_PIN, true);
    return data;
}

/**
 * @brief  Write MAX7317 reg via SPI 
 * @param  [in] reg register addres
 * @param  [in] data for write
 * @retval None
 */
void max7317_spi_write_reg(uint8_t reg, uint8_t data) {
    set_spi_mode(MAX7371_SPI_MODE);
    cxd56_gpio_write(MAX7317_CS_PIN, false);
    reg &= 0b01111111;
    uint16_t addr_data = 0;
    addr_data = (reg << 8) | data;
    spi_send_16(addr_data);
    cxd56_gpio_write(MAX7317_CS_PIN, true);
}

/**
 * @brief  MAX7317 init
 * @param  None
 * @retval None
 */
void max7317_init(void) {
    static bool isInit = false;
    if (isInit == false) {
        isInit = true;
        PINCONF_SET(MAX7317_CS_PIN, PINCONF_MODE0, PINCONF_INPUT_DISABLE, PINCONF_DRIVE_HIGH, PINCONF_BUSKEEPER);
        cxd56_gpio_write(MAX7317_CS_PIN, true);
        usleep(1000);
        spi_init();
        max7317_set_pins(0xffff);
    }
}

/**
 * @brief  MAX7317 set GPO pin
 * @param  [in] pinNum set GPO Pin pin number
 * @retval None
 */
void max7317_set_pin(uint8_t pin_num) {
    if (pin_num > MAX7317_MAX_PIN) {
        return;
    }
    max7317_spi_write_reg(pin_num, 0x01);
}

/**
 * @brief  MAX7317 reset GPO pin
 * @param  [in] pinNum reset GPO pin number
 * @retval None
 */
void max7317_reset_pin(uint8_t pin_num) {
    if (pin_num > MAX7317_MAX_PIN) {
        return;
    }
    max7317_spi_write_reg(pin_num, 0);
}

/**
 * @brief  MAX7317 Set pins
 * @param  [in] pinNum set GPO Pins
 * @retval None
 */
void max7317_set_pins(uint16_t pins) {
    for (uint8_t i = 0; i <= MAX7317_MAX_PIN; i++) {
        if ((pins & (1 << i)) != 0){
            max7317_set_pin(i);
        } else {
            max7317_reset_pin(i);
        }
    }
}

/**
 * @brief  MAX7317 get GPI pins
 * @param  None
 * @retval Pin get GPI status
 */
uint16_t max7317_get_pins(void) {
    uint16_t a = 0;
    a = max7317_spi_read_reg(0x0e);
    a|= ((uint16_t) max7317_spi_read_reg(0x0f) << 7);
    return a;
}
