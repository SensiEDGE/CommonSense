/**
 ******************************************************************************
 * @file    Uart.h
 * @date    14 Octpber 2022
 * @brief   Uart driver
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

#ifndef UART_H
#define UART_H

#include "stdint.h"

#define UART_RX_BUFER_SIZE 1024

typedef void (*callback_func) (char* arg) ;

/**
 * @brief  UART init
 * @param  None
 * @retval None
 */
void uart_init(void);

/**
 * @brief  clear UART rx buffer
 * @param  none
 * @retval none
 */
void uart_clr_rx_buf (void);

/**
 * @brief  UART activity should be called with frequency more than byterate
 * @param  None
 * @retval how many bytes alredyy received
 */
uint16_t uart_act(void);

/**
 * @brief  UART get received data
 * @param  [out] data poitner to get data
 * @retval how many bytes received
 */
uint16_t uart_get_data(char* data);

/**
 * @brief  UART send data
 * @param  [in] data poitner to data
 * @param  [in] size data size, if 0 for while 0 in data
 * @retval none
 */
void uart_send_data(char* data, uint16_t size);

/**
 * @brief  UART attaching callback aftre some synbol received
 * @param  [in] symbol symbol with compare
 * @param  [in] func pointer to callback function and sent RX pnt to it
 * @retval none
 */
void uart_attach_symbol_callback (char symbol, callback_func func);

/**
 * @brief  UART activity should be called with frequency more than byterate
 *         calling callback_func if attached
 * @param  None
 * @retval None
 */
void uart_act_callback(void);

#endif // UART_H
