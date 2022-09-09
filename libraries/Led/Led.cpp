/**
 ******************************************************************************
 * @file    Led.cpp
 * @date    23 March 2022
 * @brief   Driver for LEDs
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

#include "../../Appdefines.h"
#include "Led.h"

#if defined (PORT_EXPANDER_MAX7317)

#include "../../libraries/Max7317/Max7317.h"

void led_init (void) {
    static bool isInit = false;
    if (isInit == false) {
        max7317_init();
        isInit = true;
    }
}

void led_green_on(void) {
    max7317_reset_pin(LED_GREEN_MAX7317_PIN);
}

void led_green_off(void) {
    max7317_set_pin(LED_GREEN_MAX7317_PIN);
}

void led_red_on(void) {
    max7317_reset_pin(LED_RED_MAX7317_PIN);
}

void led_red_off(void) {
    max7317_set_pin(LED_RED_MAX7317_PIN);
}

#elif defined (PORT_EXPANDER_PCA9538)

#include "../../libraries/Pca9538/Pca9538.h"

void led_init (void) {
    static bool isInit = false;
    if (isInit == false) {
        pca9538_init();
        uint8_t pin_mode = pca9538_get_pins_mode();
        pca9538_set_pins_mode(pin_mode & (~(LED_GREEN_PCA9538_PIN | LED_RED_PCA9538_PIN)));
        isInit = true;
    }
}

void led_green_on(void) {
    pca9538_reset_pin(LED_GREEN_PCA9538_PIN);
}

void led_green_off(void) {
    pca9538_set_pin(LED_GREEN_PCA9538_PIN);
}

void led_red_on(void) {
    pca9538_reset_pin(LED_RED_PCA9538_PIN);
}

void led_red_off(void) {
    pca9538_set_pin(LED_RED_PCA9538_PIN);
}


#else

#error "Port expander doesn't selected"

#endif 
