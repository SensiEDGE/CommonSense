/****************************************************************************
 * arch/arm/src/cxd56xx/hardware/cxd5602_topreg.h
 *
 *   Copyright (C) 2008-2013 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 *   Copyright 2018 Sony Semiconductor Solutions Corporation
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Sony Semiconductor Solutions Corporation nor
 *    the names of its contributors may be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#ifndef __ARCH_ARM_SRC_CXD56XX_HARDWARE_CXD5602_TOPREG_H
#define __ARCH_ARM_SRC_CXD56XX_HARDWARE_CXD5602_TOPREG_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define CXD56_TOPREG_PWD_CTL                    (CXD56_TOPREG_BASE + 0x0000)
#define CXD56_TOPREG_ANA_PW_CTL                 (CXD56_TOPREG_BASE + 0x0004)
#define CXD56_TOPREG_ANA_EN_CTL                 (CXD56_TOPREG_BASE + 0x0008)
#define CXD56_TOPREG_SYSCPU_RAMMODE_SEL         (CXD56_TOPREG_BASE + 0x0010)
#define CXD56_TOPREG_TOP_SCU_RAMMODE_SEL        (CXD56_TOPREG_BASE + 0x0018)
#define CXD56_TOPREG_HOSTIFC_RAMMODE_SEL        (CXD56_TOPREG_BASE + 0x001c)
#define CXD56_TOPREG_PMU_FAST                   (CXD56_TOPREG_BASE + 0x0020)
#define CXD56_TOPREG_PMU_PW_CTL                 (CXD56_TOPREG_BASE + 0x0030)
#define CXD56_TOPREG_PMU_INT_STAT               (CXD56_TOPREG_BASE + 0x0040)
#define CXD56_TOPREG_PMU_RAW_INT_STAT           (CXD56_TOPREG_BASE + 0x0044)
#define CXD56_TOPREG_PMU_INT_CLR                (CXD56_TOPREG_BASE + 0x0048)
#define CXD56_TOPREG_PMU_INT_MASK               (CXD56_TOPREG_BASE + 0x004c)
#define CXD56_TOPREG_PWD_RESET0                 (CXD56_TOPREG_BASE + 0x0060)
#define CXD56_TOPREG_PWD_STAT                   (CXD56_TOPREG_BASE + 0x0200)
#define CXD56_TOPREG_PWD_PGACK_STAT             (CXD56_TOPREG_BASE + 0x0204)
#define CXD56_TOPREG_ANA_PW_STAT                (CXD56_TOPREG_BASE + 0x0208)
#define CXD56_TOPREG_SYSCPU_RAMMODE_STAT        (CXD56_TOPREG_BASE + 0x020c)
#define CXD56_TOPREG_TOP_SCU_RAMMODE_STAT       (CXD56_TOPREG_BASE + 0x0214)
#define CXD56_TOPREG_HOSTIFC_RAMMODE_STAT       (CXD56_TOPREG_BASE + 0x0218)
#define CXD56_TOPREG_PMU_HW_STAT                (CXD56_TOPREG_BASE + 0x021c)
#define CXD56_TOPREG_ANA_PW_CTL_SEL_WAKE        (CXD56_TOPREG_BASE + 0x0400)
#define CXD56_TOPREG_SYSCPU_RAMMODE_SEL_WAKE    (CXD56_TOPREG_BASE + 0x0404)
#define CXD56_TOPREG_TOP_SCU_RAMMODE_SEL_WAKE   (CXD56_TOPREG_BASE + 0x0410)
#define CXD56_TOPREG_CLSELDIV_WAKE              (CXD56_TOPREG_BASE + 0x0414)
#define CXD56_TOPREG_CKDIV_CPU_DSP_BUS_WAKE     (CXD56_TOPREG_BASE + 0x0418)
#define CXD56_TOPREG_CKSEL_ROOT_WAKE            (CXD56_TOPREG_BASE + 0x041c)
#define CXD56_TOPREG_PMIC_SLEEP_I2C0            (CXD56_TOPREG_BASE + 0x0420)
#define CXD56_TOPREG_PMIC_SLEEP_I2C1            (CXD56_TOPREG_BASE + 0x0424)
#define CXD56_TOPREG_PMIC_SLEEP_I2C2            (CXD56_TOPREG_BASE + 0x0428)
#define CXD56_TOPREG_PMIC_SLEEP_I2C3            (CXD56_TOPREG_BASE + 0x042c)
#define CXD56_TOPREG_PMIC_WAKE_I2C0             (CXD56_TOPREG_BASE + 0x0430)
#define CXD56_TOPREG_PMIC_WAKE_I2C1             (CXD56_TOPREG_BASE + 0x0434)
#define CXD56_TOPREG_PMIC_WAKE_I2C2             (CXD56_TOPREG_BASE + 0x0438)
#define CXD56_TOPREG_PMIC_WAKE_I2C3             (CXD56_TOPREG_BASE + 0x043c)
#define CXD56_TOPREG_PMIC_UNEXP_I2C0            (CXD56_TOPREG_BASE + 0x0440)
#define CXD56_TOPREG_PMIC_UNEXP_I2C1            (CXD56_TOPREG_BASE + 0x0444)
#define CXD56_TOPREG_PMIC_UNEXP_I2C2            (CXD56_TOPREG_BASE + 0x0448)
#define CXD56_TOPREG_PMIC_UNEXP_I2C3            (CXD56_TOPREG_BASE + 0x044c)
#define CXD56_TOPREG_PMIC_UNEXP_I2C             (CXD56_TOPREG_BASE + 0x0450)
#define CXD56_TOPREG_PMU_WAKE_TRIG_EN0          (CXD56_TOPREG_BASE + 0x0454)
#define CXD56_TOPREG_PMU_WAKE_TRIG_EN1          (CXD56_TOPREG_BASE + 0x0458)
#define CXD56_TOPREG_PMU_WAKE_TRIG_NEGEN0       (CXD56_TOPREG_BASE + 0x045c)
#define CXD56_TOPREG_PMU_WAKE_TRIG_NEGEN1       (CXD56_TOPREG_BASE + 0x0460)
#define CXD56_TOPREG_PMU_WAKE_TRIG_NOISECUTEN0  (CXD56_TOPREG_BASE + 0x0464)
#define CXD56_TOPREG_PMU_WAKE_TRIG_CPUINTSEL0   (CXD56_TOPREG_BASE + 0x0468)
#define CXD56_TOPREG_PMU_WAKE_TRIG_CPUINTSEL1   (CXD56_TOPREG_BASE + 0x046c)
#define CXD56_TOPREG_PMU_WAKE_TRIG_CPUINTSEL2   (CXD56_TOPREG_BASE + 0x0470)
#define CXD56_TOPREG_PMU_WAKE_TRIG_INTDET0      (CXD56_TOPREG_BASE + 0x0474)
#define CXD56_TOPREG_PMU_WAKE_TRIG_INTDET1      (CXD56_TOPREG_BASE + 0x0478)
#define CXD56_TOPREG_PMU_WAKE_TRIG_INTDET2      (CXD56_TOPREG_BASE + 0x047c)
#define CXD56_TOPREG_PMU_WAKE_PMIC_I2C          (CXD56_TOPREG_BASE + 0x0480)
#define CXD56_TOPREG_BOOT_CAUSE                 (CXD56_TOPREG_BASE + 0x0484)
#define CXD56_TOPREG_PMU_CORE_CKEN              (CXD56_TOPREG_BASE + 0x04c0)
#define CXD56_TOPREG_CKSEL_ROOT                 (CXD56_TOPREG_BASE + 0x04c4)
#define CXD56_TOPREG_CKSEL_PMU                  (CXD56_TOPREG_BASE + 0x04c8)
#define CXD56_TOPREG_CKSEL_SYSIOP               (CXD56_TOPREG_BASE + 0x04cc)
#define CXD56_TOPREG_CKSEL_SYSIOP_SUB           (CXD56_TOPREG_BASE + 0x04d0)
#define CXD56_TOPREG_CKSEL_SCU                  (CXD56_TOPREG_BASE + 0x04d4)
#define CXD56_TOPREG_CKDIV_CPU_DSP_BUS          (CXD56_TOPREG_BASE + 0x04d8)
#define CXD56_TOPREG_CKDIV_COM                  (CXD56_TOPREG_BASE + 0x04dc)
#define CXD56_TOPREG_CKDIV_HOSTIFC              (CXD56_TOPREG_BASE + 0x04e0)
#define CXD56_TOPREG_CKDIV_SCU                  (CXD56_TOPREG_BASE + 0x04e4)
#define CXD56_TOPREG_CKDIV_PMU                  (CXD56_TOPREG_BASE + 0x04e8)
#define CXD56_TOPREG_CRG_INT_CLR0               (CXD56_TOPREG_BASE + 0x04ec)
#define CXD56_TOPREG_CRG_INT_MASK0              (CXD56_TOPREG_BASE + 0x04f0)
#define CXD56_TOPREG_CRG_INT_STAT_MSK0          (CXD56_TOPREG_BASE + 0x04f4)
#define CXD56_TOPREG_CRG_INT_STAT_RAW0          (CXD56_TOPREG_BASE + 0x04f8)
#define CXD56_TOPREG_CRG_INT_CLR1               (CXD56_TOPREG_BASE + 0x04fc)
#define CXD56_TOPREG_CRG_INT_MASK1              (CXD56_TOPREG_BASE + 0x0500)
#define CXD56_TOPREG_CRG_INT_STAT_MSK1          (CXD56_TOPREG_BASE + 0x0504)
#define CXD56_TOPREG_CRG_INT_STAT_RAW1          (CXD56_TOPREG_BASE + 0x0508)
#define CXD56_TOPREG_CPU_GATECLK                (CXD56_TOPREG_BASE + 0x050c)
#define CXD56_TOPREG_USBPHY_CKEN                (CXD56_TOPREG_BASE + 0x0510)
#define CXD56_TOPREG_CRG_MON                    (CXD56_TOPREG_BASE + 0x0514)
#define CXD56_TOPREG_GEAR_STAT                  (CXD56_TOPREG_BASE + 0x0518)
#define CXD56_TOPREG_XOSC_CTRL                  (CXD56_TOPREG_BASE + 0x0580)
#define CXD56_TOPREG_XOSC_CTRL2                 (CXD56_TOPREG_BASE + 0x0584)
#define CXD56_TOPREG_SYS_PLL_CTRL1              (CXD56_TOPREG_BASE + 0x0588)
#define CXD56_TOPREG_SYS_PLL_CTRL2              (CXD56_TOPREG_BASE + 0x058c)
#define CXD56_TOPREG_RCOSC_CTRL1                (CXD56_TOPREG_BASE + 0x0590)
#define CXD56_TOPREG_RCOSC_CTRL2                (CXD56_TOPREG_BASE + 0x0594)
#define CXD56_TOPREG_RF_GPMBI_EN                (CXD56_TOPREG_BASE + 0x0598)
#define CXD56_TOPREG_WDT_SRST_EN                (CXD56_TOPREG_BASE + 0x0640)
#define CXD56_TOPREG_FORCE_CKEN                 (CXD56_TOPREG_BASE + 0x0644)
#define CXD56_TOPREG_DBG_HOSTIF_SEL             (CXD56_TOPREG_BASE + 0x0650)
#define CXD56_TOPREG_WDT_MASK                   (CXD56_TOPREG_BASE + 0x0654)
#define CXD56_TOPREG_CKGATE_CTL                 (CXD56_TOPREG_BASE + 0x0660)
#define CXD56_TOPREG_I2CRPT_SADR                (CXD56_TOPREG_BASE + 0x06c0)
#define CXD56_TOPREG_I2CRPT_REP                 (CXD56_TOPREG_BASE + 0x06c4)
#define CXD56_TOPREG_PMIC_I2C                   (CXD56_TOPREG_BASE + 0x06d0)
#define CXD56_TOPREG_RAMMODE                    (CXD56_TOPREG_BASE + 0x06e4)
#define CXD56_TOPREG_SWRESET_BUS                (CXD56_TOPREG_BASE + 0x0700)
#define CXD56_TOPREG_SWRESET_SCU                (CXD56_TOPREG_BASE + 0x0704)
#define CXD56_TOPREG_BUSROM_CKEN                (CXD56_TOPREG_BASE + 0x0710)
#define CXD56_TOPREG_SYSIOP_CKEN                (CXD56_TOPREG_BASE + 0x0714)
#define CXD56_TOPREG_SCU_CKEN                   (CXD56_TOPREG_BASE + 0x071c)
#define CXD56_TOPREG_RTC0_CTL                   (CXD56_TOPREG_BASE + 0x0730)
#define CXD56_TOPREG_FUSERDCFG0                 (CXD56_TOPREG_BASE + 0x0740)
#define CXD56_TOPREG_FUSERDCFG1                 (CXD56_TOPREG_BASE + 0x0744)
#define CXD56_TOPREG_FUSERDCFG2                 (CXD56_TOPREG_BASE + 0x0748)
#define CXD56_TOPREG_VID0                       (CXD56_TOPREG_BASE + 0x0750)
#define CXD56_TOPREG_VID1                       (CXD56_TOPREG_BASE + 0x0754)
#define CXD56_TOPREG_M0_BOOT_FLASH_DIS          (CXD56_TOPREG_BASE + 0x0758)
#define CXD56_TOPREG_LDOADJ0                    (CXD56_TOPREG_BASE + 0x0760)
#define CXD56_TOPREG_LDOADJ1                    (CXD56_TOPREG_BASE + 0x0764)
#define CXD56_TOPREG_FQFIX_CTL0                 (CXD56_TOPREG_BASE + 0x0770)
#define CXD56_TOPREG_FQFIX_AUTO                 (CXD56_TOPREG_BASE + 0x0774)
#define CXD56_TOPREG_FQFIX_SINGLE               (CXD56_TOPREG_BASE + 0x0778)
#define CXD56_TOPREG_FQFIX_STATUS               (CXD56_TOPREG_BASE + 0x077c)
#define CXD56_TOPREG_SYSTEM_CONFIG              (CXD56_TOPREG_BASE + 0x0790)
#define CXD56_TOPREG_MON_SEL                    (CXD56_TOPREG_BASE + 0x07a0)
#define CXD56_TOPREG_IOCSYS_MONSEL0             (CXD56_TOPREG_BASE + 0x07a4)
#define CXD56_TOPREG_IOCSYS_MONSEL1             (CXD56_TOPREG_BASE + 0x07a8)
#define CXD56_TOPREG_IOCSYS_INTSEL0             (CXD56_TOPREG_BASE + 0x07b0)
#define CXD56_TOPREG_IOCSYS_INTSEL1             (CXD56_TOPREG_BASE + 0x07b4)
#define CXD56_TOPREG_IOCSYS_IOMD0               (CXD56_TOPREG_BASE + 0x07c0)
#define CXD56_TOPREG_IOCSYS_IOMD1               (CXD56_TOPREG_BASE + 0x07c4)
#define CXD56_TOPREG_IOOEN_SYS                  (CXD56_TOPREG_BASE + 0x07e0)
#define CXD56_TOPREG_IO_RTC_CLK_IN              (CXD56_TOPREG_BASE + 0x0800)
#define CXD56_TOPREG_IO_I2C4_BCK                (CXD56_TOPREG_BASE + 0x0804)
#define CXD56_TOPREG_IO_I2C4_BDT                (CXD56_TOPREG_BASE + 0x0808)
#define CXD56_TOPREG_IO_PMIC_INT                (CXD56_TOPREG_BASE + 0x080c)
#define CXD56_TOPREG_IO_RTC_IRQ_OUT             (CXD56_TOPREG_BASE + 0x0810)
#define CXD56_TOPREG_IO_AP_CLK                  (CXD56_TOPREG_BASE + 0x0814)
#define CXD56_TOPREG_IO_GNSS_1PPS_OUT           (CXD56_TOPREG_BASE + 0x0818)
#define CXD56_TOPREG_IO_SPI0_CS_X               (CXD56_TOPREG_BASE + 0x0844)
#define CXD56_TOPREG_IO_SPI0_SCK                (CXD56_TOPREG_BASE + 0x0848)
#define CXD56_TOPREG_IO_SPI0_MOSI               (CXD56_TOPREG_BASE + 0x084c)
#define CXD56_TOPREG_IO_SPI0_MISO               (CXD56_TOPREG_BASE + 0x0850)
#define CXD56_TOPREG_IO_SPI1_CS_X               (CXD56_TOPREG_BASE + 0x0854)
#define CXD56_TOPREG_IO_SPI1_SCK                (CXD56_TOPREG_BASE + 0x0858)
#define CXD56_TOPREG_IO_SPI1_IO0                (CXD56_TOPREG_BASE + 0x085c)
#define CXD56_TOPREG_IO_SPI1_IO1                (CXD56_TOPREG_BASE + 0x0860)
#define CXD56_TOPREG_IO_SPI1_IO2                (CXD56_TOPREG_BASE + 0x0864)
#define CXD56_TOPREG_IO_SPI1_IO3                (CXD56_TOPREG_BASE + 0x0868)
#define CXD56_TOPREG_IO_SPI2_CS_X               (CXD56_TOPREG_BASE + 0x086c)
#define CXD56_TOPREG_IO_SPI2_SCK                (CXD56_TOPREG_BASE + 0x0870)
#define CXD56_TOPREG_IO_SPI2_MOSI               (CXD56_TOPREG_BASE + 0x0874)
#define CXD56_TOPREG_IO_SPI2_MISO               (CXD56_TOPREG_BASE + 0x0878)
#define CXD56_TOPREG_IO_HIF_IRQ_OUT             (CXD56_TOPREG_BASE + 0x087c)
#define CXD56_TOPREG_IO_HIF_GPIO0               (CXD56_TOPREG_BASE + 0x0880)
#define CXD56_TOPREG_IO_SEN_IRQ_IN              (CXD56_TOPREG_BASE + 0x0894)
#define CXD56_TOPREG_IO_SPI3_CS0_X              (CXD56_TOPREG_BASE + 0x0898)
#define CXD56_TOPREG_IO_SPI3_CS1_X              (CXD56_TOPREG_BASE + 0x089c)
#define CXD56_TOPREG_IO_SPI3_CS2_X              (CXD56_TOPREG_BASE + 0x08a0)
#define CXD56_TOPREG_IO_SPI3_SCK                (CXD56_TOPREG_BASE + 0x08a4)
#define CXD56_TOPREG_IO_SPI3_MOSI               (CXD56_TOPREG_BASE + 0x08a8)
#define CXD56_TOPREG_IO_SPI3_MISO               (CXD56_TOPREG_BASE + 0x08ac)
#define CXD56_TOPREG_IO_I2C0_BCK                (CXD56_TOPREG_BASE + 0x08b0)
#define CXD56_TOPREG_IO_I2C0_BDT                (CXD56_TOPREG_BASE + 0x08b4)
#define CXD56_TOPREG_IO_PWM0                    (CXD56_TOPREG_BASE + 0x08b8)
#define CXD56_TOPREG_IO_PWM1                    (CXD56_TOPREG_BASE + 0x08bc)
#define CXD56_TOPREG_IO_PWM2                    (CXD56_TOPREG_BASE + 0x08c0)
#define CXD56_TOPREG_IO_PWM3                    (CXD56_TOPREG_BASE + 0x08c4)
#define CXD56_TOPREG_IO_DBG_SWOCLK              (CXD56_TOPREG_BASE + 0x08d4)
#define CXD56_TOPREG_IO_DBG_SWO                 (CXD56_TOPREG_BASE + 0x08d8)
#define CXD56_TOPREG_IO_IS_CLK                  (CXD56_TOPREG_BASE + 0x08e0)
#define CXD56_TOPREG_IO_IS_VSYNC                (CXD56_TOPREG_BASE + 0x08e4)
#define CXD56_TOPREG_IO_IS_HSYNC                (CXD56_TOPREG_BASE + 0x08e8)
#define CXD56_TOPREG_IO_IS_DATA0                (CXD56_TOPREG_BASE + 0x08ec)
#define CXD56_TOPREG_IO_IS_DATA1                (CXD56_TOPREG_BASE + 0x08f0)
#define CXD56_TOPREG_IO_IS_DATA2                (CXD56_TOPREG_BASE + 0x08f4)
#define CXD56_TOPREG_IO_IS_DATA3                (CXD56_TOPREG_BASE + 0x08f8)
#define CXD56_TOPREG_IO_IS_DATA4                (CXD56_TOPREG_BASE + 0x08fc)
#define CXD56_TOPREG_IO_IS_DATA5                (CXD56_TOPREG_BASE + 0x0900)
#define CXD56_TOPREG_IO_IS_DATA6                (CXD56_TOPREG_BASE + 0x0904)
#define CXD56_TOPREG_IO_IS_DATA7                (CXD56_TOPREG_BASE + 0x0908)
#define CXD56_TOPREG_IO_UART2_TXD               (CXD56_TOPREG_BASE + 0x090c)
#define CXD56_TOPREG_IO_UART2_RXD               (CXD56_TOPREG_BASE + 0x0910)
#define CXD56_TOPREG_IO_UART2_CTS               (CXD56_TOPREG_BASE + 0x0914)
#define CXD56_TOPREG_IO_UART2_RTS               (CXD56_TOPREG_BASE + 0x0918)
#define CXD56_TOPREG_IO_SPI4_CS_X               (CXD56_TOPREG_BASE + 0x091c)
#define CXD56_TOPREG_IO_SPI4_SCK                (CXD56_TOPREG_BASE + 0x0920)
#define CXD56_TOPREG_IO_SPI4_MOSI               (CXD56_TOPREG_BASE + 0x0924)
#define CXD56_TOPREG_IO_SPI4_MISO               (CXD56_TOPREG_BASE + 0x0928)
#define CXD56_TOPREG_IO_EMMC_CLK                (CXD56_TOPREG_BASE + 0x092c)
#define CXD56_TOPREG_IO_EMMC_CMD                (CXD56_TOPREG_BASE + 0x0930)
#define CXD56_TOPREG_IO_EMMC_DATA0              (CXD56_TOPREG_BASE + 0x0934)
#define CXD56_TOPREG_IO_EMMC_DATA1              (CXD56_TOPREG_BASE + 0x0938)
#define CXD56_TOPREG_IO_EMMC_DATA2              (CXD56_TOPREG_BASE + 0x093c)
#define CXD56_TOPREG_IO_EMMC_DATA3              (CXD56_TOPREG_BASE + 0x0940)
#define CXD56_TOPREG_IO_SDIO_CLK                (CXD56_TOPREG_BASE + 0x0944)
#define CXD56_TOPREG_IO_SDIO_CMD                (CXD56_TOPREG_BASE + 0x0948)
#define CXD56_TOPREG_IO_SDIO_DATA0              (CXD56_TOPREG_BASE + 0x094c)
#define CXD56_TOPREG_IO_SDIO_DATA1              (CXD56_TOPREG_BASE + 0x0950)
#define CXD56_TOPREG_IO_SDIO_DATA2              (CXD56_TOPREG_BASE + 0x0954)
#define CXD56_TOPREG_IO_SDIO_DATA3              (CXD56_TOPREG_BASE + 0x0958)
#define CXD56_TOPREG_IO_SDIO_CD                 (CXD56_TOPREG_BASE + 0x095c)
#define CXD56_TOPREG_IO_SDIO_WP                 (CXD56_TOPREG_BASE + 0x0960)
#define CXD56_TOPREG_IO_SDIO_CMDDIR             (CXD56_TOPREG_BASE + 0x0964)
#define CXD56_TOPREG_IO_SDIO_DIR0               (CXD56_TOPREG_BASE + 0x0968)
#define CXD56_TOPREG_IO_SDIO_DIR1_3             (CXD56_TOPREG_BASE + 0x096c)
#define CXD56_TOPREG_IO_SDIO_CLKI               (CXD56_TOPREG_BASE + 0x0970)
#define CXD56_TOPREG_IO_I2S0_BCK                (CXD56_TOPREG_BASE + 0x0974)
#define CXD56_TOPREG_IO_I2S0_LRCK               (CXD56_TOPREG_BASE + 0x0978)
#define CXD56_TOPREG_IO_I2S0_DATA_IN            (CXD56_TOPREG_BASE + 0x097c)
#define CXD56_TOPREG_IO_I2S0_DATA_OUT           (CXD56_TOPREG_BASE + 0x0980)
#define CXD56_TOPREG_IO_I2S1_BCK                (CXD56_TOPREG_BASE + 0x0984)
#define CXD56_TOPREG_IO_I2S1_LRCK               (CXD56_TOPREG_BASE + 0x0988)
#define CXD56_TOPREG_IO_I2S1_DATA_IN            (CXD56_TOPREG_BASE + 0x098c)
#define CXD56_TOPREG_IO_I2S1_DATA_OUT           (CXD56_TOPREG_BASE + 0x0990)
#define CXD56_TOPREG_IO_MCLK                    (CXD56_TOPREG_BASE + 0x0994)
#define CXD56_TOPREG_IO_PDM_CLK                 (CXD56_TOPREG_BASE + 0x0998)
#define CXD56_TOPREG_IO_PDM_IN                  (CXD56_TOPREG_BASE + 0x099c)
#define CXD56_TOPREG_IO_PDM_OUT                 (CXD56_TOPREG_BASE + 0x09a0)
#define CXD56_TOPREG_IO_USB_VBUSINT             (CXD56_TOPREG_BASE + 0x09a4)
#define CXD56_TOPREG_FUSEWRST                   (CXD56_TOPREG_BASE + 0x0a00)
#define CXD56_TOPREG_FUSEWRAD                   (CXD56_TOPREG_BASE + 0x0a04)
#define CXD56_TOPREG_FUSEWRDT                   (CXD56_TOPREG_BASE + 0x0a08)
#define CXD56_TOPREG_FUSEWRPG                   (CXD56_TOPREG_BASE + 0x0a0c)
#define CXD56_TOPREG_ADSP1_BOOT_ENTRY_POINT     (CXD56_TOPREG_BASE + 0x1010)
#define CXD56_TOPREG_ADSP2_BOOT_ENTRY_POINT     (CXD56_TOPREG_BASE + 0x1014)
#define CXD56_TOPREG_ADSP3_BOOT_ENTRY_POINT     (CXD56_TOPREG_BASE + 0x1018)
#define CXD56_TOPREG_ADSP4_BOOT_ENTRY_POINT     (CXD56_TOPREG_BASE + 0x101c)
#define CXD56_TOPREG_ADSP5_BOOT_ENTRY_POINT     (CXD56_TOPREG_BASE + 0x1020)
#define CXD56_TOPREG_ADSP0_BOOT_ENTRY_POINT     (CXD56_TOPREG_BASE + 0x1400)
#define CXD56_TOPREG_USB_VBUS                   (CXD56_TOPREG_BASE + 0x1410)
#define CXD56_TOPREG_FUSERD00                   (CXD56_TOPREG_BASE + 0x1420)
#define CXD56_TOPREG_FUSERD01                   (CXD56_TOPREG_BASE + 0x1424)
#define CXD56_TOPREG_FUSERD02                   (CXD56_TOPREG_BASE + 0x1428)
#define CXD56_TOPREG_FUSERD03                   (CXD56_TOPREG_BASE + 0x142c)
#define CXD56_TOPREG_FUSERD04                   (CXD56_TOPREG_BASE + 0x1430)
#define CXD56_TOPREG_FUSERD05                   (CXD56_TOPREG_BASE + 0x1434)
#define CXD56_TOPREG_FUSERD06                   (CXD56_TOPREG_BASE + 0x1438)
#define CXD56_TOPREG_FUSERD07                   (CXD56_TOPREG_BASE + 0x143c)
#define CXD56_TOPREG_FUSERD08                   (CXD56_TOPREG_BASE + 0x1440)
#define CXD56_TOPREG_FUSERD09                   (CXD56_TOPREG_BASE + 0x1444)
#define CXD56_TOPREG_FUSERD10                   (CXD56_TOPREG_BASE + 0x1448)
#define CXD56_TOPREG_FUSERD11                   (CXD56_TOPREG_BASE + 0x144c)
#define CXD56_TOPREG_FUSERD12                   (CXD56_TOPREG_BASE + 0x1450)
#define CXD56_TOPREG_FUSERD13                   (CXD56_TOPREG_BASE + 0x1454)
#define CXD56_TOPREG_FUSERD14                   (CXD56_TOPREG_BASE + 0x1458)
#define CXD56_TOPREG_FUSERD15                   (CXD56_TOPREG_BASE + 0x145c)
#define CXD56_TOPREG_AUDIO_IF_SEL               (CXD56_TOPREG_BASE + 0x1470)
#define CXD56_TOPREG_IOOEN_APP                  (CXD56_TOPREG_BASE + 0x1474)
#define CXD56_TOPREG_IOFIX_APP                  (CXD56_TOPREG_BASE + 0x1478)
#define CXD56_TOPREG_IOCAPP_MONSEL0             (CXD56_TOPREG_BASE + 0x1480)
#define CXD56_TOPREG_IOCAPP_MONSEL1             (CXD56_TOPREG_BASE + 0x1484)
#define CXD56_TOPREG_IOCAPP_INTSEL0             (CXD56_TOPREG_BASE + 0x1490)
#define CXD56_TOPREG_IOCAPP_INTSEL1             (CXD56_TOPREG_BASE + 0x1494)
#define CXD56_TOPREG_IOCAPP_IOMD                (CXD56_TOPREG_BASE + 0x14a0)
#define CXD56_TOPREG_GP_I2C4_BCK                (CXD56_TOPREG_BASE + 0x2000)
#define CXD56_TOPREG_GP_I2C4_BDT                (CXD56_TOPREG_BASE + 0x2004)
#define CXD56_TOPREG_GP_PMIC_INT                (CXD56_TOPREG_BASE + 0x2008)
#define CXD56_TOPREG_GP_RTC_IRQ_OUT             (CXD56_TOPREG_BASE + 0x200c)
#define CXD56_TOPREG_GP_AP_CLK                  (CXD56_TOPREG_BASE + 0x2010)
#define CXD56_TOPREG_GP_GNSS_1PPS_OUT           (CXD56_TOPREG_BASE + 0x2014)
#define CXD56_TOPREG_GP_SPI0_CS_X               (CXD56_TOPREG_BASE + 0x2040)
#define CXD56_TOPREG_GP_SPI0_SCK                (CXD56_TOPREG_BASE + 0x2044)
#define CXD56_TOPREG_GP_SPI0_MOSI               (CXD56_TOPREG_BASE + 0x2048)
#define CXD56_TOPREG_GP_SPI0_MISO               (CXD56_TOPREG_BASE + 0x204c)
#define CXD56_TOPREG_GP_SPI1_CS_X               (CXD56_TOPREG_BASE + 0x2050)
#define CXD56_TOPREG_GP_SPI1_SCK                (CXD56_TOPREG_BASE + 0x2054)
#define CXD56_TOPREG_GP_SPI1_IO0                (CXD56_TOPREG_BASE + 0x2058)
#define CXD56_TOPREG_GP_SPI1_IO1                (CXD56_TOPREG_BASE + 0x205c)
#define CXD56_TOPREG_GP_SPI1_IO2                (CXD56_TOPREG_BASE + 0x2060)
#define CXD56_TOPREG_GP_SPI1_IO3                (CXD56_TOPREG_BASE + 0x2064)
#define CXD56_TOPREG_GP_SPI2_CS_X               (CXD56_TOPREG_BASE + 0x2068)
#define CXD56_TOPREG_GP_SPI2_SCK                (CXD56_TOPREG_BASE + 0x206c)
#define CXD56_TOPREG_GP_SPI2_MOSI               (CXD56_TOPREG_BASE + 0x2070)
#define CXD56_TOPREG_GP_SPI2_MISO               (CXD56_TOPREG_BASE + 0x2074)
#define CXD56_TOPREG_GP_HIF_IRQ_OUT             (CXD56_TOPREG_BASE + 0x2078)
#define CXD56_TOPREG_GP_HIF_GPIO0               (CXD56_TOPREG_BASE + 0x207c)
#define CXD56_TOPREG_GP_SEN_IRQ_IN              (CXD56_TOPREG_BASE + 0x2090)
#define CXD56_TOPREG_GP_SPI3_CS0_X              (CXD56_TOPREG_BASE + 0x2094)
#define CXD56_TOPREG_GP_SPI3_CS1_X              (CXD56_TOPREG_BASE + 0x2098)
#define CXD56_TOPREG_GP_SPI3_CS2_X              (CXD56_TOPREG_BASE + 0x209c)
#define CXD56_TOPREG_GP_SPI3_SCK                (CXD56_TOPREG_BASE + 0x20a0)
#define CXD56_TOPREG_GP_SPI3_MOSI               (CXD56_TOPREG_BASE + 0x20a4)
#define CXD56_TOPREG_GP_SPI3_MISO               (CXD56_TOPREG_BASE + 0x20a8)
#define CXD56_TOPREG_GP_I2C0_BCK                (CXD56_TOPREG_BASE + 0x20ac)
#define CXD56_TOPREG_GP_I2C0_BDT                (CXD56_TOPREG_BASE + 0x20b0)
#define CXD56_TOPREG_GP_PWM0                    (CXD56_TOPREG_BASE + 0x20b4)
#define CXD56_TOPREG_GP_PWM1                    (CXD56_TOPREG_BASE + 0x20b8)
#define CXD56_TOPREG_GP_PWM2                    (CXD56_TOPREG_BASE + 0x20bc)
#define CXD56_TOPREG_GP_PWM3                    (CXD56_TOPREG_BASE + 0x20c0)
#define CXD56_TOPREG_GP_IS_CLK                  (CXD56_TOPREG_BASE + 0x20c4)
#define CXD56_TOPREG_GP_IS_VSYNC                (CXD56_TOPREG_BASE + 0x20c8)
#define CXD56_TOPREG_GP_IS_HSYNC                (CXD56_TOPREG_BASE + 0x20cc)
#define CXD56_TOPREG_GP_IS_DATA0                (CXD56_TOPREG_BASE + 0x20d0)
#define CXD56_TOPREG_GP_IS_DATA1                (CXD56_TOPREG_BASE + 0x20d4)
#define CXD56_TOPREG_GP_IS_DATA2                (CXD56_TOPREG_BASE + 0x20d8)
#define CXD56_TOPREG_GP_IS_DATA3                (CXD56_TOPREG_BASE + 0x20dc)
#define CXD56_TOPREG_GP_IS_DATA4                (CXD56_TOPREG_BASE + 0x20e0)
#define CXD56_TOPREG_GP_IS_DATA5                (CXD56_TOPREG_BASE + 0x20e4)
#define CXD56_TOPREG_GP_IS_DATA6                (CXD56_TOPREG_BASE + 0x20e8)
#define CXD56_TOPREG_GP_IS_DATA7                (CXD56_TOPREG_BASE + 0x20ec)
#define CXD56_TOPREG_GP_UART2_TXD               (CXD56_TOPREG_BASE + 0x20f0)
#define CXD56_TOPREG_GP_UART2_RXD               (CXD56_TOPREG_BASE + 0x20f4)
#define CXD56_TOPREG_GP_UART2_CTS               (CXD56_TOPREG_BASE + 0x20f8)
#define CXD56_TOPREG_GP_UART2_RTS               (CXD56_TOPREG_BASE + 0x20fc)
#define CXD56_TOPREG_GP_SPI4_CS_X               (CXD56_TOPREG_BASE + 0x2100)
#define CXD56_TOPREG_GP_SPI4_SCK                (CXD56_TOPREG_BASE + 0x2104)
#define CXD56_TOPREG_GP_SPI4_MOSI               (CXD56_TOPREG_BASE + 0x2108)
#define CXD56_TOPREG_GP_SPI4_MISO               (CXD56_TOPREG_BASE + 0x210c)
#define CXD56_TOPREG_GP_EMMC_CLK                (CXD56_TOPREG_BASE + 0x2110)
#define CXD56_TOPREG_GP_EMMC_CMD                (CXD56_TOPREG_BASE + 0x2114)
#define CXD56_TOPREG_GP_EMMC_DATA0              (CXD56_TOPREG_BASE + 0x2118)
#define CXD56_TOPREG_GP_EMMC_DATA1              (CXD56_TOPREG_BASE + 0x211c)
#define CXD56_TOPREG_GP_EMMC_DATA2              (CXD56_TOPREG_BASE + 0x2120)
#define CXD56_TOPREG_GP_EMMC_DATA3              (CXD56_TOPREG_BASE + 0x2124)
#define CXD56_TOPREG_GP_SDIO_CLK                (CXD56_TOPREG_BASE + 0x2128)
#define CXD56_TOPREG_GP_SDIO_CMD                (CXD56_TOPREG_BASE + 0x212c)
#define CXD56_TOPREG_GP_SDIO_DATA0              (CXD56_TOPREG_BASE + 0x2130)
#define CXD56_TOPREG_GP_SDIO_DATA1              (CXD56_TOPREG_BASE + 0x2134)
#define CXD56_TOPREG_GP_SDIO_DATA2              (CXD56_TOPREG_BASE + 0x2138)
#define CXD56_TOPREG_GP_SDIO_DATA3              (CXD56_TOPREG_BASE + 0x213c)
#define CXD56_TOPREG_GP_SDIO_CD                 (CXD56_TOPREG_BASE + 0x2140)
#define CXD56_TOPREG_GP_SDIO_WP                 (CXD56_TOPREG_BASE + 0x2144)
#define CXD56_TOPREG_GP_SDIO_CMDDIR             (CXD56_TOPREG_BASE + 0x2148)
#define CXD56_TOPREG_GP_SDIO_DIR0               (CXD56_TOPREG_BASE + 0x214c)
#define CXD56_TOPREG_GP_SDIO_DIR1_3             (CXD56_TOPREG_BASE + 0x2150)
#define CXD56_TOPREG_GP_SDIO_CLKI               (CXD56_TOPREG_BASE + 0x2154)
#define CXD56_TOPREG_GP_I2S0_BCK                (CXD56_TOPREG_BASE + 0x2158)
#define CXD56_TOPREG_GP_I2S0_LRCK               (CXD56_TOPREG_BASE + 0x215c)
#define CXD56_TOPREG_GP_I2S0_DATA_IN            (CXD56_TOPREG_BASE + 0x2160)
#define CXD56_TOPREG_GP_I2S0_DATA_OUT           (CXD56_TOPREG_BASE + 0x2164)
#define CXD56_TOPREG_GP_I2S1_BCK                (CXD56_TOPREG_BASE + 0x2168)
#define CXD56_TOPREG_GP_I2S1_LRCK               (CXD56_TOPREG_BASE + 0x216c)
#define CXD56_TOPREG_GP_I2S1_DATA_IN            (CXD56_TOPREG_BASE + 0x2170)
#define CXD56_TOPREG_GP_I2S1_DATA_OUT           (CXD56_TOPREG_BASE + 0x2174)
#define CXD56_TOPREG_GP_MCLK                    (CXD56_TOPREG_BASE + 0x2178)
#define CXD56_TOPREG_GP_PDM_CLK                 (CXD56_TOPREG_BASE + 0x217c)
#define CXD56_TOPREG_GP_PDM_IN                  (CXD56_TOPREG_BASE + 0x2180)
#define CXD56_TOPREG_GP_PDM_OUT                 (CXD56_TOPREG_BASE + 0x2184)
#define CXD56_TOPREG_GP_USB_VBUSINT             (CXD56_TOPREG_BASE + 0x2188)

/* Topreg sub */

#define CXD56_TOPREG_PSW_CHECK                  (CXD56_TOPREG_SUB_BASE + 0x0000)
#define CXD56_TOPREG_UNEXP_PSW_DIG              (CXD56_TOPREG_SUB_BASE + 0x0004)
#define CXD56_TOPREG_UNEXP_PSW_ANA              (CXD56_TOPREG_SUB_BASE + 0x0008)
#define CXD56_TOPREG_UNEXP_OTHER                (CXD56_TOPREG_SUB_BASE + 0x000c)
#define CXD56_TOPREG_UNEXP_CLR                  (CXD56_TOPREG_SUB_BASE + 0x0010)
#define CXD56_TOPREG_PMU_WAIT10                 (CXD56_TOPREG_SUB_BASE + 0x0020)
#define CXD56_TOPREG_PMU_WAIT11                 (CXD56_TOPREG_SUB_BASE + 0x0024)
#define CXD56_TOPREG_SWRESET_DBG                (CXD56_TOPREG_SUB_BASE + 0x0400)
#define CXD56_TOPREG_SWRESET_GNSDSP             (CXD56_TOPREG_SUB_BASE + 0x0404)
#define CXD56_TOPREG_SWRESET_APP                (CXD56_TOPREG_SUB_BASE + 0x0408)
#define CXD56_TOPREG_SYSCPU_CKEN                (CXD56_TOPREG_SUB_BASE + 0x0410)
#define CXD56_TOPREG_APP_CKEN                   (CXD56_TOPREG_SUB_BASE + 0x0414)
#define CXD56_TOPREG_APP_CKSEL                  (CXD56_TOPREG_SUB_BASE + 0x0418)
#define CXD56_TOPREG_APP_DIV                    (CXD56_TOPREG_SUB_BASE + 0x041c)
#define CXD56_TOPREG_SYSIOP_SUB_CKEN            (CXD56_TOPREG_SUB_BASE + 0x0420)
#define CXD56_TOPREG_ROSC_MON                   (CXD56_TOPREG_SUB_BASE + 0x0428)
#define CXD56_TOPREG_TDC_MON                    (CXD56_TOPREG_SUB_BASE + 0x042c)
#define CXD56_TOPREG_PMU_WAKE_TRIG0_CLR         (CXD56_TOPREG_SUB_BASE + 0x0430)
#define CXD56_TOPREG_PMU_WAKE_TRIG1_CLR         (CXD56_TOPREG_SUB_BASE + 0x0434)
#define CXD56_TOPREG_PMU_WAKE_TRIG0_RAW         (CXD56_TOPREG_SUB_BASE + 0x0438)
#define CXD56_TOPREG_PMU_WAKE_TRIG1_RAW         (CXD56_TOPREG_SUB_BASE + 0x043c)
#define CXD56_TOPREG_PMU_WAKE_TRIG0             (CXD56_TOPREG_SUB_BASE + 0x0440)
#define CXD56_TOPREG_PMU_WAKE_TRIG1             (CXD56_TOPREG_SUB_BASE + 0x0444)
#define CXD56_TOPREG_RTC1_CTL                   (CXD56_TOPREG_SUB_BASE + 0x0470)
#define CXD56_TOPREG_GNSDSP_CKEN                (CXD56_TOPREG_SUB_BASE + 0x0c20)
#define CXD56_TOPREG_GNSS_DIV                   (CXD56_TOPREG_SUB_BASE + 0x0c28)
#define CXD56_TOPREG_APPDSP_RAMMODE_SEL0        (CXD56_TOPREG_SUB_BASE + 0x1400)
#define CXD56_TOPREG_APPDSP_RAMMODE_SEL1        (CXD56_TOPREG_SUB_BASE + 0x1404)
#define CXD56_TOPREG_APPDSP_RAMMODE_STAT0       (CXD56_TOPREG_SUB_BASE + 0x1420)
#define CXD56_TOPREG_APPDSP_RAMMODE_STAT1       (CXD56_TOPREG_SUB_BASE + 0x1424)
#define CXD56_TOPREG_BUSERR0                    (CXD56_TOPREG_SUB_BASE + 0x1470)
#define CXD56_TOPREG_BUSERR1                    (CXD56_TOPREG_SUB_BASE + 0x1474)
#define CXD56_TOPREG_BUSERR2                    (CXD56_TOPREG_SUB_BASE + 0x1478)
#define CXD56_TOPREG_CHIP_ID                    (CXD56_TOPREG_SUB_BASE + 0x1490)
#define CXD56_TOPREG_CUID0                      (CXD56_TOPREG_SUB_BASE + 0x1494)
#define CXD56_TOPREG_CUID1                      (CXD56_TOPREG_SUB_BASE + 0x1498)
#define CXD56_TOPREG_UDID0                      (CXD56_TOPREG_SUB_BASE + 0x149c)
#define CXD56_TOPREG_UDID1                      (CXD56_TOPREG_SUB_BASE + 0x14a0)
#define CXD56_TOPREG_FUSE_STATUS                (CXD56_TOPREG_SUB_BASE + 0x14a4)
#define CXD56_TOPREG_SDBG_ENB                   (CXD56_TOPREG_SUB_BASE + 0x14a8)
#define CXD56_TOPREG_DBG_MONSEL                 (CXD56_TOPREG_SUB_BASE + 0x14c0)

/* PWD_CTL, PWD_STAT */

#define PWD_APP_AUDIO  (1u<<14)
#define PWD_GNSS       (1u<<13)
#define PWD_GNSS_ITP   (1u<<12)
#define PWD_APP_SUB    (1u<<10)
#define PWD_APP_DSP    (1u<<9)
#define PWD_APP        (1u<<8)
#define PWD_SYSIOP_SUB (1u<<6)
#define PWD_SYSIOP     (1u<<5)
#define PWD_CORE       (1u<<4)
#define PWD_SCU        (1u<<0)

/* ANA_PW_CTL */

#define ANA_PW_LPADC  (1u<<13)
#define ANA_PW_HPADC  (1u<<12)
#define ANA_PW_RF_PLL (1u<<9)
#define ANA_PW_RF_LO  (1u<<8)
#define ANA_PW_RF_ADC (1u<<7)
#define ANA_PW_RF_IF  (1u<<6)
#define ANA_PW_RF_MIX (1u<<5)
#define ANA_PW_RF_LNA (1u<<4)
#define ANA_PW_SYSPLL (1u<<2)
#define ANA_PW_XOSC   (1u<<1)
#define ANA_PW_RCOSC  (1u<<0)

/* ANA_EN_CTL */

#define ON_GP_MBI_EN_SET (1u<<27)
#define OFF_GP_MBI_EN_CLR (1u<<26)
#define ON_XO_OSC_EN_SET (1u<<25)
#define OFF_XO_OSC_EN_CLR (1u<<24)
#define ON_XO_OSCOUT_EN_SET (1u<<20)
#define OFF_XO_OSCOUT_EN_CLR (1u<<19)
#define ON_XO_EXT_EN_SET (1u<<18)
#define OFF_XO_EXT_EN_CLR (1u<<17)
#define OFF_XO_CLK_EN_CLR (1u<<16)
#define ON_SP_ENPLL_SET (1u<<8)
#define OFF_SP_ENCLK_CLR (1u<<7)
#define ON_SP_ENOTHER_SET (1u<<6)
#define OFF_SP_ENOTHER_CLR (1u<<5)
#define OFF_SP_ENPLL_CLR (1u<<4)
#define ON_RO_XEN_CLR (1u<<2)
#define OFF_RO_XEN_SET (1u<<1)
#define OFF_RO_CLK_XEN_SET (1u<<0)

/* PMU_INT_STAT */

#define PMU_INT_BOOTEN (1u<<4)
#define PMU_INT_UNEXP_I2C_PMIC (1u<<3)
#define PMU_INT_UNEXP_TIMEOUT  (1u<<2)
#define PMU_INT_NOGO           (1u<<1)
#define PMU_INT_DONE           (1u<<0)

/* CKSEL_ROOT */

#define STATUS_RTC_MASK (3u<<30)
#define STATUS_RTC_SEL  (2u<<30)
#define ENABLE_RF_PLL1 (1u<<4)
#define ENABLE_SOURCE_SEL (1u<<16)

/* CRG_INT_CLR0 */

#define CRG_CK_PCLK_UART0    (1u<<0)
#define CRG_CK_UART0         (1u<<1)
#define CRG_CK_BRG_HOST      (1u<<2)
#define CRG_CK_PCLK_HOSTIFC  (1u<<3)
#define CRG_CK_HOSTIFC_SEQ   (1u<<4)
#define CRG_CK_I2CS          (1u<<5)
#define CRG_CK_RTC_ORG       (1u<<6)
#define CRG_CK_SYSIOP_RTC    (1u<<7)
#define CRG_CK_BRG_SCU       (1u<<8)
#define CRG_CK_SCU           (1u<<9)
#define CRG_CK_SCU_SPI       (1u<<10)
#define CRG_CK_SCU_I2C0      (1u<<11)
#define CRG_CK_SCU_I2C1      (1u<<12)
#define CRG_CK_SCU_SEQ       (1u<<13)
#define CRG_CK_SCU_SC        (1u<<14)
#define CRG_CK_32K           (1u<<15)
#define CRG_CK_U32KH         (1u<<16)
#define CRG_CK_U32KL         (1u<<17)
#define CRG_CK_TADC          (1u<<18)
#define CRG_CK_RTC_PCLK      (1u<<19)
#define CRG_CK_PMU_RTC_PCLK  (1u<<20)
#define CRG_CK_APP           (1u<<21)

/* CRG_INT_CLR1 */

#define CRG_CK_CPU_BUS       (1u<<0)
#define CRG_CK_CPU_BUS_TO    (1u<<1)
#define CRG_CK_RFPLL1        (1u<<2)
#define CRG_CK_RFPLL1_TO     (1u<<3)
#define CRG_CK_RTC_PRE       (1u<<4)
#define CRG_CK_RTC_PRE_TO    (1u<<5)
#define CRG_CK_APP_PRE       (1u<<6)
#define CRG_CK_APP_PRE_TO    (1u<<7)
#define CRG_CK_SEL_SP        (1u<<8)
#define CRG_CK_SEL_SP_TO     (1u<<9)
#define CRG_CK_SEL_RO_RTC    (1u<<10)
#define CRG_FREQFIX_ERR      (1u<<11)

/* SYS_PLL_CTRL1 */

#define ENABLE_DSPCLK (1u<<3)
#define ENABLE_GPADCLK (1u<<1)

/* RCOSC_CTRL2 */

#define DISABLE_SENSCLK (1u<<14)
#define DISABLE_LOGICLK (1u<<13)

/* SWRESET_BUS */

#define XRST_PMU_I2CM     (1u<<16)
#define XRST_I2CM         (1u<<11)
#define XRST_UART0        (1u<<10)
#define XRST_HOSTIFC_ISOP (1u<<9)
#define XRST_HOSTIFC      (1u<<8)
#define XRST_KAKI         (1u<<6)
#define XRST_UART1        (1u<<5)
#define XRST_SAKE         (1u<<2)
#define XRST_SFC          (1u<<1)
#define XRST_SPIM         (1u<<0)

/* SWRESET_SCU */

#define XRST_SCU_SPI      (1u<<8)
#define XRST_SCU_ISOP     (1u<<7)
#define XRST_SCU_I2C1     (1u<<6)
#define XRST_SCU_I2C2     (1u<<5)
#define XRST_SCU_LPADC    (1u<<4)
#define XRST_SCU_HPADC    (1u<<2)

/* SYSIOP_CKEN */

#define CKEN_HOSSPI        (1u<<17)
#define CKEN_HOSI2C        (1u<<16)
#define CKEN_HOSTIFC_SEQ   (1u<<15)
#define CKEN_BRG_SCU       (1u<<14)
#define CKEN_SYSIOP_RTC    (1u<<13)
#define CKEN_RCOSC_OUT     (1u<<12)
#define CKEN_AP_CLK        (1u<<11)
#define CKEN_RTC_ORG       (1u<<10)
#define CKEN_FREQDIS       (1u<<9)
#define CKEN_APB           (1u<<8)
#define CKEN_AHB_DMAC2     (1u<<7)
#define CKEN_AHB_DMAC1     (1u<<6)
#define CKEN_AHB_DMAC0     (1u<<5)
#define CKEN_BRG_HOST      (1u<<4)
#define CKEN_I2CS          (1u<<3)
#define CKEN_PCLK_HOSTIFC  (1u<<2)
#define CKEN_PCLK_UART0    (1u<<1)
#define CKEN_UART0         (1u<<0)

/* IOOEN_APP */

#define I2S1_LRCK (1u<<5)
#define I2S1_BCK (1u<<4)
#define I2S0_LRCK (1u<<1)
#define I2S0_BCK (1u<<0)

/* SYSIOP_SUB_CKEN */

#define CK_COM_UART_PCLK (1u<<16)
#define CK_SFC_HCLK_LOW  (1u<<9)
#define CK_SFC_SFCLK     (1u<<8)
#define CK_SFC_HCLK      (1u<<7)
#define CK_SFC           (CK_SFC_HCLK | CK_SFC_SFCLK | CK_SFC_HCLK_LOW)
#define CK_HCLK_SAKE     (1u<<6)
#define CK_I2CM          (1u<<5)
#define CK_SPIM          (1u<<4)
#define CK_UART1         (1u<<3)
#define CK_AHB_DMAC3     (1u<<2)
#define CK_COM_BRG       (1u<<1)
#define CK_AHB_BRG_COMIF (1u<<0)

/* PWD_CTL */

#define PWD_CTL_APP_SUB  (1u<<10)

/* SCU_CKEN */

#define SCU_SCU          (1u<<0)
#define SCU_I2C0         (1u<<1)
#define SCU_I2C1         (1u<<2)
#define SCU_SPI          (1u<<3)
#define SCU_SEQ          (1u<<4)
#define SCU_32K          (1u<<5)
#define SCU_U32KL        (1u<<6)
#define SCU_U32KH        (1u<<7)
#define SCU_SC           (1u<<8)

/* APP_CKEN */

#define APP_CKEN_CPU     (1u<<0)
#define APP_CKEN_MCLK    (1u<<1)
#define APP_CKEN_AHB     (1u<<3)

/* APP_CKSEL */

#define AUD_MCLK_MASK    (3u<<16)
#define AUD_MCLK_EXT     (0u<<16) /* External XTAL */
#define AUD_MCLK_XOSC    (1u<<16) /* Internal XOSC */
#define AUD_MCLK_RCOSC   (2u<<16) /* Internal RCOSC */

/* GNSDSP_CKEN */

#define GNSDSP_CKEN_P1   (1u<<5)
#define GNSDSP_CKEN_COP  (1u<<7)

#endif
