/************************************************************************************
 * arch/avr/src/at32uc3/at32uc3_config.h
 *
 *   Copyright (C) 2010 Gregory Nutt. All rights reserved.
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
 *
 ************************************************************************************/

#ifndef __ARCH_AVR_SRC_AT32UC3_AT32UC3_CONFIG_H
#define __ARCH_AVR_SRC_AT32UC3_AT32UC3_CONFIG_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <nuttx/config.h>
#include "chip.h"

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

/* USART can be configured as a number of different devices (Only UART is supported
 * here now, that will be extended).  Check for consistency between USART enable
 * options.
 */

#if AVR32_NUSART < 1
#  undef CONFIG_AVR32_USART0
#  undef CONFIG_AVR32_USART1
#  undef CONFIG_AVR32_USART2
#endif
#if AVR32_NUSART < 2
#  undef CONFIG_AVR32_USART1
#  undef CONFIG_AVR32_USART2
#endif
#if AVR32_NUSART < 3
#  undef CONFIG_AVR32_USART2
#endif

/* Not all USART features are supported on all chips or all USARTS */

#ifdef CONFIG_ARCH_CHIP_AT32UC3B
# undef CONFIG_AVR32_USART0_RS485
# undef CONFIG_AVR32_USART0_MAN
# undef CONFIG_AVR32_USART0_MODEM
# undef CONFIG_AVR32_USART0_IRDA
# undef CONFIG_AVR32_USART0_ISO786
# undef CONFIG_AVR32_USART1_RS485
# undef CONFIG_AVR32_USART2_RS485
# undef CONFIG_AVR32_USART2_MAN
# undef CONFIG_AVR32_USART2_MODEM
# undef CONFIG_AVR32_USART2_IRDA
# undef CONFIG_AVR32_USART2_ISO786
#endif

/* Disable configurations if USART not selected in configuration file */

#ifndef CONFIG_AVR32_USART0
#  undef CONFIG_AVR32_USART0_RS232
#  undef CONFIG_AVR32_USART0_SPI
#  undef CONFIG_AVR32_USART0_RS485
#  undef CONFIG_AVR32_USART0_MAN
#  undef CONFIG_AVR32_USART0_MODEM
#  undef CONFIG_AVR32_USART0_IRDA
#  undef CONFIG_AVR32_USART0_ISO786
#endif

#ifndef CONFIG_AVR32_USART1
#  undef CONFIG_AVR32_USART1_RS232
#  undef CONFIG_AVR32_USART1_SPI
#  undef CONFIG_AVR32_USART1_RS485
#  undef CONFIG_AVR32_USART1_MAN
#  undef CONFIG_AVR32_USART1_MODEM
#  undef CONFIG_AVR32_USART1_IRDA
#  undef CONFIG_AVR32_USART1_ISO786
#endif

#ifndef CONFIG_AVR32_USART2
#  undef CONFIG_AVR32_USART2_RS232
#  undef CONFIG_AVR32_USART2_SPI
#  undef CONFIG_AVR32_USART2_RS485
#  undef CONFIG_AVR32_USART2_MAN
#  undef CONFIG_AVR32_USART2_MODEM
#  undef CONFIG_AVR32_USART2_IRDA
#  undef CONFIG_AVR32_USART2_ISO786
#endif

/* Is any UART configured? */

#if defined(CONFIG_AVR32_USART0_RS232) || \
    defined(CONFIG_AVR32_USART1_RS232) || \
	defined(CONFIG_AVR32_USART2_RS232)
#  define HAVE_RS232_DEVICE
#else
#  undef  HAVE_RS232_DEVICE
#endif

/* Is there a serial console? */

#if defined(CONFIG_USART0_SERIAL_CONSOLE) && defined(CONFIG_AVR32_USART0_RS232)
#  undef CONFIG_USART1_SERIAL_CONSOLE
#  undef CONFIG_USART2_SERIAL_CONSOLE
#  define HAVE_SERIAL_CONSOLE 1
#elif defined(CONFIG_USART1_SERIAL_CONSOLE) && defined(CONFIG_AVR32_USART1_RS232)
#  undef CONFIG_USART0_SERIAL_CONSOLE
#  undef CONFIG_USART2_SERIAL_CONSOLE
#  define HAVE_SERIAL_CONSOLE 1
#elif defined(CONFIG_USART2_SERIAL_CONSOLE) && defined(CONFIG_AVR32_USART2_RS232)
#  undef CONFIG_USART0_SERIAL_CONSOLE
#  undef CONFIG_USART1_SERIAL_CONSOLE
#  define HAVE_SERIAL_CONSOLE 1
#else
#  undef CONFIG_USART0_SERIAL_CONSOLE
#  undef CONFIG_USART1_SERIAL_CONSOLE
#  undef CONFIG_USART2_SERIAL_CONSOLE
#  undef HAVE_SERIAL_CONSOLE
#endif

/* Determine which (if any) console driver to use */

#if CONFIG_NFILE_DESCRIPTORS == 0 || defined(CONFIG_DEV_LOWCONSOLE) || !defined(HAVE_RS232_DEVICE)
#  undef CONFIG_USE_SERIALDRIVER
#  undef CONFIG_USE_EARLYSERIALINIT
#elif defined(CONFIG_DEV_CONSOLE) && CONFIG_NFILE_DESCRIPTORS > 0
#  define CONFIG_USE_SERIALDRIVER 1
#  define CONFIG_USE_EARLYSERIALINIT 1
#endif

/* If GPIO IRQ support is defined, then a set of GPIOs must all be included */

#if CONFIG_AVR32_GPIOIRQSETA == 0 && CONFIG_AVR32_GPIOIRQSETB == 0
#  undef CONFIG_AVR32_GPIOIRQ
#endif

/************************************************************************************
 * Public Types
 ************************************************************************************/

/************************************************************************************
 * Public Data
 ************************************************************************************/

/************************************************************************************
 * Public Functions
 ************************************************************************************/

#endif /* __ARCH_AVR_SRC_AT32UC3_AT32UC3_CONFIG_H */

