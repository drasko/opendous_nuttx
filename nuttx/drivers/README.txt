README
^^^^^^

This directory contains various device drivers -- both block and
character drivers as well as other more specialized drivers.

Contents:
  - Files in this directory
  - Subdirectories of this directory
  - Skeleton files

Files in this directory
^^^^^^^^^^^^^^^^^^^^^^^

can.c
	An unfinished CAN driver.

dev_null.c and dev_zero.c
	These files provide the standard /dev/null and /dev/zero devices.
 	See include/nuttx/fs.h for functions that should be called if you
	want to register these devices (devnull_register() and
	devzero_register()).

loop.c
	Supports the standard loop device that can be used to export a
	file (or character device) as a block device.  See losetup() and
	loteardown() in	include/nuttx/fs.h.

ramdisk.c
	Can be used to set up a block of memory or (read-only) FLASH as
	a block driver that can be mounted as a files system.  See
	include/nuttx/ramdisk.h.

rwbuffer.c
	A facility that can be use by any block driver in-order to add
	writing buffering and read-ahead buffering.

Subdirectories of this directory:
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

analog/
	This directory holds implementations of analog device drivers.
    This includes drivers for Analog to Digital Conversion (ADC) as
    well as drivers for Digital to Analog Conversion (DAC).
	See include/nuttx/analog/*.h for registration information.

bch/
	Contains logic that may be used to convert a block driver into
	a character driver.  This is the complementary conversion as that
	performed by loop.c.  See include/nuttx/fs.h for registration
	information.

analog/
	This directory holds implementations of input device drivers.
    This includes such things as touchscreen and keypad drivers.
	See include/nuttx/input/*.h for registration information.

lcd/
	Drivers for parallel and serial LCD and OLED type devices.  These
	drivers support interfaces as defined in include/nuttx/lcd/lcd.h

mmcsd/
	Support for MMC/SD block drivers.  MMC/SD block drivers based on
	SPI and SDIO/MCI interfaces are supported.  See include/nuttx/mmcsd.h
	and include/nuttx/sdio.h for further information.

mtd/
	Memory Technology Device (MTD) drivers.  Some simple drivers for
	memory technologies like FLASH, EEPROM, NVRAM, etc.  See
	include/nuttx/mtd.h

	(Note: This is a simple memory interface and should not be
	confused with the "real" MTD developed at infradead.org.  This
	logic is unrelated; I just used the name MTD because I am not
	aware of any other common way to refer to this class of devices).

net/
	Network interface drivers.  See also include/nuttx/net.h

pipes/
	FIFO and named pipe drivers.  Standard interfaces are declared
	in include/unistd.h

pm/
    Power management (PM) driver interfaces.  These interfaces are used
    to manage power usage of a platform by monitoring driver activity
    and by placing drivers into reduce power usage modes when the
    drivers are not active.

sensors/
	Drivers for various sensors

serial/
	Front-end character drivers for chip-specific UARTs.  This provide
	some TTY-like functionality and are commonly used (but not required for)
	the NuttX system console.  See also include/nuttx/serial.h

usbdev/
	USB device drivers.  See also include/nuttx/usb/usbdev.h

usbhost/
	USB host drivers.  See also include/nuttx/usb/usbhost.h

wireless/
	Drivers for various wireless devices.

Skeleton Files
^^^^^^^^^^^^^^

Skeleton files a "empty" frameworks for NuttX drivers.  They are provided to
give you a good starting point if you want to create a new NuttX driver.
The following skeleton files are available:

    drivers/lcd/skeleton.c -- Skeleton LCD driver
    drivers/mtd/skeleton.c -- Skeleton memory technology device drivers
    drivers/net/skeleton.c -- Skeleton network/Ethernet drivers
    drivers/usbhost/usbhost_skeleton.c -- Skeleton USB host class driver
