/****************************************************************************
 * lib/stdio/lib_rawinstream.c
 *
 *   Copyright (C) 2007-2009, 2011 Gregory Nutt. All rights reserved.
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

#include <unistd.h>
#include <errno.h>
#include "lib_internal.h"

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: rawinstream_getc
 ****************************************************************************/

static int rawinstream_getc(FAR struct lib_instream_s *this)
{
  FAR struct lib_rawinstream_s *rthis = (FAR struct lib_rawinstream_s *)this;
  char ch;

  if (this && rthis->fd >= 0)
    {
      int nwritten;
      do
        {
          nwritten = read(rthis->fd, &ch, 1);
          if (nwritten == 1)
            {
              this->nget++;
              return ch;
            }
        }
      while (nwritten < 0 && get_errno() == EINTR);
    }

  return EOF;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: lib_rawinstream
 *
 * Description:
 *   Initializes a stream for use with a file descriptor.
 *
 * Input parameters:
 *   rawinstream - User allocated, uninitialized instance of struct
 *                 lib_rawinstream_s to be initialized.
 *   fd          - User provided file/socket descriptor (must have been opened
 *                 for the correct access).
 *
 * Returned Value:
 *   None (User allocated instance initialized).
 *
 ****************************************************************************/

void lib_rawinstream(FAR struct lib_rawinstream_s *rawinstream, int fd)
{
  rawinstream->public.get  = rawinstream_getc;
  rawinstream->public.nget = 0;
  rawinstream->fd          = fd;
}

