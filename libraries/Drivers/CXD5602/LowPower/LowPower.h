/**
 ******************************************************************************
 * @file    LowPower.h
 * @date    29 March 2022
 * @brief   LowPower driver
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

#ifndef _LOW_POWER_H_
#define _LOW_POWER_H_

#ifdef SUBCORE
#error "LowPower library is NOT supported by SubCore."
#endif

/**
 * @file LowPower.h
 * @author Sony Semiconductor Solutions Corporation
 * @brief Spresense Arduino Low Power library
 *
 * @details The Low Power library can manage the low power states of Spresense.
 */

/**
 * @defgroup lowpower Low Power Library API
 * @brief API for using Low Power API
 * @{
 */

#include <stdint.h>
//#include <arch/chip/pm.h>

#include <../../../../spresense-exported-sdk/nuttx/include/arch/chip/pm.h>

typedef enum {
    POR_SUPPLY      = 0,  /**< Power On Reset with Power Supplied */
    WDT_REBOOT      = 1,  /**< System WDT expired or Self Reboot */
    WDT_RESET       = 2,  /**< Chip WDT expired */
    DEEP_WKUPL      = 3,  /**< In DeepSleep state, Detected WKUPL signal */
    DEEP_WKUPS      = 4,  /**< In DeepSleep state, Detected WKUPS signal */
    DEEP_RTC        = 5,  /**< In DeepSleep state, RTC Alarm expired */
    DEEP_USB_ATTACH = 6,  /**< In DeepSleep state, USB Connected */
    DEEP_OTHERS     = 7,  /**< In DeepSleep state, Reserved others cause occurred */
    COLD_SCU_INT    = 8,  /**< In ColdSleep state, Detected SCU Interrupt */
    COLD_RTC_ALM0   = 9,  /**< In ColdSleep state, RTC Alarm0 expired */
    COLD_RTC_ALM1   = 10, /**< In ColdSleep state, RTC Alarm1 expired */
    COLD_RTC_ALM2   = 11, /**< In ColdSleep state, RTC Alarm2 expired */
    COLD_RTC_ALMERR = 12, /**< In ColdSleep state, RTC Alarm Error occurred */
    COLD_GPIO_IRQ36 = 16, /**< In ColdSleep state, Detected GPIO IRQ 36 */
    COLD_GPIO_IRQ37 = 17, /**< In ColdSleep state, Detected GPIO IRQ 37 */
    COLD_GPIO_IRQ38 = 18, /**< In ColdSleep state, Detected GPIO IRQ 38 */
    COLD_GPIO_IRQ39 = 19, /**< In ColdSleep state, Detected GPIO IRQ 39 */
    COLD_GPIO_IRQ40 = 20, /**< In ColdSleep state, Detected GPIO IRQ 40 */
    COLD_GPIO_IRQ41 = 21, /**< In ColdSleep state, Detected GPIO IRQ 41 */
    COLD_GPIO_IRQ42 = 22, /**< In ColdSleep state, Detected GPIO IRQ 42 */
    COLD_GPIO_IRQ43 = 23, /**< In ColdSleep state, Detected GPIO IRQ 43 */
    COLD_GPIO_IRQ44 = 24, /**< In ColdSleep state, Detected GPIO IRQ 44 */
    COLD_GPIO_IRQ45 = 25, /**< In ColdSleep state, Detected GPIO IRQ 45 */
    COLD_GPIO_IRQ46 = 26, /**< In ColdSleep state, Detected GPIO IRQ 46 */
    COLD_GPIO_IRQ47 = 27, /**< In ColdSleep state, Detected GPIO IRQ 47 */
    COLD_SEN_INT    = 28, /**< In ColdSleep state, Detected SEN_INT Interrupt */
    COLD_PMIC_INT   = 29, /**< In ColdSleep state, Detected PMIC Interrupt */
    COLD_USB_DETACH = 30, /**< In ColdSleep state, USB Disconnected */
    COLD_USB_ATTACH = 31, /**< In ColdSleep state, USB Connected */
    POR_NORMAL      = 32, /**< Power On Reset */
} bootcause_e;

typedef enum {
    CLOCK_MODE_156MHz = 0, /**< High clock mode (CPU = 156MHz) */
    CLOCK_MODE_32MHz  = 1, /**< Middle clock mode (CPU = 32MHz) */
    CLOCK_MODE_8MHz   = 2, /**< Low clock mode (CPU = 8.2MHz) */
} clockmode_e;

/**
 * @class LowPowerClass
 * @brief This provides the features fo the power saving
 *
 */
class LowPowerClass
{
public:

    LowPowerClass() : isInitialized(false), isEnabledDVFS(false) {
        hvlock.count = 0;
        hvlock.info = PM_CPUFREQLOCK_TAG('L', 'P', 0);
        hvlock.flag = PM_CPUFREQLOCK_FLAG_HV;

        lvlock.count = 0;
        lvlock.info = PM_CPUFREQLOCK_TAG('L', 'P', 1);
        lvlock.flag = PM_CPUFREQLOCK_FLAG_LV;
    }

    /**
     * @brief Initialize the Low Power library
     * @details This initializes RTC library, because this library uses the RTC library.
     */
    void begin();

    /**
     * @brief Finalize the Low Power library
     */
    void end();

    /**
     * @brief Sleep (yield) this thread
     * @param [in] seconds - the sleep period
     * @details Just call the system call of sleep()
     */
    void sleep(uint32_t seconds);

    /**
     * @brief Enter the cold sleep state
     */
    void coldSleep();

    /**
     * @brief Enter the cold sleep state during the specified seconds
     * @param [in] seconds - the period in cold sleep
     */
    void coldSleep(uint32_t seconds);

    /**
     * @brief Enter the deep sleep state
     */
    void deepSleep();

    /**
     * @brief Enter the deep sleep state during the specified seconds
     * @param [in] seconds - the period in deep sleep
     */
    void deepSleep(uint32_t seconds);

    /**
     * @brief Reboot the system
     */
    void reboot();

    /**
     * @brief Get the boot cause
     * @return a boot cause
     */
    bootcause_e bootCause();

    /**
     * @brief Check if the specified cause is permitted or not
     * @param [in] bc - a boot cause
     * @return true if permitted, otherwise return false
     */
    bool isEnabledBootCause(bootcause_e bc);

    /**
     * @brief Check if the cause by the specified pin is permitted or not
     * @param [in] pin - a pin number
     * @return true if permitted, otherwise return false
     */
    bool isEnabledBootCause(uint8_t pin);

    /**
     * @brief Enable the specified cause as the boot cause
     * @param [in] bc - a boot cause
     */
    void enableBootCause(bootcause_e bc);

    /**
     * @brief Enable the cause by the specified pin as the boot cause
     * @param [in] pin - a pin number
     */
    void enableBootCause(uint8_t pin);

    /**
     * @brief Disable the specified cause as the boot cause
     * @param [in] bc - a boot cause
     */
    void disableBootCause(bootcause_e bc);

    /**
     * @brief Disable the cause by the specified pin as the boot cause
     * @param [in] pin - a pin number
     */
    void disableBootCause(uint8_t pin);

    /**
     * @brief Get a wakeup pin number from the boot cause
     * @param [in] bc - a boot cause
     * @return a pin number.
     */
    uint8_t getWakeupPin(bootcause_e bc);

    /**
     * @brief Set clock mode and change system clock dynamically
     * @param [in] mode - the clock mode
     */
    void clockMode(clockmode_e mode);

    /**
     * @brief Get clock mode
     * @return the clock mode
     */
    clockmode_e getClockMode();

    /**
     * @brief Get the sensed battery voltage on CXD5247
     * @return sensed voltage [mV]
     */
    int getVoltage(void);

    /**
     * @brief Get the sensed battery current on CXD5247
     * @return sensed current [mA]. Negative value means discharge.
     * @attention The returned value isn't time-averaged and just instantaneous
     * value. Therefore, you can not get the strict current consumption.
     * Please use this value as a guide.
     */
    int getCurrent(void);

private:
    bootcause_e pin2bootcause(uint8_t pin);
    bool isInitialized;
    bool isEnabledDVFS;
    struct pm_cpu_freqlock_s hvlock;
    struct pm_cpu_freqlock_s lvlock;
};

extern LowPowerClass LowPower;

/** @} lowpower */

#endif
