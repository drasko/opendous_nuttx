/****************************************************************************
 * arch/mips/src/pic32mx/pic32mx-irq.c
 * arch/mips/src/chip/pic32mx-irq.c
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
#include <errno.h>
#include <debug.h>

#include <nuttx/irq.h>
#include <nuttx/arch.h>

#include <arch/irq.h>
#include <arch/pic32mx/cp0.h>

#include "up_arch.h"
#include "os_internal.h"
#include "up_internal.h"

#include "pic32mx-int.h"
#include "pic32mx-internal.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/
/* Configuration ************************************************************/

#ifdef CONFIG_PIC32MX_MVEC
#  error "Multi-vectors not supported"
#endif

/****************************************************************************
 * Public Data
 ****************************************************************************/

volatile uint32_t *current_regs;

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
 * Name: up_irqinitialize
 ****************************************************************************/

void up_irqinitialize(void)
{
  uint32_t regval;
  int irq;

  /* Disable all interrupts */

  putreg32(0xffff, PIC32MX_INT_IEC0CLR);
  putreg32(0xffff, PIC32MX_INT_IEC1CLR);

  /* Set all interrupts to the default (middle) priority */

  for (irq = 0; irq < NR_IRQS; irq++)
    {
      (void)up_prioritize_irq(irq, (INT_ICP_MID_PRIORITY << 2));
    }
  
  /* Set the CP0 cause IV bit meaning that the interrupt exception uses
   * the "special interrupt vector"
   */
 
  asm volatile("\tmfc0 %0,$13,0\n" : "=r"(regval));
  regval |= CP0_CAUSE_IV; 
  asm volatile("\tmtc0 %0,$13,0\n" : : "r"(regval));

  /* Configure multi- or single- vector interrupts */

#ifdef CONFIG_PIC32MX_MVEC
  putreg32(INT_INTCON_MVEC, PIC32MX_INT_INTCONSET);
#else
  putreg32(INT_INTCON_MVEC, PIC32MX_INT_INTCONCLR);
#endif

  /* currents_regs is non-NULL only while processing an interrupt */

  current_regs = NULL;

  /* Initialize logic to support a second level of interrupt decoding for
   * IOPORT pins.
   */
 
#ifdef CONFIG_GPIO_IRQ
  lpc17_gpioirqinitialize();
#endif

  /* And finally, enable interrupts */

#ifndef CONFIG_SUPPRESS_INTERRUPTS

  /* Interrupts are enabled by setting the IE bit in the CP0 status register */

  regval = 0;
  asm volatile("ei    %0" : "=r"(regval));

  /* Then enable all interrupt levels */

  irqrestore(CP0_STATUS_IM_ALL);
#endif
}

/****************************************************************************
 * Name: up_disable_irq
 *
 * Description:
 *   Disable the IRQ specified by 'irq'
 *
 ****************************************************************************/

void up_disable_irq(int irq)
{
  uint32_t regaddr;
  int bitno;

  /* Disable the interrupt by clearing the associated bit in the IEC register */

  DEBUGASSERT(irq >= PIC32MX_IRQSRC0_FIRST && irq <= PIC32MX_IRQSRC1_LAST)
  if (irq >= PIC32MX_IRQSRC0_FIRST)
    {
      if (irq <= PIC32MX_IRQSRC0_LAST)
        {
          /* Use IEC0 */

          regaddr = PIC32MX_INT_IEC0CLR;
          bitno  -= PIC32MX_IRQSRC0_FIRST;
        }
      else if (irq <= PIC32MX_IRQSRC1_LAST)
        {
          /* Use IEC1 */

          regaddr = PIC32MX_INT_IEC1CLR;
          bitno  -= PIC32MX_IRQSRC1_FIRST;
        }
      else
        {
          /* Value out of range.. just ignore */

          return;
        }

      /* Disable the interrupt */

      putreg32((1 << bitno), regaddr);
    }
}

/****************************************************************************
 * Name: up_enable_irq
 *
 * Description:
 *   Enable the IRQ specified by 'irq'
 *
 ****************************************************************************/

void up_enable_irq(int irq)
{
  uint32_t regaddr;
  int bitno;

  /* Enable the interrupt by setting the associated bit in the IEC register */

  DEBUGASSERT(irq >= PIC32MX_IRQSRC0_FIRST && irq <= PIC32MX_IRQSRC1_LAST)
  if (irq >= PIC32MX_IRQSRC0_FIRST)
    {
      if (irq <= PIC32MX_IRQSRC0_LAST)
        {
          /* Use IEC0 */

          regaddr = PIC32MX_INT_IEC0SET;
          bitno  -= PIC32MX_IRQSRC0_FIRST;
        }
      else if (irq <= PIC32MX_IRQSRC1_LAST)
        {
          /* Use IEC1 */

          regaddr = PIC32MX_INT_IEC1SET;
          bitno  -= PIC32MX_IRQSRC1_FIRST;
        }
      else
        {
          /* Value out of range.. just ignore */

          return;
        }

      /* Disable the interrupt */

      putreg32((1 << bitno), regaddr);
    }
}

/****************************************************************************
 * Name: up_pending_irq
 *
 * Description:
 *   Return true if the interrupt is pending and unmasked.
 *
 ****************************************************************************/

bool up_pending_irq(int irq)
{
  uint32_t ifsaddr;
  uint32_t iecaddr;
  uint32_t regval;
  int bitno;

  /* Disable the interrupt by clearing the associated bit in the IEC and then
   * acknowledge the interrupt by clearing the associated bit in the IFS
   * register.  It is necessary to do this BEFORE lowering the interrupt
   * priority level otherwise recursive interrupts would occur.
   */

  DEBUGASSERT(irq >= PIC32MX_IRQSRC0_FIRST && irq <= PIC32MX_IRQSRC1_LAST)
  if (irq >= PIC32MX_IRQSRC0_FIRST)
    {
      if (irq <= PIC32MX_IRQSRC0_LAST)
        {
          /* Use IFS0 */

          ifsaddr = PIC32MX_INT_IFS0;
          iecaddr = PIC32MX_INT_IEC0;
          bitno  -= PIC32MX_IRQSRC0_FIRST;
        }
      else if (irq <= PIC32MX_IRQSRC1_LAST)
        {
          /* Use IFS1 */

          ifsaddr = PIC32MX_INT_IFS1;
          iecaddr = PIC32MX_INT_IEC1;
          bitno  -= PIC32MX_IRQSRC1_FIRST;
        }
      else
        {
          /* Value out of range.. just ignore */

          return false;
        }

      /* Get the set of unmasked, pending interrupts.  Return true if the
       * interrupt is pending and unmask.
       */

      regval = getreg32(ifsaddr) & getreg32(iecaddr);
      return (regval & (1 << bitno)) != 0;
    }

  return false;
}

/****************************************************************************
 * Name: up_clrpend_irq
 *
 * Description:
 *   Clear any pending interrupt
 *
 ****************************************************************************/

void up_clrpend_irq(int irq)
{
  uint32_t regaddr;
  int bitno;

  /* Disable the interrupt by clearing the associated bit in the IEC and then
   * acknowledge the interrupt by clearing the associated bit in the IFS
   * register.  It is necessary to do this BEFORE lowering the interrupt
   * priority level otherwise recursive interrupts would occur.
   */

  DEBUGASSERT(irq >= PIC32MX_IRQSRC0_FIRST && irq <= PIC32MX_IRQSRC1_LAST)
  if (irq >= PIC32MX_IRQSRC0_FIRST)
    {
      if (irq <= PIC32MX_IRQSRC0_LAST)
        {
          /* Use IFS0 */

          regaddr = PIC32MX_INT_IFS0CLR;
          bitno  -= PIC32MX_IRQSRC0_FIRST;
        }
      else if (irq <= PIC32MX_IRQSRC1_LAST)
        {
          /* Use IFS1 */

          regaddr = PIC32MX_INT_IFS1CLR;
          bitno  -= PIC32MX_IRQSRC1_FIRST;
        }
      else
        {
          /* Value out of range.. just ignore */

          return;
        }

      /* Disable then acknowledge interrupt */

      putreg32((1 << bitno), regaddr);
    }
}

/****************************************************************************
 * Name: up_prioritize_irq
 *
 * Description:
 *   Set the priority of an IRQ by setting the priority and sub-priority
 *   fields in the PIC32MX IPC registers.  There are 12 IPC registers, IPC0
 *   through IPC11.  Each has sub-priority fields for 8 interrupts for a
 *   total of 96 interrupts max.
 *
 *   Each interrupt priority is represent by a group of 5 bits: a 3-bit
 *   priority and a 2-bit sub-priority.  These have different meanings to
 *   the hardware.  The priority is the priority level that is enabled
 *   or masked by the IPL field of the CAUSE register.  The sub-priority
 *   only mediates ties when two interrupts with the same priority pend
 *   simultaneously.
 *
 *   In this function, we just treat this as a single 5-bit priority.
 *   (MS 3-bits=priority; LS 2-bits=sub-priority).
 *
 *   The 5-bit priority/sub-priority fields are arranged at byte boundaries
 *   within each IPC register:
 *
 *     xxxP PPSS xxxP PPSS xxxP PPSS xxxP PPSS
 *
 ****************************************************************************/

#ifdef CONFIG_ARCH_IRQPRIO
int up_prioritize_irq(int irq, int priority)
{
  int regndx;
  int shift;

  /* Don't allow this function to be used for disabling interrupts.  There is
   * no good reason for this restriction other than I want to make sure that
   * the 5-bit priority values passed to this function are *not* confused with
   * the 3-bit hardware priority values.
   */

  DEBUGASSERT((unsigned)irq < NR_IRQS && (unsigned)(priority >> 2) > 0);
  if (irq < NR_IRQS)
    {
      /* Get the index to the IPC register and the shift to the 5-bit priority
       * field for this IRQ.
       */

      regndx = irq >> 2;       /* Range: 0-11 */
      shift  = (irq & 3) << 3; /* {0, 8, 16, 24 } */

      /* Set the new interrupt priority (momentarily disabling interrupts) */

      putreg32(0x1f << shift, PIC32MX_INT_IPCCLR(regndx));
      putreg32(priority << shift, PIC32MX_INT_IPCSET(regndx));
      return OK;
    }

  return -EINVAL;
}
#endif
