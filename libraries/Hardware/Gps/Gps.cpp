/**
 ******************************************************************************
 * @file    Gps.cpp
 * @date    19 December 2022
 * @brief   Driver for Gps
 ******************************************************************************
 *
 * COPYRIGHT(c) 2021 EDGE Impulse
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

#include <string.h>
#include "Gps.h"
#include "../../Drivers/CXD5602/Gnss/Gnss.h"

static gps_mode_t gps_mode = {0};



#define STRING_BUFFER_SIZE  2048       /**< %Buffer size */

static char StringBuffer[STRING_BUFFER_SIZE];

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
    if (pNavData == nullptr) {
        return;
    }

    /* print time */
    snprintf(StringBuffer + strlen(StringBuffer), STRING_BUFFER_SIZE - strlen(StringBuffer), "%04d/%02d/%02d ", pNavData->time.year, pNavData->time.month, pNavData->time.day);
    
    snprintf(StringBuffer + strlen(StringBuffer), STRING_BUFFER_SIZE - strlen(StringBuffer), "%02d:%02d:%02d.%06ld, ", pNavData->time.hour, pNavData->time.minute, pNavData->time.sec, pNavData->time.usec);
    
    /* print satellites count */
    snprintf(StringBuffer + strlen(StringBuffer), STRING_BUFFER_SIZE - strlen(StringBuffer), "numSat:%2d, ", pNavData->numSatellites);
    
    /* print position data */
    if (pNavData->posFixMode == FixInvalid) {
        strcat(StringBuffer, "No-Fix, ");
    } else {
        strcat(StringBuffer, "Fix, ");
    }
    if (pNavData->posDataExist == 0) {
        strcat(StringBuffer, "No Position");
    } else {
        strcat(StringBuffer, "Lat=");
        snprintf(StringBuffer + strlen(StringBuffer), STRING_BUFFER_SIZE - strlen(StringBuffer), "%f", pNavData->latitude, 6);
        strcat(StringBuffer, ", Lon=");
        snprintf(StringBuffer + strlen(StringBuffer), STRING_BUFFER_SIZE - strlen(StringBuffer), "%f", pNavData->longitude, 6);
    }

    strcat(StringBuffer, " \r\n");
}

/**
 * @brief %Print satellite condition.
 */
static void print_condition(SpNavData *pNavData) {
    if (pNavData == nullptr) {
        return;
    }
    unsigned long cnt;

    /* Print satellite count. */
    snprintf(StringBuffer + strlen(StringBuffer), STRING_BUFFER_SIZE - strlen(StringBuffer), "numSatellites:%2d\n", pNavData->numSatellites);
    
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
        snprintf(StringBuffer + strlen(StringBuffer), STRING_BUFFER_SIZE - strlen(StringBuffer), "[%2ld] Type:%s, Id:%2ld, Elv:%2ld, Azm:%3ld, CN0:%f %d\r\n", cnt, pType, Id, Elv, Azm, sigLevel, 6);
    }
}




void gps_init(void) {
    int error_flag = 0;
    usleep(1000 * 3000);
    gnss.setDebugMode(PrintInfo);
    int result;
      /* Activate GNSS device */
    result = gnss.begin();

    if (result != 0) {
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
            error_flag = 1;
        }
    }
}

gps_act_status_t gps_act(void) {
    static int LoopCount = 0;
    static int LastPrintMin = 0;
    bool new_data = GPS_NO_DATA;
    /* Check update. */
    if (gnss.waitUpdate(-1)) {
        memset(StringBuffer, 0, sizeof(StringBuffer));
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
        new_data = true;
    } 

    /* Check loop count. */
    LoopCount++;
    if (LoopCount >= RESTART_CYCLE) {
        int error_flag = 0;

        /* Restart gnss. */
        if (gnss.stop() != 0) {
            error_flag = 1;
        } else if (gnss.end() != 0) {
            error_flag = 1;
        } else {
        }

        if (gnss.begin() != 0) {
            error_flag = 1;
        } else if (gnss.start(HOT_START) != 0) {
            error_flag = 1;
        } else {
        }

        LoopCount = 0;

        if (error_flag == 1) {
            return GPS_ERROR;
        }
    }
    return GPS_NEW_DATA;
}

/**
 * @brief  get gps string data
 * @param  none
 * @retval pointer to gps data
 */
char *gps_get_data(void) {
    return StringBuffer;
}

/**
 * @brief  get gps current mode
 * @param  [out] mode from gps_mode_t
 * @retval none
 */
void gps_get_mode(gps_mode_t* mode) {
    if (mode == nullptr) {
        return;
    }
    memcpy(mode, &gps_mode, sizeof(gps_mode_t));
}

/**
 * @brief  set gps current mode
 * @param  [in] mode from gps_mode_t
 * @retval none
 */
void gps_set_mode(gps_mode_t* mode) {
    if (mode == nullptr) {
        return;
    }
    memcpy(&gps_mode, mode, sizeof(gps_mode_t));
    if (gps_mode.enable != false) {
        gps_enable();
    } else {
        gps_disable();
    }
}

/**
 * @brief  enable gps
 * @param  None
 * @retval None
 */
void gps_enable(void) {
    gps_init();
    gps_mode.enable = true;
}

/**
 * @brief  disable gps
 * @param  None
 * @retval None
 */
void gps_disable(void) {
    gnss.stop();
    gps_mode.enable = false;
}
