/**
 ******************************************************************************
 * @file    Sdcard.cpp
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

#include "Sdcard.h"
#include "../../Drivers/CXD5602/Spi/Spi.h"
#include "cxd56_gpio.h"
#include "cxd56_pinconfig.h"
#include <stdio.h>


#define HAL_MAX_DELAY 0

int usleep(useconds_t usec);

static void sdcard_cs_init(void) {
    PINCONF_SET(SD_CARD_CS_PIN, PINCONF_MODE0, PINCONF_INPUT_DISABLE, PINCONF_DRIVE_HIGH, PINCONF_BUSKEEPER);
}

static void sdcard_select() {
    cxd56_gpio_write(SD_CARD_CS_PIN, false);
}

void sdcard_unselect() {
    cxd56_gpio_write(SD_CARD_CS_PIN, true);
}

static void HAL_SPI_TransmitReceive (void *ptr, uint8_t *tx, uint8_t *rx, uint16_t size, uint32_t delay)
{
    for (uint16_t i = 0; i < size; i++){
        if (rx != nullptr) {
            rx [i] = spi_send(tx[i]);
        } else {
            spi_send(tx[i]);
        }
    }
}

static void HAL_SPI_Transmit (void *ptr, uint8_t *tx, uint16_t size, uint32_t delay)
{
    for (uint16_t i = 0; i < size; i++){
        spi_send(tx[i]);
    }
}

/*
R1: 0abcdefg
     ||||||`- 1th bit (g): card is in idle state
     |||||`-- 2th bit (f): erase sequence cleared
     ||||`--- 3th bit (e): illigal command detected
     |||`---- 4th bit (d): crc check error
     ||`----- 5th bit (c): error in the sequence of erase commands
     |`------ 6th bit (b): misaligned addres used in command
     `------- 7th bit (a): command argument outside allowed range
             (8th bit is always zero)
*/
static uint8_t sdcard_read_r1() {
    uint8_t r1;
    // make sure FF is transmitted during receive
    uint8_t tx = 0xFF;
    for(;;) {
        HAL_SPI_TransmitReceive(nullptr, &tx, &r1, sizeof(r1), HAL_MAX_DELAY);
        if((r1 & 0x80) == 0) // 8th bit alwyas zero, r1 recevied
            break;
    }
    return r1;
}

// data token for CMD9, CMD17, CMD18 and CMD24 are the same
#define DATA_TOKEN_CMD9  0xFE
#define DATA_TOKEN_CMD17 0xFE
#define DATA_TOKEN_CMD18 0xFE
#define DATA_TOKEN_CMD24 0xFE
#define DATA_TOKEN_CMD25 0xFC

static int sdcard_wait_data_token(uint8_t token) {
    uint8_t fb;
    // make sure FF is transmitted during receive
    uint8_t tx = 0xFF;
    for(;;) {
        HAL_SPI_TransmitReceive(nullptr, &tx, &fb, sizeof(fb), HAL_MAX_DELAY);
        if(fb == token)
            break;

        if(fb != 0xFF)
            return -1;
    }
    return 0;
}

static int sdcard_read_bytes(uint8_t* buff, size_t buff_size) {
    // make sure FF is transmitted during receive
    uint8_t tx = 0xFF;
    while(buff_size > 0) {
        HAL_SPI_TransmitReceive(nullptr, &tx, buff, 1, HAL_MAX_DELAY);
        buff++;
        buff_size--;
    }

    return 0;
}

static int sdcard_wait_not_busy() {
    uint8_t busy;
    do {
        if(sdcard_read_bytes(&busy, sizeof(busy)) < 0) {
            return -1;
        }
    } while(busy != 0xFF);

    return 0;
}
 
int sdcard_init() 
{
    sdcard_cs_init();
    sdcard_unselect();
    spi_init();
    spi_set_speed(250);
    usleep(10 * 1000);
    /*
    Step 1.

    Set DI and CS high and apply 74 or more clock pulses to SCLK. Without this
    step under certain circumstances SD-card will not work. For instance, when
    multiple SPI devices are sharing the same bus (i.e. MISO, MOSI, CS).
    */
    uint8_t high = 0xFF;
    for(int i = 0; i < 10; i++) { // 80 clock pulses
        HAL_SPI_Transmit(nullptr, &high, sizeof(high), HAL_MAX_DELAY);
    }
    PINCONF(PIN_SPI5_MOSI, 1, 0, 1, 0);
    sdcard_select();

    /*
    Step 2.
    
    Send CMD0 (GO_IDLE_STATE): Reset the SD card.
    */
    usleep(10 * 1000);
 #if 0 // Doesn't need
    if(sdcard_wait_not_busy() < 0) { // keep this!
        sdcard_unselect();
        return -11;
    }
#endif 
    uint16_t count = 0;
    while (1) {
        {
            static const uint8_t cmd[] =
                { 0x40 | 0x00 /* CMD0 */, 0x00, 0x00, 0x00, 0x00 /* ARG = 0 */, (0x4A << 1) | 1 /* CRC7 + end bit */ };
            HAL_SPI_Transmit(nullptr, (uint8_t*)cmd, sizeof(cmd), HAL_MAX_DELAY);
        }

        uint8_t r1 = sdcard_read_r1();
        if(r1 != 0x01) {
            if (count > 20) {
                sdcard_unselect();
                return -12;
            }
            count++;
        }
        if(r1 == 0x01) {
            break;
        }
    }
    /*
    Step 3.

    After the card enters idle state with a CMD0, send a CMD8 with argument of
    0x000001AA and correct CRC prior to initialization process. If the CMD8 is
    rejected with illigal command error (0x05), the card is SDC version 1 or
    MMC version 3. If accepted, R7 response (R1(0x01) + 32-bit return value)
    will be returned. The lower 12 bits in the return value 0x1AA means that
    the card is SDC version 2 and it can work at voltage range of 2.7 to 3.6
    volts. If not the case, the card should be rejected.
    */
    if(sdcard_wait_not_busy() < 0) { // keep this!
        sdcard_unselect();
        return -13;
    }

    {
        static const uint8_t cmd[] =
            { 0x40 | 0x08 /* CMD8 */, 0x00, 0x00, 0x01, 0xAA /* ARG */, (0x43 << 1) | 1 /* CRC7 + end bit */ };
        HAL_SPI_Transmit(nullptr, (uint8_t*)cmd, sizeof(cmd), HAL_MAX_DELAY);
    }

    if(sdcard_read_r1() != 0x01) {
        sdcard_unselect();
        return -2; // not an SDHC/SDXC card (i.e. SDSC, not supported)
    }

    {
        uint8_t resp[4];
        if(sdcard_read_bytes(resp, sizeof(resp)) < 0) {
            sdcard_unselect();
            return -3;
        }

        if(((resp[2] & 0x01) != 1) || (resp[3] != 0xAA)) {
            sdcard_unselect();
            return -4;
        }
    }

    /*
    Step 4.

    And then initiate initialization with ACMD41 with HCS flag (bit 30).
    */
    for(;;) {
        if(sdcard_wait_not_busy() < 0) { // keep this!
            sdcard_unselect();
            return -14;
        }

        {
            static const uint8_t cmd[] =
                { 0x40 | 0x37 /* CMD55 */, 0x00, 0x00, 0x00, 0x00 /* ARG */, (0x7F << 1) | 1 /* CRC7 + end bit */ };
            HAL_SPI_Transmit(nullptr, (uint8_t*)cmd, sizeof(cmd), HAL_MAX_DELAY);
        }

        if(sdcard_read_r1() != 0x01) {
            sdcard_unselect();
            return -5;
        }

        if(sdcard_wait_not_busy() < 0) { // keep this!
            sdcard_unselect();
            return -15;
        }

        {
            static const uint8_t cmd[] =
                { 0x40 | 0x29 /* ACMD41 */, 0x40, 0x00, 0x00, 0x00 /* ARG */, (0x7F << 1) | 1 /* CRC7 + end bit */ };
            HAL_SPI_Transmit(nullptr, (uint8_t*)cmd, sizeof(cmd), HAL_MAX_DELAY);
        }

        uint8_t r1 = sdcard_read_r1();
        if(r1 == 0x00) {
            break;
        }

        if(r1 != 0x01) {
            sdcard_unselect();
            return -6;
        }
    }

    /*
    Step 5.

    After the initialization completed, read OCR register with CMD58 and check
    CCS flag (bit 30). When it is set, the card is a high-capacity card known
    as SDHC/SDXC.
    */
    if(sdcard_wait_not_busy() < 0) { // keep this!
        sdcard_unselect();
        return -16;
    }

    {
        static const uint8_t cmd[] =
            { 0x40 | 0x3A /* CMD58 */, 0x00, 0x00, 0x00, 0x00 /* ARG */, (0x7F << 1) | 1 /* CRC7 + end bit */ };
        HAL_SPI_Transmit(nullptr, (uint8_t*)cmd, sizeof(cmd), HAL_MAX_DELAY);
    }

    if(sdcard_read_r1() != 0x00) {
        sdcard_unselect();
        return -7;
    }

    {
        uint8_t resp[4];
        if(sdcard_read_bytes(resp, sizeof(resp)) < 0) {
            sdcard_unselect();
            return -8;
        }

        if((resp[0] & 0xC0) != 0xC0) {
            sdcard_unselect();
            return -9;
        }
    }
    spi_set_speed(4000000);
    sdcard_unselect();
    return 0;
}


int sdcard_get_blocks_number(uint32_t* num) {
    uint8_t csd[16];
    uint8_t crc[2];

    sdcard_select();

    if(sdcard_wait_not_busy() < 0) { // keep this!
        sdcard_unselect();
        return -1;
    }

    /* CMD9 (SEND_CSD) command */
    {
        static const uint8_t cmd[] =
            { 0x40 | 0x09 /* CMD9 */, 0x00, 0x00, 0x00, 0x00 /* ARG */, (0x7F << 1) | 1 /* CRC7 + end bit */ };
        HAL_SPI_Transmit(nullptr, (uint8_t*)cmd, sizeof(cmd), HAL_MAX_DELAY);
    }

    if(sdcard_read_r1() != 0x00) {
        sdcard_unselect();
        return -2;
    }

    if(sdcard_wait_data_token(DATA_TOKEN_CMD9) < 0) {
        sdcard_unselect();
        return -3;
    }

    if(sdcard_read_bytes(csd, sizeof(csd)) < 0) {
        sdcard_unselect();
        return -4;
    }

    if(sdcard_read_bytes(crc, sizeof(crc)) < 0) {
        sdcard_unselect();
        return -5;
    }

    sdcard_unselect();

    // first byte is VVxxxxxxxx where VV is csd.version
    if((csd[0] & 0xC0) != 0x40) // csd.version != 1
        return -6;

    uint32_t tmp = csd[7] & 0x3F; // two bits are reserved
    tmp = (tmp << 8) | csd[8];
    tmp = (tmp << 8) | csd[9];
    // Full volume: (C_SIZE+1)*512KByte == (C_SIZE+1)<<19
    // Block size: 512Byte == 1<<9
    // Blocks number: CARD_SIZE/BLOCK_SIZE = (C_SIZE+1)*(1<<19) / (1<<9) = (C_SIZE+1)*(1<<10)
    tmp = (tmp + 1) << 10;
    *num = tmp;

    return 0;
}

int sdcard_read_single_block(uint32_t block_num, uint8_t* buff) {
    uint8_t crc[2];

    sdcard_select();

    if(sdcard_wait_not_busy() < 0) { // keep this!
        sdcard_unselect();
        return -1;
    }

    /* CMD17 (SEND_SINGLE_BLOCK) command */
    uint8_t cmd[] = {
        0x40 | 0x11 /* CMD17 */,
        (block_num >> 24) & 0xFF, /* ARG */
        (block_num >> 16) & 0xFF,
        (block_num >> 8) & 0xFF,
        block_num & 0xFF,
        (0x7F << 1) | 1 /* CRC7 + end bit */
    };
    HAL_SPI_Transmit(nullptr, (uint8_t*)cmd, sizeof(cmd), HAL_MAX_DELAY);

    if(sdcard_read_r1() != 0x00) {
        sdcard_unselect();
        return -2;
    }

    if(sdcard_wait_data_token(DATA_TOKEN_CMD17) < 0) {
        sdcard_unselect();
        return -3;
    }

    if(sdcard_read_bytes(buff, 512) < 0) {
        sdcard_unselect();
        return -4;
    }

    if(sdcard_read_bytes(crc, 2) < 0) {
        sdcard_unselect();
        return -5;
    }

    sdcard_unselect();
    return 0;
}


int sdcard_write_single_block(uint32_t block_num, const uint8_t* buff) {
    sdcard_select();

    if(sdcard_wait_not_busy() < 0) { // keep this!
        sdcard_unselect();
        return -1;
    }

    /* CMD24 (WRITE_BLOCK) command */
    uint8_t cmd[] = {
        0x40 | 0x18 /* CMD24 */,
        (block_num >> 24) & 0xFF, /* ARG */
        (block_num >> 16) & 0xFF,
        (block_num >> 8) & 0xFF,
        block_num & 0xFF,
        (0x7F << 1) | 1 /* CRC7 + end bit */
    };
    HAL_SPI_Transmit(nullptr, (uint8_t*)cmd, sizeof(cmd), HAL_MAX_DELAY);

    if(sdcard_read_r1() != 0x00) {
        sdcard_unselect();
        return -2;
    }

    uint8_t dataToken = DATA_TOKEN_CMD24;
    uint8_t crc[2] = { 0xFF, 0xFF };
    HAL_SPI_Transmit(nullptr, &dataToken, sizeof(dataToken), HAL_MAX_DELAY);
    HAL_SPI_Transmit(nullptr, (uint8_t*)buff, 512, HAL_MAX_DELAY);
    HAL_SPI_Transmit(nullptr, crc, sizeof(crc), HAL_MAX_DELAY);

    /*
        dataResp:
        xxx0abc1
            010 - Data accepted
            101 - Data rejected due to CRC error
            110 - Data rejected due to write error
    */
    uint8_t dataResp;
    sdcard_read_bytes(&dataResp, sizeof(dataResp));
    if((dataResp & 0x1F) != 0x05) { // data rejected
        sdcard_unselect();
        return -3;
    }

    if(sdcard_wait_not_busy() < 0) {
        sdcard_unselect();
        return -4;
    }

    sdcard_unselect();
    return 0;
}

int sdcard_read_begin(uint32_t block_num) {
    sdcard_select();

    if(sdcard_wait_not_busy() < 0) { // keep this!
        sdcard_unselect();
        return -1;
    }

    /* CMD18 (READ_MULTIPLE_BLOCK) command */
    uint8_t cmd[] = {
        0x40 | 0x12 /* CMD18 */,
        (block_num >> 24) & 0xFF, /* ARG */
        (block_num >> 16) & 0xFF,
        (block_num >> 8) & 0xFF,
        block_num & 0xFF,
        (0x7F << 1) | 1 /* CRC7 + end bit */
    };
    HAL_SPI_Transmit(nullptr, (uint8_t*)cmd, sizeof(cmd), HAL_MAX_DELAY);

    if(sdcard_read_r1() != 0x00) {
        sdcard_unselect();
        return -2;
    }

    sdcard_unselect();
    return 0;
}

int sdcard_read_data(uint8_t* buff) {
    uint8_t crc[2];
    sdcard_select();

    if(sdcard_wait_data_token(DATA_TOKEN_CMD18) < 0) {
        sdcard_unselect();
        return -1;
    }

    if(sdcard_read_bytes(buff, 512) < 0) {
        sdcard_unselect();
        return -2;
    }

    if(sdcard_read_bytes(crc, 2) < 0) {
        sdcard_unselect();
        return -3;
    }

    sdcard_unselect();
    return 0;

}

int sdcard_read_end() {
    sdcard_select();

    /* CMD12 (STOP_TRANSMISSION) */
    {
        static const uint8_t cmd[] = { 0x40 | 0x0C /* CMD12 */, 0x00, 0x00, 0x00, 0x00 /* ARG */, (0x7F << 1) | 1 };
        HAL_SPI_Transmit(nullptr, (uint8_t*)cmd, sizeof(cmd), HAL_MAX_DELAY);
    }

    /*
    The received byte immediataly following CMD12 is a stuff byte, it should be
    discarded before receive the response of the CMD12
    */
    uint8_t stuffByte;
    if(sdcard_read_bytes(&stuffByte, sizeof(stuffByte)) < 0) {
        sdcard_unselect();
        return -1;
    }

    if(sdcard_read_r1() != 0x00) {
        sdcard_unselect();
        return -2;
    }
    
    sdcard_unselect();
    return 0;
}


int sdcard_write_begin(uint32_t block_num) {
    sdcard_select();

    if(sdcard_wait_not_busy() < 0) { // keep this!
        sdcard_unselect();
        return -1;
    }

    /* CMD25 (WRITE_MULTIPLE_BLOCK) command */
    uint8_t cmd[] = {
        0x40 | 0x19 /* CMD25 */,
        (block_num >> 24) & 0xFF, /* ARG */
        (block_num >> 16) & 0xFF,
        (block_num >> 8) & 0xFF,
        block_num & 0xFF,
        (0x7F << 1) | 1 /* CRC7 + end bit */
    };
    HAL_SPI_Transmit(nullptr, (uint8_t*)cmd, sizeof(cmd), HAL_MAX_DELAY);

    if(sdcard_read_r1() != 0x00) {
        sdcard_unselect();
        return -2;
    }

    sdcard_unselect();
    return 0;
}

int sdcard_write_data(const uint8_t* buff) {
    sdcard_select();

    uint8_t dataToken = DATA_TOKEN_CMD25;
    uint8_t crc[2] = { 0xFF, 0xFF };
    HAL_SPI_Transmit(nullptr, &dataToken, sizeof(dataToken), HAL_MAX_DELAY);
    HAL_SPI_Transmit(nullptr, (uint8_t*)buff, 512, HAL_MAX_DELAY);
    HAL_SPI_Transmit(nullptr, crc, sizeof(crc), HAL_MAX_DELAY);

    /*
        dataResp:
        xxx0abc1
            010 - Data accepted
            101 - Data rejected due to CRC error
            110 - Data rejected due to write error
    */
    uint8_t dataResp;
    sdcard_read_bytes(&dataResp, sizeof(dataResp));
    if((dataResp & 0x1F) != 0x05) { // data rejected
        sdcard_unselect();
        return -1;
    }

    if(sdcard_wait_not_busy() < 0) {
        sdcard_unselect();
        return -2;
    }

    sdcard_unselect();
    return 0;
}

int sdcard_write_end() {
    sdcard_select();

    uint8_t stopTran = 0xFD; // stop transaction token for CMD25
    HAL_SPI_Transmit(nullptr, &stopTran, sizeof(stopTran), HAL_MAX_DELAY);

    // skip one byte before readyng "busy"
    // this is required by the spec and is necessary for some real SD-cards!
    uint8_t skipByte;
    sdcard_read_bytes(&skipByte, sizeof(skipByte));

    if(sdcard_wait_not_busy() < 0) {
        sdcard_unselect();
        return -1;
    }

    sdcard_unselect();
    return 0;
}

