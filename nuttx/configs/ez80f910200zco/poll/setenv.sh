#!/bin/bash
# configs/ez80f910200zco/poll/setenv.sh
#
#   Copyright (C) 2009 Gregory Nutt. All rights reserved.
#   Author: Gregory Nutt <spudmonkey@racsa.co.cr>
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in
#    the documentation and/or other materials provided with the
#    distribution.
# 3. Neither the name NuttX nor the names of its contributors may be
#    used to endorse or promote products derived from this software
#    without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
# OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
# AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
# Check how we were executed
#
if [ "$(basename $0)" = "setenv.sh" ] ; then
  echo "You must source this script, not run it!" 1>&2
  exit 1
fi

#
# The ZDS-II toolchain lies outside of the Cygwin "sandbox" and
# attempts to set the PATH variable do not have the desired effect.
# Instead, alias are provided for all of the ZDS-II command line tools.
# Version 4.10.1 installed in the default location is assumed here.
#
ZDSBINDIR="C:/Program\ Files/ZiLOG/ZDSII_eZ80Acclaim!_4.11.1/bin"
alias ez8asm="${ZDSBINDIR}/ez8asm.exe"
alias ez8cc="${ZDSBINDIR}/ez8cc.exe"
alias ez8lib="${ZDSBINDIR}/ez8lib.exe"
alias ez8link="${ZDSBINDIR}/ez8link.exe"

