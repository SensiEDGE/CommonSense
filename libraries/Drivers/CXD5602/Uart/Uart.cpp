/**
 ******************************************************************************
 * @file    Uart.cpp
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

#include "Uart.h"
#include <cxd56_uart.h>
#include <../../include/nuttx/serial/serial.h>
#include <string.h>
#include <stdio.h>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

/* Registers read/write macro's ------------------------------------------- */
#define putreg32(v,a)   (*(volatile uint32_t *)(a) = (v))
#define getreg32(a)     (*(volatile uint32_t *)(a))
#define CONSOLE_BASE    CXD56_UART1_BASE

static char uart_rx_buf [UART_RX_BUFER_SIZE];
uint16_t uart_rx_pnt = 0;

static char callback_char = 0;
static callback_func callback_function = nullptr;

static int fd = 0;

/**
 * @brief  get non zero char
 * @param  none
 * @retval received char
 */
static char uart_getchar(void)
{
    uint32_t reg = 0;
    reg = getreg32(CONSOLE_BASE + CXD56_UART_FR);
    if ((UART_FR_RXFE & reg) && !(UART_FR_RXFF & reg)) {
        return 0;
    }
    else {
        return (char)getreg32(CONSOLE_BASE + CXD56_UART_DR);
    }
}

/**
 * @brief  Check if some byte received
 * @param  [out] data char from console
 * @retval true is some data received
 */
static bool uart_check_and_get_char (char* data) {
    uint32_t reg = 0;
    reg = getreg32(CONSOLE_BASE + CXD56_UART_FR);
    if ((UART_FR_RXFE & reg) && !(UART_FR_RXFF & reg)) {
        *data = 0;
        return false;
    } else {
        *data = (char)getreg32(CONSOLE_BASE + CXD56_UART_DR);
        return true; 
    }
}

/**
 * @brief  Send some byte via UART
 * @param  [in] data char from console
 * @retval true is some data received
 */
static void uart_putchar(char byte)
{
    while ((getreg32(CONSOLE_BASE + CXD56_UART_FR) & UART_FLAG_TXFF));
    putreg32((uint32_t)byte, CONSOLE_BASE + CXD56_UART_DR);
}



void uart_init(void) {
#if USE_RX_GET_CHAR    
    putreg32((uint32_t)0x0, CONSOLE_BASE + CXD56_UART_IMSC); // IRQ is attached to NUTTX handler. Disable to you use Uart directly.
#else
    fd = open("/dev/ttyS0", O_RDWR);
    if (fd < 0) {
        printf("/dev/ttyS0 open FALSE\r\n");
    }
#endif
    uart_clr_rx_buf();
}

void uart_clr_rx_buf (void) {
    memset(uart_rx_buf, 0, UART_RX_BUFER_SIZE);
    uart_rx_pnt = 0;
}

uint16_t uart_act(void) {
#if USE_RX_GET_CHAR
    char data = 0;
    if (uart_check_and_get_char(&data) != false) {
        if (uart_rx_pnt < UART_RX_BUFER_SIZE){
            uart_rx_buf[uart_rx_pnt] = data;
        } else {
            uart_clr_rx_buf();
            uart_rx_buf[uart_rx_pnt] = data;
        }
        uart_rx_pnt++;
    }
    return uart_rx_pnt;
#else
    char data = 0;
    if (fd < 0) {
        return 0;
    }
    uint16_t count = 0;
    ioctl(fd, FIONREAD, (long unsigned int)&count);
    for (uint16_t i = 0; i < count; i++) {
        int ret = read(fd, &data, sizeof(data));
        if (ret > 0) {
            if (uart_rx_pnt < UART_RX_BUFER_SIZE) {
                uart_rx_buf[uart_rx_pnt] = data;
            } else {
                uart_clr_rx_buf();
                uart_rx_buf[uart_rx_pnt] = data;
            }
            uart_rx_pnt++;
        } else {
            break;
        }
    }
    return uart_rx_pnt;
#endif
}

uint16_t uart_get_data(char* data) {
    memcpy(data, uart_rx_buf, uart_rx_pnt);
    uint16_t size = uart_rx_pnt;
    uart_clr_rx_buf();
    return size;
}

void uart_send_data(char* data, uint16_t size) {
    if (size!=0){
        for (uint16_t i = 0; i < size; i++){
            uart_putchar(data[i]);
        }
    }else{
        for (uint16_t i = 0; data[i] != 0;i++){
            uart_putchar(data[i]);
        }
    }
}

void uart_attach_symbol_callback (char symbol, callback_func func) {
    callback_char = symbol;
    callback_function = func;
}

void uart_act_callback(void) {
#if USE_RX_GET_CHAR
    char data = 0;
    if (uart_check_and_get_char(&data) != false) {
        if (uart_rx_pnt < UART_RX_BUFER_SIZE){
            uart_rx_buf[uart_rx_pnt] = data;
        } else {
            uart_clr_rx_buf();
            uart_rx_buf[uart_rx_pnt] = data;
        }
        if (callback_function != nullptr) {
            if (callback_char == uart_rx_buf[uart_rx_pnt]) {
                callback_function(uart_rx_buf);
            }
        }
        uart_rx_pnt++;
    }
#else
    char data = 0;
    if (fd < 0) {
        return;
    }
    uint16_t count = 0;
    ioctl(fd, FIONREAD, (long unsigned int)&count);
    for (uint16_t i = 0; i < count; i++) {
        int ret = read(fd, &data, sizeof(data));
        if (ret > 0) {
            if (uart_rx_pnt < UART_RX_BUFER_SIZE){
                uart_rx_buf[uart_rx_pnt] = data;
            } else {
                uart_clr_rx_buf();
                uart_rx_buf[uart_rx_pnt] = data;
            }
            if (callback_function != nullptr) {
                if (callback_char == uart_rx_buf[uart_rx_pnt]) {
                    callback_function(uart_rx_buf);
                }
            }
            uart_rx_pnt++;
        } else {
            break;
        }
    }
#endif
}
