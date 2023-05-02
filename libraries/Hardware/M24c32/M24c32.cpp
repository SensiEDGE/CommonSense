/**
 ******************************************************************************
 * @file    M24c32.cpp
 * @date    21 Octpber 2022
 * @brief   M24C32 driver
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

#include "M24c32.h"
#include "../../Drivers/CXD5602/I2c/I2c.h"
#include <stdint.h>

#define M42C32_I2C_ADDRES_MEMORY 0xA0 >> 1
#define M42C32_I2C_ADDRES_ID 0xB0 >> 1

//int     usleep(useconds_t usec);

/**
 * @brief  M24C32 I2C write data
 * @param  [In] addres Addres
 * @param  [Out] data pnt to data
 * @param  [In] size data size
 * @retval none
 */
static void m24c32_i2c_write (uint16_t addres, uint8_t* data, uint16_t size) {
    if ((data == nullptr) || (size == 0)) {
        return;
    }
    i2c_write_regs_16addr(M42C32_I2C_ADDRES_MEMORY, addres, data, size);
}

/**
 * @brief  M24C32 I2C read data
 * @param  [In] addres Addres
 * @param  [Out] data pnt to data
 * @param  [In] size data size
 * @retval none
 */
static void m24c32_i2c_read (uint16_t addres, uint8_t* data, uint16_t size) {
    if ((data == nullptr) || (size == 0)) {
        return;
    }
    i2c_read_regs_16addr(M42C32_I2C_ADDRES_MEMORY, addres, data, size);
}

/**
 * @brief  check if M24C32 ready
 * @param  None
 * @retval false if not ready
 */
static bool is_m24c32_ready (void) {
    return i2c_is_bevice_present(M42C32_I2C_ADDRES_MEMORY);
}

/**
 * @brief  M24C32 init
 * @param  None
 * @retval None
 */
void m24c32_init(void) {
    i2c_init();
}

/**
 * @brief  M24C32 read byte
 * @param  [In] addres Addres
 * @retval readed byte
 */
uint8_t m24c32_read_byte(uint16_t addres) {
    uint8_t data = 0;
    while (is_m24c32_ready() == false);
    m24c32_i2c_read(addres, &data, 1);
    return data;
}

/**
 * @brief  M24C32 write byte
 * @param  [In] addres Addres
 * @param  [In] data byte to write
 * @retval none
 */
void m24c32_write_byte(uint16_t addres, uint8_t data) {
    while (is_m24c32_ready() == false);
    m24c32_i2c_write(addres, &data, 1);
    while (is_m24c32_ready() == false);
}

/**
 * @brief  M24C32 read page
 * @param  [In] addres Addres b16-b5
 * @param  [Out] data pnt to data
 * @retval none
 */
void m24c32_read_page(uint16_t addres, uint8_t* data) {
    if (data == nullptr) {
        return;
    }
    addres &= 0b1111111111100000;
    m24c32_i2c_read(addres, data, M24C32_PAGE_SIIZE);
}

/**
 * @brief  M24C32 write page
 * @param  [In] addres Addres b16-b5
 * @param  [In] data pnt to data
 * @retval none
 */
void m24c32_write_page(uint16_t addres, uint8_t* data) {
    if (data == nullptr) {
        return;
    }
    addres &= 0b1111111111100000;
    m24c32_i2c_write(addres, data, M24C32_PAGE_SIIZE);
}

