/****************************************************************************
 * graphics/nxmu/nx_filltrapezoid.c
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

#include <string.h>
#include <mqueue.h>
#include <errno.h>
#include <debug.h>

#include <nuttx/nx/nx.h>

#include "nxfe.h"

/****************************************************************************
 * Pre-Processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: nx_filltrapezoid
 *
 * Description:
 *  Fill the specified trapezoidal region in the window with the specified color
 *
 * Input Parameters:
 *   hwnd  - The window handle
 *   clip - Clipping region (may be null)
 *   trap  - The trapezoidal region to be filled
 *   color - The color to use in the fill
 *
 * Return:
 *   OK on success; ERROR on failure with errno set appropriately
 *
 ****************************************************************************/

int nx_filltrapezoid(NXWINDOW hwnd, FAR const struct nxgl_rect_s *clip,
                     FAR const struct nxgl_trapezoid_s *trap,
                     nxgl_mxpixel_t color[CONFIG_NX_NPLANES])
{
  FAR struct nxbe_window_s *wnd = (FAR struct nxbe_window_s *)hwnd;
  struct nxsvrmsg_filltrapezoid_s  outmsg;
  int ret;
  int i;

#ifdef CONFIG_DEBUG
  if (!wnd || !wnd->conn || !trap || !color)
    {
      errno = EINVAL;
      return ERROR;
    }
#endif

  /* Format the fill command */

  outmsg.msgid = NX_SVRMSG_FILLTRAP;
  outmsg.wnd   = wnd;

  if (clip)
    {
      nxgl_rectcopy(&outmsg.clip, clip);
    }
  else
    {
      memset(&outmsg.clip, 0, sizeof(struct nxgl_rect_s));
    }
  nxgl_trapcopy(&outmsg.trap, trap);

  for (i = 0; i < CONFIG_NX_NPLANES; i++)
    {
      outmsg.color[i] = color[i];
    }

  /* Forward the fill command to the server */

  ret = mq_send(wnd->conn->cwrmq, &outmsg,
                sizeof(struct nxsvrmsg_filltrapezoid_s), NX_SVRMSG_PRIO);
  if (ret < 0)
    {
      gdbg("mq_send failed: %d\n", errno);
    }
  return ret;
}
