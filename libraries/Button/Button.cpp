/**
 ******************************************************************************
 * @file    Button.cpp
 * @date    23 March 2022
 * @brief   Driver for Button
 ******************************************************************************
 *
 * COPYRIGHT(c) 2021 Droid-Technologies LLC
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
#include "Button.h"
#include "stdio.h"

#if defined (PORT_EXPANDER_MAX7317)

#include "../../libraries/Max7317/Max7317.h"
void button_init(void) {
    max7317_init();
}

bool button_is_pressed(void) {
    uint16_t a = max7317_get_pins();
    //printf("max7317_get_pins = %d\r\n", a);
    if ((a & (1 << BUTTON_MAX7317_PIN)) != 0) {
        return true;
    }
    return false;
}

#elif defined (PORT_EXPANDER_PCA9538)

#include "../../libraries/Pca9538/Pca9538.h"

void button_init(void) {

    static bool isInit = false;
    if (isInit == false) {
        pca9538_init();
        uint8_t pin_mode = pca9538_get_pins_mode();
        pca9538_set_pins_mode(pin_mode | BUTTON_PCA9538_PIN);
    }

}

bool button_is_pressed(void) {
    uint16_t a = pca9538_get_pins();
    if ((a & (1 << BUTTON_PCA9538_PIN)) != 0) {
        return true;
    }
    return false;
}

#else

#error "Port expander doesn't selected"

#endif 
