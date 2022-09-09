/**
 ******************************************************************************
 * @file    Pwm.cpp
 * @date    21 April 2022
 * @brief   Pwm driver
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

#include "Pwm.h"
#include "arch/board/board.h"
#include "../../Appdefines.h"
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
#include "../../spresense-exported-sdk/nuttx/include/nuttx/timers/pwm.h"
#include "../../spresense-exported-sdk/nuttx/include/nuttx/timers/timer.h"
#include <cxd56_pwm.h>
#include <cxd56_timer.h>

#define PWM_PIN PIN_UART2_RXD // GPIO_LED1

static uint64_t pwm_time_ns = 0;
static uint64_t pwm_period_ns = 0;
static uint64_t pwm_duty_ns = 0;
static uint64_t pwm_start_time_ns = 0;

extern "C" void spresense_time_cb(uint32_t *sec, uint32_t *nano);


static void pwm_gpio_init(void) {
    PINCONF(PWM_PIN, PINCONF_MODE0, PINCONF_INPUT_DISABLE, PINCONF_DRIVE_HIGH, PINCONF_BUSKEEPER);
}

void pwm_gpio_reset(void) {
    cxd56_gpio_write(PWM_PIN, false);
}

void pwm_gpio_set(void) {
    cxd56_gpio_write(PWM_PIN, true);
}

void pwm_init(void) {
    static bool isInit = false;
    if (isInit == false) {
        pwm_gpio_init();
    }
}

uint64_t get_current_time_ns(void) {
    uint32_t sec = 0;
    uint32_t nsec = 0;
    spresense_time_cb(&sec, &nsec);
    uint64_t result = (uint64_t) sec * 1000000000ULL + nsec;
    return result;
}

uint64_t get_passed_time_ns(uint64_t in_time) {
    uint64_t time_now = get_current_time_ns();
    if (in_time <= time_now) {
        return time_now - in_time;
    }
    return (0xffffffffffffffff - in_time) + time_now;
}

void pwm_run(uint16_t freq_hz, uint8_t duty, uint16_t time_ms) {
    pwm_time_ns = (uint64_t) time_ms * 1000000ULL;
    pwm_period_ns = (uint64_t) 1000000000ULL / freq_hz;
    pwm_duty_ns = (pwm_period_ns * duty) / 100ULL;
    pwm_start_time_ns = get_current_time_ns();
}

pwm_work_status_t pwm_act(void) {
    if (pwm_time_ns == 0){
        pwm_gpio_reset();
        return PWM_STOP;
    } else {
        uint64_t passed = get_passed_time_ns (pwm_start_time_ns);
        if (passed < pwm_duty_ns) {
            pwm_gpio_set();
        } else {
            pwm_gpio_reset();
        }
        if (passed >= pwm_period_ns) {
            pwm_start_time_ns = get_current_time_ns();
            if (pwm_time_ns > passed){
                pwm_time_ns -= passed;
            } else {
                pwm_time_ns = 0;
                pwm_gpio_reset();
                return PWM_STOP;
            }
        }
        return PWM_WORK;
    }
}
