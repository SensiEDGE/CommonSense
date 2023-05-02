/**
 ******************************************************************************
 * @file    Bq27441.cpp
 * @date    26 Octpber 2022
 * @brief   BQ27441 driver
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

#include "Bq27441.h"
#include "../../Drivers/CXD5602/I2c/I2c.h"

bq27441_g1_t bq27441;

bq27441_g1_t * bq27441_g1 = &bq27441;

/**
 * @brief  Write device registers via I2C
 * @param  [in] i2c_addr I2C devices address
 * @param  [in] reg_addr I2C devices register address
 * @param  [in] data dete wich should be read from register
 * @param  [in] size size of data to write
 * @retval None
 */
void bq27441_i2c_write(uint16_t addr, uint8_t reg_offset, uint8_t data) {
    i2c_write_regs(addr, reg_offset, &data, 1);
}

/**
 * @brief  Read device registers via I2C
 * @param  [in] i2c_addr I2C devices address
 * @param  [in] reg_addr I2C devices register address
 * @param  [out] data dete wich should be read from register
 * @param  [in] size size of data to read
 * @retval TWI_SUCCESS (0) if ok
 */
uint16_t bq27441_i2c_read(uint16_t addr, uint8_t reg_offset) {
    uint8_t data [2];
    i2c_read_regs(addr, reg_offset, data, 2);
    return (data[1] << 8) | (data[0]);
}

void bq27441_init(void) {
    i2c_init();
    bq27441.read = bq27441_i2c_read;
    bq27441.write = bq27441_i2c_write;
}

void bq27441_g1_parse_lags(uint16_t regval) {
    //low byte
    bq27441_g1->flags.dsg = (regval & 0x01);
    bq27441_g1->flags.socf = (regval & 0x02) >> 1;
    bq27441_g1->flags.soc1 = (regval & 0x04) >> 2;
    bq27441_g1->flags.bat_det = (regval & 0x08) >> 3;
    bq27441_g1->flags.cfgupmode = (regval & 0x10) >> 4;
    bq27441_g1->flags.itpor = (regval & 0x20) >> 5;
    bq27441_g1->flags.ocvtaken = (regval & 0x80) >> 7;

    //high byte
    bq27441_g1->flags.chg = (regval & 0x100)  >> 8;
    bq27441_g1->flags.fc = (regval & 0x200)  >> 9;
    bq27441_g1->flags.ut = (regval & 0x4000) >> 14;
    bq27441_g1->flags.ot = (regval & 0x8000) >> 15;
}

void bq27441_g1_parse_control_status(uint16_t regval) {
    //low byte
    bq27441_g1->control_status.vok = (regval & 0x02) >> 1;
    bq27441_g1->control_status.rup_dis = (regval & 0x04) >> 2;
    bq27441_g1->control_status.ldmd = (regval & 0x08) >> 3;
    bq27441_g1->control_status.sleep = (regval & 0x10) >> 4;
    bq27441_g1->control_status.hibernate = (regval & 0x40) >> 6;
    bq27441_g1->control_status.initcomp = (regval & 0x80) >> 7;

    //high byte
    bq27441_g1->control_status.res_up = (regval & 0x100)  >> 8;
    bq27441_g1->control_status.qmax_up = (regval & 0x200)  >> 9;
    bq27441_g1->control_status.bca = (regval & 0x400)  >> 10;
    bq27441_g1->control_status.cca = (regval & 0x800)  >> 11;
    bq27441_g1->control_status.calmode = (regval & 0x1000) >> 12;
    bq27441_g1->control_status.ss = (regval & 0x2000) >> 13;
    bq27441_g1->control_status.wdreset = (regval & 0x4000) >> 14;
    bq27441_g1->control_status.shutdownen = (regval & 0x8000) >> 15;
}

void bq27441_g1_parse_op_config(uint16_t regval) {
    //low byte
    bq27441_g1->op_config.temps = (regval & 0x01);
    bq27441_g1->op_config.batlowen = (regval & 0x04) >>2;
    bq27441_g1->op_config.rmfcc = (regval & 0x10) >> 4;
    bq27441_g1->op_config.sleep = (regval & 0x20) >> 5;

    //high byte
    bq27441_g1->op_config.gpiopol = (regval & 0x800) >> 11;
    bq27441_g1->op_config.bi_pu_en = (regval & 0x1000) >> 12;
    bq27441_g1->op_config.bie = (regval & 0x2000) >> 13;
}

/* _______________STANDART COMMANDS FUNCTIONS_______________ */

float bq27441_g1_get_temperature(void) {
    float result = 0;

    uint16_t buffer;
    buffer = bq27441_g1->read(BQ27441_G1_ADDR, BQ27441_G1_TEMPERATURE_CMD);

    //Kelvin to Celsius
    result = ((float) buffer * 0.1) - 273;

    return result;
}

uint16_t bq27441_g1_get_voltage(void) {
    uint16_t buffer;
    buffer = bq27441_g1->read(BQ27441_G1_ADDR, BQ27441_G1_VOLTAGE_CMD);
    return buffer;
}


uint16_t bq27441_g1_get_flags(void) {
    uint16_t buffer;
    buffer = bq27441_g1->read(BQ27441_G1_ADDR, BQ27441_G1_FLAGS_CMD);
    bq27441_g1_parse_lags(buffer);
    return buffer;
}


uint16_t bq27441_g1_get_nominal_available_capacity(void) {
    uint16_t buffer;
    buffer = bq27441_g1->read(BQ27441_G1_ADDR, BQ27441_G1_NOMINAL_AVALIABLE_CAPACITY_CMD);
    return buffer;
}


uint16_t bq27441_g1_get_full_availiable_capacity(void) {
    uint16_t buffer;
    buffer = bq27441_g1->read(BQ27441_G1_ADDR, BQ27441_G1_FULL_AVALIABLE_CAPACITY_CMD);
    return buffer;
}


uint16_t bq27441_g1_get_remaining_capacity(void) {
    uint16_t buffer;
    buffer = bq27441_g1->read(BQ27441_G1_ADDR, BQ27441_G1_REMAINING_CAPACITY_CMD);
    return buffer;
}


uint16_t bq27441_g1_get_full_charge_capacity(void) {
    uint16_t buffer;
    buffer = bq27441_g1->read(BQ27441_G1_ADDR, BQ27441_G1_FULL_CHARGE_CAPACITY_CMD);
    return buffer;
}


int16_t  bq27441_g1_get_average_current(void) {
    uint16_t buffer;
    buffer = bq27441_g1->read(BQ27441_G1_ADDR, BQ27441_G1_AVERAGE_CURRENT_CMD);
    return buffer;
}


int16_t  bq27441_g1_get_standby_current(void) {
    uint16_t buffer;
    buffer = bq27441_g1->read(BQ27441_G1_ADDR, BQ27441_G1_STANDBY_CURRENT_CMD);
    return buffer;
}


int16_t  bq27441_g1_get_max_load_current(void) {
    uint16_t buffer;
    buffer = bq27441_g1->read(BQ27441_G1_ADDR, BQ27441_G1_MAX_LOAD_CURRENT_CMD);
    return buffer;
}


int16_t  bq27441_g1_get_average_power(void) {
    uint16_t buffer;
    buffer = bq27441_g1->read(BQ27441_G1_ADDR, BQ27441_G1_AVERAGE_POWER_CMD);
    return buffer;
}


uint16_t bq27441_g1_get_state_of_charge(void) {
    uint16_t buffer;
    buffer = bq27441_g1->read(BQ27441_G1_ADDR, BQ27441_G1_STATE_OF_CHARGE_CMD);
    return buffer;
}


uint16_t bq27441_g1_get_internal_temperature(void) {
    float result = 0;
    uint16_t buffer;

    buffer = bq27441_g1->read(BQ27441_G1_ADDR, BQ27441_G1_INTERNAL_TEMPERATURE_CMD);

    //Kelvin to Celsius
    result = ((float) buffer * 0.1) - 273;

    return result;
}


uint16_t bq27441_g1_get_state_of_health(void) {
    uint16_t buffer;
    buffer = bq27441_g1->read(BQ27441_G1_ADDR, BQ27441_G1_STATE_OF_HEALTH_CMD);
    return buffer;
}


uint16_t bq27441_g1_get_remaining_capacity_unfiltered(void) {
    uint16_t buffer;
    buffer = bq27441_g1->read(BQ27441_G1_ADDR, BQ27441_G1_REMAINING_CAPACITY_UNFILTERED_CMD);
    return buffer;
}


uint16_t bq27441_g1_get_remaining_capacity_filtered(void) {
    uint16_t buffer;
    buffer = bq27441_g1->read(BQ27441_G1_ADDR, BQ27441_G1_REMAINING_CAPACITY_FILTERED_CMD);
    return buffer;
}


uint16_t bq27441_g1_get_full_charge_capacity_unfiltered(void) {
    uint16_t buffer;
    buffer = bq27441_g1->read(BQ27441_G1_ADDR, BQ27441_G1_FULL_CHARGE_CAPACITY_UNFILTERED_CMD);
    return buffer;
}


uint16_t bq27441_g1_get_full_charge_capacity_filtered(void) {
    uint16_t buffer;
    buffer = bq27441_g1->read(BQ27441_G1_ADDR, BQ27441_G1_FULL_CHARGE_CAPACITY_FILTERED_CMD);
    return buffer;
}


uint16_t bq27441_g1_get_state_of_charge_unfiltered(void) {
    uint16_t buffer;
    buffer = bq27441_g1->read(BQ27441_G1_ADDR, BQ27441_G1_STATE_OF_CHARGE_UNFILTERED_CMD);
    return buffer;
}

/* _________________________________________________________ */



/* _______________CONTROL SUBCOMANDS FUNCTIONS______________ */


void bq27441_g1_get_control_status(void) {
    uint16_t buffer;
    bq27441_g1->write(BQ27441_G1_ADDR, BQ27441_G1_CONTROL_CMD, BQ27441_G1_CONTROL_STATUS_SUBCMD);
    buffer = bq27441_g1->read(BQ27441_G1_ADDR, BQ27441_G1_CONTROL_CMD);
    bq27441_g1_parse_control_status(buffer);
}


uint16_t bq27441_g1_get_device_type(void) {
    uint16_t buffer;
    bq27441_g1->write(BQ27441_G1_ADDR, BQ27441_G1_CONTROL_CMD, BQ27441_G1_DEVICE_TYPE_SUBCMD);
    buffer = bq27441_g1->read(BQ27441_G1_ADDR, BQ27441_G1_CONTROL_CMD);
    return buffer;
}


uint16_t bq27441_g1_get_fw_version(void) {
    uint16_t buffer;
    bq27441_g1->write(BQ27441_G1_ADDR, BQ27441_G1_CONTROL_CMD, BQ27441_G1_FW_VERSION_SUBCMD);
    buffer = bq27441_g1->read(BQ27441_G1_ADDR, BQ27441_G1_CONTROL_CMD);
    return buffer;
}


uint16_t bq27441_g1_get_dm_code(void) {
    uint16_t buffer;
    bq27441_g1->write(BQ27441_G1_ADDR, BQ27441_G1_CONTROL_CMD, BQ27441_G1_DM_CODE_SUBCMD);
    buffer = bq27441_g1->read(BQ27441_G1_ADDR, BQ27441_G1_CONTROL_CMD);
    return buffer;
}


uint16_t bq27441_g1_get_prev_macwrite(void) {
    uint16_t buffer;
    bq27441_g1->write(BQ27441_G1_ADDR, BQ27441_G1_CONTROL_CMD, BQ27441_G1_PREV_MACWRITE_SUBCMD);
    buffer = bq27441_g1->read(BQ27441_G1_ADDR, BQ27441_G1_CONTROL_CMD);
    return buffer;
}


uint16_t bq27441_g1_get_chem_id(void) {
    uint16_t buffer;
    bq27441_g1->write(BQ27441_G1_ADDR, BQ27441_G1_CONTROL_CMD, BQ27441_G1_CHEM_ID_SUBCMD);
    buffer = bq27441_g1->read(BQ27441_G1_ADDR, BQ27441_G1_CONTROL_CMD);
    return buffer;
}


void bq27441_g1_bat_insert (void) {
    bq27441_g1->write(BQ27441_G1_ADDR, BQ27441_G1_CONTROL_CMD, BQ27441_G1_BAT_INSERT_SUBCMD);
}


void bq27441_g1_bat_remove (void) {
    bq27441_g1->write(BQ27441_G1_ADDR, BQ27441_G1_CONTROL_CMD, BQ27441_G1_BAT_REMOVE_SUBCMD);
}


void bq27441_g1_set_hibernate (void) {
    bq27441_g1->write(BQ27441_G1_ADDR, BQ27441_G1_CONTROL_CMD, BQ27441_G1_SET_HIBERNATE_SUBCMD);
}


void bq27441_g1_clear_hibernate (void) {
    bq27441_g1->write(BQ27441_G1_ADDR, BQ27441_G1_CONTROL_CMD, BQ27441_G1_CLEAR_HIBERNATE_SUBCMD);
}


void bq27441_g1_set_cfg_update (void) {
    bq27441_g1->write(BQ27441_G1_ADDR, BQ27441_G1_CONTROL_CMD, BQ27441_G1_SET_CFGUPDATE_SUBCMD);
}


void bq27441_g1_set_shutdown_enable (void) {
    bq27441_g1->write(BQ27441_G1_ADDR, BQ27441_G1_CONTROL_CMD, BQ27441_G1_SHUTDOWN_ENABLE_SUBCMD);
}


void bq27441_g1_set_shutdown (void) {
    bq27441_g1->write(BQ27441_G1_ADDR, BQ27441_G1_CONTROL_CMD, BQ27441_G1_SHUTDOWN_SUBCMD);
}


void bq27441_g1_set_sealed (void) {
    bq27441_g1->write(BQ27441_G1_ADDR, BQ27441_G1_CONTROL_CMD, BQ27441_G1_SEALED_SUBCMD);
}


void bq27441_g1_toggle_gpout (void) {
    bq27441_g1->write(BQ27441_G1_ADDR, BQ27441_G1_CONTROL_CMD, BQ27441_G1_TOGGLE_GPOUT_SUBCMD);
}


void bq27441_g1_reset (void) {
    bq27441_g1->write(BQ27441_G1_ADDR, BQ27441_G1_CONTROL_CMD, bq27441_g1_reset_SUBCMD);
}


void bq27441_g1_soft_reset (void) {
    bq27441_g1->write(BQ27441_G1_ADDR, BQ27441_G1_CONTROL_CMD, BQ27441_G1_SOFT_RESET_SUBCMD);
}


void bq27441_g1_exit_cfg_update (void) {
    bq27441_g1->write(BQ27441_G1_ADDR, BQ27441_G1_CONTROL_CMD, BQ27441_G1_EXIT_CFGUPDATE_SUBCMD);
}


void bq27441_g1_exit_resim (void) {
    bq27441_g1->write(BQ27441_G1_ADDR, BQ27441_G1_CONTROL_CMD, BQ27441_G1_EXIT_RESIM_SUBCMD);
}



/* _________________________________________________________ */


/* ________________EXTENDED COMMANDS FUNCTION_______________ */



uint16_t bq27441_g1_get_op_config(void) {
    uint16_t buffer;
    buffer = bq27441_g1->read(BQ27441_G1_ADDR, BQ27441_G1_OP_CONFIG_CMD);
    bq27441_g1_parse_op_config(buffer);
    return buffer;
}


uint16_t bq27441_g1_get_design_capacity(void) {
    uint16_t buffer;
    buffer = bq27441_g1->read(BQ27441_G1_ADDR, BQ27441_G1_DESIGN_CAPACITY_CMD);
    return buffer;
}


uint16_t bq27441_g1_get_data_block (uint8_t value) {
    uint16_t buffer;
    bq27441_g1->write(BQ27441_G1_ADDR, BQ27441_G1_DATA_BLOCK_CMD, value);
    buffer = bq27441_g1->read(BQ27441_G1_ADDR, BQ27441_G1_DATA_BLOCK_CMD);
    return buffer;
}


uint16_t bq27441_g1_GetChecksum (void) {
    uint16_t buffer;
    buffer = bq27441_g1->read(BQ27441_G1_ADDR, BQ27441_G1_BLOCK_DATA_CHECKSUM_CMD);
    return buffer;
}

 
void bq27441_g1_block_data_control (void) {

    bq27441_g1->write(BQ27441_G1_ADDR, BQ27441_G1_BLOCK_DATA_CONTROL_CMD, 0x00);

}
