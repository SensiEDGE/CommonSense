/**
 ******************************************************************************
 * @file    Spi.cpp
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

#include "Spi.h"
#include "arch/board/board.h"
#include "../../../../Appdefines.h"
#include "cxd56_gpio.h"
#include <nuttx/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <debug.h>
#include <nuttx/spi/spi_transfer.h>
#include <sys/types.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include <nuttx/kmalloc.h>
#include <nuttx/fs/fs.h>
#include <nuttx/semaphore.h>
#include <sdk/config.h>


#include <nuttx/compiler.h>
#include <nuttx/arch.h>
#include <arch/cxd56xx/irq.h>
#include <cxd56_spi.h>
#include <chip/hardware/cxd5602_memorymap.h>
#include <chip/cxd56_spi.h>
#include <cxd56_clock.h>
#include <nuttx/spi/spi.h>

#define SPIDEV_PORT_5     (5)

#define MSBFIRST 0
#define LSBFIRST 1

FAR struct spi_dev_s* spi_dev = 0;
static uint32_t spi_base_clock = 0;
static uint8_t spi_bit_order = MSBFIRST;
static spi_mode_t spi_corrent_mode = SPI_MODE3;

static void spi_gpio_init(void) {
    PINCONF(PIN_SPI5_SCK, 1, 0, 1, 0);
    PINCONF(PIN_SPI5_MOSI, 1, 0, 1, 0);
    PINCONF(PIN_SPI5_MISO, 1, 1, 1, 0);
}

void spi_init(void) {
    static bool isInit = false;
    if (isInit == false) {
        isInit = true;
        spi_dev = cxd56_spibus_initialize(SPIDEV_PORT_5);
        if (!spi_dev) {
            printf("Failed to initialize SPI bus on port %d!\n", SPIDEV_PORT_5);
            return;
        }

        spi_base_clock = cxd56_get_spi_baseclock(SPIDEV_PORT_5);
        spi_bit_order = MSBFIRST;

        set_spi_mode(spi_corrent_mode);
        SPI_SETBITS(spi_dev, 8);
        SPI_SETFREQUENCY(spi_dev, 4000000);
    }
}

uint8_t spi_send(uint8_t data) {
    uint8_t received = 0;
    SPI_SETBITS(spi_dev, 8);
    SPI_EXCHANGE(spi_dev, (void*)(&data), (void*)(&received), 1);

    return received;
}

uint16_t spi_send_16(uint16_t data) {
    uint16_t received = 0;
    SPI_SETBITS(spi_dev, 16);
    SPI_EXCHANGE(spi_dev, (void*)(&data), (void*)(&received), 1);

    return received;
}

void set_spi_mode(spi_mode_t spi_mode) {
    switch (spi_mode) {
        case SPI_MODE0:
        SPI_SETMODE(spi_dev, SPIDEV_MODE0);
        spi_corrent_mode = SPI_MODE0;
        break;
        case SPI_MODE1:
        SPI_SETMODE(spi_dev, SPIDEV_MODE1);
        spi_corrent_mode = SPI_MODE1;
        break;
        case SPI_MODE2:
        SPI_SETMODE(spi_dev, SPIDEV_MODE2);
        spi_corrent_mode = SPI_MODE2;
        break;
        case SPI_MODE3:
        SPI_SETMODE(spi_dev, SPIDEV_MODE3);
        spi_corrent_mode = SPI_MODE3;
        break;
        default:
        break;
    }
}

spi_mode_t get_spi_mode(void) {
    return spi_corrent_mode;
}

void spi_set_speed(uint32_t speed) {
    SPI_SETFREQUENCY(spi_dev, speed);
}
