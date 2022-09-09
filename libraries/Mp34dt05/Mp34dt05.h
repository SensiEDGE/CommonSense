/**
 ******************************************************************************
 * @file    Mp34dt05.h
 * @date    29 May 2022
 * @brief   MP34DT05 driver
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

#ifndef MP34DT05_H
#define MP34DT05_H

#include "stdint.h"


/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <pthread.h>

#if 0
#define CONFIG_EXAMPLES_I2SCHAR_RX
/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Configuration ************************************************************/

/* CONFIG_EXAMPLES_I2SCHAR_DEVPATH - The default path to the I2S character
 *   device. Default: /dev/i2schar0
 */

#ifndef CONFIG_AUDIO_I2SCHAR
#error "I2S character device support is not enabled (CONFIG_AUDIO_I2SCHAR)"
#endif

#ifndef CONFIG_EXAMPLES_I2SCHAR_DEVPATH
#  define CONFIG_EXAMPLES_I2SCHAR_DEVPATH "/dev/is2char0"
#endif

#ifdef CONFIG_EXAMPLES_I2SCHAR_TX
#ifndef CONFIG_EXAMPLES_I2SCHAR_TXBUFFERS
#define CONFIG_EXAMPLES_I2SCHAR_TXBUFFERS 4
#endif
#ifndef CONFIG_EXAMPLES_I2SCHAR_TXSTACKSIZE
#define CONFIG_EXAMPLES_I2SCHAR_TXSTACKSIZE 1536
#endif
#else
#undef CONFIG_EXAMPLES_I2SCHAR_TXBUFFERS
#define CONFIG_EXAMPLES_I2SCHAR_TXBUFFERS 0
#endif

#ifdef CONFIG_EXAMPLES_I2SCHAR_RX
#ifndef CONFIG_EXAMPLES_I2SCHAR_RXBUFFERS
#define CONFIG_EXAMPLES_I2SCHAR_RXBUFFERS 4
#endif
#ifndef CONFIG_EXAMPLES_I2SCHAR_RXSTACKSIZE
#define CONFIG_EXAMPLES_I2SCHAR_RXSTACKSIZE 1536
#endif
#else
#undef CONFIG_EXAMPLES_I2SCHAR_RXBUFFERS
#define CONFIG_EXAMPLES_I2SCHAR_RXBUFFERS 0
#endif

#ifndef CONFIG_EXAMPLES_I2SCHAR_BUFSIZE
#define CONFIG_EXAMPLES_I2SCHAR_BUFSIZE 256
#endif

/****************************************************************************
 * Public Types
 ****************************************************************************/

struct i2schar_state_s
{
  bool      initialized;
  FAR char *devpath;
#ifdef CONFIG_EXAMPLES_I2SCHAR_TX
  int       txcount;
#endif
#ifdef CONFIG_EXAMPLES_I2SCHAR_RX
  int       rxcount;
#endif
};


/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: i2schar_devinit()
 *
 * Description:
 *   Perform architecture-specific initialization of the I2s hardware and
 *   registration of the I2S character driver CHAR hardware.  If
 *   CONFIG_EXAMPLES_I2SCHAR_DEVINIT is defined, then board-specific logic
 *   must provide this function.
 *
 ****************************************************************************/

#ifdef CONFIG_EXAMPLES_I2SCHAR_DEVINIT
int i2schar_devinit(void);
#endif

/****************************************************************************
 * Name: i2schar_transmitter()
 *
 * Description:
 *   This is the entry point for the transmitter thread.
 *
 ****************************************************************************/

#ifdef CONFIG_EXAMPLES_I2SCHAR_TX
pthread_addr_t i2schar_transmitter(pthread_addr_t arg);
#endif

/****************************************************************************
 * Name: i2schar_receiver()
 *
 * Description:
 *   This is the entry point for the receiver thread.
 *
 ****************************************************************************/

#ifdef CONFIG_EXAMPLES_I2SCHAR_RX
pthread_addr_t i2schar_receiver(pthread_addr_t arg);
#endif

#endif

void Mp34dt05Init (void);

#if 0
#define MIC_BUF_SIZE 512

typedef enum {
    BUF_OVF_NONE,
    BUF_OVF_0,
    BUF_OVF_1,
    BUF_OVF_BOTH
} buf_ovf_state_t;

/**
 * @brief  MP34DT05 init
 * @param  None
 * @retval None
 */


/**
 * @brief  Check buffers overflow
 * @param  None
 * @retval buffer state from buf_ovf_state_t
 */
buf_ovf_state_t getAndClrBufState (void);

/**
 * @brief  Get one of buffers pnt
 * @param  [in] buf BUF_OVF_0 or BUF_OVF_1 from buf_ovf_state_t
 * @retval nullptr if error input param or pnt to buffer
 */
uint16_t* getBuf (buf_ovf_state_t buf);

#endif

#endif // MP34DT05_H
