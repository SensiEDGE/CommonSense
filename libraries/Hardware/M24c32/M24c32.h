/**
 ******************************************************************************
 * @file    M24c32.h
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

#ifndef M24C32_H
#define M24C32_H

#include <stdint.h>

#define M24C32_PAGE_SIIZE 32

/**
 * @brief  M24C32 init
 * @param  None
 * @retval None
 */
void m24c32_init(void);

/**
 * @brief  M24C32 read byte
 * @param  [In] addres Addres
 * @retval readed byte
 */
uint8_t m24c32_read_byte(uint16_t addres);

/**
 * @brief  M24C32 write byte
 * @param  [In] addres Addres
 * @param  [In] data byte to write
 * @retval none
 */
void m24c32_write_byte(uint16_t addres, uint8_t data);

/**
 * @brief  M24C32 read page
 * @param  [In] addres Addres b16-b5
 * @param  [Out] data pnt to data
 * @retval none
 */
void m24c32_read_page(uint16_t addres, uint8_t* data);

/**
 * @brief  M24C32 write page
 * @param  [In] addres Addres b16-b5
 * @param  [In] data pnt to data
 * @retval none
 */
void m24c32_write_page(uint16_t addres, uint8_t* data);



#endif // M24C32_H
