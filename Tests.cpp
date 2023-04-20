/**
 ******************************************************************************
 * @file    Tests.cpp
 * @date    20 May 2022
 * @brief   Tests
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


#include "cxd56_gpio.h"
#include "arch/board/board.h"
#include "spresense-exported-sdk/nuttx/include/unistd.h"
#include "spresense-exported-sdk/nuttx/include/nuttx/timers/pwm.h"

#include "Appdefines.h"
#include "libraries/Hardware/Max7317/Max7317.h"
#include "libraries/Drivers/CXD5602/I2c/I2c.h"
#include "libraries/Hardware/Hts221/Hts221.h"
#include "libraries/Hardware/Lis2mdl/Lis2mdl.h"
#include "libraries/Hardware/Lps22hh/Lps22hh.h"
#include "libraries/Hardware/Lsm6dso32/Lsm6dso32.h"
#include "libraries/Hardware/Vl53l1x/VL53L1X_api.h"
#include "libraries/Hardware/Sgp4x/sensirion_common.h"
#include "libraries/Hardware/Sgp4x/sensirion_i2c.h"
#include "libraries/Hardware/Sgp4x/sensirion_i2c_hal.h"
#include "libraries/Hardware/Sgp4x/sgp41_i2c.h"
#include "libraries/Hardware/Sgp4x/sgp40_i2c.h"
#include "libraries/Hardware/Apds9250/Apds9250.h"
#include "libraries/Drivers/CXD5602/Pwm/Pwm.h"
//#include "libraries/Drivers/CXD5602/LowPower/LowPower.h"
//#include "libraries/Hardware/Mp34dt05/Mp34dt05.h"
#include "libraries/Hardware/Sdcard/Sdcard.h"
#include "libraries/ThirdParty/Fatfs/ff.h"
#include "libraries/Hardware/Led/Led.h"
#include "libraries/Hardware/Button/Button.h"
#include "libraries/Drivers/CXD5602/Gnss/Gnss.h"
#include "libraries/Drivers/CXD5602/Uart/Uart.h"
#include "libraries/Drivers/CXD5602/Spi/Spi.h"
#include "libraries/Protocol/Protocol.h"
#include "libraries/Tools/Settings/Settings.h"
#include "libraries/Hardware/Bq27441/Bq27441.h"
#include "libraries/Hardware/M24c32/M24c32.h"
#include "libraries/Hardware/Microphone/Microphone.h"
#include "cxd56_gpio.h"
#include "arch/board/board.h"
#include "Appdefines.h"
// TESTS

//#define ALL_TESTS

#ifdef ALL_TESTS

#define HTS221_TEST
#define LIS2MDL_TEST
#define LPS22HH_TEST
#define LSM6DSO32_TEST
#define VL53L1_TEST
#define PWM_TEST
#define APDS9250_TEST
//#define LOW_POWER_TEST
#define SGP40_TEST
//#define SD_CARD_TEST
//#define FAT_FS_TEST
#define LED_TEST
#define BTN_TEST
#define GPS_TEST
#define UART_TEST
#define PROTOCOL_TEST
#define BQ27441_TEST
#define EEPROM_TEST


#else

//#define HTS221_TEST
//#define LIS2MDL_TEST
//#define LPS22HH_TEST
//#define LSM6DSO32_TEST
//#define VL53L1_TEST
//#define PWM_TEST
//#define SGP41_TEST
//#define APDS9250_TEST
//#define LOW_POWER_TEST
//#define SGP40_TEST
//#define SD_CARD_TEST
#define FAT_FS_TEST
//#define LED_TEST
//#define BTN_TEST
//#define GPS_TEST
//#define UART_TEST
//#define PROTOCOL_TEST
//#define BQ27441_TEST
//#define EEPROM_TEST
//#define MIC_GPIO_TEST


#endif



#ifdef HTS221_TEST

typedef struct {
    float x0;
    float y0;
    float x1;
    float y1;
} lin_t;

static uint8_t hts221_whoamI = 0;
static int16_t hts221_data_raw_humidity;
static int16_t hts221_data_raw_temperature;
static float hts221_humidity_perc;
static float hts221_temperature_degC;
static lin_t lin_hum;
static lin_t lin_temp;

static float linear_interpolation(lin_t *lin, int16_t x) {    
  return ((lin->y1 - lin->y0) * x + ((lin->x1 * lin->y0) -
                                     (lin->x0 * lin->y1)))
         / (lin->x1 - lin->x0);
}

static void hts221_initt(void) {
    i2c_init();
    hts221_device_id_get(nullptr, &hts221_whoamI);
    //printf("hts221_whoamI = %d\r\n", hts221_whoamI);

    /* Read humidity calibration coefficient */
    hts221_hum_adc_point_0_get(nullptr, &lin_hum.x0);
    hts221_hum_rh_point_0_get(nullptr, &lin_hum.y0);
    hts221_hum_adc_point_1_get(nullptr, &lin_hum.x1);
    hts221_hum_rh_point_1_get(nullptr, &lin_hum.y1);
    /* Read temperature calibration coefficient */
    hts221_temp_adc_point_0_get(nullptr, &lin_temp.x0);
    hts221_temp_deg_point_0_get(nullptr, &lin_temp.y0);
    hts221_temp_adc_point_1_get(nullptr, &lin_temp.x1);
    hts221_temp_deg_point_1_get(nullptr, &lin_temp.y1);
    /* Enable Block Data Update */
    hts221_block_data_update_set(nullptr, PROPERTY_ENABLE);
    /* Set Output Data Rate */
    hts221_data_rate_set(nullptr, HTS221_ODR_1Hz);
    /* Device power on */
    hts221_power_on_set(nullptr, PROPERTY_ENABLE);
}

static void hts221_loop(void) {
    /* Read output only if new value is available */
    hts221_reg_t reg;
    hts221_status_get(nullptr, &reg.status_reg);

    if (reg.status_reg.h_da) {
        /* Read humidity data */
        memset(&hts221_data_raw_humidity, 0x00, sizeof(int16_t));
        hts221_humidity_raw_get(nullptr, (uint8_t*) &hts221_data_raw_humidity);
        hts221_humidity_perc = linear_interpolation(&lin_hum, hts221_data_raw_humidity);
        printf("Hymidity_raw = %d\r\n", hts221_data_raw_humidity);
        //hts221_humidity_perc = 0.0 - hts221_humidity_perc;
        if (hts221_humidity_perc < 0.0) {
            hts221_humidity_perc = 0.0;
        }

        if (hts221_humidity_perc > 100.0) {
            hts221_humidity_perc = 100.0;
        }

        printf("Humidity [\%]:%.2lf\r\n", hts221_humidity_perc);
        reg.status_reg.h_da = 0;
    }

    if (reg.status_reg.t_da) {
        /* Read temperature data */
        memset(&hts221_data_raw_temperature, 0x00, sizeof(int16_t));
        hts221_temperature_raw_get(nullptr, (uint8_t*) &hts221_data_raw_temperature);
        hts221_temperature_degC = linear_interpolation(&lin_temp, hts221_data_raw_temperature);
        printf("Temperature_raw = %d\r\n", hts221_data_raw_temperature);
        printf("Temperature [degC]:%.2lf\r\n", hts221_temperature_degC );
        reg.status_reg.t_da = 0;
    }
}


static void hts221_test (void) {    
    hts221_init();
    while (1) {
        hts221_loop();
        usleep(1000 * 1000);
    }
}
#endif // HTS221_TEST


#ifdef LIS2MDL_TEST

static int16_t data_raw_magnetic[3];
static int16_t lis2_data_raw_temperature;
static float magnetic_mG[3];
static float lis2_temperature_degC;
static uint8_t lis2_whoamI, lis2_rst;

static void lis2mdl_test_init(void) {
    i2c_init();
    /* Check device ID */
    lis2mdl_device_id_get(nullptr, &lis2_whoamI);

    printf("LIS2MDL_ID = %d\r\n", lis2_whoamI);

    if (lis2_whoamI != LIS2MDL_ID) {
        while (1) {
            /* manage here device not found */
        }
    }

    /* Restore default configuration */
    lis2mdl_reset_set(nullptr, PROPERTY_ENABLE);

    do {
        lis2mdl_reset_get(nullptr, &lis2_rst);
    } while (lis2_rst);

    /* Enable Block Data Update */
    lis2mdl_block_data_update_set(nullptr, PROPERTY_ENABLE);
    /* Set Output Data Rate */
    lis2mdl_data_rate_set(nullptr, LIS2MDL_ODR_10Hz);
    /* Set / Reset sensor mode */
    lis2mdl_set_rst_mode_set(nullptr, LIS2MDL_SENS_OFF_CANC_EVERY_ODR);
    /* Enable temperature compensation */
    lis2mdl_offset_temp_comp_set(nullptr, PROPERTY_ENABLE);
    /* Set Low-pass bandwidth to ODR/4 */
    //lis2mdl_low_pass_bandwidth_set(nullptr, LIS2MDL_ODR_DIV_4);
    /* Set device in continuous mode */
    lis2mdl_operating_mode_set(nullptr, LIS2MDL_CONTINUOUS_MODE);
    /* Enable interrupt generation on new data ready */
    lis2mdl_drdy_on_pin_set(nullptr, PROPERTY_ENABLE);
}

static void lis2mdl_loop(void) {
    uint8_t reg;
    /* Read output only if new value is available */
    lis2mdl_mag_data_ready_get(nullptr, &reg);

    if (reg) {
        /* Read magnetic field data */
        memset(data_raw_magnetic, 0x00, 3 * sizeof(int16_t));
        lis2mdl_magnetic_raw_get(nullptr, (uint8_t*) data_raw_magnetic);
        magnetic_mG[0] = lis2mdl_from_lsb_to_mgauss( data_raw_magnetic[0]);
        magnetic_mG[1] = lis2mdl_from_lsb_to_mgauss( data_raw_magnetic[1]);
        magnetic_mG[2] = lis2mdl_from_lsb_to_mgauss( data_raw_magnetic[2]);
        printf("Mag field [mG]:%4.2f\t%4.2f\t%4.2f\r\n", magnetic_mG[0], magnetic_mG[1], magnetic_mG[2]);
        /* Read temperature data */
        memset(&lis2_data_raw_temperature, 0x00, sizeof(int16_t));
        lis2mdl_temperature_raw_get(nullptr, (uint8_t*) &lis2_data_raw_temperature);
        lis2_temperature_degC =
        lis2mdl_from_lsb_to_celsius(lis2_data_raw_temperature);
        printf("Temperature [degC]:%6.2f\r\n", lis2_temperature_degC);
    }
}

static void lis2mdl_test(void) {    
    lis2mdl_test_init();
    while (1) {
        lis2mdl_loop();
        usleep(1000 * 1000);
    }
}

#endif // LIS2MDL_TEST


#ifdef LPS22HH_TEST

/* Private variables ---------------------------------------------------------*/
static uint32_t data_raw_pressure;
static int16_t lps22_data_raw_temperature;
static float pressure_hPa;
static float lps22_temperature_degC;
static uint8_t lps22_whoamI, lps22_rst;

static int32_t platform_write(void *handle, uint8_t reg, const uint8_t *bufp,
                              uint16_t len);
static int32_t platform_read(void *handle, uint8_t reg, uint8_t *bufp,
                             uint16_t len);
static void tx_com( uint8_t *tx_buffer, uint16_t len );
static void platform_delay(uint32_t ms);
static void platform_init(void);

static void lps22hh_test_init(void) {
    i2c_init();
    /* Check device ID */
    lps22_whoamI = 0;
    lps22hh_device_id_get(nullptr, &lps22_whoamI);

    printf("LPS22HH_ID = %d", lps22_whoamI);

    if (lps22_whoamI != LPS22HH_ID ) {
        printf("LPS22HH_ID Init Error\r\n");
        while (1); /*manage here device not found */
    }

    /* Restore default configuration */
    lps22hh_reset_set(nullptr, PROPERTY_ENABLE);

    do {
        lps22hh_reset_get(nullptr, &lps22_rst);
    } while (lps22_rst);

    /* Enable Block Data Update */
    lps22hh_block_data_update_set(nullptr, PROPERTY_ENABLE);
    /* Set Output Data Rate */
    lps22hh_data_rate_set(nullptr, LPS22HH_10_Hz_LOW_NOISE);
}

static void lps22hh_loop(void) {    
    lps22hh_reg_t reg;
    /* Read output only if new value is available */
    lps22hh_read_reg(nullptr, LPS22HH_STATUS, (uint8_t *)&reg, 1);

    if (reg.status.p_da) {
        memset(&data_raw_pressure, 0x00, sizeof(uint32_t));
        lps22hh_pressure_raw_get(nullptr, (uint8_t *) &data_raw_pressure);
        pressure_hPa = lps22hh_from_lsb_to_hpa( data_raw_pressure);
        printf("pressure [hPa]:%6.2f\r\n", pressure_hPa);
    }

    if (reg.status.t_da) {
        memset(&lps22_data_raw_temperature, 0x00, sizeof(int16_t));
        lps22hh_temperature_raw_get(nullptr, (uint8_t *) &lps22_data_raw_temperature);
        lps22_temperature_degC = lps22hh_from_lsb_to_celsius(
                            lps22_data_raw_temperature );
        printf("temperature [degC]:%6.2f\r\n", lps22_temperature_degC );
    }
}

static void lps22hh_test(void) {    
    
    lps22hh_test_init();

    /* Read samples in polling mode (no int) */
    while (1) {
        lps22hh_loop();
        usleep(1000 * 1000);
    }
}

#endif // LPS22HH_TEST


#ifdef LSM6DSO32_TEST

static int16_t data_raw_acceleration[3];
static int16_t data_raw_angular_rate[3];
static int16_t lsm6_data_raw_temperature;
static float acceleration_mg[3];
static float angular_rate_mdps[3];
static float lsm6_temperature_degC;
static uint8_t lsm6_whoamI, rst;
static uint8_t tx_buffer[1000];
static stmdev_ctx_t dev_ctx;

/* Extern variables ----------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/* WARNING:
 * Functions declare in this section are defined at the end of this file
 * and are strictly related to the hardware platform used.
 */
static int32_t platform_write(void *handle, uint8_t reg, const uint8_t *bufp,
                              uint16_t len);
static int32_t platform_read(void *handle, uint8_t reg, uint8_t *bufp,
                             uint16_t len);
static void tx_com( uint8_t *tx_buffer, uint16_t len );
static void platform_delay(uint32_t ms);
static void platform_init(void);

static void lsm6dso32_initt(void) {
    
    i2c_init();
    /* Check device ID */
    lsm6dso32_device_id_get(&dev_ctx, &lsm6_whoamI);

    printf("LSM6DS32_ID = %d\r\n", lsm6_whoamI);

    if (lsm6_whoamI != LSM6DSO32_ID)
    while (1);

    /* Restore default configuration */
    lsm6dso32_reset_set(&dev_ctx, PROPERTY_ENABLE);

    do {
        lsm6dso32_reset_get(&dev_ctx, &rst);
    } while (rst);

    /* Disable I3C interface */
    lsm6dso32_i3c_disable_set(&dev_ctx, LSM6DSO32_I3C_DISABLE);
    /* Enable Block Data Update */
    lsm6dso32_block_data_update_set(&dev_ctx, PROPERTY_ENABLE);
    /* Set full scale */
    lsm6dso32_xl_full_scale_set(&dev_ctx, LSM6DSO32_4g);
    lsm6dso32_gy_full_scale_set(&dev_ctx, LSM6DSO32_2000dps);
    /* Set ODR (Output Data Rate) and power mode*/
    lsm6dso32_xl_data_rate_set(&dev_ctx, LSM6DSO32_XL_ODR_12Hz5_LOW_PW);
    lsm6dso32_gy_data_rate_set(&dev_ctx,
                                LSM6DSO32_GY_ODR_12Hz5_HIGH_PERF);
}

static void lsm6dso32_loop(void) {
    lsm6dso32_reg_t reg;
    /* Read output only if new data is available */
    lsm6dso32_status_reg_get(&dev_ctx, &reg.status_reg);

    if (reg.status_reg.xlda) {
        /* Read acceleration data */
        memset(data_raw_acceleration, 0x00, 3 * sizeof(int16_t));
        lsm6dso32_acceleration_raw_get(&dev_ctx, (uint8_t*) data_raw_acceleration);
        acceleration_mg[0] =
        lsm6dso32_from_fs4_to_mg(data_raw_acceleration[0]);
        acceleration_mg[1] =
        lsm6dso32_from_fs4_to_mg(data_raw_acceleration[1]);
        acceleration_mg[2] =
        lsm6dso32_from_fs4_to_mg(data_raw_acceleration[2]);
        printf("Acceleration [mg]:%4.2f\t%4.2f\t%4.2f\r\n",
                acceleration_mg[0], acceleration_mg[1], acceleration_mg[2]);
    }

    if (reg.status_reg.gda) {
        /* Read angular rate field data */
        memset(data_raw_angular_rate, 0x00, 3 * sizeof(int16_t));
        lsm6dso32_angular_rate_raw_get(&dev_ctx, (uint8_t*) data_raw_angular_rate);
        angular_rate_mdps[0] =
        lsm6dso32_from_fs2000_to_mdps(data_raw_angular_rate[0]);
        angular_rate_mdps[1] =
        lsm6dso32_from_fs2000_to_mdps(data_raw_angular_rate[1]);
        angular_rate_mdps[2] =
        lsm6dso32_from_fs2000_to_mdps(data_raw_angular_rate[2]);
        printf("Angular rate [mdps]:%4.2f\t%4.2f\t%4.2f\r\n",
                angular_rate_mdps[0], angular_rate_mdps[1], angular_rate_mdps[2]);
    }

    if (reg.status_reg.tda) {
        /* Read temperature data */
        memset(&lsm6_data_raw_temperature, 0x00, sizeof(int16_t));
        lsm6dso32_temperature_raw_get(&dev_ctx, (uint8_t*) &lsm6_data_raw_temperature);
        lsm6_temperature_degC = lsm6dso32_from_lsb_to_celsius(
                            lsm6_data_raw_temperature);
        printf("Temperature [degC]:%6.2f\r\n", lsm6_temperature_degC);
    }
}

static void lsm6dso32_test(void) {
    lsm6dso32_init();

    /* Read samples in polling mode (no int) */
    while (1) {
        lsm6dso32_loop();
        usleep(1000 * 1000);
    }
}

#endif // LSM6DSO32_TEST

#ifdef VL53L1_TEST

static uint8_t sensorState = 0;
static uint8_t first_range = 1;
static int status;
static uint8_t byteData;
static uint16_t wordData;
static uint16_t Dev = 0;
static VL53L1X_Result_t Results;

static void vl53l1_init(void) {
    i2c_init();
    usleep(1000 * 1000);

    status = VL53L1_RdByte(Dev, 0x010F, &byteData);
    printf("VL53L1X Model_ID: %X\n", byteData);
    status += VL53L1_RdByte(Dev, 0x0110, &byteData);
    printf("VL53L1X Module_Type: %X\n", byteData);
    status += VL53L1_RdByte(Dev, 0x0111, &byteData);
    printf("VL53L1X Revision: %X\n", byteData);
    while (sensorState == 0) {
        status += vl53l1x_boot_state(Dev, &sensorState);
        VL53L1_WaitMs(Dev, 20);
    }
    printf("Chip booted\n");

    status = vl53l1x_sensor_init(Dev);
    /* status += vl53l1x_setInterrupt_polarity(Dev, 0); */
    status += vl53l1x_set_distance_mode(Dev, 2); /* 1=short, 2=long */
    status += vl53l1x_set_timing_budgetIn_ms(Dev, 100);
    status += vl53l1x_set_inter_measurementIn_ms(Dev, 100);
    status += vl53l1x_start_ranging(Dev);
}

static void vl53l1_loop(void) {
    static uint8_t dataReady = 0;

    while (dataReady == 0) {
        status = vl53l1x_check_for_data_ready(Dev, &dataReady);
        //printf("status = %u, dataReady = %u\r\n", status, dataReady);
        usleep(20 * 1000);
    }
    dataReady = 0;
    
    /* Get the data the new way */
    status += vl53l1x_get_result(Dev, &Results);

    printf("Status = %2d, dist = %5d, Ambient = %2d, Signal = %5d, #ofSpads = %5d\n",
        Results.Status, Results.Distance, Results.Ambient,
                            Results.SigPerSPAD, Results.NumSPADs);

    /* trigger next ranging */
    status += vl53l1x_clear_interrupt(Dev);
    if (first_range) {
        /* very first measurement shall be ignored
            * thus requires twice call
            */
        status += vl53l1x_clear_interrupt(Dev);
        first_range = 0;
    }
    status = vl53l1x_sensor_init(Dev);
    status += vl53l1x_set_distance_mode(Dev, 2); /* 1=short, 2=long */
    status += vl53l1x_set_timing_budgetIn_ms(Dev, 100);
    status += vl53l1x_set_inter_measurementIn_ms(Dev, 100);
    status += vl53l1x_start_ranging(Dev);
}

static void vl53l1_test (void) {    

    vl53l1_init();

    while (1) {
        vl53l1_loop();
        usleep(1000 * 1000);
    }

}

#endif // VL53L1_TEST

#ifdef SGP41_TEST


// Parameters for deactivated humidity compensation:
static uint16_t default_rh = 0x8000;
static uint16_t default_t = 0x6666;
static int16_t error = 0;

static void sgp41_init(void) {
    
    sensirion_i2c_hal_init();

    uint16_t serial_number[3];
    uint8_t serial_number_size = 3;

    error = sgp41_get_serial_number(serial_number, serial_number_size);
    if (error) {
        printf("Error executing sgp41_get_serial_number(): %i\n", error);
    } else {
        printf("serial: 0x%04x%04x%04x\n", serial_number[0], serial_number[1],
               serial_number[2]);
    }

    uint16_t test_result;

    error = sgp41_execute_self_test(&test_result);
    if (error) {
        printf("Error executing sgp41_execute_self_test(): %i\n", error);
    } else {
        printf("Test result: %u\n", test_result);
    }

    // sgp41 conditioning during 10 seconds before measuring
    for (int i = 0; i < 10; i++) {
        uint16_t sraw_voc;

        sensirion_i2c_hal_sleep_usec(1000000);

        error = sgp41_execute_conditioning(default_rh, default_t, &sraw_voc);
        if (error) {
            printf("Error executing sgp41_execute_conditioning(): "
                   "%i\n",
                   error);
        } else {
            printf("SRAW VOC: %u\n", sraw_voc);
            printf("SRAW NOx: conditioning\n");
        }
    }
}

static void sgp41_loop(void) {
    uint16_t sraw_voc;
    uint16_t sraw_nox;


    error = sgp41_measure_raw_signals(default_rh, default_t, &sraw_voc,
                                    &sraw_nox);
    if (error) {
        printf("Error executing sgp41_measure_raw_signals(): "
            "%i\n",
            error);
    } else {
        printf("SRAW VOC: %u\n", sraw_voc);
        printf("SRAW NOx: %u\n", sraw_nox);
    }
}

static void sgp41_test (void) {    
    sgp41_init();

    while (1) {
        sgp41_loop();
        usleep(1000 * 1000);
    }
}

#endif

#ifdef APDS9250_TEST

Apds9250 test_pds9250;

static void apds9259_init(void) {
    i2c_init();
    if (test_pds9250.begin())
    {
        printf("test_pds9250.begin() OK\r\n");
    }

    test_pds9250.setMode(modeColorSensor);
    test_pds9250.setResolution(res18bit);
    test_pds9250.setGain(gain1);
    test_pds9250.setMeasurementRate(rate100ms);
}

static void apds9259_loop(void) {
    uint32_t red = 0;
    uint32_t green = 0;
    uint32_t blue = 0;
    uint32_t ir = 0;
    test_pds9250.getAll(&red, &green, &blue, &ir);
    printf("red = %lu\r\ngreen = %lu\r\nblue = %lu\r\nir = %lu\r\n", red, green, blue, ir); 
}

static void apds9259_test (void) {    
    apds9259_init();
    while (1) {
        apds9259_loop();
        usleep(1000 * 1000);
    }
}

#endif // APDS9250_TEST

#ifdef PWM_TEST

static void pwm_loop(void) {
    pwm_run(10, 20, 500);
    while (pwm_act()) {}
    pwm_run(100, 80, 500);
    while (pwm_act()) {}
    pwm_run(1000, 50, 500);
    while (pwm_act()) {}
}

static void pwm_test (void) {    
    pwm_init();
    
    while (1) {    
        pwm_loop();
    }
}

#endif // PWM_TEST

#ifdef LOW_POWER_TEST

static void low_power_test (void) 
{
    board_gpio_config(GPIO_LED1, 0, false, true, 0);
    cxd56_gpio_write(GPIO_LED2, true);
    usleep(1000 * 1000);
    cxd56_gpio_write(GPIO_LED2, false);
    for (uint16_t i = 0; i < 500; i++) {
        usleep(500);
    }
    cxd56_gpio_write(GPIO_LED2, true);
    usleep(1000 * 1000);
    cxd56_gpio_write(GPIO_LED2, false);
    LowPower.begin();
    while (1) {
        LowPower.deepSleep();
    }
}

#endif // LOW_POWER_TEST

#ifdef SGP40_TEST

static int16_t error = 0;
static uint16_t serial_number[3];
static uint8_t serial_number_size = 3;
// Parameters for deactivated humidity compensation:
static uint16_t default_rh = 0x8000;
static uint16_t default_t = 0x6666;

static void sgp40_test_init(void) {
    i2c_init();
    error = sgp40_get_serial_number(serial_number, serial_number_size);

    if (error) {
        printf("Error executing sgp40_get_serial_number(): %i\r\n", error);
    } else {
        printf("serial: 0x%04x%04x%04x\r\n", serial_number[0], serial_number[1],
                serial_number[2]);
        printf("\r\n");
    }
}

static void sgp40_loop(void) {
    uint16_t sraw_voc;
    error = sgp40_measure_raw_signal(default_rh, default_t, &sraw_voc);
    if (error) {
            printf("Error executing sgp40_measure_raw_signal(): %i\n", error);
    } else {
        printf("SRAW VOC: %u\n", sraw_voc);
    }
}

static void sgp40_test(void) {    

    sgp40_test_init();

    while (1) {
        sgp40_loop();
        usleep(1000 * 1000);
    }
}

#endif // SGP40_TEST

#ifdef SD_CARD_TEST



static void sd_card_test(void) {    
    int result = sdcard_init();
    if (result == 0) {    
        printf("SD card init OK\r\n");
    } else {
        printf("SD card init FALSE, result = %d\r\n", result);
        while (1) {}
    }
    uint32_t blocks = 0;
    sdcard_get_blocks_number(&blocks);
    printf("Bloks = %u\r\n", blocks);
    uint8_t blockData [512] = {0};
    uint8_t blockDataWr [512] = {0};
    for (uint8_t i = 0; i < 100; i++) {    
        blockDataWr[i] = i;
    }
    sdcard_write_single_block(0, blockDataWr);
    sdcard_read_single_block(0, blockData);
    for (uint8_t i = 0; i < 64; i++) {    
        for (uint8_t j = 0; j < 8; j++) {    
            uint8_t data = blockData[i*8 + j];
            //if ((data >= ' ') && (data <= '~')) {    
            printf("%02X ",data);
        }
        printf("\r\n");
    }
    while (1)
    {

    }

}

#endif // SD_CARD_TEST

#ifdef FAT_FS_TEST

static uint8_t retUSER = 0;    /* Return value for USER */
static char USERPath[4] = {"0:/"};   /* USER logical drive path */
static FATFS USERFatFS = {0};    /* File system object for USER logical drive */
static FIL USERFile = {0};       /* File object for USER */

static void fatfs_test (void) {    
    int result = sdcard_init();
    if( f_mount(&USERFatFS,(TCHAR const*)USERPath, 0) != FR_OK) {    
        printf("FS mount error\r\n");
    }
    printf("FS %s driver mounted\r\n", USERPath);
    char FilePath[32]={0};

    strcat (FilePath, USERPath);
    strcat (FilePath, "hello.txt");

    if(f_open(&USERFile, FilePath, FA_CREATE_ALWAYS | FA_WRITE) !=FR_OK) {    
        printf("FS open file false\r\n");
    } else {
        UINT byteswritten = 0;
        retUSER = f_write(&USERFile, "Hello", strlen("Hello"), &byteswritten);
        if((byteswritten == 0) || (retUSER != FR_OK)) {    
            f_close(&USERFile);
            printf("FS write FALSE\r\n");
        } else {
            f_close(&USERFile);
            printf("FS write OK\r\n");
        }
    }
    while (1)
    {
        /* code */
    }
}

#endif // FAT_FS_TEST

#ifdef LED_TEST

static void led_loop(void) {
    led_green_off();
    usleep(1000);
    led_red_on();
    usleep(1000 * 1000);
    led_red_off();
    usleep(1000);
    led_green_on();
    usleep(1000 * 1000);
}

static void led_test(void) {
    led_init();
    while (1) {
        led_loop();
    }
}

#endif // LED_TEST

#ifdef BTN_TEST

static bool old_btn_state = false;

static void btn_init(void) {
    button_init();
}

static void btn_loop(void) {
    bool btn_state = button_is_pressed();
    if (btn_state != old_btn_state) {
        old_btn_state = btn_state;
        if (btn_state == false) {
            printf("Btn_relased\r\n");
        } else {
            printf("Btn_pressed\r\n");
        }
    }
}

static void btn_test(void) {
    
    btn_init();
    while (1) {
        btn_loop();
    }
}

#endif // BTN_TEST

#ifdef GPS_TEST

#define STRING_BUFFER_SIZE  128       /**< %Buffer size */

#define RESTART_CYCLE       (60 * 5)  /**< positioning test term */

static SpGnss gnss;                   /**< SpGnss object */

/**
 * @enum ParamSat
 * @brief Satellite system
 */
enum ParamSat {
    eSatGps,            /**< GPS                     World wide coverage  */
    eSatGlonass,        /**< GLONASS                 World wide coverage  */
    eSatGpsSbas,        /**< GPS+SBAS                North America        */
    eSatGpsGlonass,     /**< GPS+Glonass             World wide coverage  */
    eSatGpsBeidou,      /**< GPS+BeiDou              World wide coverage  */
    eSatGpsGalileo,     /**< GPS+Galileo             World wide coverage  */
    eSatGpsQz1c,        /**< GPS+QZSS_L1CA           East Asia & Oceania  */
    eSatGpsGlonassQz1c, /**< GPS+Glonass+QZSS_L1CA   East Asia & Oceania  */
    eSatGpsBeidouQz1c,  /**< GPS+BeiDou+QZSS_L1CA    East Asia & Oceania  */
    eSatGpsGalileoQz1c, /**< GPS+Galileo+QZSS_L1CA   East Asia & Oceania  */
    eSatGpsQz1cQz1S,    /**< GPS+QZSS_L1CA+QZSS_L1S  Japan                */
};

/* Set this parameter depending on your current region. */
static enum ParamSat satType =  eSatGps;

/**
 * @brief %Print position information.
 */
static void print_pos(SpNavData *pNavData) {
    char StringBuffer[STRING_BUFFER_SIZE];

    /* print time */
    snprintf(StringBuffer, STRING_BUFFER_SIZE, "%04d/%02d/%02d ", pNavData->time.year, pNavData->time.month, pNavData->time.day);
    printf(StringBuffer);

    snprintf(StringBuffer, STRING_BUFFER_SIZE, "%02d:%02d:%02d.%06ld, ", pNavData->time.hour, pNavData->time.minute, pNavData->time.sec, pNavData->time.usec);
    printf(StringBuffer);

    /* print satellites count */
    snprintf(StringBuffer, STRING_BUFFER_SIZE, "numSat:%2d, ", pNavData->numSatellites);
    printf(StringBuffer);

    /* print position data */
    if (pNavData->posFixMode == FixInvalid) {
        printf("No-Fix, ");
    } else {
        printf("Fix, ");
    }
    if (pNavData->posDataExist == 0) {
        printf("No Position");
    } else {
        printf("Lat=");
        printf("%f", pNavData->latitude, 6);
        printf(", Lon=");
        printf("%f", pNavData->longitude, 6);
    }

    printf(" \r\n");
}

/**
 * @brief %Print satellite condition.
 */
static void print_condition(SpNavData *pNavData) {
    char StringBuffer[STRING_BUFFER_SIZE];
    unsigned long cnt;

    /* Print satellite count. */
    snprintf(StringBuffer, STRING_BUFFER_SIZE, "numSatellites:%2d\n", pNavData->numSatellites);
    printf(StringBuffer);

    for (cnt = 0; cnt < pNavData->numSatellites; cnt++) {
        const char *pType = "---";
        SpSatelliteType sattype = pNavData->getSatelliteType(cnt);

        /* Get satellite type. */
        /* Keep it to three letters. */
        switch (sattype)
        {
        case GPS:
            pType = "GPS";
            break;

        case GLONASS:
            pType = "GLN";
            break;

        case QZ_L1CA:
            pType = "QCA";
            break;

        case SBAS:
            pType = "SBA";
            break;

        case QZ_L1S:
            pType = "Q1S";
            break;

        case BEIDOU:
            pType = "BDS";
            break;

        case GALILEO:
            pType = "GAL";
            break;

        default:
            pType = "UKN";
            break;
        }

        /* Get print conditions. */
        unsigned long Id  = pNavData->getSatelliteId(cnt);
        unsigned long Elv = pNavData->getSatelliteElevation(cnt);
        unsigned long Azm = pNavData->getSatelliteAzimuth(cnt);
        float sigLevel = pNavData->getSatelliteSignalLevel(cnt);

        /* Print satellite condition. */
        snprintf(StringBuffer, STRING_BUFFER_SIZE, "[%2ld] Type:%s, Id:%2ld, Elv:%2ld, Azm:%3ld, CN0:", cnt, pType, Id, Elv, Azm );
        printf(StringBuffer);
        printf("%f %d\r\n", sigLevel, 6);
    }
}




static void gps_test_init(void) {
    int error_flag = 0;
    usleep(1000 * 3000);
    gnss.setDebugMode(PrintInfo);
    int result;
      /* Activate GNSS device */
    result = gnss.begin();

    if (result != 0) {
        printf("Gnss begin error!!\r\n");
        error_flag = 1;
    } else {
        /* Setup GNSS
        *  It is possible to setup up to two GNSS satellites systems.
        *  Depending on your location you can improve your accuracy by selecting different GNSS system than the GPS system.
        *  See: https://developer.sony.com/develop/spresense/developer-tools/get-started-using-nuttx/nuttx-developer-guide#_gnss
        *  for detailed information.
        */
        switch (satType) {
            case eSatGps:
            gnss.select(GPS);
            break;

            case eSatGpsSbas:
            gnss.select(GPS);
            gnss.select(SBAS);
            break;

            case eSatGlonass:
            gnss.select(GLONASS);
            break;

            case eSatGpsGlonass:
            gnss.select(GPS);
            gnss.select(GLONASS);
            break;

            case eSatGpsBeidou:
            gnss.select(GPS);
            gnss.select(BEIDOU);
            break;

            case eSatGpsGalileo:
            gnss.select(GPS);
            gnss.select(GALILEO);
            break;

            case eSatGpsQz1c:
            gnss.select(GPS);
            gnss.select(QZ_L1CA);
            break;

            case eSatGpsQz1cQz1S:
            gnss.select(GPS);
            gnss.select(QZ_L1CA);
            gnss.select(QZ_L1S);
            break;

            case eSatGpsBeidouQz1c:
            gnss.select(GPS);
            gnss.select(BEIDOU);
            gnss.select(QZ_L1CA);
            break;

            case eSatGpsGalileoQz1c:
            gnss.select(GPS);
            gnss.select(GALILEO);
            gnss.select(QZ_L1CA);
            break;

            case eSatGpsGlonassQz1c:
            default:
            gnss.select(GPS);
            gnss.select(GLONASS);
            gnss.select(QZ_L1CA);
            break;
        }

        /* Start positioning */
        result = gnss.start(COLD_START);
        if (result != 0) {
            printf("Gnss start error!!\r\n");
            error_flag = 1;
        } else {
            printf("Gnss setup OK\r\n");
        }
    }
}

static void gps_loop(void) {
    static int LoopCount = 0;
    static int LastPrintMin = 0;
    /* Check update. */
    if (gnss.waitUpdate(-1)) {
        /* Get NaviData. */
        SpNavData NavData;
        gnss.getNavData(&NavData);

        /* Set posfix LED. */
        bool LedSet = (NavData.posDataExist && (NavData.posFixMode != FixInvalid));

        /* Print satellite information every minute. */
        if (NavData.time.minute != LastPrintMin) {
            print_condition(&NavData);
            LastPrintMin = NavData.time.minute;
        }

        /* Print position information. */
        print_pos(&NavData);
    } else {
        /* Not update. */
        printf("data not update\r\n");
    }

    /* Check loop count. */
    LoopCount++;
    if (LoopCount >= RESTART_CYCLE) {
        int error_flag = 0;

        /* Restart gnss. */
        if (gnss.stop() != 0) {
            printf("Gnss stop error!!\r\n");
            error_flag = 1;
        } else if (gnss.end() != 0) {
            printf("Gnss end error!!\r\n");
            error_flag = 1;
        } else {
            printf("Gnss stop OK.\r\n");
        }

        if (gnss.begin() != 0) {
            printf("Gnss begin error!!\r\n");
            error_flag = 1;
        } else if (gnss.start(HOT_START) != 0) {
            printf("Gnss start error!!\r\n");
            error_flag = 1;
        } else {
            printf("Gnss restart OK.\r\n");
        }

        LoopCount = 0;

        if (error_flag == 1) {
            printf("Gnss Error\r\n");
            while (1) {
                
            }
        }
    }
}

static void gps_test(void) {
    
    gps_test_init();
    while (1) {
        gps_loop();
    }
}

#endif // GPS_TEST

#ifdef UART_TEST

static void uart_loop (void) {
    uint16_t rx_size = uart_act();
    if (rx_size != 0) {
        char data[UART_RX_BUFER_SIZE];
        uint16_t size = uart_get_data(data);
        uart_send_data(data, size);
    }
    usleep(1000*500);
}

static void uart_test (void) {
    uart_init();
    while (1)
    {
        uart_loop();
    }
    
}


#endif // UART_TEST


#ifdef PROTOCOL_TEST

static void protocol_loop (void) {
    protocol_act();
}

static void protocol_test (void) {
    protocol_init();
    while (1)
    {
        protocol_act();
    }
    
}


#endif // PROTOCOL_TEST


#ifdef BQ27441_TEST

static void bq27441_loop (void) {
    bq27441_g1_get_control_status();
    float temp = bq27441_g1_get_temperature();
    uint16_t soc = bq27441_g1_get_state_of_charge_unfiltered();
    bq27441_g1_get_nominal_available_capacity();

    uint16_t devt = bq27441_g1_get_device_type();
    //fwv = bq27441_g1_GetFwVersion();
    //code = bq27441_g1_GetDmCode();
    //chem = bq27441_g1_GetChemId();


    bq27441_g1_get_flags();
    uint16_t descap = bq27441_g1_get_design_capacity();
    bq27441_g1_get_state_of_health();
    uint16_t cur = bq27441_g1_get_average_current();
    bq27441_g1_get_op_config();
    uint16_t remcap = bq27441_g1_get_remaining_capacity_unfiltered();
    uint16_t fullcap = bq27441_g1_get_full_charge_capacity_unfiltered();
    printf("bq27441_g1_get_temperature = %f\r\n", temp);
    printf("bq27441_g1_get_state_of_charge_unfiltered = %d\r\n", soc);
    printf("bq27441_g1_get_device_type = %d\r\n", devt);
    printf("bq27441_g1_get_design_capacity = %d\r\n", descap);
    printf("bq27441_g1_get_average_current = %d\r\n", cur);
    printf("bq27441_g1_get_remaining_capacity_unfiltered = %d\r\n", remcap);
    printf("bq27441_g1_get_full_charge_capacity_unfiltered = %d\r\n", fullcap);
}

static void bq27441_test (void) {
    bq27441_init();
    while (1) {
        bq27441_loop();
        usleep(1000 * 1000);
    }
    
}


#endif // BQ27441_TEST

#ifdef EEPROM_TEST

static void eeprom_test (void) {
    uint8_t blockDataWr [100] = {0};
    for (uint8_t i = 0; i < sizeof(blockDataWr); i++) {    
        blockDataWr[i] = i;
    }
    printf("m24c32_init:\r\n");
    m24c32_init();
    for (uint8_t i = 0; i < sizeof(blockDataWr); i++) {  
        m24c32_write_byte(i, blockDataWr[i]);
        //usleep(200 * 1000);
    }
    printf("EEPROM data:\r\n");
    for (uint8_t i = 0; i < sizeof(blockDataWr); i++) {  
        if ((i % 8) == 0) {
            printf("\r\n");
        }     
        uint8_t data = m24c32_read_byte(i);  
        printf("%02X ",data);
    }
    
}

#endif // EEPROM_TEST

#ifdef MIC_GPIO_TEST

#define MIC_DATA_SIZE 2048
uint16_t mic_data[MIC_DATA_SIZE];

static void mic_gpio_test(void) {
    

    uint8_t retUSER = 0;    /* Return value for USER */
    char USERPath[4] = {"0:/"};   /* USER logical drive path */
    FATFS USERFatFS = {0};    /* File system object for USER logical drive */
    FIL USERFile = {0};       /* File object for USER */
    char FilePath[32]={0};
    bool measure_run = false;


    if (sdcard_init() == 0) {
        printf("SD card OK\r\n");
    } else {
        printf("SD card FALSE\r\n");
    }
    if( f_mount(&USERFatFS,(TCHAR const*)USERPath, 0) != FR_OK) {    
        printf("FS mount FALSE\r\n");
    } else {
        printf("FS mount OK\r\n");
    }
    printf("Recording START\r\n");
    microphone_gpio_simple_test(mic_data, MIC_DATA_SIZE);
    printf("Recording DONE\r\n");
    printf("Printing data:\r\n");
    for(volatile uint32_t i = 0; i < MIC_DATA_SIZE; i++) {
        if ((i % 16) == 0) {
            printf(" \r\n");
        }
        printf ("%04X ", mic_data[i]);
    }
    printf(" \r\n");
    sprintf(FilePath, "%s%s\0", USERPath, "mic.wav");
    retUSER = f_open(&USERFile, FilePath, FA_WRITE | FA_OPEN_ALWAYS);
    if (retUSER == FR_OK) {
        printf("File write START\r\n");
        microphone_write_to_wav(&USERFile, (uint8_t*) mic_data, sizeof(mic_data));
        f_close(&USERFile);
        printf("File write DONE\r\n");
    } else {
        printf("File open false\r\n");
    }

}

#endif // MIC_GPIO_TEST

void tests (void) {
    printf("tests();\r\n");
#ifdef ALL_TESTS
    hts221_init();
    usleep(1000 * 100);
    lis2mdl_test_init();
    usleep(1000 * 100);
    lps22hh_test_init();
    usleep(1000 * 100);
    lsm6dso32_init();
    usleep(1000 * 100);
    vl53l1_init();
    usleep(1000 * 100);
    apds9259_init();
    usleep(1000 * 100);
    pwm_init();
    sgp40_test_init();
    usleep(1000 * 100);
    led_init();
    btn_init();
    gps_test_init();
    uart_init();
    protocol_init();
    bq27441_init();
    eeprom_test();
    while(1) {
        printf("hts221:\r\n");
        hts221_loop();
        printf("lis2mdl:\r\n");
        lis2mdl_loop();
        printf("lps22hh:\r\n");
        lps22hh_loop();
        printf("lsm6dso32:\r\n");
        lsm6dso32_loop();
        printf("vl53l1:\r\n");
        vl53l1_loop();
        printf("apds9259:\r\n");
        apds9259_loop();
        printf("pwm:\r\n");
        pwm_loop();
        printf("sgp40:\r\n");
        sgp40_loop();
        // printf("led:\r\n");
        // led_loop();
        // printf("btn:\r\n");
        // btn_loop();
        printf("gps:\r\n");
        gps_loop();
        //uart_act();
        protocol_act();
        printf("bq27441:\r\n");
        bq27441_loop();
        protocol_loop();
        usleep(1000 * 1000);
    }


#else
    
#ifdef HTS221_TEST

    hts221_test ();

#endif // HTS221_TEST

#ifdef LIS2MDL_TEST
    lis2mdl_test();
#endif // LIS2MDL_TEST

#ifdef LPS22HH_TEST

    lps22hh_test();

#endif // LPS22HH_TEST

#ifdef LSM6DSO32_TEST

    lsm6dso32_test();

#endif // LSM6DSO32_TEST

#ifdef VL53L1_TEST

    vl53l1_test();

#endif // VL53L1_TEST

#ifdef SGP41_TEST

    sgp41_test();
    
#endif // SGP41_TEST

#ifdef APDS9250_TEST

    apds9259_test();

#endif // APDS9250_TEST

#ifdef PWM_TEST

    pwm_test();

#endif // PWM_TEST

#ifdef LOW_POWER_TEST

    low_power_test();

#endif // LOW_POWER_TEST

#ifdef SGP40_TEST

    sgp40_test();

#endif // SGP40_TEST

#ifdef MP34DT05_TEST

    mp34dt05_test ();

#endif // MP34DT05_TEST

#ifdef SD_CARD_TEST

    sd_card_test();

#endif // SD_CARD_TEST

#ifdef FAT_FS_TEST

    fatfs_test ();

#endif // FAT_FS_TEST

#ifdef LED_TEST

    led_test();

#endif // LED_TEST

#ifdef BTN_TEST

    btn_test();

#endif // BTN_TEST

#ifdef GPS_TEST

    gps_test();

#endif // GPS_TEST

#ifdef UART_TEST

    uart_test();

#endif // UART_TEST

#ifdef PROTOCOL_TEST

    protocol_test();

#endif // PROTOCOL_TEST

#ifdef BQ27441_TEST

    bq27441_test ();

#endif // BQ27441_TEST

#ifdef EEPROM_TEST
    printf("EERPOM TEST\r\n");
    eeprom_test();
    while(1);

#endif // EEPROM_TEST

#ifdef MIC_GPIO_TEST
    mic_gpio_test();
    while(1);
#endif // MIC_GPIO_TEST

#endif // ALL_TESTS
}