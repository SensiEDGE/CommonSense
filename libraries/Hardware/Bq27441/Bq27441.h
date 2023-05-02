/**
 ******************************************************************************
 * @file    Bq27441.h
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

#ifndef BQ27441_H
#define BQ27441_H

#include <stdint.h>


/* ______________________DEFINE Section______________________ */

/*Device address*/
#define BQ27441_G1_ADDR 0x55

/* _________________________________________________________ */


/* ____________________STANDART COMMANDS____________________ */

#define BQ27441_G1_CONTROL_CMD 0x00
#define BQ27441_G1_TEMPERATURE_CMD 0x02
#define BQ27441_G1_VOLTAGE_CMD 0x04
#define BQ27441_G1_FLAGS_CMD 0x06
#define BQ27441_G1_NOMINAL_AVALIABLE_CAPACITY_CMD 0x08
#define BQ27441_G1_FULL_AVALIABLE_CAPACITY_CMD 0x0A
#define BQ27441_G1_REMAINING_CAPACITY_CMD 0x0C
#define BQ27441_G1_FULL_CHARGE_CAPACITY_CMD 0x0E
#define BQ27441_G1_AVERAGE_CURRENT_CMD 0x10
#define BQ27441_G1_STANDBY_CURRENT_CMD 0x12
#define BQ27441_G1_MAX_LOAD_CURRENT_CMD 0x14
#define BQ27441_G1_AVERAGE_POWER_CMD 0x18
#define BQ27441_G1_STATE_OF_CHARGE_CMD 0x1C
#define BQ27441_G1_INTERNAL_TEMPERATURE_CMD 0x1E
#define BQ27441_G1_STATE_OF_HEALTH_CMD 0x20
#define BQ27441_G1_REMAINING_CAPACITY_UNFILTERED_CMD 0x28
#define BQ27441_G1_REMAINING_CAPACITY_FILTERED_CMD 0x2A
#define BQ27441_G1_FULL_CHARGE_CAPACITY_UNFILTERED_CMD 0x2C
#define BQ27441_G1_FULL_CHARGE_CAPACITY_FILTERED_CMD 0x2E
#define BQ27441_G1_STATE_OF_CHARGE_UNFILTERED_CMD 0x30

/* ____________BQ27441-G1_CONTROL_CMD SUBCOMMANDS____________ */

#define BQ27441_G1_CONTROL_STATUS_SUBCMD 0x0000
#define BQ27441_G1_DEVICE_TYPE_SUBCMD 0x0001
#define BQ27441_G1_FW_VERSION_SUBCMD 0x0002
#define BQ27441_G1_DM_CODE_SUBCMD 0x0004
#define BQ27441_G1_PREV_MACWRITE_SUBCMD 0x0007
#define BQ27441_G1_CHEM_ID_SUBCMD 0x0008
#define BQ27441_G1_BAT_INSERT_SUBCMD 0x000C
#define BQ27441_G1_BAT_REMOVE_SUBCMD 0x000D
#define BQ27441_G1_SET_HIBERNATE_SUBCMD 0x0011
#define BQ27441_G1_CLEAR_HIBERNATE_SUBCMD 0x0012
#define BQ27441_G1_SET_CFGUPDATE_SUBCMD 0x0013
#define BQ27441_G1_SHUTDOWN_ENABLE_SUBCMD 0x001B
#define BQ27441_G1_SHUTDOWN_SUBCMD 0x001C
#define BQ27441_G1_SEALED_SUBCMD 0x0020
#define BQ27441_G1_TOGGLE_GPOUT_SUBCMD 0x0023
#define bq27441_g1_reset_SUBCMD 0x0041
#define BQ27441_G1_SOFT_RESET_SUBCMD 0x0042
#define BQ27441_G1_EXIT_CFGUPDATE_SUBCMD 0x0043
#define BQ27441_G1_EXIT_RESIM_SUBCMD 0x0044

/* _________________________________________________________ */

/* ____________________EXTENDED COMMANDS____________________ */

#define  BQ27441_G1_OP_CONFIG_CMD 0x3A
#define  BQ27441_G1_DESIGN_CAPACITY_CMD 0x3C
#define  BQ27441_G1_DATA_CLASS_CMD 0x3E
#define  BQ27441_G1_DATA_BLOCK_CMD 0x3F
#define  BQ27441_G1_BLOCK_DATA_CMD 0x40
#define  BQ27441_G1_BLOCK_DATA_CHECKSUM_CMD 0x60
#define  BQ27441_G1_BLOCK_DATA_CONTROL_CMD 0x61

/* _________________________________________________________ */

/* ______________________Types  Section_____________________ */

typedef struct {
    //high byte
    uint8_t ot;
    uint8_t ut;
    uint8_t fc;
    uint8_t chg;
    //low byte
    uint8_t ocvtaken;
    uint8_t itpor;
    uint8_t cfgupmode;
    uint8_t bat_det;
    uint8_t soc1;
    uint8_t socf;
    uint8_t dsg;
} flags_t;

typedef struct {
    //high byte
    uint8_t shutdownen;
    uint8_t wdreset;
    uint8_t ss;
    uint8_t calmode;
    uint8_t cca;
    uint8_t bca;
    uint8_t qmax_up;
    uint8_t res_up;
    //low byte
    uint8_t initcomp;
    uint8_t hibernate;
    uint8_t sleep;
    uint8_t ldmd;
    uint8_t rup_dis;
    uint8_t vok;
} control_status_t;

typedef struct {
    //low byte
    uint8_t sleep;
    uint8_t rmfcc;
    uint8_t batlowen;
    uint8_t temps;
    //high byte
    uint8_t bie;
    uint8_t bi_pu_en;
    uint8_t gpiopol;
} op_config_t;

typedef struct {
    void (*write)(uint16_t addr, uint8_t reg_offset, uint8_t data);
    uint16_t (*read)(uint16_t addr, uint8_t reg_offset);
    flags_t flags;
    control_status_t control_status;
    op_config_t op_config;
} bq27441_g1_t;


/* _________________________________________________________ */



/* ____________________Prototypes Section___________________ */

/**
 * @brief  BQ27441 init
 * @param  None
 * @retval None
 */
void bq27441_init(void);

/**
* @brief Parsing of Flag Register result
* @param [in] regval Flag register value
* @retval none
*/
void bq27441_g1_parse_lags(uint16_t regval);

/**
* @brief Parsing of Control Status Register result
* @param [in] regval Control Status register value
* @retval none
*/
void bq27441_g1_parse_control_status(uint16_t regval);

/**
* @brief Parsing of Operation Configuration Register result
* @param [in] regval Operation Configuration Register value
* @retval none
*/
void bq27441_g1_parse_op_config(uint16_t regval);

/* _______________STANDART COMMANDS FUNCTIONS_______________ */


/**
* @brief Getting value of temperature, measured by the fuel gauge 
* @param None
* @retval Value of temperature in Celsius
*/
float bq27441_g1_get_temperature(void);

/**
* @brief Getting value of the measured cell-pack voltage with a range of 0 to 6000 mV 
* @param None
* @retval Value of voltage in mV
*/
uint16_t bq27441_g1_get_voltage(void);

/**
* @brief Getting the contents of the fuel gauging status register, depicting the current operating status
* @param None
* @retval Value of Flag Register
*/
uint16_t bq27441_g1_get_flags(void);

/**
* @brief Getting value of  uncompensated (less than C/20 load) battery capacity remaining
* @param None
* @retval Value of capacity in mAh
*/
uint16_t bq27441_g1_get_nominal_available_capacity(void);

/**
* @brief Getting value of  uncompensated (less than C/20 load) capacity of the battery when fully charged
* @param None
* @retval Value of capacity in mAh
*/
uint16_t bq27441_g1_get_full_availiable_capacity(void);

/**
* @brief Getting value of remaining battery capacity compensated for load and temperature
* @param None
* @retval Value of capacity in mAh
*/
uint16_t bq27441_g1_get_remaining_capacity(void);

/**
* @brief Getting value of compensated capacity of the battery when fully charged
* @param None
* @retval Value of capacity in mAh
*/
uint16_t bq27441_g1_get_full_charge_capacity(void);

/**
* @brief Getting value  that is the average current flow through the sense resistor
* @param None
* @retval Value of current in mA
*/
int16_t  bq27441_g1_get_average_current(void);

/**
* @brief Getting value of the measured standby current through the sense resistor
* @param None
* @retval Value of current in mA
*/
int16_t  bq27441_g1_get_standby_current(void);

/**
* @brief Getting value of the maximum load conditions
* @param None
* @retval Value of current in mA
*/
int16_t  bq27441_g1_get_max_load_current(void);

/**
* @brief Getting value  of the average power during charging and discharging of the battery.  It is negative during discharge and positive during charge. A value of 0
indicates that the battery is not being discharged
* @param None
* @retval Value of power in mW
*/
int16_t  bq27441_g1_get_average_power(void);

/**
* @brief Getting value of the predicted remaining battery capacity expressed as a percentage of FullChargeCapacity() with a range of 0 to 100%
* @param None
* @retval Value of predicted remaining battery capacity in %
*/
uint16_t bq27441_g1_get_state_of_charge(void);

/**
* @brief Getting value of the internal temperature sensor
* @param None
* @retval Value of temperature in Celsius
*/
uint16_t bq27441_g1_get_internal_temperature(void);

/**
* @brief Getting value, expressed as a percentage of the ratio of predicted FCC(25°C, SOH LoadI) over the DesignCapacity
* @param None
* @retval Value of SOH in %
*/
uint16_t bq27441_g1_get_state_of_health(void);

/**
* @brief Getting value of the true battery capacity remaining
* @param None
* @retval Value of capacity in mAh
*/
uint16_t bq27441_g1_get_remaining_capacity_unfiltered(void);

/**
* @brief Getting value of the filtered battery capacity remaining
* @param None
* @retval Value of capacity in mAh
*/
uint16_t bq27441_g1_get_remaining_capacity_filtered(void);

/**
* @brief Getting value of the compensated capacity of the battery when fully charged
* @param None
* @retval Value of capacity in mAh
*/
uint16_t bq27441_g1_get_full_charge_capacity_unfiltered(void);

/**
* @brief Getting value of the filtered compensated capacity of the battery when fully charged
* @param None
* @retval Value of capacity in mAh
*/
uint16_t bq27441_g1_get_full_charge_capacity_filtered(void);

/**
* @brief Getting value of the true state-of-charge
* @param None
* @retval Value of SOC in %
*/
uint16_t bq27441_g1_get_state_of_charge_unfiltered(void);

/* _________________________________________________________ */



/* _______________CONTROL SUBCOMANDS FUNCTIONS______________ */


/**
* @brief Reports the status of device.
* @param None
* @retval Value of Conrol Status Register 
*/
void bq27441_g1_get_control_status(void);

/**
* @brief Reports the device type
* @param None
* @retval Value of device type (0x0421)
*/
uint16_t bq27441_g1_get_device_type(void);

/**
* @brief Reports the firmware version of the device
* @param None
* @retval Value of the firmware version
*/
uint16_t bq27441_g1_get_fw_version(void);    //return the firmware version

/**
* @brief Reports the configuration code stored in Data Memory
* @param None
* @retval Value of the DM code
*/
uint16_t bq27441_g1_get_dm_code(void);        //return the 8-bit DM Code as the least significant byte

/**
* @brief Getting previous MAC command code
* @param None
* @retval Previous command code
*/
uint16_t bq27441_g1_get_prev_macwrite(void);    //return the previous command written to CONTROL

/**
* @brief Reports the chemical identifier of the battery profile used by the fuel gauge
* @param None
* @retval Expected value for bq27441-G1A - 0x0128, for bq27441-G1B - 0x0312.
*/
uint16_t bq27441_g1_get_chem_id(void);        //return the chemical identifier for the Impedance Track� configuration

/**
* @brief Forces the Flags() [BAT_DET] bit to set when the battery insertion detection is disabled via OpConfig [BIE] = 0. 
In this case, the gauge does not detect battery insertion from the BIN pin logic state, but relies
on the BAT_INSERT host subcommand to indicate battery presence in the system.
* @param None
* @retval none
*/
void bq27441_g1_bat_insert (void);            //forces the Flags() [BAT_DET] bit to set when the battery insertion detection is disabled via OpConfig[BIE] = 0.

/**
* @brief Forces the Flags() [BAT_DET] bit to clear when the battery insertion detection is disabled via OpConfig
[BIE] = 0. In this case, the gauge does not detect battery removal from the BIN pin logic state, but relies
on the BAT_REMOVE host subcommand to indicate battery removal from the system.
* @param None
* @retval none
*/
void bq27441_g1_bat_remove (void);            //forces the Flags() [BAT_DET] bit to clear when the battery insertion detection is disabled via OpConfig[BIE] = 0.

/**
* @brief Instructs the fuel gauge to force the CONTROL_STATUS [HIBERNATE] bit to 1. If the necessary conditions are met, 
this enables the gauge to enter the HIBERNATE power mode after the transition to SLEEP power state is detected.
* @param None
* @retval none
*/
void bq27441_g1_set_hibernate (void);        //force the CONTROL_STATUS [HIBERNATE] bit to 1.

/**
* @brief Instructs the fuel gauge to force the CONTROL_STATUS [HIBERNATE] bit to 0. This prevents the gauge
from entering the HIBERNATE power mode after the transition to SLEEP power state is detected.
* @param None
* @retval none
*/
void bq27441_g1_clear_hibernate (void);        //force the CONTROL_STATUS [HIBERNATE] bit to 0.

/**
* @brief Instructs the fuel gauge to enter CONFIG UPDATE mode
* @param None
* @retval none
*/
void bq27441_g1_set_cfg_update (void);        //set the Flags() [CFGUPMODE] bit to 1 and enter CONFIG UPDATE mode.

/**
* @brief Instructs the fuel gauge to enable SHUTDOWN mode
* @param None
* @retval none
*/
void bq27441_g1_set_shutdown_enable (void);    //enable SHUTDOWN mode and set the CONTROL_STATUS [SHUTDOWNEN]status bit.

/**
* @brief Instructs the fuel gauge to immediately enter SHUTDOWN mode
* @param None
* @retval none
*/
void bq27441_g1_set_shutdown (void);        //immediately enter SHUTDOWN mode after receiving this subcommand.

/**
* @brief Instructs the fuel gauge to  transition from UNSEALED state to SEALED state
* @param None
* @retval none
*/
void bq27441_g1_set_sealed (void);            //transition from UNSEALED state to SEALED state

/**
* @brief Commands the device to toggle the GPOUT pin for 1 ms
* @param None
* @retval none
*/
void bq27441_g1_toggle_gpout (void);        //pulse the GPOUT pin for approximately 1 ms within 1 second of receiving the command

/**
* @brief Performs a full device reset
* @param None
* @retval none
*/
void bq27441_g1_reset (void);                //perform a full device reset

/**
* @brief Performs a partial (soft) reset from any mode with an OCV (open-circuit voltage) measurement
* @param None
* @retval none
*/
void bq27441_g1_soft_reset (void);            //perform a partial (soft) reset from any mode with an OCV measurement

/**
* @brief Exits CONFIG UPDATE mode without an OCV (open-circuit voltage) measurement and without resimulating to update StateOfCharge()
* @param None
* @retval none
*/
void bq27441_g1_exit_cfg_update (void);

/**
* @brief Exits CONFIG UPDATE mode without an OCV measurement and resimulates with the updated configuration data to update StateOfCharge()
* @param None
* @retval none
*/
void bq27441_g1_exit_resim (void);


//void bq27441_g1_set_unsealed (void);

/* _________________________________________________________ */



/* ________________EXTENDED COMMANDS FUNCTION_______________ */

/**
* @brief Getting value of the  Operation Configuration Register (which is most useful for system level debug to quickly determine device configuration)
* @param None
* @retval Value of the Operation Configuration Register
*/
uint16_t bq27441_g1_get_op_config(void);                        //return OpConfig Data Memory register setting

/**
* @brief Getting Design Capacity Data Memory value (which is most useful for system level debug to quickly determine device configuration)
* @param None
* @retval Value of the capacity in mAh
*/
uint16_t bq27441_g1_get_design_capacity(void);                //returns the Design Capacity Data Memory value

//void bq27441_g1_WriteDesignCapacity(uint16_t value);

//void bq27441_g1_WriteDataClass (uint8_t value);        //sets the data class to be accessed

/**
* @brief Setting the data block to be accessed. When 0x00 is written to BlockDataControl(), DataBlock() holds the block number of the data to be read or written.
* @param None
* @param [in] value value = 0x00 to access data located at offset 0 to 31, value = 0x01 to access data located at offset 32 to 41.
* @retval Value of the data block
*/
uint16_t bq27441_g1_get_data_block (uint8_t value);    //To access data located at offset 0 to 31, value = 0x00. To access data located at offset 32 to 41, value = 0x01.

//uint16_t bq27441_g1_block_data (void);                        // remainder of the 32-byte data block when accessing general block data

/**
* @brief Getting value of checksum on the 32 bytes of block data
* @param None
* @retval Value of checksum
*/
uint16_t bq27441_g1_GetChecksum (void);                    // contains the checksum on the 32 bytes of block data read or written.

void bq27441_g1_WriteChecksum (uint16_t value);

/**
* @brief Used to control the data access mode. Writing 0x00 to this command enables BlockData() to access to RAM.
* @param None
* @retval none
*/ 
void bq27441_g1_block_data_control (void);                    // control the data access mode.

#endif // BQ27441_H
