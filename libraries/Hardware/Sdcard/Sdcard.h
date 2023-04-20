/**
 ******************************************************************************
 * @file    Sdcard.h
 * @date    29 March 2022
 * @brief   Sdcard driver
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

/* vim: set ai et ts=4 sw=4: */
#ifndef __SDCARD_H__
#define __SDCARD_H__

#include <stdint.h>

#define SD_CARD_CS_PIN PIN_SPI5_CS_X

/**
 * @brief  Unselect CS pin of SD card, call it before initializing any SPI devices
 * @param  None
 * @retval None
 */
void sdcard_unselect(void);

/**
 * @brief  SD card init
 * @param  None
 * @retval 0 on success, < 0 on failure
 */
int sdcard_init(void);

/**
 * @brief  Get block numbers of SD card
 * @param  [out] num block numbers
 * @retval 0 on success, < 0 on failure
 */
int sdcard_get_blocks_number(uint32_t* num);

/**
 * @brief  Read data from SD card block
 * @param  [in] block_num block number
 * @param  [out] buff pointer for data
 * @retval 0 on success, < 0 on failure
 */
int sdcard_read_single_block(uint32_t block_num, uint8_t* buff);

/**
 * @brief  Write data from SD card block
 * @param  [in] block_num block number
 * @param  [out] buff pointer with data
 * @retval 0 on success, < 0 on failure
 */
int sdcard_write_single_block(uint32_t block_num, const uint8_t* buff);


// Read Multiple Blocks

/**
 * @brief  Start reading of moltiple blocks
 * @param  [in] block_num block number
 * @retval 0 on success, < 0 on failure
 */
int sdcard_read_begin(uint32_t block_num);

/**
 * @brief  Read one or next of multiple blocks 
 * @param  [out] buff pointer with data
 * @retval 0 on success, < 0 on failure
 */
int sdcard_read_data(uint8_t* buff);

/**
 * @brief  End reading of multiple blocks
 * @param  None
 * @retval 0 on success, < 0 on failure
 */
int sdcard_read_end(void);


// Write Multiple Blocks

/**
 * @brief  Start writing of moltiple blocks
 * @param  [in] block_num block number
 * @retval 0 on success, < 0 on failure
 */
int sdcard_write_begin(uint32_t block_num);

/**
 * @brief  Write one or next of multiple blocks 
 * @param  [out] buff pointer with data
 * @retval 0 on success, < 0 on failure
 */
int sdcard_write_data(const uint8_t* buff);

/**
 * @brief  End writing of multiple blocks
 * @param  None
 * @retval 0 on success, < 0 on failure
 */
int sdcard_write_end(void);

#endif // __SDCARD_H__
