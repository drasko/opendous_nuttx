/****************************************************************************
 * arch/z80/include/ez80/limits.h
 *
 *   Copyright (C) 2007-2009 Gregory Nutt. All rights reserved.
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

#ifndef __ARCH_Z80_INCLUDE_EZ80_LIMITS_H 
#define __ARCH_Z80_INCLUDE_EZ80_LIMITS_H 

/****************************************************************************
 * Included Files
 ****************************************************************************/

/****************************************************************************
 * Definitions
 ****************************************************************************/

#define CHAR_BIT    8
#define SCHAR_MIN   0x80
#define SCHAR_MAX   0x7f
#define UCHAR_MAX   0xff

/* These could be different on machines where char is unsigned */

#define CHAR_MIN    SCHAR_MIN
#define CHAR_MAX    SCHAR_MAX

#define SHRT_MIN    0x8000
#define SHRT_MAX    0x7fff
#define USHRT_MAX   0xffff

#define INT_MIN     0x8000
#define INT_MAX     0x7fff
#define UINT_MAX    0xffff

/* These change on 32-bit and 64-bit platforms */

#define LONG_MAX    0x80000000
#define LONG_MIN    0x7fffffff
#define ULONG_MAX   0xffffffff

/* A pointer is 2 or 3 bytes, depending upon if the ez80 is in z80
 * compatibility mode or not
 *
 *   Z80 mode - 16 bits
 *   ADL mode - 24 bits
 */

#ifdef CONFIG_EZ80_Z80MODE
#define PTR_MIN     0x8000
#define PTR_MAX     0x7fff
#define UPTR_MAX    0xffff
#else
#define PTR_MIN     0x800000
#define PTR_MAX     0x7fffff
#define UPTR_MAX    0xffffff
#endif

#endif /* __ARCH_Z80_INCLUDE_EZ80_LIMITS_H */
