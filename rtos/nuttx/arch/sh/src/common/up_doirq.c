/****************************************************************************
 * arch/sh/src/common/up_doirq.c
 *
 *   Copyright (C) 2008-2009, 2011 Gregory Nutt. All rights reserved.
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
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdint.h>
#include <assert.h>

#include <nuttx/irq.h>
#include <nuttx/arch.h>

#include "up_arch.h"
#include "os_internal.h"
#include "up_internal.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

uint32_t *up_doirq(int irq, uint32_t* regs)
{
  up_ledon(LED_INIRQ);
#ifdef CONFIG_SUPPRESS_INTERRUPTS
  PANIC(OSERR_ERREXCEPTION);
#else
  if ((unsigned)irq < NR_IRQS)
    {
       uint32_t *savestate;

       /* Current regs non-zero indicates that we are processing
        * an interrupt; current_regs is also used to manage
        * interrupt level context switches.
        */

       savestate    = (uint32_t*)current_regs;
       current_regs = regs;

       /* Mask and acknowledge the interrupt (if supported by the chip) */

#ifndef CONFIG_ARCH_NOINTC
       up_maskack_irq(irq);
#endif

       /* Deliver the IRQ */

       irq_dispatch(irq, regs);

       /* Get the current value of regs... it may have changed because
        * of a context switch performed during interrupt processing.
        */

       regs = current_regs;

       /* Restore the previous value of current_regs.  NULL would indicate that
        * we are no longer in an interrupt handler.  It will be non-NULL if we
        * are returning from a nested interrupt.
        */

       current_regs = savestate;

       /* Unmask the last interrupt (global interrupts are still
        * disabled.
        */

#ifndef CONFIG_ARCH_NOINTC
       up_enable_irq(irq);
#endif
    }
  up_ledoff(LED_INIRQ);
#endif
  return regs;
}
