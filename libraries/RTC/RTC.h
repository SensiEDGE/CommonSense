/**
 ******************************************************************************
 * @file    RTC.h
 * @date    29 March 2022
 * @brief   RTC driver
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

#ifndef __RTC_H__
#define __RTC_H__

/**
 * @file RTC.h
 * @author Sony Semiconductor Solutions Corporation
 * @brief Spresense Arduino RTC library
 *
 * @details The RTC library allows for getting/setting from/to the date and
 *  time or the alarm of Real-Time Clock.
 */

/**
 * @defgroup rtc RTC Library API
 * @brief API for using Real Time Clock library API
 * @{
 */

#include "RtcTime.h"

/**
 * @class RtcClass
 * @brief This is the interface to the RTC Hardware.
 *
 */
class RtcClass
{
public:
    /**
     * @brief Create RtcClass object
     */
    RtcClass() : _fd(-1), _pid(-1) {}

    /**
     * @brief Initialize the RTC library
     * @details When RTC library is used, this API must be called at first.
     * This API will wait until RTC hardware is available.
     */
    void begin();

    /**
     * @brief Finalize the RTC library
     */
    void end();

    /**
     * @brief Set RTC time
     * @param [in] rtc a object of RtcTime to set
     */
    void setTime(RtcTime &rtc);

    /**
     * @brief Get RTC time
     * @return a object of the current RtcTime
     */
    RtcTime getTime();

#ifndef SUBCORE
    /**
     * @brief Set RTC alarm time
     * @param [in] rtc a object of RtcTime to set the alarm
     */
    void setAlarm(RtcTime &rtc);

    /**
     * @brief Set RTC alarm time after the specified seconds
     * @param [in] seconds to set the alarm
     */
    void setAlarmSeconds(uint32_t seconds);

    /**
     * @brief Cancel RTC alarm time
     */
    void cancelAlarm();

    /**
     * @brief Attach the alarm handler
     * @param [in] isr the alarm handler which is executed on the task context.
     */
    void attachAlarm(void (*isr)(void));

    /**
     * @brief Detach the alarm handler
     */
    void detachAlarm();

#endif /* !SUBCORE */

private:
    int _fd;
    int16_t _pid;

};

extern RtcClass RTC;

/** @} rtc */

#endif // __RTC_H__
