/**
 ******************************************************************************
 * @file    Spi.h
 * @date    25 March 2022
 * @brief   Spi driver
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

#ifndef SPI_H
#define SPI_H

#include <stdint.h>

typedef enum {
    SPI_MODE0,
    SPI_MODE1,
    SPI_MODE2,
    SPI_MODE3
} spi_mode_t;

/**
 * @brief  SPI init
 * @param  None
 * @retval None
 */
void spi_init(void);

/**
 * @brief  Send and receive data via SPI
 * @param  [in] data to send
 * @retval Receive data
 */
uint8_t spi_send(uint8_t data);

/**
 * @brief  Send and receive data via SPI 16 bits
 * @param  [in] data to send
 * @retval Receive data
 */
uint16_t spi_send_16(uint16_t data);

/**
 * @brief  Set SPI mode
 * @param  [in] spi_mode spi mode from spi_mode_t
 * @retval Receive data
 */
void set_spi_mode(spi_mode_t spi_mode);

/**
 * @brief  Get current SPI mode
 * @param  none
 * @retval current SPI mode from spi_mode_t
 */
spi_mode_t get_spi_mode(void);

/**
 * @brief  Set SPI speed Hz
 * @param  none
 * @retval current SPI mode from spi_mode_t
 */
void spi_set_speed(uint32_t speed);


#endif // SPI_H
