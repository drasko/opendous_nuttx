z80sim README
^^^^^^^^^^^^^

This port uses a primitive, emulated Z80 and the SDCC toolchain.
The instruction set emulator can be found in the NuttX SVN at
http://nuttx.svn.sourceforge.net/viewvc/nuttx/trunk/misc/sims/z80sim

The SDCC toolchain is available from http://sdcc.sourceforge.net/.  All
testing has been performed using version 2.6.0 of the SDCC toolchain.

Configuring NuttX
^^^^^^^^^^^^^^^^^

  ostest
    This configuration performs a simple, minimal OS test using
    examples/ostest.  This can be configurated as follows:

	cd tools
	./configure.sh z80sim/ostest
	cd -
	. ./setenv.sh


  nsh
    This configuration file builds NSH (examples/nsh).  This
    configuration is not functional due to issues with use of the
    simulated serial driver (see the TODO list).

    This configuration can be selected by:

	cd tools
	./configure.sh z80sim/nsh
	cd -
	. ./setenv.sh

 pashello
    Configures to use examples/pashello for execution from FLASH
    See examples/README.txt for information about pashello.

    This configuration is not usable because the resulting binary
    is too large for the z80 address space.

    This configuration can be selected by:

	cd tools
	./configure.sh z80sim/pashello
	cd -
	. ./setenv.sh

Building the SDCC toolchain
^^^^^^^^^^^^^^^^^^^^^^^^^^^

The SDCC toolchain is built with the standard configure/make/make install
sequence.  However, some special actions are required to generate libraries
compatible with this build.  First start with the usual steps

  download
  unpack
  cd sdcc
  ./configure

But before making, we need to apply a patch to the SDCC 2.6.0 source
so that the z80 assembler can handle long symbol names

  Apply sdcc-2.6.0-asz80-symlen.patch
  cd sdcc/device/lib

Then make the SDCC binaries

  cd sdcc
  make

and install SDCC:

  sudo make install

SDCC Update
^^^^^^^^^^^

I have had some problems building sdcc-2.6.0 on my current UBUNTU
release (9.10).  I had other problems building sdcc-2.9.0 on UBUNTU 9.10.
I suspect that the newer gcc toolchains are causing problems for these
older SDCC releases.

A 20091106 snapshot did build with no major problems on UBUNTU 9.10, but
has some compatibilty problems with the older SDCC compiler.  For one, you
will need to change the Z80 assember name and options in the Make.defs
files as follows:

-AS			= as-z80
+AS			= sdasz80
 
-	@$(AS) $(ASFLAGS) $2 $1
+	$(AS) $(ASFLAGS) $1

For another, I had other problems building with that 20091106 that look
like compiler bugs.  If you are using UBUNTU 9.10, you may have to either
(1) downgrade your GCC compiler to a version 3.x compiler and use one of
the older stable releases, or (2) wait for the next stable SDCC release
after 2.9.0.


 

