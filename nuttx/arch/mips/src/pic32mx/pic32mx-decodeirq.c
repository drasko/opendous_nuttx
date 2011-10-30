/****************************************************************************
 * arch/mips/src/pic32mx/pic32mx-decodeirq.c
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
#include <arch/board/board.h>

#include "up_arch.h"
#include "os_internal.h"

#include "pic32mx-int.h"
#include "pic32mx-internal.h"

/****************************************************************************
 * Pre-processor Definitions
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

/****************************************************************************
 * Name: pic32mx_decodeirq
 *
 * Description:
 *   Called from assembly language logic when an interrrupt exception occurs.
 *   This function decodes and dispatches the interrupt.
 *
 ****************************************************************************/

uint32_t *pic32mx_decodeirq(uint32_t *regs)
{
#ifdef CONFIG_SUPPRESS_INTERRUPTS

  up_ledon(LED_INIRQ);
  PANIC(OSERR_ERREXCEPTION);
  up_ledoff(LED_INIRQ); /* Won't get here */
  return regs;

#else
  uint32_t *savestate;
  uint32_t regval;
  int irq;

  /* If the board supports LEDs, turn on an LED now to indicate that we are
   * processing an interrupt.
   */

  up_ledon(LED_INIRQ);

  /* Save the current value of current_regs (to support nested interrupt
   * handling).  Then set current_regs to regs, indicating that this is
   * the interrupted context that is being processed now.
   */

  savestate    = (uint32_t*)current_regs;
  current_regs = regs;

  /* Loop while there are pending interrupts with priority greater than zero */

  for (;;)
    {
      /* Read the INTSTAT register.  This register contains both the priority
       * and the interrupt vector number.
       */

      regval = getreg32(PIC32MX_INT_INTSTAT);
      if ((regval & INT_INTSTAT_RIPL_MASK) == 0)
        {
          /* Break out of the loop when the priority is zero meaning that
           * there are no further pending interrupts.
           */

          break;
        }

      /* Get the vector number.  The IRQ numbers have been arranged so that
       * vector numbers and NuttX IRQ numbers are the same value.
       */

      irq = ((regval) & INT_INTSTAT_VEC_MASK) >> INT_INTSTAT_VEC_SHIFT;
      
      /* Disable further interrupts from this source until the driver has
       * cleared the pending interrupt sources.
       */

      up_disable_irq(irq);

      /* Deliver the IRQ */

      irq_dispatch(irq, regs);

      /* Unmask the last interrupt (global interrupt below the current interrupt
       * level are are still disabled)
       */

      up_enable_irq(irq);
    }

  /* If a context switch occurred while processing the interrupt then
   * current_regs may have change value.  If we return any value different
   * from the input regs, then the lower level will know that a context
   * switch occurred during interrupt processing.
   */

  regs = (uint32_t*)current_regs;

  /* Restore the previous value of current_regs.  NULL would indicate that
   * we are no longer in an interrupt handler.  It will be non-NULL if we
   * are returning from a nested interrupt.
   */

  current_regs = savestate;
  if (current_regs == NULL)
    {
      up_ledoff(LED_INIRQ);
    }

  return regs;
#endif
}
