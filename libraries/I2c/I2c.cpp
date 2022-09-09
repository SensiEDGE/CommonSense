/**
 ******************************************************************************
 * @file    I2c.cpp
 * @date    29 March 2022
 * @brief   I2C driver
 ******************************************************************************
 *
 * COPYRIGHT(c) 2022 Droid-Technologies LLC
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of Droid-Technologies LLC nor the names of its contributors may
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

#include "I2c.h"
#include <sdk/config.h>

#include <string.h>
#include <stdio.h>
extern "C" {
#include <cxd56_i2c.h>
#include <cxd56_pmic.h>
}
#include <errno.h>

// buffer
#define BUFFER_LENGTH       (32)
#define TWI_TX_BUF_LEN      BUFFER_LENGTH
#define TWI_RX_BUF_LEN      BUFFER_LENGTH

uint8_t _tx_buf[TWI_TX_BUF_LEN];
uint8_t _tx_buf_index = 0;
uint8_t _tx_buf_len = 0;
uint8_t _rx_buf[TWI_RX_BUF_LEN];
uint8_t _rx_buf_index = 0;
uint8_t _rx_buf_len = 0;
uint32_t _freq = TWI_FREQ_100KHZ;

FAR struct i2c_master_s* _dev;



void i2c_init(void) {
    static bool isInit = false;
    if (isInit == false) {
        isInit = true;
        memset(_tx_buf, 0, sizeof(_tx_buf));
        memset(_rx_buf, 0, sizeof(_rx_buf));

        _dev = cxd56_i2cbus_initialize(0);
        if (_dev == 0){
            printf("ERROR: Failed to init I2C device\n");
        } else {
            printf("I2C successfull\r\n");
        }
    }
}

void i2c_set_freq(uint32_t freq) {
    _freq = freq;
}

static uint8_t I2cRequest(uint8_t address, uint8_t quantity, uint8_t sendStop) {
    struct i2c_msg_s msg;
    unsigned int flags = 0;
    int ret;

    if (!_dev) return 0;

    // clamp to buffer length
    if (quantity > TWI_RX_BUF_LEN) {
        quantity = TWI_RX_BUF_LEN;
    }


    if (!sendStop) {
        flags |= I2C_M_NOSTOP;
    }

    // Setup for the transfer
    msg.frequency = _freq;
    msg.addr      = address;
    msg.flags     = (flags | I2C_M_READ);
    msg.buffer    = _rx_buf;
    msg.length    = quantity;

    // Then perform the transfer.
    ret = I2C_TRANSFER(_dev, &msg, 1);
    if (ret < 0) {
        ::printf("ERROR: Failed to read from i2c (errno = %d)\n", errno);
        return 0;
    }

    // set rx buffer iterator vars
    _rx_buf_index = 0;
    _rx_buf_len = quantity;

    return quantity;
}

static uint8_t I2cTransmit(uint8_t _tx_address, bool sendStop) {
    struct i2c_msg_s msg;
    unsigned int flags = 0;
    int ret;

    if (!sendStop) {
        flags |= I2C_M_NOSTOP;
    }

    // Setup for the transfer
    msg.frequency = _freq;
    msg.addr      = _tx_address;
    msg.flags     =  flags;
    msg.buffer    = _tx_buf;
    msg.length    = _tx_buf_len;

    // Then perform the transfer.
    ret = I2C_TRANSFER(_dev, &msg, 1);
     _tx_buf_len = 0;

    if (ret == -ENODEV) {
        // device not found
        return TWI_NACK_ON_ADDRESS;
    }
    return (ret < 0) ? TWI_OTHER_ERROR : TWI_SUCCESS;
}

uint8_t i2c_write_reg(uint8_t i2c_addr, uint8_t reg_addr, uint8_t data) {
    _tx_buf[0] = reg_addr;
    _tx_buf[1] = data;
    _tx_buf_len = 2;
    
    return I2cTransmit(i2c_addr, true);
 }

uint8_t i2c_read_reg(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *data) {
    _tx_buf[0] = reg_addr;
    _tx_buf_len = 1;

    if (I2cTransmit(i2c_addr, false) != TWI_SUCCESS) {
        return TWI_OTHER_ERROR;
    }

    I2cRequest(i2c_addr, 1, true);

    *data = _rx_buf[0];
    return TWI_SUCCESS;
}

uint8_t i2c_write_regs(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *data, uint16_t size) {
    _tx_buf[0] = reg_addr;
    memcpy(&_tx_buf[1], data, size);
    _tx_buf_len = size + 1;
    
    return I2cTransmit(i2c_addr, true);
 }

uint8_t i2c_read_regs(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *data, uint16_t size) {
    _tx_buf[0] = reg_addr;
    _tx_buf_len = 1;

    if (I2cTransmit(i2c_addr, false) != TWI_SUCCESS) {
        printf("i2c_read_regs, I2cTransmit err\r\n");
        return TWI_OTHER_ERROR;
    }

    uint16_t read = I2cRequest(i2c_addr, size, true);

    memcpy(data, _rx_buf, size);

    return TWI_SUCCESS;
}

uint8_t i2c_read_data(uint8_t i2c_addr, uint8_t *data, uint16_t size) {

    uint16_t read = I2cRequest(i2c_addr, size, true);

    memcpy(data, _rx_buf, size);

    return TWI_SUCCESS;
}

uint8_t i2c_write_data(uint8_t i2c_addr, uint8_t *data, uint16_t size) {
    memcpy(_tx_buf, data, size);
    _tx_buf_len = size;
    
    return I2cTransmit(i2c_addr, true);
}

uint8_t i2c_write_regs_16addr(uint8_t i2c_addr, uint16_t reg_addr, uint8_t *data, uint16_t size) {
    uint8_t reg [2] = {0};
    memcpy(&reg, &reg_addr, 2);
    _tx_buf [0] = reg[1];
    _tx_buf [1] = reg[0];
    _tx_buf_len = 2;
    memcpy(&_tx_buf[2], data, size);
    _tx_buf_len += size;
    
    return I2cTransmit(i2c_addr, true);
}

uint8_t i2c_read_regs_16addr(uint8_t i2c_addr, uint16_t reg_addr, uint8_t *data, uint16_t size) {
    uint8_t reg [2] = {0};
    memcpy(&reg, &reg_addr, 2);
    _tx_buf [0] = reg[1];
    _tx_buf [1] = reg[0];
    _tx_buf_len = 2;

    if (I2cTransmit(i2c_addr, false) != TWI_SUCCESS) {
        printf("i2c_read_regs_16addr, I2cTransmit err\r\n");
        return TWI_OTHER_ERROR;
    }
    uint16_t read = I2cRequest(i2c_addr, size, true);
    memcpy(data, _rx_buf, size);

    return TWI_SUCCESS;
}
