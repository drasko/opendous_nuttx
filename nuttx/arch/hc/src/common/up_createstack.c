/****************************************************************************
 * arch/hc/src/common/up_createstack.c
 *
 *   Copyright (C) 2009 Gregory Nutt. All rights reserved.
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

#include <sys/types.h>
#include <stdint.h>
#include <sched.h>
#include <debug.h>

#include <nuttx/kmalloc.h>
#include <nuttx/arch.h>

#include "up_arch.h"
#include "up_internal.h"

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Global Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_create_stack
 *
 * Description:
 *   Allocate a stack for a new thread and setup up stack-related information
 *   in the TCB.
 *
 *   The following TCB fields must be initialized:
 *   adj_stack_size: Stack size after adjustment for hardware, processor, etc.
 *     This value is retained only for debug purposes.
 *   stack_alloc_ptr: Pointer to allocated stack
 *   adj_stack_ptr: Adjusted stack_alloc_ptr for HW. The initial value of the
 *     stack pointer.
 *
 * Inputs:
 *   tcb: The TCB of new task
 *   stack_size:  The requested stack size.  At least this much must be
 *      allocated.
 *
 ****************************************************************************/

int up_create_stack(_TCB *tcb, size_t stack_size)
{
  if (tcb->stack_alloc_ptr && tcb->adj_stack_size != stack_size)
    {
      sched_free(tcb->stack_alloc_ptr);
      tcb->stack_alloc_ptr = NULL;
    }

   if (!tcb->stack_alloc_ptr)
     {
       tcb->stack_alloc_ptr = (uint32_t *)malloc(stack_size);
     }

   if (tcb->stack_alloc_ptr)
     {
       size_t top_of_stack;
       size_t size_of_stack;

       /* The CPU12 uses a push-down stack: the stack grows
        * toward lower addresses in memory. Because the CPU12 stack
        * operates as a decrement then store stack, the value assigned
        * to the initial stack pointer is one more than the last valid
        * stack address.
        */

       top_of_stack = (size_t)tcb->stack_alloc_ptr + stack_size;

       /* The CPU12 stack should be aligned at half-word (2 byte)
        * boundaries. If necessary top_of_stack must be rounded
        * down to the next boundary
        */

       top_of_stack &= ~1;
       size_of_stack = top_of_stack - (size_t)tcb->stack_alloc_ptr;

       /* Save the adjusted stack values in the _TCB */

       tcb->adj_stack_ptr  = (uint32_t*)top_of_stack;
       tcb->adj_stack_size = size_of_stack;

       up_ledon(LED_STACKCREATED);
       return OK;
     }

   return ERROR;
}
