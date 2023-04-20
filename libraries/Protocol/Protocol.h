/**
 ******************************************************************************
 * @file    Protocol.cpp
 * @date    17 Octpber 2022
 * @brief   Protocol routine
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

#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

/**
 * @brief  Get protocol LED control time
 * @param  none
 * @retval LED control time ns
 */
uint64_t get_protocol_led_control_time_ns(void);

/**
 * @brief  Reset protocol LED control time
 * @param  none
 * @retval none
 */
void reset_protocol_led_control_time(void);

/**
 * @brief  CRC16 calculation
 * @param  [in] pc_block poitner to data
 * @param  [in] len data size
 * @retval 16 bit CRC16
 */
uint16_t culate_crc16(char *pc_block, uint16_t len);

/**
 * @brief  Protocol init
 * @param  None
 * @retval None
 */
void protocol_init(void);

/**
 * @brief  Protocol activity should be called with frequency more than byterate
 * @param  None
 * @retval None
 */
void protocol_act(void);

/**
 * @brief  Protocol send data calc crc16 and send it
 * @param  [in] data ptr to data
 * @param  [in] size data size
 * @retval None
 */
void protocol_send_data (char* data, uint16_t size);

/**
 * @brief  Protocol check if data record should be stoped
 * @param  None
 * @retval Stop status
 */
bool protocol_stop(void);

/**
 * @brief  Protocol reset stop status
 * @param  None
 * @retval None
 */
void reset_stop(void);


#endif // PROTOCOL_H
