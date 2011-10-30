/************************************************************************************
 * arch/mips/src/pic32mx/pic32mx-memorymap.h
 *
 *   Copyright (C) 2011 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <spudmonkey@racsa.co.cr>
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
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
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
 ****
 // *********************************************************************************/

#ifndef __ARCH_MIPS_SRC_PIC32MX_PIC32MX_MEMORYMAP_H
#define __ARCH_MIPS_SRC_PIC32MX_PIC32MX_MEMORYMAP_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <nuttx/config.h>

#include "chip.h"
#include "mips32-memorymap.h"

/************************************************************************************
 * Pre-Processor Definitions
 ************************************************************************************/
/* This memory may be valid for other chips as well, but I don't know that */

#if defined(CHIP_PIC32MX3) || defined(CHIP_PIC32MX4)

/* Physical Memory Map **************************************************************/

#  define PIC32MX_DATAMEM_PBASE     0x00000000 /* Size depends on CHIP_DATAMEM_KB */
#  define PIC32MX_PROGFLASH_PBASE   0x1d000000 /* Size depends on CHIP_PROGFLASH_KB */
#  define PIC32MX_SFR_PBASE         0x1f800000 /* Special function registers */
#  define PIC32MX_BOOTFLASH_PBASE   0x1fc00000 /* Size depends on CHIP_BOOTFLASH_KB */
#  define PIC32MX_DEVCFG_PBASE      0x1fc02ff0 /* Device configuration registers */

/* Virtual Memory Map ***************************************************************/

#  define PIC32MX_DATAMEM_K0BASE    (KSEG0_BASE + PIC32MX_DATAMEM_PBASE)
#  define PIC32MX_PROGFLASH_K0BASE  (KSEG0_BASE + PIC32MX_PROGFLASH_PBASE)
#  define PIC32MX_BOOTFLASH_K0BASE  (KSEG0_BASE + PIC32MX_BOOTFLASH_PBASE)
#  define PIC32MX_DEVCFG_K0BASE     (KSEG0_BASE + PIC32MX_DEVCFG_PBASE)

#  define PIC32MX_DATAMEM_K1BASE    (KSEG1_BASE + PIC32MX_DATAMEM_PBASE)
#  define PIC32MX_PROGFLASH_K1BASE  (KSEG1_BASE + PIC32MX_PROGFLASH_PBASE)
#  define PIC32MX_SFR_K1BASE        (KSEG1_BASE + PIC32MX_SFR_PBASE)
#  define PIC32MX_BOOTFLASH_K1BASE  (KSEG1_BASE + PIC32MX_BOOTFLASH_PBASE)
#  define PIC32MX_DEVCFG_K1BASE     (KSEG1_BASE + PIC32MX_DEVCFG_PBASE)

/* Register Base Addresses **********************************************************/

/* Watchdog Register Base Address */

#  define PIC32MX_WDT_K1BASE        (PIC32MX_SFR_K1BASE + 0x00000000)

/* RTCC Register Base Address */

#  define PIC32MX_RTCC_K1BASE       (PIC32MX_SFR_K1BASE + 0x00000200)

/* Timer 1-5 Register Base Addresses */

#  define PIC32MX_TIMER_K1BASE(n)   (PIC32MX_SFR_K1BASE + 0x00000600 + 0x200*(n-1))
#  define PIC32MX_TIMER1_K1BASE     (PIC32MX_SFR_K1BASE + 0x00000600)
#  define PIC32MX_TIMER2_K1BASE     (PIC32MX_SFR_K1BASE + 0x00000800)
#  define PIC32MX_TIMER3_K1BASE     (PIC32MX_SFR_K1BASE + 0x00000a00)
#  define PIC32MX_TIMER4_K1BASE     (PIC32MX_SFR_K1BASE + 0x00000c00)
#  define PIC32MX_TIMER5_K1BASE     (PIC32MX_SFR_K1BASE + 0x00000e00)

/* Input Capture 1-5 Register Base Addresses */

#  define PIC32MX_IC_K1BASE(n)     (PIC32MX_SFR_K1BASE + 0x00002000 + 0x200*(n-1))
#  define PIC32MX_IC1_K1BASE       (PIC32MX_SFR_K1BASE + 0x00002000)
#  define PIC32MX_IC2_K1BASE       (PIC32MX_SFR_K1BASE + 0x00002200)
#  define PIC32MX_IC3_K1BASE       (PIC32MX_SFR_K1BASE + 0x00002400)
#  define PIC32MX_IC4_K1BASE       (PIC32MX_SFR_K1BASE + 0x00002600)
#  define PIC32MX_IC5_K1BASE       (PIC32MX_SFR_K1BASE + 0x00002800)

/* Output Compare 1-5 Register Base Addresses */

#  define PIC32MX_OC_K1BASE(n)     (PIC32MX_SFR_K1BASE + 0x00003000 + 0x200*(n-1))
#  define PIC32MX_OC1_K1BASE       (PIC32MX_SFR_K1BASE + 0x00003000)
#  define PIC32MX_OC2_K1BASE       (PIC32MX_SFR_K1BASE + 0x00003200)
#  define PIC32MX_OC3_K1BASE       (PIC32MX_SFR_K1BASE + 0x00003400)
#  define PIC32MX_OC4_K1BASE       (PIC32MX_SFR_K1BASE + 0x00003600)
#  define PIC32MX_OC5_K1BASE       (PIC32MX_SFR_K1BASE + 0x00003800)

/* I2C 1-2 Register Base Addresses */

#  define PIC32MX_I2C1_K1BASE       (PIC32MX_SFR_K1BASE + 0x00005000)
#  define PIC32MX_I2C2_K1BASE       (PIC32MX_SFR_K1BASE + 0x00005200)

/* SPI 1-2 Register Base Addresses */

#  define PIC32MX_SPI1_K1BASE       (PIC32MX_SFR_K1BASE + 0x00005800)
#  define PIC32MX_SPI2_K1BASE       (PIC32MX_SFR_K1BASE + 0x00005a00)

/* UART 1-2 Register Base Addresses */

#  define PIC32MX_UART1_K1BASE      (PIC32MX_SFR_K1BASE + 0x00006000)
#  define PIC32MX_UART2_K1BASE      (PIC32MX_SFR_K1BASE + 0x00006200)

/* Parallel Master Register Base Address */

#  define PIC32MX_PMP_K1BASE        (PIC32MX_SFR_K1BASE + 0x00007000)

/* ADC Register Base Addresses */

#  define PIC32MX_ADC_K1BASE        (PIC32MX_SFR_K1BASE + 0x00009000)

/* Comparator Voltage Reference Register Base Addresses */

#  define PIC32MX_CVR_K1BASE        (PIC32MX_SFR_K1BASE + 0x00009800)

/* Comparator Register Base Addresses */

#  define PIC32MX_CM_K1BASE         (PIC32MX_SFR_K1BASE + 0x0000a000)
#  define PIC32MX_CM1_K1BASE        (PIC32MX_SFR_K1BASE + 0x0000a000)
#  define PIC32MX_CM2_K1BASE        (PIC32MX_SFR_K1BASE + 0x0000a010)

/* Oscillator Control Register Base Addresses */

#  define PIC32MX_OSC_K1BASE        (PIC32MX_SFR_K1BASE + 0x0000f000)

/* Programming and Diagnostics Register Base Addresses */

#  define PIC32MX_DDP_K1BASE        (PIC32MX_SFR_K1BASE + 0x0000f200)

/* FLASH Controller Register Base Addresses */

#  define PIC32MX_FLASH_K1BASE      (PIC32MX_SFR_K1BASE + 0x0000f400)

/* Reset Control Register Base Address */

#  define PIC32MX_RESET_K1BASE      (PIC32MX_SFR_K1BASE + 0x0000f600)

/* Interrupt Register Base Address */

#  define PIC32MX_INT_K1BASE        (PIC32MX_SFR_K1BASE + 0x00081000)

/* Bus Matrix Register Base Address */

#  define PIC32MX_BMX_K1BASE        (PIC32MX_SFR_K1BASE + 0x00082000)

/* DMA Register Base Address */

#  define PIC32MX_DMA_K1BASE        (PIC32MX_SFR_K1BASE + 0x00083000)
#  define PIC32MX_DMACH_K1BASE(n)   (PIC32MX_SFR_K1BASE + 0x00083060 + 0xc0*(n))
#  define PIC32MX_DMACH0_K1BASE     (PIC32MX_SFR_K1BASE + 0x00083060)
#  define PIC32MX_DMACH1_K1BASE     (PIC32MX_SFR_K1BASE + 0x00083120)
#  define PIC32MX_DMACH2_K1BASE     (PIC32MX_SFR_K1BASE + 0x000831e0)
#  define PIC32MX_DMACH3_K1BASE     (PIC32MX_SFR_K1BASE + 0x000832a0)

/* Prefetch Cache Register Base Address */

#  define PIC32MX_CHE_K1BASE        (PIC32MX_SFR_K1BASE + 0x00084000)

/* USB2 Register Base Addresses */

#  define PIC32MX_USB_K1BASE        (PIC32MX_SFR_K1BASE + 0x00085000)

/* Port Register Base Addresses */

#  define PIC32MX_IOPORTA           0
#  define PIC32MX_IOPORTB           1
#  define PIC32MX_IOPORTC           2
#  define PIC32MX_IOPORTD           3
#  define PIC32MX_IOPORTE           4
#  define PIC32MX_IOPORTF           5
#  define PIC32MX_IOPORTG           6
#  define PIC32MX_IOPORT_K1BASE(n) (PIC32MX_SFR_K1BASE + 0x00086000 + 0x40*(n))

#  define PIC32MX_IOPORTA_K1BASE   (PIC32MX_SFR_K1BASE + 0x00086000)
#  define PIC32MX_IOPORTB_K1BASE   (PIC32MX_SFR_K1BASE + 0x00086040)
#  define PIC32MX_IOPORTC_K1BASE   (PIC32MX_SFR_K1BASE + 0x00086080)
#  define PIC32MX_IOPORTD_K1BASE   (PIC32MX_SFR_K1BASE + 0x000860c0)
#  define PIC32MX_IOPORTE_K1BASE   (PIC32MX_SFR_K1BASE + 0x00086100)
#  define PIC32MX_IOPORTF_K1BASE   (PIC32MX_SFR_K1BASE + 0x00086140)
#  define PIC32MX_IOPORTG_K1BASE   (PIC32MX_SFR_K1BASE + 0x00086180)

#  define PIC32MX_IOPORTCN_K1BASE  (PIC32MX_SFR_K1BASE + 0x000861c0)

#else
#  error "Memory map unknown for this PIC32 chip"
#endif

/************************************************************************************
 * Public Types
 ************************************************************************************/

#ifndef __ASSEMBLY__

/************************************************************************************
 * Inline Functions
 ************************************************************************************/

/************************************************************************************
 * Public Function Prototypes
 ************************************************************************************/

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif /* __ASSEMBLY__ */
#endif /* __ARCH_MIPS_SRC_PIC32MX_PIC32MX_MEMORYMAP_H */
